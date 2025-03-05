#include "stdafx.h"


CDBTransCreatePlayer::CDBTransCreatePlayer(CPlayerDBAdapter * pParent, OBJECT_ID CallerID, const CSmartStruct& CreateInfo)
{
	m_pParent=pParent;
	m_CallerID=CallerID;
	m_PlayerInfo.Create(CreateInfo.GetDataLen() + 1024);
	m_PlayerInfo.Append(CreateInfo);
	if (m_PlayerInfo.IsMemberExist(CPlayerInfo::SST_PI_CREATE_TIME))
	{
		m_PlayerInfo.GetMember(CPlayerInfo::SST_PI_CREATE_TIME).SetValue((UINT)time(NULL));
	}
	else
	{
		m_PlayerInfo.AddMember(CPlayerInfo::SST_PI_CREATE_TIME, (UINT)time(NULL));
	}
	m_AccountID = m_PlayerInfo.GetMember(CPlayerInfo::SST_PI_ACCOUNT_ID);
	m_Result=COMMON_RESULT_FAILED;
}

CDBTransCreatePlayer::~CDBTransCreatePlayer(void)
{
}

bool CDBTransCreatePlayer::OnExecute(IDBConnection* pConnection)
{
	char szSQL[1024];
	//CSmartPtr<IDBRecordSet> pRecordSet;
	CDBParameterSet ParameterSet;
	DB_PARAM_INFO ParamInfo;
	int DBResult;



	//pRecordSet = pConnection->CreateRecordSet(DB_RS_TYPE_DYNAMIC);
	//if (pRecordSet == NULL)
	//{
	//	Log("CDBTransCreatePlayer::OnExecute:无法创建结果集");
	//	return false;
	//}


	//pRecordSet->Destory();


	UINT MaxDataSize = (UINT)m_PlayerInfo.GetDataLen();
	MaxDataSize += 1024;

	CEasyBuffer CompressBuff(MaxDataSize, _T("PlayerDBAdapterDBTrans"));
	CEasyBuffer LZOCompressWorkMemory(LZO1X_1_MEM_COMPRESS, _T("PlayerDBAdapterDBTrans"));

	UINT VIPLevel = m_PlayerInfo.GetMember(CPlayerInfo::SST_PI_VIPLEVEL);

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
		if (lzo1x_1_compress((BYTE*)m_PlayerInfo.GetData(), m_PlayerInfo.GetDataLen(), (BYTE*)CompressBuff.GetBuffer(), &OutLen, LZOCompressWorkMemory.GetBuffer()) != LZO_E_OK)
		{
			Log("玩家数据压缩失败");
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



	sprintf_s(szSQL, 1024, "insert into t_player_info(AccountID,UID,VIPLevel,PlayerData,LastUpdateTime) "
		"values(%lld,?,%d,?,now())",
		m_AccountID,

		VIPLevel
	);



	LogDebug("CDBTransCreatePlayer::OnExecute:执行SQL:%s", szSQL);
	DBResult = pConnection->ExecuteSQL(szSQL, 0, &ParameterSet);

	if (DBResult != DBERR_SUCCEED)
	{
		Log("CDBTransCreatePlayer::OnExecute:执行SQL出错:(%u)%s",
			pConnection->GetLastDatabaseErrorCode(),
			pConnection->GetLastDatabaseErrorString());
		return false;
	}
	m_Result = COMMON_RESULT_SUCCEED;


	return true;
}
void CDBTransCreatePlayer::OnFinish()
{
	m_pParent->OnDBRetCreatePlayer(m_Result, m_CallerID, m_AccountID, m_PlayerInfo);
}
