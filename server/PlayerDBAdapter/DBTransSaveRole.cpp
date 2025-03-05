#include "stdafx.h"

bool CDBTransSaveRole::OnExecute(IDBConnection* pConnection)
{
	char szSQL[2048];
	CSmartPtr<IDBRecordSet> pRecordSet;
	CDBParameterSet ParameterSet;
	DB_PARAM_INFO ParamInfo;
	int DBResult;


	UINT MaxDataSize = (UINT)m_RoleInfo.GetDataLen();

	MaxDataSize += 1024;

	LogDebug("数据块大小%u", MaxDataSize);

	CEasyBuffer CompressBuff(MaxDataSize, _T("PlayerDBAdapterDBTrans"));
	CEasyBuffer LZOCompressWorkMemory(LZO1X_1_MEM_COMPRESS, _T("PlayerDBAdapterDBTrans"));


	{
		lzo_uint OutLen = CompressBuff.GetBufferSize();
		if (lzo1x_1_compress((BYTE*)m_RoleInfo.GetData(), m_RoleInfo.GetDataLen(), (BYTE*)CompressBuff.GetBuffer(), &OutLen, LZOCompressWorkMemory.GetBuffer()) != LZO_E_OK)
		{
			Log("玩家数据压缩失败");
			return false;
		}
		LogDebug("压缩后数据块大小%u", OutLen);
		CompressBuff.SetUsedSize((UINT)OutLen);
	}
	ParamInfo.Type = DB_TYPE_BINARY;
	ParamInfo.ParamType = DB_PARAM_TYPE_INPUT;
	ParamInfo.IsNull = false;
	ParamInfo.Size = CompressBuff.GetUsedSize();
	ParamInfo.DigitSize = 0;
	ParameterSet.AddParam(ParamInfo);
	ParameterSet.GetParam(ParameterSet.GetCount() - 1).SetValue(DB_TYPE_BINARY, CompressBuff.GetBuffer(), CompressBuff.GetUsedSize(), 0);

	sprintf_s(szSQL, 2048, "update t_role_info set "
		"	RoleData=?,LastUpdateTime=now() "
		"where RoleID=%lld",
		m_RoleID
	);

	LogDebug("执行SQL:%s", szSQL);
	DBResult = pConnection->ExecuteSQL(szSQL, 0, &ParameterSet);
	if (DBResult != DBERR_SUCCEED)
	{
		Log("执行SQL出错:(%u)%s",
			pConnection->GetLastDatabaseErrorCode(),
			pConnection->GetLastDatabaseErrorString());
		return false;
	}

	return true;
}
void CDBTransSaveRole::OnFinish()
{
	m_pParent->OnDBRetSaveRole(m_IsSucceed, m_CallerID, m_RoleID);
}
