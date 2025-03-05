#pragma once

class CPlayerDBAdapter;

class CDBTransSaveRole :
    public CDBTransaction
{
protected:
	CPlayerDBAdapter* m_pParent;
	OBJECT_ID						m_CallerID;
	UINT64							m_RoleID;
	CSmartStruct					m_RoleInfo;
public:
	CDBTransSaveRole(CPlayerDBAdapter* pParent, OBJECT_ID CallerID, const CSmartStruct& RoleInfo)
	{
		m_pParent = pParent;
		m_CallerID = CallerID;
		m_RoleInfo.CloneFrom(RoleInfo);
		m_RoleID = m_RoleInfo.GetMember(CBaseCharInfo::SST_MII_OBJECT_ID);
	}
	~CDBTransSaveRole(void)
	{

	}

	virtual bool OnExecute(IDBConnection* pConnection);
	virtual void OnFinish();
};

