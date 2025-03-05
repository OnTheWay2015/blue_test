#pragma once
class CPlayerDBAdapter;

class CDBTransLoadRoleList :
    public CDBTransaction
{
protected:
    CPlayerDBAdapter*               m_pParent;
    OBJECT_ID						m_CallerID;
    UINT64                          m_AccountID;
    CEasyBuffer						m_CompressBuff;
    CEasyArray<CSmartStruct>        m_RoleList;
    int								m_Result;
public:
    CDBTransLoadRoleList(CPlayerDBAdapter* pParent, OBJECT_ID CallerID, UINT64 AccountID)
    {
        m_pParent = pParent;
        m_CallerID = CallerID;
        m_AccountID = AccountID;
        m_Result = COMMON_RESULT_FAILED;
    }
    ~CDBTransLoadRoleList()
    {

    }

	virtual bool OnExecute(IDBConnection* pConnection);
	virtual void OnFinish();
};

