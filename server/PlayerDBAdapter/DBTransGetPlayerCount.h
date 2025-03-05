#pragma once

class CDBTransGetPlayerCount :
	public CDBTransaction
{
protected:
	CPlayerDBAdapter *				m_pParent;
	OBJECT_ID						m_CallerID;
	OBJECT_ID						m_Param;
	UINT							m_Count;
	int								m_Result;
public:
	CDBTransGetPlayerCount(CPlayerDBAdapter * pParent, OBJECT_ID CallerID, OBJECT_ID Param);
	~CDBTransGetPlayerCount();

	virtual bool OnExecute(IDBConnection * pConnection);
	virtual void OnFinish();
};

