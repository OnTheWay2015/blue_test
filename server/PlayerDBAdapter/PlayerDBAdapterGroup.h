#pragma once
class CPlayerDBAdapterGroup :
	public IDistributedObject,
	public CNameObject
{
protected:

	enum STATUS_TYPE
	{
		STATUS_NONE,
		STATUS_WORKING,
		STATUS_SHUTDOWN,
	};


	IDistributedObjectOperator *			m_pOperator;
	IDatabase *								m_pDatabase;
	CDBTransationManager *					m_pDBTransManager;
	bool									m_IsDBCheckOk;

	int										m_Status;

	CEasyArray<CPlayerDBAdapter *>			m_InstanceList;
	CEasyArray<INSTANCE_CONFIG>				m_InstanceConfigList;
	CEasyTimer								m_ShutdownTimer;

	DECLARE_DOS_MSG_MAP(CPlayerDBAdapter);
public:
	CPlayerDBAdapterGroup();
	~CPlayerDBAdapterGroup();

	virtual BOOL Initialize(IDistributedObjectOperator * pOperator) override;
	virtual void Destory() override;
	virtual UINT AddUseRef() override;
	virtual void Release() override;

protected:
	virtual int Update(int ProcessPacketLimit) override;
	virtual void OnShutDown(BYTE Level, UINT Param) override;
	void CheckSystemReady();
public:
	void OnDBRetCheckDB(bool IsSucceed);
};

