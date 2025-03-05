#pragma once

class CPlayerDBAdapter;

class CDBTransGetRoleIDSeed :
    public CDBTransaction
{
protected:
	CPlayerDBAdapter*				m_pParent;
	UINT64							m_IDSeed;
public:
	CDBTransGetRoleIDSeed(CPlayerDBAdapter* pParent)
	{
		m_pParent = pParent;
		m_IDSeed = 0;
	}
	~CDBTransGetRoleIDSeed(void)
	{

	}

	virtual bool OnExecute(IDBConnection* pConnection);
	virtual void OnFinish();
};

