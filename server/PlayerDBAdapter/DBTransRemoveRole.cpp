#include "stdafx.h"

bool CDBTransRemoveRole::OnExecute(IDBConnection* pConnection)
{
	char szSQL[1024];
	int DBResult;




	sprintf_s(szSQL, 1024, "update t_role_info set ManageStatus=1,BackupRoleName=RoleName,RoleName=NULL "
		"where AccountID=%lld and RoleID=%lld and ManageStatus<>1 ",
		m_AccountID, m_RoleID);

	LogDebug("执行SQL:%s", szSQL);
	DBResult = pConnection->ExecuteSQL(szSQL, 0);

	if (DBResult != DBERR_SUCCEED)
	{
		Log("执行SQL出错:(%u)%s",
			pConnection->GetLastDatabaseErrorCode(),
			pConnection->GetLastDatabaseErrorString());
		Log("CDBTransRemoveRole::OnExecute:错的SQL:%s", szSQL);
		return false;
	}

	if (pConnection->GetAffectedRowCount() <= 0)
	{
		Log("执行未产生结果");
		return false;
	}

	return true;
}
void CDBTransRemoveRole::OnFinish()
{
	m_pParent->OnDBRetRemoveRole(m_IsSucceed, m_CallerID, m_AccountID, m_RoleID);
}
