#pragma once

class CPlayerDBAdapter;

class CDBTransGetPlayerInfo :
	public CDBTransaction
{	
protected:
	CPlayerDBAdapter *				m_pParent;
	OBJECT_ID						m_CallerID;
	BYTE							m_QueryType;
	CSmartStruct					m_QueryParam;
	UINT64							m_AccountID;
	CSmartStruct					m_PlayerInfo;
	CEasyBuffer						m_CompressBuff;
	int								m_Result;
public:
	CDBTransGetPlayerInfo(CPlayerDBAdapter * pParent, OBJECT_ID CallerID, UINT64 AccountID, BYTE QueryType, const CSmartStruct& QueryParam);
	~CDBTransGetPlayerInfo(void);

	virtual bool OnExecute(IDBConnection * pConnection);
	virtual void OnFinish();
};
