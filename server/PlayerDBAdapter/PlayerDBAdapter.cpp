#include "stdafx.h"

START_DOS_MSG_MAP(CPlayerDBAdapter);
DOS_MSG_MAP(CPlayerDBAdapterMsgHandler);
DOS_MSG_MAP(CGMCommandMsgHandler);
END_DOS_MSG_MAP(CPlayerDBAdapter);


CPlayerDBAdapter::CPlayerDBAdapter(const INSTANCE_CONFIG& InstanceConfig, CPlayerDBAdapterGroup * pGroup, IDatabase * pDatabase)
{
	INIT_DOS_MSG_MAP;
	m_Status = STATUS_NONE;
	m_InstanceConfig = InstanceConfig;
	m_pGroup = pGroup;
	m_pDatabase = pDatabase;
	m_pOperator=NULL;
	m_pDBTransManager=NULL;
	m_PlayerSavePtr=0;
	m_RoleSavePtr = 0;
	m_DataClearPtr=0;	
	m_TotalDataCount = 0;
	m_RoleIDSeed = 1;

	m_PlayerInfoPool.SetTag(_T("CPlayerDBAdapter"));
	m_PacketBuffer1.SetTag(_T("CPlayerDBAdapter"));
	m_PacketBuffer2.SetTag(_T("CPlayerDBAdapter"));

	m_ESVariableList.Create(32);
	m_ESFunctionList.Create(128);
	CESFunctionLib::GetInstance()->AddFunction(&m_ESFunctionList);
	m_ESThread.SetVariableList(&m_ESVariableList);
	m_ESThread.SetFunctionList(&m_ESFunctionList);
	m_ESThread.SetScript(&m_Script);

	m_ESFunctionList.AddCFunction("ClearPlayerDataCache", 0, this, &CPlayerDBAdapter::ESFClearPlayerDataCache);
}

CPlayerDBAdapter::~CPlayerDBAdapter(void)
{
}

BOOL CPlayerDBAdapter::Initialize(IDistributedObjectOperator * pOperator)
{
	m_pOperator=pOperator;

	

	if(m_PlayerInfoPool.Create(CMainConfig::GetInstance()->GetPlayerPoolConfig()))
	{
		LogDebug("玩家信息对象池(%u,%u,%u)已创建",
			CMainConfig::GetInstance()->GetPlayerPoolConfig().StartSize,
			CMainConfig::GetInstance()->GetPlayerPoolConfig().GrowSize,
			CMainConfig::GetInstance()->GetPlayerPoolConfig().GrowLimit);
	}
	else
	{
		Log("玩家信息对象池(%u,%u,%u)创建失败",
			CMainConfig::GetInstance()->GetPlayerPoolConfig().StartSize,
			CMainConfig::GetInstance()->GetPlayerPoolConfig().GrowSize,
			CMainConfig::GetInstance()->GetPlayerPoolConfig().GrowLimit);
		return FALSE;
	}

	if (m_RoleInfoPool.Create(CMainConfig::GetInstance()->GetRolePoolConfig()))
	{
		LogDebug("角色信息对象池(%u,%u,%u)已创建",
			CMainConfig::GetInstance()->GetRolePoolConfig().StartSize,
			CMainConfig::GetInstance()->GetRolePoolConfig().GrowSize,
			CMainConfig::GetInstance()->GetRolePoolConfig().GrowLimit);
	}
	else
	{
		Log("角色信息对象池(%u,%u,%u)创建失败",
			CMainConfig::GetInstance()->GetRolePoolConfig().StartSize,
			CMainConfig::GetInstance()->GetRolePoolConfig().GrowSize,
			CMainConfig::GetInstance()->GetRolePoolConfig().GrowLimit);
		return FALSE;
	}

	m_pDBTransManager = MONITORED_NEW(_T("CPlayerDBAdapter"), CDBTransationManager);
	m_pDBTransManager->SetName("PlayerDBAdapter");
	if (!m_pDBTransManager->Init(m_pDatabase,
		m_InstanceConfig.DBConfig.DBConnectString,
		m_InstanceConfig.DBConfig.ThreadCount,
		m_InstanceConfig.DBConfig.QueueSize,
		m_InstanceConfig.DBConfig.IsLogPerformance ? CDBTransationManager::DBTM_FLAG_LOG_PERFORMANCE : 0))
	{
		Log("数据库初始化失败");
	}

	m_PlayerSaveTimer.SaveTime();
	m_PlayerSavePtr=1;

	m_RoleSaveTimer.SaveTime();
	m_RoleSavePtr = 1;

	m_DataClearTimer.SaveTime();
	m_DataClearPtr=1;

	m_PacketBuffer1.Create(6553600);
	m_PacketBuffer2.Create(655360);


	CDBTransGetRoleIDSeed* pDBTrans = MONITORED_NEW(_T("PlayerDBAdapterDBTrans"), CDBTransGetRoleIDSeed, this);
	if (!m_pDBTransManager->AddTransaction(pDBTrans))	
	{
		Log("提交数据库事务CDBTransGetRoleIDSeed失败");
		SAFE_RELEASE(pDBTrans);
	}
	
	m_Status = STATUS_WORKING;
	Log("PlayerDBAdapter初始化成功");

	return TRUE;
}
void CPlayerDBAdapter::Destory()
{
	SAFE_RELEASE(m_pDBTransManager);
	m_pOperator=NULL;


}
UINT CPlayerDBAdapter::AddUseRef()
{
	return CNameObject::AddUseRef();
}
void CPlayerDBAdapter::Release()
{
	CNameObject::Release();
}

int CPlayerDBAdapter::Update(int ProcessPacketLimit)
{
	int ProcessCount=0;
	

	ProcessCount+=m_pDBTransManager->Update(m_InstanceConfig.DBConfig.RunLimit);
		
	if (m_Status == STATUS_WORKING)
	{
		if (m_PlayerSaveTimer.IsTimeOut(CMainConfig::GetInstance()->GetDataCacheConfig().DataSaveTime))
		{
			m_PlayerSaveTimer.SaveTime();
			int Factor = (int)CMainConfig::GetInstance()->GetDataCacheConfig().DataSaveFactor*CACHE_HIT_FACTOR;
			while (Factor > 0)
			{
				DB_PLAYER_INFO * pPlayerInfo = m_PlayerInfoPool.GetObject(m_PlayerSavePtr);
				if (pPlayerInfo)
				{
					ProcessCount++;
					if (pPlayerInfo->Status == DATA_STATUS_MODIFIED)
					{
						CDBTransSavePlayer * pDBTrans = MONITORED_NEW(_T("PlayerDBAdapterDBTrans"), CDBTransSavePlayer, this, 0, pPlayerInfo->PlayerInfo);
						if (m_pDBTransManager->AddTransaction(pDBTrans))
						{
							LogDebug("玩家[%lld]已提交保存", pPlayerInfo->AccountID);
							pPlayerInfo->Status = DATA_STATUS_SAVING;							
						}
						else
						{
							Log("提交数据库事务失败");
							SAFE_RELEASE(pDBTrans);
						}
					}
					m_PlayerSavePtr++;
					Factor -= CACHE_HIT_FACTOR;
				}
				else
				{
					m_PlayerSavePtr++;
					Factor -= CACHE_NOT_HIT_FACTOR;
				}
				if (m_PlayerSavePtr > m_PlayerInfoPool.GetBufferSize())
				{
					m_PlayerSavePtr = 1;
					break;
				}
			}
		}

		if (m_RoleSaveTimer.IsTimeOut(CMainConfig::GetInstance()->GetDataCacheConfig().DataSaveTime))
		{
			m_RoleSaveTimer.SaveTime();
			int Factor = (int)CMainConfig::GetInstance()->GetDataCacheConfig().DataSaveFactor * CACHE_HIT_FACTOR;
			while (Factor > 0)
			{
				DB_ROLE_INFO* pRoleInfo = m_RoleInfoPool.GetObject(m_RoleSavePtr);
				if (pRoleInfo)
				{
					ProcessCount++;
					if (pRoleInfo->Status == DATA_STATUS_MODIFIED)
					{
						CDBTransSaveRole* pDBTrans = MONITORED_NEW(_T("PlayerDBAdapterDBTrans"), CDBTransSaveRole, this, 0, pRoleInfo->RoleInfo);
						if (m_pDBTransManager->AddTransaction(pDBTrans))
						{
							LogDebug("角色0x[%llX]已提交保存", pRoleInfo->RoleID);
							pRoleInfo->Status = DATA_STATUS_SAVING;
						}
						else
						{
							Log("提交数据库事务失败");
							SAFE_RELEASE(pDBTrans);
						}
					}
					m_RoleSavePtr++;
					Factor -= CACHE_HIT_FACTOR;
				}
				else
				{
					m_RoleSavePtr++;
					Factor -= CACHE_NOT_HIT_FACTOR;
				}
				if (m_RoleSavePtr > m_RoleInfoPool.GetBufferSize())
				{
					m_RoleSavePtr = 1;
					break;
				}
			}
		}

		if (m_DataClearTimer.IsTimeOut(CMainConfig::GetInstance()->GetDataCacheConfig().DataClearTime))
		{
			m_DataClearTimer.SaveTime();
			int Factor = (int)CMainConfig::GetInstance()->GetDataCacheConfig().DataClearFactor*CACHE_HIT_FACTOR;
			while (Factor > 0)
			{
				DB_PLAYER_INFO * pPlayerInfo = m_PlayerInfoPool.GetObject(m_DataClearPtr);
				if (pPlayerInfo)
				{
					ProcessCount++;
					if (PlayerInfoCanClear(pPlayerInfo))
					{
						if (ClearPlayerInfo(pPlayerInfo->AccountID))
						{							
							LogDebug("玩家[0x%llX]已被清理", pPlayerInfo->AccountID);
						}
						else
						{
							LogDebug("玩家[0x%llX]清理失败", pPlayerInfo->AccountID);
						}
					}
					m_DataClearPtr++;
					Factor -= CACHE_HIT_FACTOR;
				}
				else
				{
					m_DataClearPtr++;
					Factor -= CACHE_NOT_HIT_FACTOR;
				}
				if (m_DataClearPtr >= m_PlayerInfoPool.GetBufferSize())
				{
					m_DataClearPtr = 1;
					break;
				}
			}
		}
	}
	else if (m_Status == STATUS_SHUTDOWN)
	{
		UINT SaveCount = 0;
		int Factor = (int)CMainConfig::GetInstance()->GetDataCacheConfig().DataSaveFactor*CACHE_HIT_FACTOR;
		while (Factor > 0)
		{
			DB_PLAYER_INFO * pPlayerInfo = m_PlayerInfoPool.GetObject(m_PlayerSavePtr);
			if (pPlayerInfo)
			{
				if (pPlayerInfo->Status == DATA_STATUS_MODIFIED)
				{
					CDBTransSavePlayer * pDBTrans = MONITORED_NEW(_T("PlayerDBAdapterDBTrans"), CDBTransSavePlayer, this, 0, pPlayerInfo->PlayerInfo);
					if (m_pDBTransManager->AddTransaction(pDBTrans))
					{
						LogDebug("玩家[%lld]已提交保存", pPlayerInfo->AccountID);
						pPlayerInfo->Status = DATA_STATUS_SAVING;
						SaveCount++;
						ProcessCount++;
					}
					else
					{
						Log("提交数据库事务失败");
						SAFE_RELEASE(pDBTrans);
					}
				}
				else if (pPlayerInfo->Status == DATA_STATUS_SAVING)
				{
					SaveCount++;
				}
				m_PlayerSavePtr++;
				Factor -= CACHE_HIT_FACTOR;
			}
			else
			{
				m_PlayerSavePtr++;
				Factor -= CACHE_NOT_HIT_FACTOR;
			}
			if (m_PlayerSavePtr > m_PlayerInfoPool.GetBufferSize())
			{
				m_PlayerSavePtr = 1;
				break;
			}
		}

		Factor = (int)CMainConfig::GetInstance()->GetDataCacheConfig().DataSaveFactor * CACHE_HIT_FACTOR;
		while (Factor > 0)
		{
			DB_ROLE_INFO* pRoleInfo = m_RoleInfoPool.GetObject(m_RoleSavePtr);
			if (pRoleInfo)
			{
				if (pRoleInfo->Status == DATA_STATUS_MODIFIED)
				{
					CDBTransSaveRole* pDBTrans = MONITORED_NEW(_T("PlayerDBAdapterDBTrans"), CDBTransSaveRole, this, 0, pRoleInfo->RoleInfo);
					if (m_pDBTransManager->AddTransaction(pDBTrans))
					{
						LogDebug("角色[%lld]已提交保存", pRoleInfo->RoleID);
						pRoleInfo->Status = DATA_STATUS_SAVING;
						SaveCount++;
						ProcessCount++;
					}
					else
					{
						Log("提交数据库事务失败");
						SAFE_RELEASE(pDBTrans);
					}
				}
				else if (pRoleInfo->Status == DATA_STATUS_SAVING)
				{
					SaveCount++;
				}
				m_RoleSavePtr++;
				Factor -= CACHE_HIT_FACTOR;
			}
			else
			{
				m_RoleSavePtr++;
				Factor -= CACHE_NOT_HIT_FACTOR;
			}
			if (m_RoleSavePtr > m_RoleInfoPool.GetBufferSize())
			{
				m_RoleSavePtr = 1;
				break;
			}
		}

		ProcessCount++;

		if (SaveCount == 0)
		{
			if (m_PlayerSaveTimer.IsTimeOut(SHUTDOWN_CHECK_TIME))
			{
				m_PlayerSaveTimer.SaveTime();
				Log("已无数据需要保存");
				if (m_pDBTransManager->IsIdle())
				{
					Log("已无需要处理的数据库事务,退出");
					m_pOperator->Release();
				}
				else
				{
					Log("还有数据库事务处理中");
				}
			}
		}
		else
		{
			Log("已保存%u", SaveCount);
		}
	}

	return ProcessCount;
}



void CPlayerDBAdapter::OnShutDown(BYTE Level, UINT Param)
{
	Log("Level=%u,Type=%u", Level, Param);
	if(Level==SYSTEM_SHUTDOWN_LEVEL2)
	{
		bool NeedShutdown = false;
		if (CMainConfig::GetInstance()->IsCoreShutdown())
		{
			if (Param&SYSTEM_SHUTDOWN_TYPE_CORE)
			{
				Log("请求核心关闭");
				NeedShutdown = true;
			}
		}
		else
		{
			if (Param&SYSTEM_SHUTDOWN_TYPE_PLAYER_DB_ADAPTER)
			{
				Log("请求关闭PlayerDBAdapter模块");
				NeedShutdown = true;
			}
		}
		if (NeedShutdown)
		{
			Log("系统准备关闭");

			m_Status = STATUS_SHUTDOWN;
		}
	}
}

BOOL CPlayerDBAdapter::OnFindObject(OBJECT_ID CallerID)
{
	UINT HashMask = m_InstanceConfig.HashMask;
	return m_pOperator->ReportObject(CallerID, &HashMask, sizeof(HashMask));
}

DB_PLAYER_INFO * CPlayerDBAdapter::FindPlayerInfo(UINT64 AccountID)
{
	return m_PlayerInfoPool.Find(AccountID);
}



DB_PLAYER_INFO * CPlayerDBAdapter::CreatePlayerInfo(UINT64 AccountID)
{
	DB_PLAYER_INFO * pPlayerInfo=NULL;
	m_PlayerInfoPool.New(AccountID, &pPlayerInfo);
	if(pPlayerInfo)
	{
		pPlayerInfo->Clear();
		pPlayerInfo->AccountID = AccountID;
	}
	else
	{
		Log("Alloc Error %u/%u",
			m_PlayerInfoPool.GetObjectCount(),m_PlayerInfoPool.GetBufferSize());
	}
	return pPlayerInfo;
}

bool CPlayerDBAdapter::DeletePlayerInfo(UINT64 AccountID)
{
	return m_PlayerInfoPool.Delete(AccountID);
}
bool CPlayerDBAdapter::ClearPlayerInfo(UINT64 AccountID)
{
	DB_PLAYER_INFO* pPlayerInfo = FindPlayerInfo(AccountID);
	if(pPlayerInfo)
	{
		for (UINT64 RoleID : pPlayerInfo->RoleList)
		{
			DeleteRoleInfo(RoleID);
		}
		return DeletePlayerInfo(pPlayerInfo->AccountID);
	}
	return false;
}
void CPlayerDBAdapter::ClearIdlePlayerInfo()
{
	Log("开始清理空闲的玩家数据缓冲");
	UINT Count = 0;
	void * Pos = m_PlayerInfoPool.GetFirstObjectPos();
	while (Pos)
	{
		DB_PLAYER_INFO * pPlayerInfo = m_PlayerInfoPool.GetNextObject(Pos);
		if (PlayerInfoCanClear(pPlayerInfo))
		{
			DeletePlayerInfo(pPlayerInfo->AccountID);
			Count++;
		}
	}
	Log("已清理%u个玩家数据缓冲,还有%u个数据", Count, m_PlayerInfoPool.GetObjectCount());
}

bool CPlayerDBAdapter::PlayerInfoCanClear(DB_PLAYER_INFO * pPlayerInfo)
{
	if (pPlayerInfo->StatusTimer.GetPastTime() >= CMainConfig::GetInstance()->GetDataCacheConfig().MaxDataIdleTime)
	{
		if (pPlayerInfo->Status == DATA_STATUS_MODIFIED || pPlayerInfo->Status == DATA_STATUS_SAVING)
			return false;
		for (UINT64 RoleID : pPlayerInfo->RoleList)
		{
			DB_ROLE_INFO* pRoleInfo = FindRoleInfo(RoleID);
			if (pRoleInfo)
			{
				if (pRoleInfo->Status == DATA_STATUS_MODIFIED || pRoleInfo->Status == DATA_STATUS_SAVING)
					return false;
			}
			else
			{
				Log("角色[0x%llX]不存在", RoleID);
			}
		}
		return true;
	}
	return false;
}

DB_ROLE_INFO* CPlayerDBAdapter::FindRoleInfo(UINT64 RoleID)
{
	return m_RoleInfoPool.Find(RoleID);
}
DB_ROLE_INFO* CPlayerDBAdapter::CreateRoleInfo(UINT64 RoleID)
{
	DB_ROLE_INFO* pRoleInfo = NULL;
	m_RoleInfoPool.New(RoleID, &pRoleInfo);
	if (pRoleInfo)
	{
		pRoleInfo->Clear();
		pRoleInfo->RoleID = RoleID;
	}
	else
	{
		Log("Alloc Error %u/%u",
			m_RoleInfoPool.GetObjectCount(), m_RoleInfoPool.GetBufferSize());
	}
	return pRoleInfo;
}
bool CPlayerDBAdapter::DeleteRoleInfo(UINT64 RoleID)
{
	return m_RoleInfoPool.Delete(RoleID);
}


CSmartStruct CPlayerDBAdapter::PackPlayerInfo(DB_PLAYER_INFO * pPlayerInfo, BYTE QueryType)
{
	if (QueryType == PLAYER_DATA_FETCH_TYPE_SELF)
	{
		return pPlayerInfo->PlayerInfo;
	}
	else
	{
		CPlayerInfo PlayerInfo;
		m_PacketBuffer1.Clear();
		m_PacketBuffer2.Clear();
		switch (QueryType)
		{		
		case PLAYER_DATA_FETCH_TYPE_OTHER:
			PlayerInfo.ParsePacket(pPlayerInfo->PlayerInfo, DOMF_PLAYER_INFO_FOR_OTHER);
			PlayerInfo.MakePacket(m_PacketBuffer1, DOMF_PLAYER_INFO_FOR_OTHER);
			break;		
		case PLAYER_DATA_FETCH_TYPE_FIND_ROLE:
			PlayerInfo.ParsePacket(pPlayerInfo->PlayerInfo, DOMF_PLAYER_INFO_FOR_FIND_ROLE);
			PlayerInfo.MakePacket(m_PacketBuffer1, DOMF_PLAYER_INFO_FOR_FIND_ROLE);
			break;
		case PLAYER_DATA_FETCH_TYPE_SIMPLE:
			PlayerInfo.ParsePacket(pPlayerInfo->PlayerInfo, DOMF_PLAYER_INFO_FOR_SIMPLE);
			PlayerInfo.MakePacket(m_PacketBuffer1, DOMF_PLAYER_INFO_FOR_SIMPLE);
			break;
		}
		return m_PacketBuffer1;		
	}	
}


bool CPlayerDBAdapter::UnpackPlayerInfo(const CSmartStruct& Packet,CPlayerInfo& PlayerInfo,bool IsCreate)
{
	if(IsCreate)
		PlayerInfo.ParsePacket(Packet,DOMF_PLAYER_INFO_FOR_CREATE);
	else
		PlayerInfo.ParsePacket(Packet,DOMF_PLAYER_INFO_FOR_SAVE);
	return true;
}

CSmartStruct CPlayerDBAdapter::PackRoleList(DB_PLAYER_INFO* pPlayerInfo)
{
	m_PacketBuffer1.Clear();
	CRoleInfo RoleInfo;
	CSmartArray SubPacket = m_PacketBuffer1.PrepareSubArray();
	for (UINT64 RoleID : pPlayerInfo->RoleList)
	{
		DB_ROLE_INFO* pRoleInfo = FindRoleInfo(RoleID);
		if (pRoleInfo)
		{
			RoleInfo.ParsePacket(pRoleInfo->RoleInfo, DOMF_ROLE_INFO_LIST_FOR_GET);
			CSmartStruct InfoPacket = SubPacket.PrepareSubStruct();
			RoleInfo.MakePacket(InfoPacket, DOMF_ROLE_INFO_LIST_FOR_GET);
			SubPacket.FinishMember(InfoPacket.GetDataLen());
		}
		else
		{
			Log("无法找到角色信息[0x%llX]", RoleID);
		}
	}
	m_PacketBuffer1.FinishMember(CRoleList::SST_RIL_LIST, SubPacket.GetDataLen());
	return m_PacketBuffer1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CPlayerDBAdapter::ESFClearPlayerDataCache(CESThread * pESThread, ES_BOLAN* pResult, ES_BOLAN* pParams, int ParamCount)
{
	CGMCommandAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), m_RecentGMCmdCaller);
	ClearIdlePlayerInfo();
	MsgCaller.ExecCommandAck(MSR_SUCCEED, m_RecentGMCmdCallerParam, "Player Data Cache Cleaned");
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CPlayerDBAdapter::DBCreatePlayer(UINT64 AccountID, const CSmartStruct& PlayerCreateInfo, const ACCOUNT_STATUS_INFO& AccountStatusInfo)
{
	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());

	
	UINT64 DataAccountID = PlayerCreateInfo.GetMember(CPlayerInfo::SST_PI_ACCOUNT_ID);
	if (DataAccountID == AccountID)
	{
		LogDebug("[0x%llX]请求创建玩家", AccountID);

		CDBTransCreatePlayer * pDBTrans = MONITORED_NEW(_T("PlayerDBAdapterDBTrans"), CDBTransCreatePlayer, this, GetCurMsgSenderID(), PlayerCreateInfo);
		if (!m_pDBTransManager->AddTransaction(pDBTrans))
		{
			Log("提交数据库事务失败");
			SAFE_RELEASE(pDBTrans);
			MsgCaller.DBCreatePlayerAck(COMMON_RESULT_FAILED, CSmartStruct());
		}
	}
	else
	{
		Log("账户ID不一致[0x%llX,0x%llX]", DataAccountID, AccountID);
		MsgCaller.DBCreatePlayerAck(COMMON_RESULT_FAILED, CSmartStruct());
	}
	
	

	return COMMON_RESULT_SUCCEED;
}
int CPlayerDBAdapter::DBDeletePlayer(UINT64 AccountID)
{
	LogDebug("请求删除玩家[%lld]", AccountID);

	bool NeedSave = false;
	DB_PLAYER_INFO* pPlayerInfo = FindPlayerInfo(AccountID);
	if (pPlayerInfo)
	{
		NeedSave = !PlayerInfoCanClear(pPlayerInfo);
	}
	if(!NeedSave)
	{
		CDBTransDeletePlayer* pDBTrans = MONITORED_NEW(_T("PlayerDBAdapterDBTrans"), CDBTransDeletePlayer, this, GetCurMsgSenderID(), AccountID);
		if (!m_pDBTransManager->AddTransaction(pDBTrans))
		{
			Log("提交数据库事务失败");
			SAFE_RELEASE(pDBTrans);

			CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());
			MsgCaller.DBDeletePlayerAck(MSR_SUCCEED, AccountID);
		}
	}
	else
	{
		CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());
		MsgCaller.DBDeletePlayerAck(MSR_DATA_NEED_SAVE, AccountID);
	}

	return COMMON_RESULT_SUCCEED;
}
int CPlayerDBAdapter::DBGetPlayerInfo(UINT64 AccountID, BYTE QueryType, const CSmartStruct& QueryParam)
{
	LogDebug("请求[0x%llX]玩家信息,QueryType=%u", AccountID, QueryType);
	

	DB_PLAYER_INFO * pPlayerInfo = FindPlayerInfo(AccountID);
	if (pPlayerInfo)
	{
		CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());

		CSmartStruct Packet = PackPlayerInfo(pPlayerInfo, QueryType);
		MsgCaller.DBGetPlayerInfoAck(MSR_SUCCEED, AccountID, QueryType, QueryParam, Packet);
			
	}
	else
	{
		CDBTransGetPlayerInfo * pDBTrans = MONITORED_NEW(_T("PlayerDBAdapterDBTrans"), CDBTransGetPlayerInfo, this, GetCurMsgSenderID(), AccountID, QueryType, QueryParam);
		if (!m_pDBTransManager->AddTransaction(pDBTrans))
		{
			Log("提交数据库事务失败");
			SAFE_RELEASE(pDBTrans);
			CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());
			MsgCaller.DBGetPlayerInfoAck(COMMON_RESULT_FAILED, AccountID, QueryType, QueryParam, CSmartStruct());
				
		}
	}
	return COMMON_RESULT_SUCCEED;
}
int CPlayerDBAdapter::DBSavePlayerInfo(UINT64 AccountID, const CSmartStruct& PlayerInfo)
{
	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());

	
	UINT64 DataAccountID = PlayerInfo.GetMember(CPlayerInfo::SST_PI_ACCOUNT_ID);
	
	if (AccountID == DataAccountID)
	{
		LogDebug("请求保存玩家[0x%llX]信息", AccountID);

		DB_PLAYER_INFO * pPlayerInfo = FindPlayerInfo(AccountID);
		if (pPlayerInfo)
		{
			pPlayerInfo->PlayerInfo.CloneFrom(PlayerInfo);
			pPlayerInfo->Status = DATA_STATUS_MODIFIED;
			pPlayerInfo->StatusTimer.SaveTime();
			MsgCaller.DBSavePlayerInfoAck(MSR_SUCCEED, AccountID);
		}
		else
		{
			Log("玩家[0x%llX]信息不存在,创建一份", AccountID);
			DB_PLAYER_INFO * pPlayerInfo = CreatePlayerInfo(AccountID);
			if (pPlayerInfo)
			{
				pPlayerInfo->PlayerInfo.CloneFrom(PlayerInfo);
				pPlayerInfo->Status = DATA_STATUS_MODIFIED;
				pPlayerInfo->StatusTimer.SaveTime();
				MsgCaller.DBSavePlayerInfoAck(MSR_SUCCEED, AccountID);
			}
			else
			{
				MsgCaller.DBSavePlayerInfoAck(COMMON_RESULT_FAILED, AccountID);
			}
		}
	}
	else
	{
		Log("玩家[0x%llX]AccountID不一致", AccountID);
	}
	
	return COMMON_RESULT_SUCCEED;
}


int CPlayerDBAdapter::DBClearDataCache(UINT64 AccountID, OBJECT_ID CallerID)
{
	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());

	DB_PLAYER_INFO * pPlayerInfo = FindPlayerInfo(AccountID);
	if (pPlayerInfo)
	{
		if (pPlayerInfo->Status == DATA_STATUS_READY)
		{
			ClearPlayerInfo(pPlayerInfo->AccountID);
		}

		MsgCaller.DBClearDataCacheAck(MSR_SUCCEED, CallerID);
	}
	else
	{
		MsgCaller.DBClearDataCacheAck(COMMON_RESULT_FAILED, CallerID);
	}
	return COMMON_RESULT_SUCCEED;
}
int CPlayerDBAdapter::DBPlayerSearch(const CEasyString& NickName, OBJECT_ID CallerID)
{
	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());

	CDBTransPlayerSearch * pDBTrans = MONITORED_NEW(_T("PlayerDBAdapterDBTrans"), CDBTransPlayerSearch, this, GetCurMsgSenderID(), CallerID, NickName);
	if (!m_pDBTransManager->AddTransaction(pDBTrans))
	{
		Log("提交数据库事务失败");
		SAFE_RELEASE(pDBTrans);
		MsgCaller.DBPlayerSearchAck(COMMON_RESULT_FAILED, CallerID, CSmartStruct());
	}
	return COMMON_RESULT_SUCCEED;
}
int CPlayerDBAdapter::DBSerachPlayerByTID(const CEasyString& TID, OBJECT_ID CallerID)
{
	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());

	CDBTransSearchPlayerByTID * pDBTrans = MONITORED_NEW(_T("PlayerDBAdapterDBTrans"), CDBTransSearchPlayerByTID, this, GetCurMsgSenderID(), CallerID, TID);
	if (!m_pDBTransManager->AddTransaction(pDBTrans))
	{
		Log("提交数据库事务失败");
		SAFE_RELEASE(pDBTrans);
		MsgCaller.DBSerachPlayerByTIDAck(COMMON_RESULT_FAILED, CallerID, CSmartStruct());
	}
	return COMMON_RESULT_SUCCEED;
}

int CPlayerDBAdapter::DBGetPlayerCount(OBJECT_ID CallerID)
{
	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());

	CDBTransGetPlayerCount * pDBTrans = MONITORED_NEW(_T("PlayerDBAdapterDBTrans"), CDBTransGetPlayerCount, this, GetCurMsgSenderID(), CallerID);
	if (!m_pDBTransManager->AddTransaction(pDBTrans))
	{
		Log("提交数据库事务失败");
		SAFE_RELEASE(pDBTrans);
		MsgCaller.DBGetPlayerCountAck(COMMON_RESULT_FAILED, 0, CallerID);
	}
	return COMMON_RESULT_SUCCEED;
}

int CPlayerDBAdapter::DBGetPlayerIDByTID(const CEasyString& TID, const CSmartStruct& Param)
{
	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());

	CDBTransGetIDByTID * pDBTrans = MONITORED_NEW(_T("PlayerDBAdapterDBTrans"), CDBTransGetIDByTID, this, GetCurMsgSenderID(), TID, Param);
	if (!m_pDBTransManager->AddTransaction(pDBTrans))
	{
		Log("提交数据库事务失败");
		SAFE_RELEASE(pDBTrans);
		MsgCaller.DBGetPlayerIDByTIDAck(COMMON_RESULT_FAILED, 0, Param);
	}
	return COMMON_RESULT_SUCCEED;
}
int CPlayerDBAdapter::DBGetRoleList(UINT64 PlayerID)
{
	LogDebug("请求[0x%llX]角色列表", PlayerID);

	DB_PLAYER_INFO* pPlayerInfo = FindPlayerInfo(PlayerID);
	if (pPlayerInfo)
	{
		if(pPlayerInfo->IsRoleFetched)
		{
			CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());

			CSmartStruct Packet = PackRoleList(pPlayerInfo);
			MsgCaller.DBGetRoleListAck(MSR_SUCCEED, PlayerID, Packet);
		}
		else
		{
			CDBTransLoadRoleList* pDBTrans = MONITORED_NEW(_T("PlayerDBAdapterDBTrans"), CDBTransLoadRoleList, this, GetCurMsgSenderID(), PlayerID);
			if (!m_pDBTransManager->AddTransaction(pDBTrans))
			{
				Log("提交数据库事务失败");
				SAFE_RELEASE(pDBTrans);
				CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());
				MsgCaller.DBGetRoleListAck(COMMON_RESULT_FAILED, PlayerID, CSmartStruct());

			}
		}
	}
	else
	{
		CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());
		MsgCaller.DBGetRoleListAck(MSR_PLAYER_DATA_NOT_EXIST, PlayerID, CSmartStruct());
	}
	return COMMON_RESULT_SUCCEED;
}
int CPlayerDBAdapter::DBCreateRole(UINT64 PlayerID, const CSmartStruct& RoleInfo)
{
	LogDebug("请求[0x%llX]创建角色", PlayerID);

	if (RoleInfo.IsMemberExist(CBaseCharInfo::SST_MII_OBJECT_ID))
	{
		UINT64 RoleID = MAKE_OBJECT_ID(OBJECT_ID_ROLE_ID, CMainConfig::GetInstance()->GetServerID(), m_RoleIDSeed);
		m_RoleIDSeed++;
		RoleInfo.GetMember(CBaseCharInfo::SST_MII_OBJECT_ID).SetValue(RoleID);		

		CDBTransCreateRole* pDBTrans = MONITORED_NEW(_T("PlayerDBAdapterDBTrans"), CDBTransCreateRole, this, GetCurMsgSenderID(), PlayerID, RoleID, RoleInfo);
		if (!m_pDBTransManager->AddTransaction(pDBTrans))
		{
			Log("提交数据库事务失败");
			SAFE_RELEASE(pDBTrans);
			CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());
			MsgCaller.DBCreateRoleAck(COMMON_RESULT_FAILED, CSmartStruct());

		}
	}
	else
	{
		CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());
		MsgCaller.DBCreateRoleAck(MSR_INVALID_PARAM, CSmartStruct());
	}
	return COMMON_RESULT_SUCCEED;
}
int CPlayerDBAdapter::DBRemoveRole(UINT64 PlayerID, UINT64 RoleID)
{
	LogDebug("请求[0x%llX]删除角色[0x%llX]", PlayerID, RoleID);
	CDBTransRemoveRole* pDBTrans = MONITORED_NEW(_T("PlayerDBAdapterDBTrans"), CDBTransRemoveRole, this, GetCurMsgSenderID(), PlayerID, RoleID);
	if (!m_pDBTransManager->AddTransaction(pDBTrans))
	{
		Log("提交数据库事务失败");
		SAFE_RELEASE(pDBTrans);
		CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());
		MsgCaller.DBRemoveRoleAck(COMMON_RESULT_FAILED, RoleID);

	}
	return COMMON_RESULT_SUCCEED;
}
int CPlayerDBAdapter::DBChangeRoleName(UINT64 PlayerID, UINT64 RoleID, const CEasyString& RoleName)
{
	LogDebug("[0x%llX]请求修改角色[0x%llX]名", PlayerID, RoleID);

	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());

	CDBTransChangeRoleName* pDBTrans = new CDBTransChangeRoleName(this, GetCurMsgSenderID(), RoleName, RoleID);
	if (!m_pDBTransManager->AddTransaction(pDBTrans))
	{
		Log("提交数据库事务失败");
		SAFE_RELEASE(pDBTrans);
		MsgCaller.DBChangeRoleNameAck(COMMON_RESULT_FAILED, RoleID, RoleName);
	}
	return COMMON_RESULT_SUCCEED;
}
int CPlayerDBAdapter::DBGetRoleInfo(UINT64 PlayerID, UINT64 RoleID)
{
	LogDebug("请求[0x%llX]角色数据[0x%llX]", PlayerID, RoleID);
	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());

	DB_PLAYER_INFO* pPlayerInfo = FindPlayerInfo(PlayerID);
	if (pPlayerInfo)
	{
		DB_ROLE_INFO* pRoleInfo = FindRoleInfo(RoleID);
		if (pRoleInfo)
		{
			if (pRoleInfo->AccountID == PlayerID)
			{
				MsgCaller.DBGetRoleInfoAck(MSR_SUCCEED, pRoleInfo->RoleInfo);
			}
			else
			{
				MsgCaller.DBGetRoleInfoAck(MSR_INVALID_ID, CSmartStruct());
			}
		}
		else
		{
			MsgCaller.DBGetRoleInfoAck(MSR_ROLE_NOT_EXIST, CSmartStruct());
		}
	}
	else
	{
		MsgCaller.DBGetRoleInfoAck(MSR_PLAYER_DATA_NOT_EXIST, CSmartStruct());
	}
	return COMMON_RESULT_SUCCEED;
}
int CPlayerDBAdapter::DBSaveRole(UINT64 PlayerID, const CSmartStruct& RoleInfo)
{
	UINT64 RoleID = RoleInfo.GetMember(CBaseCharInfo::SST_MII_OBJECT_ID);
	LogDebug("保存[0x%llX]角色数据[0x%llX]", PlayerID, RoleID);
	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());

	DB_PLAYER_INFO* pPlayerInfo = FindPlayerInfo(PlayerID);
	if (pPlayerInfo)
	{
		DB_ROLE_INFO* pRoleInfo = FindRoleInfo(RoleID);
		if (pRoleInfo)
{
			if (pRoleInfo->AccountID == PlayerID)
			{
				pRoleInfo->RoleInfo.CloneFrom(RoleInfo);
				pRoleInfo->Status = DATA_STATUS_MODIFIED;
				MsgCaller.DBSaveRoleAck(MSR_SUCCEED, RoleID);
			}
			else
			{
				MsgCaller.DBSaveRoleAck(MSR_INVALID_ID, RoleID);
			}
		}
		else
		{
			MsgCaller.DBSaveRoleAck(MSR_ROLE_NOT_EXIST, RoleID);
		}
	}
	else
	{
		MsgCaller.DBSaveRoleAck(MSR_PLAYER_DATA_NOT_EXIST, RoleID);
	}
	return COMMON_RESULT_SUCCEED;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CPlayerDBAdapter::ExecCommand(OBJECT_ID PlayerProxyObjectID, const CEasyString& CmdStr)
{
	CGMCommandAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());



	int RetCode;
	ES_BOLAN Result;

	Log("执行命令:%s", (LPCTSTR)CmdStr);

	m_RecentGMCmdCaller = GetCurMsgSenderID();
	m_RecentGMCmdCallerParam = PlayerProxyObjectID;

	RetCode = m_ESThread.PushScript(CmdStr);
	CEasyString ResultMsg;
	if (RetCode)
	{
		ResultMsg.Format("解析命令出错:Line=%d,%s",
			m_ESThread.GetLastLine(),
			ESGetErrorMsg(RetCode));
		Log(ResultMsg);
		MsgCaller.ExecCommandAck(COMMON_RESULT_FAILED, m_RecentGMCmdCallerParam, ResultMsg);

	}
	else
	{
		RetCode = m_ScriptExecutor.ExecScript(m_ESThread);
		if (RetCode)
		{
			ResultMsg.Format("执行命令出错:Line=%d,%s",
				m_ESThread.GetLastLine(),
				ESGetErrorMsg(RetCode));
			Log(ResultMsg);
			MsgCaller.ExecCommandAck(COMMON_RESULT_FAILED, m_RecentGMCmdCallerParam, ResultMsg);
		}		
	}

	return COMMON_RESULT_SUCCEED;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPlayerDBAdapter::OnDBRetGetPlayerInfo(int Result, OBJECT_ID CallerID, BYTE QueryType, CSmartStruct& QueryParam, UINT64 AccountID, CSmartStruct& PlayerInfo)
{
	LogDebug("AccountID=0x%llX,Result=%d,QueryType=%d",
		AccountID, Result, QueryType);
	
	if (Result == MSR_SUCCEED)
	{
		DB_PLAYER_INFO * pPlayerInfo = FindPlayerInfo(AccountID);
		if (pPlayerInfo == NULL)
		{
			pPlayerInfo = CreatePlayerInfo(AccountID);
			if (pPlayerInfo)
			{
				pPlayerInfo->PlayerInfo.CloneFrom(PlayerInfo);
				pPlayerInfo->Status = DATA_STATUS_READY;
				pPlayerInfo->StatusTimer.SaveTime();
			}
		}
		
		if(pPlayerInfo)
		{
			CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), CallerID);

			CSmartStruct Packet = PackPlayerInfo(pPlayerInfo, QueryType);

			MsgCaller.DBGetPlayerInfoAck(MSR_SUCCEED, AccountID, QueryType, QueryParam, Packet);
				
		}
		else
		{
			Log("玩家[0x%llX]创建失败", AccountID);
			
			CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), CallerID);
			MsgCaller.DBGetPlayerInfoAck(COMMON_RESULT_FAILED, AccountID, QueryType, QueryParam, CSmartStruct());				
		}
	}
	else
	{
		CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), CallerID);
		MsgCaller.DBGetPlayerInfoAck(Result, AccountID, QueryType, QueryParam, CSmartStruct());
	}
}



void CPlayerDBAdapter::OnDBRetCreatePlayer(int Result, OBJECT_ID CallerID, UINT64 AccountID, CSmartStruct& PlayerInfo)
{
	LogDebug("OnDBRetCreateChar:AccountID=0x%llX,Result=%d",
		AccountID, Result);
	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), CallerID);
	if (Result == MSR_SUCCEED)
	{
		DB_PLAYER_INFO * pPlayerInfo = CreatePlayerInfo(AccountID);
		if (pPlayerInfo)
		{
			pPlayerInfo->AccountID = AccountID;
			pPlayerInfo->PlayerInfo.CloneFrom(PlayerInfo);
			pPlayerInfo->Status = DATA_STATUS_READY;
			pPlayerInfo->StatusTimer.SaveTime();
			m_PacketBuffer1.Clear();
			
			MsgCaller.DBCreatePlayerAck(MSR_SUCCEED, pPlayerInfo->PlayerInfo);
			
		}
		else
		{
			Log("分配玩家空间失败");
			MsgCaller.DBCreatePlayerAck(COMMON_RESULT_FAILED, CSmartStruct());
		}
	}
	else
	{
		MsgCaller.DBCreatePlayerAck(Result, CSmartStruct());
	}
}

void CPlayerDBAdapter::OnDBRetDeletePlayer(bool IsSucceed, OBJECT_ID CallerID, UINT64 AccountID)
{
	LogDebug("AccountID=0x%llX,Result=%s", AccountID, IsSucceed ? "成功" : "失败");
	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator,m_pOperator->GetObjectID(),CallerID);
	if(IsSucceed)
	{
		ClearPlayerInfo(AccountID);
		MsgCaller.DBDeletePlayerAck(MSR_SUCCEED, AccountID);
	}
	else
	{
		MsgCaller.DBDeletePlayerAck(COMMON_RESULT_FAILED, AccountID);
	}
}


void CPlayerDBAdapter::OnDBRetSavePlayer(bool IsSucceed, OBJECT_ID CallerID, UINT64 AccountID)
{
	LogDebug("AccountID=0x%llX,Result=%s", AccountID, IsSucceed ? "成功" : "失败");
	if (IsSucceed)
	{
		DB_PLAYER_INFO * pPlayerInfo = FindPlayerInfo(AccountID);
		if (pPlayerInfo)
		{
			pPlayerInfo->Status = DATA_STATUS_READY;
		}
	}
}




void CPlayerDBAdapter::OnDBRetPlayerSearch(int Result, OBJECT_ID CallerID, OBJECT_ID Param, CSmartStruct& DataPacket)
{
	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), CallerID);
	MsgCaller.DBPlayerSearchAck(Result, Param, DataPacket);
	
}

void CPlayerDBAdapter::OnDBRetSearchPlayerByTID(int Result, OBJECT_ID CallerID, OBJECT_ID Param, CSmartStruct& DataPacket)
{
	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), CallerID);
	MsgCaller.DBSerachPlayerByTIDAck(Result, Param, DataPacket);
}

void CPlayerDBAdapter::OnDBRetPlayerSearch(int Result, OBJECT_ID CallerID, OBJECT_ID Param, UINT Count)
{
	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), CallerID);
	MsgCaller.DBGetPlayerCountAck(Result, Count, Param);
}

void CPlayerDBAdapter::OnDBRetGetIDByTID(int Result, OBJECT_ID CallerID, UINT64 PlayerID, const CSmartStruct& Param)
{
	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), CallerID);

	MsgCaller.DBGetPlayerIDByTIDAck(COMMON_RESULT_FAILED, PlayerID, Param);
}
void CPlayerDBAdapter::OnDBRetGetRoleIDSeed(UINT64 IDSeed)
{
	LogDebug("RoleIDSeed=0x%llX", IDSeed);
	m_RoleIDSeed = IDSeed;
}
void CPlayerDBAdapter::OnDBRetLoadRoleList(int Result, OBJECT_ID CallerID, UINT64 AccountID, CEasyArray<CSmartStruct>& RoleList)
{
	LogDebug("Result=%d,AccountID=0x%llX,RoleCount=%u", Result, AccountID, RoleList.GetCount());
	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), CallerID);

	if (Result == MSR_SUCCEED)
	{
		DB_PLAYER_INFO* pPlayerInfo = FindPlayerInfo(AccountID);
		if (pPlayerInfo)
		{
			pPlayerInfo->RoleList.Clear();
			pPlayerInfo->IsRoleFetched = true;
			for (CSmartStruct& Packet : RoleList)
			{
				UINT64 RoleID = Packet.GetMember(CBaseCharInfo::SST_MII_OBJECT_ID);
				if (RoleID)
				{
					DB_ROLE_INFO* pRoleInfo = CreateRoleInfo(RoleID);
					if(pRoleInfo)
					{
						pRoleInfo->AccountID = AccountID;
						pRoleInfo->RoleInfo.CloneFrom(Packet);
						pRoleInfo->Status = DATA_STATUS_READY;
						pRoleInfo->StatusTimer.SaveTime();
						pPlayerInfo->RoleList.Add(RoleID);
					}
					else
					{
						Log("AccountID=0x%llX, RoleInfo RoleID[0x%llX] CreateRoleInfo error",  AccountID, RoleID);
					}
				}			
				else
				{
					Log("AccountID=0x%llX, RoleInfo RoleID==0 error",  AccountID);
				}	
			}

			CSmartStruct Packet = PackRoleList(pPlayerInfo);
			MsgCaller.DBGetRoleListAck(MSR_SUCCEED, AccountID, Packet);
		}
		else
		{
			MsgCaller.DBGetRoleListAck(MSR_CALL_SEQUENCE_ERROR, AccountID, CSmartStruct());
		}
	}
	else
	{		
		MsgCaller.DBGetRoleListAck(COMMON_RESULT_FAILED, AccountID, CSmartStruct());
	}
}
void CPlayerDBAdapter::OnDBRetCreateRole(int Result, OBJECT_ID CallerID, UINT64 AccountID, UINT64 RoleID, CSmartStruct& RoleInfo)
{
	LogDebug("Result=%d,AccountID=0x%llX,RoleID=0x%llX", Result, AccountID, RoleID);
	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), CallerID);
	MsgCaller.DBCreateRoleAck(Result, RoleInfo);
	if (Result == MSR_SUCCEED)
	{
		DB_PLAYER_INFO* pPlayerInfo = FindPlayerInfo(AccountID);
		if (pPlayerInfo)
		{
			DB_ROLE_INFO* pRoleInfo = CreateRoleInfo(RoleID);
			if (pRoleInfo)
			{
				pRoleInfo->AccountID = AccountID;
				pRoleInfo->RoleInfo.CloneFrom(RoleInfo);
				pRoleInfo->Status = DATA_STATUS_READY;
				pRoleInfo->StatusTimer.SaveTime();
				pPlayerInfo->RoleList.Add(RoleID);
			}
		}
		else
		{
			Log("玩家数据[0x%llX]不存在", AccountID);
		}
	}
}
void CPlayerDBAdapter::OnDBRetRemoveRole(bool IsSucceed, OBJECT_ID CallerID, UINT64 AccountID, UINT64 RoleID)
{
	LogDebug("IsSucceed=%s,AccountID=0x%llX,RoleID=0x%llX", IsSucceed ? "true" : "false", AccountID, RoleID);
	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), CallerID);
	MsgCaller.DBRemoveRoleAck(IsSucceed ? MSR_SUCCEED : COMMON_RESULT_FAILED, RoleID);
	if (IsSucceed)
	{
		DB_PLAYER_INFO* pPlayerInfo = FindPlayerInfo(AccountID);
		if (pPlayerInfo)
		{
			DeleteRoleInfo(RoleID);
			RemoveFromList(pPlayerInfo->RoleList, RoleID);
		}
		else
		{
			Log("玩家数据[0x%llX]不存在", AccountID);
		}
	}
}
void CPlayerDBAdapter::OnDBRetSaveRole(bool IsSucceed, OBJECT_ID CallerID, UINT64 RoleID)
{
	LogDebug("IsSucceed=%s,AccountID=0x%llX,RoleID=0x%llX", IsSucceed ? "true" : "false", RoleID);
	if (IsSucceed)
	{
		DB_ROLE_INFO* pRoleInfo = FindRoleInfo(RoleID);
		if (pRoleInfo)
		{
			if (pRoleInfo->Status == DATA_STATUS_SAVING)
				pRoleInfo->Status = DATA_STATUS_READY;
		}
		else
		{
			Log("角色数据[0x%llX]不存在", RoleID);
		}
	}
}

void CPlayerDBAdapter::OnDBRetChangeRoleName(int Result, OBJECT_ID CallerID, UINT64 RoleID, LPCTSTR RoleName)
{
	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), CallerID);
	MsgCaller.DBChangeRoleNameAck(Result, RoleID, RoleName);
}