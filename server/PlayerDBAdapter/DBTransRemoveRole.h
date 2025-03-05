#pragma once

class CPlayerDBAdapter;

class CDBTransRemoveRole :
    public CDBTransaction
{
protected:
	CPlayerDBAdapter*				m_pParent;
	OBJECT_ID						m_CallerID;
	UINT64							m_AccountID;
	UINT64							m_RoleID;
public:
	CDBTransRemoveRole(CPlayerDBAdapter* pParent, OBJECT_ID CallerID, UINT64 AccountID, UINT64 RoleID)
	{
		m_pParent = pParent;
		m_CallerID = CallerID;
		m_AccountID = AccountID;
		m_RoleID = RoleID;
	}
	~CDBTransRemoveRole(void)
	{

	}

	virtual bool OnExecute(IDBConnection* pConnection);
	virtual void OnFinish();
};

