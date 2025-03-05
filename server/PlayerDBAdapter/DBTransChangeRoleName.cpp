#include "stdafx.h"

CDBTransChangeRoleName::CDBTransChangeRoleName(CPlayerDBAdapter * pParent, OBJECT_ID CallerID, LPCTSTR RoleName, UINT64 RoleID)
{
	m_pParent = pParent;
	m_CallerID = CallerID;
	m_RoleName = RoleName;
	m_RoleID = RoleID;
	m_Result = COMMON_RESULT_FAILED;
}

CDBTransChangeRoleName::~CDBTransChangeRoleName(void)
{

}

bool CDBTransChangeRoleName::OnExecute(IDBConnection * pConnection)
{
	char szSQL[1024];
	int DBResult;
	CDBParameterSet ParameterSet;
	DB_PARAM_INFO ParamInfo;
	CSmartPtr<IDBRecordSet> pRecordSet;

	pRecordSet = pConnection->CreateRecordSet(DB_RS_TYPE_DYNAMIC);
	if (pRecordSet == NULL)
	{
		Log("无法创建结果集");
		return false;
	}
	BYTE StrBuffer[256];
	UINT StrLen = (UINT)m_RoleName.GetStringBytes(StrBuffer, 255, CEasyString::STRING_CODE_PAGE_UTF8);

	ParameterSet.Destory();
	ParamInfo.Type = DB_TYPE_STRING;
	ParamInfo.ParamType = DB_PARAM_TYPE_INPUT;
	ParamInfo.IsNull = false;
	ParamInfo.Size = StrLen;
	ParamInfo.DigitSize = 0;
	ParameterSet.AddParam(ParamInfo);
	ParameterSet.GetParam(ParameterSet.GetCount() - 1).SetValue(DB_TYPE_STRING, StrBuffer, StrLen, 0);

	sprintf_s(szSQL, 1024, "select RoleID from t_role_info where RoleName = ?");

	LogDebug("执行SQL:%s", szSQL);
	DBResult = pConnection->ExecuteSQL(szSQL, 0, &ParameterSet);

	if (DBResult != DBERR_SUCCEED && DBResult != DBERR_NO_RECORDS)
	{
		Log("执行SQL出错:(%u)%s",
			pConnection->GetLastDatabaseErrorCode(),
			pConnection->GetLastDatabaseErrorString());
		Log("出错的SQL:%s", szSQL);
		return false;
	}

	DBResult = pConnection->GetResults(pRecordSet);
	if (DBResult != DBERR_SUCCEED && DBResult != DBERR_NO_RECORDS)
	{
		Log("获取结果集出错:(%u)%s",
			pConnection->GetLastDatabaseErrorCode(),
			pConnection->GetLastDatabaseErrorString());
		Log("出错的SQL:%s", szSQL);
		return false;
	}

	if (DBResult != DBERR_NO_RECORDS)
	{
		m_Result = MSR_NAME_ALREAD_USED;
		return true;
	}


	sprintf_s(szSQL, 1024, "update t_role_info set RoleName =  ? where RoleID = %lld", m_RoleID);

	LogDebug("执行SQL:%s", szSQL);
	DBResult = pConnection->ExecuteSQL(szSQL, 0, &ParameterSet);

	if (DBResult != DBERR_SUCCEED&&DBResult != DBERR_NO_RECORDS)
	{
		Log("执行SQL出错:(%u)%s",
			pConnection->GetLastDatabaseErrorCode(),
			pConnection->GetLastDatabaseErrorString());
		Log("出错的SQL:%s", szSQL);
		return false;
	}

	m_Result = COMMON_RESULT_SUCCEED;
	return true;
}

void CDBTransChangeRoleName::OnFinish()
{
	m_pParent->OnDBRetChangeRoleName(m_Result, m_CallerID, m_RoleID, m_RoleName);
}
