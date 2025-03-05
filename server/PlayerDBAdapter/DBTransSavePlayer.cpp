#include "stdafx.h"


CDBTransSavePlayer::CDBTransSavePlayer(CPlayerDBAdapter * pParent, OBJECT_ID CallerID, const CSmartStruct& PlayerInfo)
{
	m_pParent=pParent;
	m_CallerID=CallerID;
	m_PlayerInfo.CloneFrom(PlayerInfo);
	m_AccountID = m_PlayerInfo.GetMember(CPlayerInfo::SST_PI_ACCOUNT_ID);
}

CDBTransSavePlayer::~CDBTransSavePlayer(void)
{
}

bool CDBTransSavePlayer::OnExecute(IDBConnection * pConnection)
{
	char szSQL[2048];
	CSmartPtr<IDBRecordSet> pRecordSet;
	CDBParameterSet ParameterSet;
	DB_PARAM_INFO ParamInfo;
	int DBResult;
	

	UINT MaxDataSize = (UINT)m_PlayerInfo.GetDataLen();
	
	MaxDataSize += 1024;

	LogDebug("数据块大小%u", MaxDataSize);

	CEasyBuffer CompressBuff(MaxDataSize, _T("PlayerDBAdapterDBTrans"));
	CEasyBuffer LZOCompressWorkMemory(LZO1X_1_MEM_COMPRESS, _T("PlayerDBAdapterDBTrans"));

	
	UINT VIPLevel = m_PlayerInfo.GetMember(CPlayerInfo::SST_PI_VIPLEVEL);
	UINT64 VIPExp = m_PlayerInfo.GetMember(CPlayerInfo::SST_PI_VIPEXP);
	CSmartStruct OtherData = m_PlayerInfo.GetMember(CPlayerInfo::SST_PI_OTHER_DATA);
	CSmartStruct IAPStatInfo = OtherData.GetMember(PLAYER_OTHER_DATA::SST_PLAYER_OTHER_DATA_IAPSTAT_INFO);
	UINT TotalAMOUNT = IAPStatInfo.GetMember(IAP_STAT_INFO::SST_IAP_STAT_INFO_TOTAL_AMOUNT);

	BYTE StrBuffer[256];

	UINT StrLen = (UINT)m_PlayerInfo.GetMember(CPlayerInfo::SST_PI_PLANTFORM_UID).GetStringRef().GetStringBytes(StrBuffer, 250, CEasyString::STRING_CODE_PAGE_UTF8);

	ParamInfo.Type = DB_TYPE_STRING;
	ParamInfo.ParamType = DB_PARAM_TYPE_INPUT;
	ParamInfo.IsNull = false;
	ParamInfo.Size = StrLen;
	ParameterSet.AddParam(ParamInfo);
	ParameterSet.GetParam(ParameterSet.GetCount() - 1).SetValue(DB_TYPE_STRING, StrBuffer, StrLen, 0);
		
	{
		lzo_uint OutLen = CompressBuff.GetBufferSize();
		if (lzo1x_1_compress((BYTE *)m_PlayerInfo.GetData(), m_PlayerInfo.GetDataLen(), (BYTE *)CompressBuff.GetBuffer(), &OutLen, LZOCompressWorkMemory.GetBuffer()) != LZO_E_OK)
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

	sprintf_s(szSQL, 2048, "update t_player_info set UID=?,VIPLevel=%d,VIPExp=%lld,IAPTotalAmount=%d,"
		"	PlayerData=?,LastUpdateTime=now() "
		"where AccountID=%lld",
		VIPLevel,
		VIPExp,
		TotalAMOUNT,

		m_AccountID
		);
	
	LogDebug("执行SQL:%s",szSQL);
	DBResult=pConnection->ExecuteSQL(szSQL,0,&ParameterSet);
	if(DBResult!=DBERR_SUCCEED)
	{
		Log("执行SQL出错:(%u)%s",
			pConnection->GetLastDatabaseErrorCode(),
			pConnection->GetLastDatabaseErrorString());
		return false;
	}

	return true;
}
void CDBTransSavePlayer::OnFinish()
{
	m_pParent->OnDBRetSavePlayer(m_IsSucceed, m_CallerID, m_AccountID);
}
