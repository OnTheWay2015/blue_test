#pragma once

class CProxyServiceDefault;
class CoreSessionMessage;
class CProxyConnectionGroup :
	public CEasyThread
{
protected:
	CProxyServiceDefault *								m_pService;
	UINT														m_Index;
	CHashMap<SESSION_ID,CSmartPtr<CBaseNetConnectionInterface> >			m_ConnectionPool;
	CEasyCriticalSection										m_EasyCriticalSection;


public:
	CProxyConnectionGroup();
	~CProxyConnectionGroup();

	void Init();
	bool AddConnection( CSmartPtr<CBaseNetConnectionInterface> pConnection);
	bool RemoveConnection( CSmartPtr<CBaseNetConnectionInterface> pConnection);
    void SendSessionMessage(CSmartPtr<CoreSessionMessage> msg);


	
	virtual BOOL OnStart() override;
	virtual BOOL OnRun() override;
	virtual void OnTerminate() override;

};

