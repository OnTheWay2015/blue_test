#pragma once

class CPlayerDBAdapter;

class CDBTransChangeRoleName :
	public CDBTransaction
{
protected:
	CPlayerDBAdapter *				m_pParent;
	OBJECT_ID						m_CallerID;
	short							m_Result;
	CEasyString						m_RoleName;
	UINT64							m_RoleID;
public:
	CDBTransChangeRoleName(CPlayerDBAdapter * pParent, OBJECT_ID CallerID, LPCTSTR RoleName, UINT64 RoleID);
	~CDBTransChangeRoleName(void);

	virtual bool OnExecute(IDBConnection * pConnection);
	virtual void OnFinish();
};
