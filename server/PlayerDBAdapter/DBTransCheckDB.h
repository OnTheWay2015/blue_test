#pragma once

class CDBTransCheckDB :
	public CDBTransaction
{
protected:
	CPlayerDBAdapterGroup *						m_pParent;
	CEasyArray<INSTANCE_CONFIG>					m_InstanceConfigList;
public:
	CDBTransCheckDB(CPlayerDBAdapterGroup * pParent, const CEasyArray<INSTANCE_CONFIG>& InstanceConfigList);
	~CDBTransCheckDB();

	virtual bool OnExecute(IDBConnection * pConnection);
	virtual void OnFinish();
};

