#pragma once

class CPlayerDBAdapter;

class CDBTransDeletePlayer :
	public CDBTransaction
{
protected:
	CPlayerDBAdapter *				m_pParent;
	OBJECT_ID						m_CallerID;
	UINT64							m_AccountID;
public:
	CDBTransDeletePlayer(CPlayerDBAdapter * pParent, OBJECT_ID CallerID, UINT64 AccountID);
	~CDBTransDeletePlayer(void);

	virtual bool OnExecute(IDBConnection * pConnection);
	virtual void OnFinish();
};
