#include "stdafx.h"

bool CDBTransGetRoleIDSeed::OnExecute(IDBConnection* pConnection)
{
	char szSQL[1024];
	CSmartPtr<IDBRecordSet> pRecordSet;
	int DBResult;

	pRecordSet = pConnection->CreateRecordSet(DB_RS_TYPE_DYNAMIC);
	if (pRecordSet == NULL)
	{
		Log("�޷����������");
		return false;
	}

	sprintf_s(szSQL, 1024, "select max(RoleID&0xFFFFFFFF) from t_role_info");

	LogDebug("ִ��SQL:%s", szSQL);
	DBResult = pConnection->ExecuteSQL(szSQL, 0);

	if (DBResult != DBERR_SUCCEED)
	{
		Log("ִ��SQL����:(%u)%s",
			pConnection->GetLastDatabaseErrorCode(),
			pConnection->GetLastDatabaseErrorString());
		return false;
	}

	DBResult = pConnection->GetResults(pRecordSet);
	if (DBResult != DBERR_SUCCEED && DBResult != DBERR_NO_RECORDS)
	{
		Log("��ȡ���������:(%u)%s",
			pConnection->GetLastDatabaseErrorCode(),
			pConnection->GetLastDatabaseErrorString());
		return false;
	}
	if (!pRecordSet->IsEOF())
	{
		if (!pRecordSet->GetField(0).IsNull())
		{
			m_IDSeed = pRecordSet->GetField(0);
		}
	}
	m_IDSeed++;
	return true;
}
void CDBTransGetRoleIDSeed::OnFinish()
{
	m_pParent->OnDBRetGetRoleIDSeed(m_IDSeed);
}
