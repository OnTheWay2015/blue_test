#include "stdafx.h"


CDBTransCreatePlayer::CDBTransCreatePlayer(CPlayerDBAdapter * pParent,const CSmartStruct& CreateInfo)
{
	m_pParent=pParent;
	m_PlayerInfo.Create(CreateInfo.GetDataLen() + 1024);
	m_PlayerInfo.Append(CreateInfo);
	//if (m_PlayerInfo.IsMemberExist(CPlayerInfo::SST_PI_CREATE_TIME))
	//{
	//	m_PlayerInfo.GetMember(CPlayerInfo::SST_PI_CREATE_TIME).SetValue((UINT)time(NULL));
	//}
	//else
	//{
	//	m_PlayerInfo.AddMember(CPlayerInfo::SST_PI_CREATE_TIME, (UINT)time(NULL));
	//}
	//m_AccountID = m_PlayerInfo.GetMember(CPlayerInfo::SST_PI_ACCOUNT_ID);
	m_AccountID = 123;
	m_Result=false;
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


	UINT VIPLevel = 11;


	sprintf_s(szSQL, 1024, "insert into student(a1,a2,a3,a4,a5) "
		"values(%lld,%s,%s,%s,%s)",
		123
		,"a2"
		,"a3"
		,"a4"
		,"a5"
	);



	//LogDebug("CDBTransCreatePlayer::OnExecute:执行SQL:%s", szSQL);
	DBResult = pConnection->ExecuteSQL(szSQL, 0, &ParameterSet);

	if (DBResult != DBERR_SUCCEED)
	{
		//Log("CDBTransCreatePlayer::OnExecute:执行SQL出错:(%u)%s",
		//	pConnection->GetLastDatabaseErrorCode(),
		//	pConnection->GetLastDatabaseErrorString());
		return false;
	}
	m_Result = true;


	return true;
}
void CDBTransCreatePlayer::OnFinish()
{
	m_pParent->OnDBRetCreatePlayer(m_Result,m_AccountID, 0, m_PlayerInfo);
}
