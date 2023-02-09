#pragma once

class CProxyServiceDefault;

class CProxyConnectionGroup :
	public CEasyThread
{
protected:
	CProxyServiceDefault *								m_pService;
	UINT														m_Index;
	CStaticMap<UINT, CSmartPtr<CProxyConnectionDefault>>			m_ConnectionPool;
	CEasyCriticalSection										m_EasyCriticalSection;


public:
	CProxyConnectionGroup();
	~CProxyConnectionGroup();

	void Init();
	bool AddConnection( CSmartPtr<CProxyConnectionDefault> pConnection);


	
	virtual BOOL OnStart() override;
	virtual BOOL OnRun() override;
	virtual void OnTerminate() override;

};

