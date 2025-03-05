#include "stdafx.h"



CDBTransGetPlayerCount::CDBTransGetPlayerCount(CPlayerDBAdapter * pParent, OBJECT_ID CallerID, OBJECT_ID Param)
{
	m_pParent = pParent;
	m_CallerID = CallerID;
	m_Param = Param;
	m_Count = 0;
	m_Result = COMMON_RESULT_FAILED;
}


CDBTransGetPlayerCount::~CDBTransGetPlayerCount()
{
}

bool CDBTransGetPlayerCount::OnExecute(IDBConnection * pConnection)
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

	

	sprintf_s(szSQL, 1024, "select count(AccountID) "
		"from t_player_info where ManageStatus<>1 ");


	LogDebug("ִ��SQL:%s", szSQL);
	DBResult = pConnection->ExecuteSQL(szSQL, 0);

	if (DBResult != DBERR_SUCCEED)
	{
		Log("ִ��SQL����:(%u)%s",
			pConnection->GetLastDatabaseErrorCode(),
			pConnection->GetLastDatabaseErrorString());
		Log("�����SQL:%s", szSQL);
		return false;
	}

	DBResult = pConnection->GetResults(pRecordSet);	
	if ((DBResult != DBERR_SUCCEED) && (DBResult != DBERR_NO_RECORDS))
	{
		Log("��ȡ���������:(%u)%s",
			pConnection->GetLastDatabaseErrorCode(),
			pConnection->GetLastDatabaseErrorString());
		Log("�����SQL:%s", szSQL);
		return false;
	}
	if (!pRecordSet->IsEOF())
	{
		m_Count = pRecordSet->GetField(0);
		m_Result = MSR_SUCCEED;
	}
	return true;
}
void CDBTransGetPlayerCount::OnFinish()
{
	m_pParent->OnDBRetPlayerSearch(m_Result, m_CallerID, m_Param, m_Count);
}