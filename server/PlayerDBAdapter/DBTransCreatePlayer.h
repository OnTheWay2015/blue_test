#pragma once

class CPlayerDBAdapter;

class CDBTransCreatePlayer :
	public CDBTransaction
{
protected:
	CPlayerDBAdapter *				m_pParent;
	OBJECT_ID						m_CallerID;
	UINT64							m_AccountID;
	CSmartStruct					m_PlayerInfo;
	int								m_Result;
public:
	CDBTransCreatePlayer(CPlayerDBAdapter * pParent,OBJECT_ID CallerID, const CSmartStruct& CreateInfo);
	~CDBTransCreatePlayer(void);

	virtual bool OnExecute(IDBConnection * pConnection);
	virtual void OnFinish();
};
