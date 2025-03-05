#include "stdafx.h"

bool CDBTransCreateRole::OnExecute(IDBConnection* pConnection)
{
	char szSQL[1024];
	CSmartPtr<IDBRecordSet> pRecordSet;
	CDBParameterSet ParameterSet;
	DB_PARAM_INFO ParamInfo;
	int DBResult;



	pRecordSet = pConnection->CreateRecordSet(DB_RS_TYPE_DYNAMIC);
	if (pRecordSet == NULL)
	{
		Log("CDBTransCreateRole::OnExecute:无法创建结果集");
		return false;
	}


	pRecordSet->Destory();


	UINT MaxDataSize = (UINT)m_RoleInfo.GetDataLen();
	MaxDataSize += 1024;

	CEasyBuffer CompressBuff(MaxDataSize, _T("PlayerDBAdapterDBTrans"));
	CEasyBuffer LZOCompressWorkMemory(LZO1X_1_MEM_COMPRESS, _T("PlayerDBAdapterDBTrans"));

	BYTE StrBuffer[256];

	UINT StrLen = (UINT)m_RoleInfo.GetMember(CBaseCharInfo::SST_MII_NAME).GetStringRef().GetStringBytes(StrBuffer, 250, CEasyString::STRING_CODE_PAGE_UTF8);

	ParamInfo.Type = DB_TYPE_STRING;
	ParamInfo.ParamType = DB_PARAM_TYPE_INPUT;
	ParamInfo.IsNull = false;
	ParamInfo.Size = StrLen;
	ParamInfo.DigitSize = 0;
	ParameterSet.AddParam(ParamInfo);
	ParameterSet.GetParam(ParameterSet.GetCount() - 1).SetValue(DB_TYPE_STRING, StrBuffer, StrLen, 0);

	sprintf_s(szSQL, 1024, "select RoleID from t_Role_info where RoleName=? ");

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

	if (DBResult == DBERR_NO_RECORDS)
	{
		{
			lzo_uint OutLen = CompressBuff.GetBufferSize();
			if (lzo1x_1_compress((BYTE*)m_RoleInfo.GetData(), m_RoleInfo.GetDataLen(), (BYTE*)CompressBuff.GetBuffer(), &OutLen, LZOCompressWorkMemory.GetBuffer()) != LZO_E_OK)
			{
				Log("角色数据压缩失败");
				return false;
			}
			CompressBuff.SetUsedSize((UINT)OutLen);
		}
		ParamInfo.Type = DB_TYPE_BINARY;
		ParamInfo.ParamType = DB_PARAM_TYPE_INPUT;
		ParamInfo.IsNull = false;
		ParamInfo.Size = CompressBuff.GetUsedSize();
		ParamInfo.DigitSize = 0;
		ParameterSet.AddParam(ParamInfo);
		ParameterSet.GetParam(ParameterSet.GetCount() - 1).SetValue(DB_TYPE_BINARY, CompressBuff.GetBuffer(), CompressBuff.GetUsedSize(), 0);



		sprintf_s(szSQL, 1024, "insert into t_role_info(AccountID,RoleID,RoleName,RoleData,LastUpdateTime) "
			"values(%lld,%lld,?,?,now())",
			m_AccountID,
			m_RoleID
		);


		LogDebug("执行SQL:%s", szSQL);
		DBResult = pConnection->ExecuteSQL(szSQL, 0, &ParameterSet);

		if (DBResult != DBERR_SUCCEED)
		{
			Log("CDBTransCreateRole::OnExecute:行SQL出错:(%u)%s",
				pConnection->GetLastDatabaseErrorCode(),
				pConnection->GetLastDatabaseErrorString());
			return false;
		}
		m_Result = COMMON_RESULT_SUCCEED;
	}
	else
	{
		m_Result = MSR_NAME_ALREAD_USED;
	}

	return true;
}
void CDBTransCreateRole::OnFinish()
{
	m_pParent->OnDBRetCreateRole(m_Result, m_CallerID, m_AccountID, m_RoleID, m_RoleInfo);
}
