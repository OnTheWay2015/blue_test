#include "stdafx.h"


START_DOS_MSG_MAP(CPlayerDBAdapterGroup);
END_DOS_MSG_MAP(CPlayerDBAdapterGroup);

CPlayerDBAdapterGroup::CPlayerDBAdapterGroup()
{
	INIT_DOS_MSG_MAP;
	m_pOperator = NULL;
	m_pDatabase = NULL;
	m_pDBTransManager = NULL;
	m_Status = STATUS_NONE;
}


CPlayerDBAdapterGroup::~CPlayerDBAdapterGroup()
{
}

BOOL CPlayerDBAdapterGroup::Initialize(IDistributedObjectOperator * pOperator)
{
	m_pOperator = pOperator;

	if (CMainConfig::GetInstance()->GetServerID() == 0)
		CMainConfig::GetInstance()->SetServerID(pOperator->GetRouterID());

	m_pDatabase = MONITORED_NEW(_T("CPlayerDBAdapterGroup"), CMySQLDatabase);

	const DB_CONNECT_CONFIG * pDBConfig = CMainConfig::GetInstance()->GetCommonConfig().GetDBConfig("Player");
	if (pDBConfig)
	{
		m_pDBTransManager = MONITORED_NEW(_T("CPlayerDBAdapterGroup"), CDBTransationManager);
		m_pDBTransManager->SetName("PlayerDBAdapterGroup");
		if (!m_pDBTransManager->Init(m_pDatabase,
			pDBConfig->DBConnectString,
			1, 16, 0))
		{
			Log("数据库初始化失败");
		}

		UINT InstanceShift = CMainConfig::GetInstance()->GetSystemConfig().InstanceShift;
		UINT InstanceCount = 1 << InstanceShift;
		for (UINT i = 0; i < InstanceCount; i++)
		{
			INSTANCE_CONFIG InstanceConfig;

			InstanceConfig.DBConfig = *pDBConfig;
			InstanceConfig.DBConfig.DBName.Format("%s_%u", (LPCTSTR)pDBConfig->DBName, i);
			InstanceConfig.DBConfig.DBConnectString.Format("DATABASE=%s;", (LPCTSTR)InstanceConfig.DBConfig.DBName);
			InstanceConfig.DBConfig.DBConnectString += pDBConfig->DBConnectString;

			InstanceConfig.IDSeedStep = InstanceCount;
			InstanceConfig.IDSeedIndex = i;
			InstanceConfig.HashMask = 0;
			for (UINT j = 0; j < 32 / InstanceCount; j++)
			{
				InstanceConfig.HashMask |= ((UINT)1) << (i + j * InstanceCount);
			}

			m_InstanceConfigList.Add(InstanceConfig);
		}
	}
	else
	{
		Log("无法获得数据库配置");
		return FALSE;
	}

	CDBTransCheckDB * pDBTrans = MONITORED_NEW(_T("PlayerDBAdapterDBTrans"), CDBTransCheckDB, this, m_InstanceConfigList);
	if (!m_pDBTransManager->AddTransaction(pDBTrans))
	{
		Log("提交数据库事务CDBTransCheckDB失败");
		SAFE_RELEASE(pDBTrans);
		return FALSE;
	}

	Log("PlayerDBAdapterGroup初始化成功");

	return TRUE;
}
void CPlayerDBAdapterGroup::Destory()
{
	SAFE_DELETE(m_pDBTransManager);
	SAFE_RELEASE(m_pDatabase);
	m_pOperator = NULL;

	for (auto item : m_InstanceList)
	{
		SAFE_RELEASE(item);
	}
	m_InstanceList.Clear();
}
UINT CPlayerDBAdapterGroup::AddUseRef()
{
	return CNameObject::AddUseRef();
}
void CPlayerDBAdapterGroup::Release()
{
	CNameObject::Release();
}

extern UINT g_PluginID;

int CPlayerDBAdapterGroup::Update(int ProcessPacketLimit)
{
	int ProcessCount = 0;

	if (m_pDBTransManager)
		m_pDBTransManager->Update(ProcessPacketLimit);

	if (m_Status == STATUS_SHUTDOWN)
	{
		if (m_ShutdownTimer.IsTimeOut(SHUTDOWN_CHECK_TIME))
		{
			m_ShutdownTimer.SaveTime();
			UINT ActiveCount = 0;
			for (UINT i = 0; i < m_InstanceList.GetCount(); i++)
			{
				if (m_InstanceList[i]->GetUseRef() > 1)
				{
					ActiveCount++;
				}
			}
			Log("Active=%u", ActiveCount);
			if (ActiveCount == 0)
			{
				for (UINT i = 0; i < m_InstanceList.GetCount(); i++)
				{
					SAFE_DELETE(m_InstanceList[i]);
				}
				m_InstanceList.Clear();
				m_pOperator->Release();
				m_pOperator->ShutDown(g_PluginID);
				m_Status = STATUS_NONE;
				LogDebug("退出");
			}
		}
	}
	else if (m_Status == STATUS_WORKING)
	{
		SAFE_DELETE(m_pDBTransManager);
	}

	return ProcessCount;
}





void CPlayerDBAdapterGroup::OnShutDown(BYTE Level, UINT Param)
{
	Log("Level=%u,Type=%u", Level, Param);
	if (Level == SYSTEM_SHUTDOWN_LEVEL2)
	{
		Log("系统关闭");
		m_Status = STATUS_SHUTDOWN;
	}		
}

void CPlayerDBAdapterGroup::CheckSystemReady()
{
	if (m_Status != STATUS_WORKING)
	{
		if (m_IsDBCheckOk)
		{
			m_Status = STATUS_WORKING;

			DOS_OBJECT_REGISTER_INFO_EX RegisterInfo;
			for (INSTANCE_CONFIG& InstanceConfig : m_InstanceConfigList)
			{
				CPlayerDBAdapter * pObject = MONITORED_NEW(_T("CPlayerDBAdapterGroup"), CPlayerDBAdapter, InstanceConfig, this, m_pDatabase);

				RegisterInfo.pObject = pObject;
				RegisterInfo.ObjectID.ObjectTypeID = OBJECT_TYPE_PLAYER_DB_ADAPTER;
				RegisterInfo.szObjectTypeName = "PlayerDBAdapter";
				RegisterInfo.Weight = CMainConfig::GetInstance()->GetPlayerDBAdapterRunConfig().Weight;
				RegisterInfo.MsgProcessLimit = CMainConfig::GetInstance()->GetPlayerDBAdapterRunConfig().MsgProcessLimit;
				m_pOperator->RegisterObject(RegisterInfo);

				m_InstanceList.Add(pObject);
			}

			Log("已创建%u个玩家数据适配器", m_InstanceConfigList.GetCount());
		}
	}
}

void CPlayerDBAdapterGroup::OnDBRetCheckDB(bool IsSucceed)
{
	m_IsDBCheckOk = IsSucceed;
	CheckSystemReady();
}