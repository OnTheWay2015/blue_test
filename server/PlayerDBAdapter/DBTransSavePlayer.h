#pragma once

class CPlayerDBAdapter;

class CDBTransSavePlayer :
	public CDBTransaction
{
protected:
	CPlayerDBAdapter *				m_pParent;
	OBJECT_ID						m_CallerID;
	UINT64							m_AccountID;
	CSmartStruct					m_PlayerInfo;
public:
	CDBTransSavePlayer(CPlayerDBAdapter * pParent, OBJECT_ID CallerID, const CSmartStruct& PlayerInfo);
	~CDBTransSavePlayer(void);

	virtual bool OnExecute(IDBConnection * pConnection);
	virtual void OnFinish();
};
