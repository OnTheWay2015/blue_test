#pragma once

class CPlayerDBAdapter;

class CDBTransCreateRole :
    public CDBTransaction
{
protected:
	CPlayerDBAdapter*				m_pParent;
	OBJECT_ID						m_CallerID;
	UINT64							m_AccountID;
	UINT64							m_RoleID;
	CSmartStruct					m_RoleInfo;
	int								m_Result;
public:
	CDBTransCreateRole(CPlayerDBAdapter* pParent, OBJECT_ID CallerID, UINT64 AccountID, UINT64 RoleID, const CSmartStruct& CreateInfo)
	{
		m_pParent = pParent;
		m_CallerID = CallerID;
		m_AccountID = AccountID;
		m_RoleID = RoleID;
		m_RoleInfo.Create(CreateInfo.GetDataLen() + 1024);
		m_RoleInfo.Append(CreateInfo);
		if (m_RoleInfo.IsMemberExist(CRoleInfo::SST_RI_CREATE_TIME))
		{
			m_RoleInfo.GetMember(CRoleInfo::SST_RI_CREATE_TIME).SetValue((UINT)time(NULL));
		}
		else
		{
			m_RoleInfo.AddMember(CRoleInfo::SST_RI_CREATE_TIME, (UINT)time(NULL));
		}
		m_Result = COMMON_RESULT_FAILED;
	}
	~CDBTransCreateRole(void)
	{

	}

	virtual bool OnExecute(IDBConnection* pConnection);
	virtual void OnFinish();
};

