#include "stdafx.h"



CPlayerDBAdapter::CPlayerDBAdapter(const INSTANCE_CONFIG& InstanceConfig, IDatabase * pDatabase)
{
	m_Status = STATUS_NONE;
	m_InstanceConfig = InstanceConfig;
	m_pDatabase = pDatabase;
	m_pDBTransManager=NULL;
	m_PlayerSavePtr=0;
	m_RoleSavePtr = 0;
	m_DataClearPtr=0;	
	m_TotalDataCount = 0;
	m_RoleIDSeed = 1;

}

CPlayerDBAdapter::~CPlayerDBAdapter(void)
{
}

BOOL CPlayerDBAdapter::Initialize()
{
	m_pDBTransManager = MONITORED_NEW(_T("CPlayerDBAdapter"), CDBTransationManager);
	if (!m_pDBTransManager->Init(m_pDatabase,
		m_InstanceConfig.DBConfig.DBConnectString,
		m_InstanceConfig.DBConfig.ThreadCount,
		m_InstanceConfig.DBConfig.QueueSize,
		m_InstanceConfig.DBConfig.IsLogPerformance ? CDBTransationManager::DBTM_FLAG_LOG_PERFORMANCE : 0))
	{
		//Log("数据库初始化失败"); //todo error
	}

	m_PlayerSaveTimer.SaveTime();
	m_PlayerSavePtr=1;

	m_RoleSaveTimer.SaveTime();
	m_RoleSavePtr = 1;

	m_DataClearTimer.SaveTime();
	m_DataClearPtr=1;

	m_PacketBuffer1.Create(6553600);
	m_PacketBuffer2.Create(655360);


	//CDBTransGetRoleIDSeed* pDBTrans = MONITORED_NEW(_T("PlayerDBAdapterDBTrans"), CDBTransGetRoleIDSeed, this);
	//if (!m_pDBTransManager->AddTransaction(pDBTrans))	
	//{
	//	Log("提交数据库事务CDBTransGetRoleIDSeed失败");
	//	SAFE_RELEASE(pDBTrans);
	//}
	//
	//m_Status = STATUS_WORKING;
	//Log("PlayerDBAdapter初始化成功");

	return TRUE;
}
void CPlayerDBAdapter::Destory()
{
	SAFE_RELEASE(m_pDBTransManager);
}
UINT CPlayerDBAdapter::AddUseRef()
{

	return 0;
	//return CNameObject::AddUseRef();
}
void CPlayerDBAdapter::Release()
{
	//CNameObject::Release();
}

int CPlayerDBAdapter::Update(int ProcessPacketLimit)
{
	int ProcessCount=0;
	

	ProcessCount+=m_pDBTransManager->Update(m_InstanceConfig.DBConfig.RunLimit);
		
	if (m_Status == STATUS_WORKING)
	{

	}
	else if (m_Status == STATUS_SHUTDOWN)
	{
	}
	return ProcessCount;
}



void CPlayerDBAdapter::OnShutDown(BYTE Level, UINT Param)
{
	//Log("Level=%u,Type=%u", Level, Param);
}

BOOL CPlayerDBAdapter::OnFindObject(UINT64 CallerID)
{
return false;
}

DB_PLAYER_INFO * CPlayerDBAdapter::FindPlayerInfo(UINT64 AccountID)
{
	//return m_PlayerInfoPool.Find(AccountID);
	return nullptr;
}



DB_PLAYER_INFO * CPlayerDBAdapter::CreatePlayerInfo(UINT64 AccountID)
{
	return nullptr;
	//DB_PLAYER_INFO * pPlayerInfo=NULL;
	//m_PlayerInfoPool.New(AccountID, &pPlayerInfo);
	//if(pPlayerInfo)
	//{
	//	pPlayerInfo->Clear();
	//	pPlayerInfo->AccountID = AccountID;
	//}
	//else
	//{
	//	Log("Alloc Error %u/%u",
	//		m_PlayerInfoPool.GetObjectCount(),m_PlayerInfoPool.GetBufferSize());
	//}
	//return pPlayerInfo;
}

bool CPlayerDBAdapter::DeletePlayerInfo(UINT64 AccountID)
{
	return false;
	//return m_PlayerInfoPool.Delete(AccountID);
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
{}

bool CPlayerDBAdapter::PlayerInfoCanClear(DB_PLAYER_INFO * pPlayerInfo)
{
	return false;
}

DB_ROLE_INFO* CPlayerDBAdapter::FindRoleInfo(UINT64 RoleID)
{
	return nullptr;
	//return m_RoleInfoPool.Find(RoleID);
}
DB_ROLE_INFO* CPlayerDBAdapter::CreateRoleInfo(UINT64 RoleID)
{
	return nullptr;
	//DB_ROLE_INFO* pRoleInfo = NULL;
	//m_RoleInfoPool.New(RoleID, &pRoleInfo);
	//if (pRoleInfo)
	//{
	//	pRoleInfo->Clear();
	//	pRoleInfo->RoleID = RoleID;
	//}
	//else
	//{
	//	Log("Alloc Error %u/%u",
	//		m_RoleInfoPool.GetObjectCount(), m_RoleInfoPool.GetBufferSize());
	//}
	//return pRoleInfo;
}
bool CPlayerDBAdapter::DeleteRoleInfo(UINT64 RoleID)
{
	return false;
//	return m_RoleInfoPool.Delete(RoleID);
}


CSmartStruct CPlayerDBAdapter::PackPlayerInfo(DB_PLAYER_INFO * pPlayerInfo, BYTE QueryType)
{
	//if (QueryType == PLAYER_DATA_FETCH_TYPE_SELF)
	//{
	//	return pPlayerInfo->PlayerInfo;
	//}
	//else
	//{
	//	CPlayerInfo PlayerInfo;
	//	m_PacketBuffer1.Clear();
	//	m_PacketBuffer2.Clear();
	//	switch (QueryType)
	//	{		
	//	case PLAYER_DATA_FETCH_TYPE_OTHER:
	//		PlayerInfo.ParsePacket(pPlayerInfo->PlayerInfo, DOMF_PLAYER_INFO_FOR_OTHER);
	//		PlayerInfo.MakePacket(m_PacketBuffer1, DOMF_PLAYER_INFO_FOR_OTHER);
	//		break;		
	//	case PLAYER_DATA_FETCH_TYPE_FIND_ROLE:
	//		PlayerInfo.ParsePacket(pPlayerInfo->PlayerInfo, DOMF_PLAYER_INFO_FOR_FIND_ROLE);
	//		PlayerInfo.MakePacket(m_PacketBuffer1, DOMF_PLAYER_INFO_FOR_FIND_ROLE);
	//		break;
	//	case PLAYER_DATA_FETCH_TYPE_SIMPLE:
	//		PlayerInfo.ParsePacket(pPlayerInfo->PlayerInfo, DOMF_PLAYER_INFO_FOR_SIMPLE);
	//		PlayerInfo.MakePacket(m_PacketBuffer1, DOMF_PLAYER_INFO_FOR_SIMPLE);
	//		break;
	//	}
	//	return m_PacketBuffer1;		
	//}	
	m_PacketBuffer1.Clear();
	return m_PacketBuffer1;		
}


//bool CPlayerDBAdapter::UnpackPlayerInfo(const CSmartStruct& Packet,CPlayerInfo& PlayerInfo,bool IsCreate)
//{
//	return true;
//}

CSmartStruct CPlayerDBAdapter::PackRoleList(DB_PLAYER_INFO* pPlayerInfo)
{
	//m_PacketBuffer1.Clear();
	//CRoleInfo RoleInfo;
	//CSmartArray SubPacket = m_PacketBuffer1.PrepareSubArray();
	//for (UINT64 RoleID : pPlayerInfo->RoleList)
	//{
	//	DB_ROLE_INFO* pRoleInfo = FindRoleInfo(RoleID);
	//	if (pRoleInfo)
	//	{
	//		RoleInfo.ParsePacket(pRoleInfo->RoleInfo, DOMF_ROLE_INFO_LIST_FOR_GET);
	//		CSmartStruct InfoPacket = SubPacket.PrepareSubStruct();
	//		RoleInfo.MakePacket(InfoPacket, DOMF_ROLE_INFO_LIST_FOR_GET);
	//		SubPacket.FinishMember(InfoPacket.GetDataLen());
	//	}
	//	else
	//	{
	//		Log("无法找到角色信息[0x%llX]", RoleID);
	//	}
	//}
	//m_PacketBuffer1.FinishMember(CRoleList::SST_RIL_LIST, SubPacket.GetDataLen());
	//return m_PacketBuffer1;
	return m_PacketBuffer1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//int CPlayerDBAdapter::DBCreatePlayer(UINT64 AccountID, const CSmartStruct& PlayerCreateInfo, const ACCOUNT_STATUS_INFO& AccountStatusInfo)
//{
//	CPlayerDBAdapterAckMsgCaller MsgCaller(m_pOperator, m_pOperator->GetObjectID(), GetCurMsgSenderID());
//
//	
//	UINT64 DataAccountID = PlayerCreateInfo.GetMember(CPlayerInfo::SST_PI_ACCOUNT_ID);
//	if (DataAccountID == AccountID)
//	{
//		LogDebug("[0x%llX]请求创建玩家", AccountID);
//
//		CDBTransCreatePlayer * pDBTrans = MONITORED_NEW(_T("PlayerDBAdapterDBTrans"), CDBTransCreatePlayer, this, GetCurMsgSenderID(), PlayerCreateInfo);
//		if (!m_pDBTransManager->AddTransaction(pDBTrans))
//		{
//			Log("提交数据库事务失败");
//			SAFE_RELEASE(pDBTrans);
//			MsgCaller.DBCreatePlayerAck(COMMON_RESULT_FAILED, CSmartStruct());
//		}
//	}
//	else
//	{
//		Log("账户ID不一致[0x%llX,0x%llX]", DataAccountID, AccountID);
//		MsgCaller.DBCreatePlayerAck(COMMON_RESULT_FAILED, CSmartStruct());
//	}
//	
//	
//
//	return COMMON_RESULT_SUCCEED;
//}
int CPlayerDBAdapter::DBDeletePlayer(UINT64 AccountID)
{
	return 0;
}
int CPlayerDBAdapter::DBGetPlayerInfo(UINT64 AccountID, BYTE QueryType, const CSmartStruct& QueryParam)
{
	return 0;
}
int CPlayerDBAdapter::DBSavePlayerInfo(UINT64 AccountID, const CSmartStruct& PlayerInfo)
{
	return 0;
}


int CPlayerDBAdapter::DBClearDataCache(UINT64 AccountID, UINT64 CallerID)
{
	return 0;
}
int CPlayerDBAdapter::DBPlayerSearch(const CEasyString& NickName, UINT64 CallerID)
{
	return 0;
}


int CPlayerDBAdapter::DBSerachPlayerByTID(const CEasyString& TID, UINT64 CallerID)
{

	return 0;
}



int CPlayerDBAdapter::DBGetPlayerCount(UINT64 CallerID)
{
	return 0;

}



int CPlayerDBAdapter::DBGetPlayerIDByTID(const CEasyString& TID, const CSmartStruct& Param)
{

	return 0;
}


int CPlayerDBAdapter::DBGetRoleList(UINT64 PlayerID)
{

	return 0;
}


int CPlayerDBAdapter::DBCreateRole(UINT64 PlayerID, const CSmartStruct& RoleInfo)
{

	return 0;
}


int CPlayerDBAdapter::DBRemoveRole(UINT64 PlayerID, UINT64 RoleID)
{

	return 0;
}


int CPlayerDBAdapter::DBChangeRoleName(UINT64 PlayerID, UINT64 RoleID, const CEasyString& RoleName)
{

return 0;
}


int CPlayerDBAdapter::DBGetRoleInfo(UINT64 PlayerID, UINT64 RoleID)
{

return 0;
}


int CPlayerDBAdapter::DBSaveRole(UINT64 PlayerID, const CSmartStruct& RoleInfo)
{

return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CPlayerDBAdapter::ExecCommand(UINT64 PlayerProxyObjectID, const CEasyString& CmdStr)
{

return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPlayerDBAdapter::OnDBRetGetPlayerInfo(int Result, UINT64 CallerID, BYTE QueryType, CSmartStruct& QueryParam, UINT64 AccountID, CSmartStruct& PlayerInfo)
{

}





void CPlayerDBAdapter::OnDBRetCreatePlayer(int Result, UINT64 CallerID, UINT64 AccountID, CSmartStruct& PlayerInfo)
{

}



void CPlayerDBAdapter::OnDBRetDeletePlayer(bool IsSucceed, UINT64 CallerID, UINT64 AccountID)
{

}




void CPlayerDBAdapter::OnDBRetSavePlayer(bool IsSucceed, UINT64 CallerID, UINT64 AccountID)
{

}






void CPlayerDBAdapter::OnDBRetPlayerSearch(int Result, UINT64 CallerID, UINT64 Param, CSmartStruct& DataPacket)
{

}



void CPlayerDBAdapter::OnDBRetSearchPlayerByTID(int Result, UINT64 CallerID, UINT64 Param, CSmartStruct& DataPacket)
{

}



void CPlayerDBAdapter::OnDBRetPlayerSearch(int Result, UINT64 CallerID, UINT64 Param, UINT Count)
{

}



void CPlayerDBAdapter::OnDBRetGetIDByTID(int Result, UINT64 CallerID, UINT64 PlayerID, const CSmartStruct& Param)
{

}


void CPlayerDBAdapter::OnDBRetGetRoleIDSeed(UINT64 IDSeed)
{

}


void CPlayerDBAdapter::OnDBRetLoadRoleList(int Result, UINT64 CallerID, UINT64 AccountID, CEasyArray<CSmartStruct>& RoleList)
{

}


void CPlayerDBAdapter::OnDBRetCreateRole(int Result, UINT64 CallerID, UINT64 AccountID, UINT64 RoleID, CSmartStruct& RoleInfo)
{

}


void CPlayerDBAdapter::OnDBRetRemoveRole(bool IsSucceed, UINT64 CallerID, UINT64 AccountID, UINT64 RoleID)
{

}


void CPlayerDBAdapter::OnDBRetSaveRole(bool IsSucceed, UINT64 CallerID, UINT64 RoleID)
{

}



void CPlayerDBAdapter::OnDBRetChangeRoleName(int Result, UINT64 CallerID, UINT64 RoleID, LPCTSTR RoleName)
{

}

