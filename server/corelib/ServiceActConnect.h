#pragma once






class ServiceActConnect:public CBaseNetServiceInterface
{
public:
    ServiceActConnect();
    ~ServiceActConnect();
private:
	CNetServer *								m_pServer;	
	NetHandlerInterface* m_NetHandler;
	//CSmartPtr<CBaseNetConnectionInterface>
	//std::map<SESSION_ID ,CSmartPtr<CBaseNetConnectionInterface>> m_Connections;



public:
	void Init();
    void SetHandler(NetHandlerInterface* h)
	{
		m_NetHandler = h;
	}

    CSmartPtr<CBaseNetConnectionInterface> CreateConnection(CIPAddress& remoteAddress,SERVICE_TYPE type,CLIENT_PROXY_MODE mode);



public:
	virtual void SetServer(CNetServer* Svr) override;//todo  CNetServerInterface
	virtual CNetServer* GetServer()  override;
	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT) override;
	virtual void OnRecvData(CSmartPtr<CBaseNetConnectionInterface> pConnection, DOS_SIMPLE_MESSAGE_HEAD* pData ) override;

	
	virtual bool StartListen(const CIPAddress& Address) override {return false;}

	virtual void OnStartUp() {};
	virtual void OnClose() {};
	virtual CSmartPtr<CBaseNetConnectionInterface> CreateConnection(CIPAddress& RemoteAddress){return nullptr;};
	virtual bool DeleteConnection(CSmartPtr<CBaseNetConnectionInterface>  pConnection) {return false;};
	virtual void OnRecvData(const CIPAddress& IPAddress, const BYTE * pData, UINT DataSize) {}; //UDP

	virtual UINT GetServiceID() override { return 0;}
	virtual SERVICE_TYPE GetServiceType() override { return SERVICE_TYPE::NONE ;}

	virtual void OnConnection(CSmartPtr<CBaseNetConnectionInterface> s, bool IsSucceed) override ;
	virtual void OnDisconnection(CSmartPtr<CBaseNetConnectionInterface> s) override;
	virtual void OnRecvMessage(CSmartPtr<CBaseNetConnectionInterface> s,  DOS_SIMPLE_MESSAGE_HEAD* pMsg) override; 





}; 



