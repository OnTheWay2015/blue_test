#include "stdafx.h"



CDBTransPlayerSearch::CDBTransPlayerSearch(CPlayerDBAdapter * pParent, OBJECT_ID CallerID, OBJECT_ID Param, LPCTSTR NickName)
{
	m_pParent = pParent;
	m_CallerID = CallerID;
	m_Param = Param;
	m_NickName = NickName;
	m_Result = COMMON_RESULT_FAILED;
	m_DataPacket.SetTag(_T("CDBTransPlayerSearch"));
}


CDBTransPlayerSearch::~CDBTransPlayerSearch()
{
}

bool CDBTransPlayerSearch::OnExecute(IDBConnection * pConnection)
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

	pRecordSet->SetBlobMaxProcessSize(1048576);

	CEasyBuffer CompressBuff(1048576, _T("PlayerDBAdapterDBTrans"));

	
	m_NickName = "%" + m_NickName;
	m_NickName = m_NickName + "%";

	ParamInfo.Type = DB_TYPE_STRING;
	ParamInfo.ParamType = DB_PARAM_TYPE_INPUT;
	ParamInfo.IsNull = false;
	ParamInfo.Size = m_NickName.GetLength();
	ParamInfo.DigitSize = 0;
	ParameterSet.AddParam(ParamInfo, (LPCTSTR)m_NickName);

	sprintf_s(szSQL, 1024, "select PlayerData,unix_timestamp(CreateTime) "
		"from t_player_info where NickName like ? and ManageStatus<>1 order by LastUpdateTime desc limit 10");

	


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
	CPlayerInfoList InfoList;
	while (!pRecordSet->IsEOF())
	{
		int FieldIndex = 0;
		CPlayerInfo * pPlayerInfo = InfoList.GetList().AddEmpty();
		{
			CDBValue& Data = pRecordSet->GetField(FieldIndex++);

			if (!Data.IsNull())
			{
				lzo_uint OutLen = CompressBuff.GetBufferSize();
				int Error = lzo1x_decompress_safe((BYTE *)Data.GetBuffer(), Data.GetLength(),
					(BYTE *)CompressBuff.GetBuffer(), &OutLen, NULL);
				if (Error != LZO_E_OK)
				{
					Log("玩家数据解压失败%d", Error);
					return false;
				}

				CSmartStruct Packet(CompressBuff.GetBuffer(), (UINT)OutLen, false);
				pPlayerInfo->ParsePacket(Packet, DOMF_PLAYER_INFO_FULL);
			}
		}

		pPlayerInfo->SetCreateTime(pRecordSet->GetField(FieldIndex++));

		pRecordSet->MoveNext();
	}
	m_DataPacket.Create(InfoList.GetSmartStructSize(DOMF_PLAYER_INFO_LIST_FOR_SEARCH));
	InfoList.MakePacket(m_DataPacket, DOMF_PLAYER_INFO_LIST_FOR_SEARCH);

	m_Result = MSR_SUCCEED;

	return true;
}
void CDBTransPlayerSearch::OnFinish()
{
	m_pParent->OnDBRetPlayerSearch(m_Result, m_CallerID, m_Param, m_DataPacket);
}