#include "stdafx.h"



CDBTransGetIDByTID::CDBTransGetIDByTID(CPlayerDBAdapter * pParent, OBJECT_ID CallerID, LPCTSTR TID, const CSmartStruct& Param)
{
	m_pParent = pParent;
	m_CallerID = CallerID;
	m_Param.CloneFrom(Param);
	m_TID = TID;
	m_PlayerID = 0;
	m_Result = COMMON_RESULT_FAILED;
}


CDBTransGetIDByTID::~CDBTransGetIDByTID()
{
}

bool CDBTransGetIDByTID::OnExecute(IDBConnection * pConnection)
{
	char szSQL[1024];
	CSmartPtr<IDBRecordSet> pRecordSet;
	CDBParameterSet ParameterSet;
	DB_PARAM_INFO ParamInfo;
	int DBResult;

	pRecordSet = pConnection->CreateRecordSet(DB_RS_TYPE_DYNAMIC);
	if (pRecordSet == NULL)
	{
		Log("无法创建结果集");
		return false;
	}
	

	ParamInfo.Type = DB_TYPE_STRING;
	ParamInfo.ParamType = DB_PARAM_TYPE_INPUT;
	ParamInfo.IsNull = false;
	ParamInfo.Size = m_TID.GetLength();
	ParamInfo.DigitSize = 0;
	ParameterSet.AddParam(ParamInfo, (LPCTSTR)m_TID);

	sprintf_s(szSQL, 1024, "select AccountID "
		"from t_player_info where UID=? ");




	LogDebug("执行SQL:%s", szSQL);
	DBResult = pConnection->ExecuteSQL(szSQL, 0, &ParameterSet);

	if (DBResult != DBERR_SUCCEED)
	{
		Log("执行SQL出错:(%u)%s",
			pConnection->GetLastDatabaseErrorCode(),
			pConnection->GetLastDatabaseErrorString());
		Log("出错的SQL:%s", szSQL);
		return false;
	}

	DBResult = pConnection->GetResults(pRecordSet);
	if (DBResult == DBERR_NO_RECORDS)
	{
		return true;
	}
	else if (DBResult != DBERR_SUCCEED)
	{
		Log("获取结果集出错:(%u)%s",
			pConnection->GetLastDatabaseErrorCode(),
			pConnection->GetLastDatabaseErrorString());
		Log("出错的SQL:%s", szSQL);
		return false;
	}
	if (!pRecordSet->IsEOF())
	{
		m_PlayerID = pRecordSet->GetField(0);
		m_Result = MSR_SUCCEED;
	}
	else
	{
		m_Result = MSR_PLAYER_DATA_NOT_EXIST;
	}


	return true;
}
void CDBTransGetIDByTID::OnFinish()
{
	m_pParent->OnDBRetGetIDByTID(m_Result, m_CallerID, m_PlayerID, m_Param);
}