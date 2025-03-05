#include "stdafx.h"


CDBTransGetPlayerInfo::CDBTransGetPlayerInfo(CPlayerDBAdapter * pParent, OBJECT_ID CallerID, UINT64 AccountID, BYTE QueryType, const CSmartStruct& QueryParam)
	:m_CompressBuff(_T("PlayerDBAdapterDBTrans"))
{
	m_pParent=pParent;
	m_CallerID=CallerID;
	m_QueryType = QueryType;
	m_QueryParam.CloneFrom(QueryParam);
	m_AccountID = AccountID;
	m_Result = COMMON_RESULT_FAILED;
}

CDBTransGetPlayerInfo::~CDBTransGetPlayerInfo(void)
{
}

bool CDBTransGetPlayerInfo::OnExecute(IDBConnection * pConnection)
{
	char szSQL[1024];
	CSmartPtr<IDBRecordSet> pRecordSet;
	int DBResult;

	pRecordSet=pConnection->CreateRecordSet(DB_RS_TYPE_DYNAMIC);
	if(pRecordSet==NULL)
	{
		Log("无法创建结果集");
		return false;
	}

	pRecordSet->SetBlobMaxProcessSize(1048576);

	m_CompressBuff.Create(1048576 * 2);

	
	sprintf_s(szSQL, 1024, "select PlayerData,unix_timestamp(CreateTime) "
		"from t_player_info where AccountID=%lld and ManageStatus<>1",
		m_AccountID);
	

	LogDebug("执行SQL:%s",szSQL);
	DBResult=pConnection->ExecuteSQL(szSQL,0);

	if(DBResult!=DBERR_SUCCEED)
	{
		Log("执行SQL出错:(%u)%s",
			pConnection->GetLastDatabaseErrorCode(),
			pConnection->GetLastDatabaseErrorString());
		Log("出错的SQL:%s",szSQL);
		return false;
	}

	DBResult=pConnection->GetResults(pRecordSet);
	if (DBResult == DBERR_NO_RECORDS)
	{
		m_Result = MSR_PLAYER_DATA_NOT_EXIST;
		return true;
	}
	else if(DBResult!=DBERR_SUCCEED)
	{
		Log("获取结果集出错:(%u)%s",
			pConnection->GetLastDatabaseErrorCode(),
			pConnection->GetLastDatabaseErrorString());
		Log("出错的SQL:%s",szSQL);
		return false;
	}
	if(!pRecordSet->IsEOF())
	{
		int FieldIndex = 0;		
		
		{
			CDBValue& Data = pRecordSet->GetField(FieldIndex++);

			if (!Data.IsNull())
			{
				lzo_uint OutLen = m_CompressBuff.GetBufferSize();
				int Error = lzo1x_decompress_safe((BYTE *)Data.GetBuffer(), Data.GetLength(),
					(BYTE *)m_CompressBuff.GetBuffer(), &OutLen, NULL);
				if (Error != LZO_E_OK)
				{
					Log("玩家数据解压失败%d", Error);
					return true;
				}
				m_PlayerInfo.Attach(m_CompressBuff.GetBuffer(), m_CompressBuff.GetBufferSize(), false);
			}
		}
		
		UINT CreateTime = pRecordSet->GetField(FieldIndex++);
		
		if (m_PlayerInfo.IsMemberExist(CPlayerInfo::SST_PI_CREATE_TIME))
		{
			m_PlayerInfo.GetMember(CPlayerInfo::SST_PI_CREATE_TIME).SetValue(CreateTime);
		}
		else
		{
			m_PlayerInfo.AddMember(CPlayerInfo::SST_PI_CREATE_TIME, CreateTime);
		}

		UINT64 AccountID = m_PlayerInfo.GetMember(CPlayerInfo::SST_PI_ACCOUNT_ID);
		if (AccountID == m_AccountID)
		{
			m_Result = COMMON_RESULT_SUCCEED;
		}
		else
		{
			Log("玩家ID不一致[0x%llX][0x%llX]", m_AccountID, AccountID);
			//m_PlayerInfo.GetMember(CPlayerInfo::SST_PI_ACCOUNT_ID).SetValue(m_AccountID);
			//m_Result = COMMON_RESULT_SUCCEED;
		}		
	}
	else
	{
		m_Result = MSR_PLAYER_DATA_NOT_EXIST;
		Log("玩家[0x%llX]不存在", m_AccountID);
	}
	
	return true;
}
void CDBTransGetPlayerInfo::OnFinish()
{
	m_pParent->OnDBRetGetPlayerInfo(m_Result, m_CallerID, m_QueryType, m_QueryParam, m_AccountID, m_PlayerInfo);
}
