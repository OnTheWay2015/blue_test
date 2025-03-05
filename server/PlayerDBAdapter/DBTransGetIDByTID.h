#pragma once

class CDBTransGetIDByTID :
	public CDBTransaction
{
protected:
	CPlayerDBAdapter *				m_pParent;
	OBJECT_ID						m_CallerID;
	CSmartStruct					m_Param;
	CEasyString						m_TID;
	UINT64							m_PlayerID;
	int								m_Result;
public:
	CDBTransGetIDByTID(CPlayerDBAdapter * pParent, OBJECT_ID CallerID, LPCTSTR TID, const CSmartStruct& Param);
	~CDBTransGetIDByTID();

	virtual bool OnExecute(IDBConnection * pConnection);
	virtual void OnFinish();
};

