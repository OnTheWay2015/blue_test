#pragma once

class CDBTransPlayerSearch :
	public CDBTransaction
{
protected:
	CPlayerDBAdapter *				m_pParent;
	OBJECT_ID						m_CallerID;
	OBJECT_ID						m_Param;
	CEasyString						m_NickName;
	CSmartStruct					m_DataPacket;
	int								m_Result;
public:
	CDBTransPlayerSearch(CPlayerDBAdapter * pParent, OBJECT_ID CallerID, OBJECT_ID Param, LPCTSTR NickName);
	~CDBTransPlayerSearch();

	virtual bool OnExecute(IDBConnection * pConnection);
	virtual void OnFinish();
};

