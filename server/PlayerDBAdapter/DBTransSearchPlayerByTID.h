#pragma once

class CDBTransSearchPlayerByTID :
	public CDBTransaction
{
protected:
	CPlayerDBAdapter *				m_pParent;
	OBJECT_ID						m_CallerID;
	OBJECT_ID						m_Param;
	CEasyString						m_TID;
	CSmartStruct					m_DataPacket;
	int								m_Result;
public:
	CDBTransSearchPlayerByTID(CPlayerDBAdapter * pParent, OBJECT_ID CallerID, OBJECT_ID Param, LPCTSTR TID);
	~CDBTransSearchPlayerByTID();

	virtual bool OnExecute(IDBConnection * pConnection);
	virtual void OnFinish();
};

