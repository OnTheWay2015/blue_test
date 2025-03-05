#include "stdafx.h"


CDBTransDeletePlayer::CDBTransDeletePlayer(CPlayerDBAdapter * pParent, OBJECT_ID CallerID, UINT64 AccountID)
{
	m_pParent=pParent;
	m_CallerID=CallerID;
	m_AccountID = AccountID;
}

CDBTransDeletePlayer::~CDBTransDeletePlayer(void)
{
}

bool CDBTransDeletePlayer::OnExecute(IDBConnection * pConnection)
{
	char szSQL[1024];
	int DBResult;

	


	sprintf_s(szSQL,1024,"update t_player_info set ManageStatus=1 "
		"where AccountID=%lld and ManageStatus<>1 ",
		m_AccountID);

	LogDebug("CDBTransDeletePlayer::OnExecute:执行SQL:%s",szSQL);
	DBResult=pConnection->ExecuteSQL(szSQL,0);

	if(DBResult!=DBERR_SUCCEED)
	{
		Log("CDBTransDeletePlayer::OnExecute:执行SQL出错:(%u)%s",
			pConnection->GetLastDatabaseErrorCode(),
			pConnection->GetLastDatabaseErrorString());
		Log("CDBTransDeletePlayer::OnExecute:出错的SQL:%s",szSQL);
		return false;
	}

	if(pConnection->GetAffectedRowCount()<=0)
	{
		Log("CDBTransDeletePlayer::OnExecute:执行未产生结果");
		return false;
	}
	
	return true;
}
void CDBTransDeletePlayer::OnFinish()
{
	m_pParent->OnDBRetDeletePlayer(m_IsSucceed, m_CallerID, m_AccountID);
}
