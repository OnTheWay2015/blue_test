/****************************************************************************/
/*                                                                          */
/*      文件名:    BaseService.h                                            */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CNetServer;
class NetHandlerInterface;



//class CBaseNetConnectionHolderInterface
//{
//public:
//	virtual void OnConnection(CSmartPtr<CBaseNetConnectionInterface> s, bool IsSucceed) =0;
//
//	virtual void OnDisconnection(CSmartPtr<CBaseNetConnectionInterface> s)=0;
//	//virtual void OnRecvData(CSmartPtr<CBaseNetConnectionInterface> s, const BYTE * pData, UINT DataSize)=0;
//	virtual void OnRecvMessage(CSmartPtr<CBaseNetConnectionInterface> s, const BYTE * pData, UINT DataSize)=0; //todo
//};

class CBaseNetServiceInterface
{
public:
	virtual bool StartListen(const CIPAddress& Address)=0;
	virtual void SetServer(CNetServer* Svr) =0;//todo  CNetServerInterface
	virtual CNetServer* GetServer() =0;
	//virtual void SetHandler(NetHandlerInterface* h) {};
	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT) =0;

	virtual void OnStartUp()=0;
	virtual void OnClose()=0;
	virtual CSmartPtr<CBaseNetConnectionInterface> CreateConnection(CIPAddress& RemoteAddress)=0;
	virtual bool DeleteConnection(CSmartPtr<CBaseNetConnectionInterface>  pConnection) = 0;


	virtual void OnRecvData(const CIPAddress& IPAddress, const BYTE * pData, UINT DataSize) = 0; //UDP
	virtual void OnRecvData(CSmartPtr<CBaseNetConnectionInterface> pConnection, DOS_SIMPLE_MESSAGE_HEAD* pData ) = 0;
//DOS_SIMPLE_MESSAGE_HEAD
	virtual UINT GetServiceID()=0;
	virtual UINT GetServiceType()=0;

//Connection holder
	virtual void OnConnection(CSmartPtr<CBaseNetConnectionInterface> s, bool IsSucceed) =0;
	virtual void OnDisconnection(CSmartPtr<CBaseNetConnectionInterface> s)=0;
	virtual void OnRecvMessage(CSmartPtr<CBaseNetConnectionInterface> s, DOS_SIMPLE_MESSAGE_HEAD* pMsg)=0; 

};



class CBaseNetService :
    public CNameObject
{
protected:
    CNetSocket								m_Socket;
    bool m_StopFlag;
	NetHandlerInterface* m_NetHandler;
    CLIENT_PROXY_TYPE	m_ClientProxyType;
    CLIENT_PROXY_MODE	m_ClientProxyMode;
    UINT ServiceID;
    UINT ServiceType;

public:
	CBaseNetService(void);
	~CBaseNetService(void);

	CNetSocket * GetSocket();

	void SetRemoteAddress(const CIPAddress& IPAddress);
	void SetLocalAddress(const CIPAddress& IPAddress);
	CIPAddress& GetRemoteAddress();
	CIPAddress& GetLocalAddress();
	void SetAddressPair(const CIPAddressPair& AddressPair);
	CIPAddressPair& GetAddressPair();

	bool IsWorking();

	
	void SetStop();
	bool IsStop();
	void SetHandler(NetHandlerInterface* h)
	{
		m_NetHandler = h;
	}

	void SetServiceType(UINT Type, UINT Sid)
	{
		ServiceID = Sid;
		ServiceType = Type;
	}

	void SetClientProxy(CLIENT_PROXY_TYPE Type, CLIENT_PROXY_MODE Mode)
    {
        m_ClientProxyType = Type;
        m_ClientProxyMode = Mode;
    }

	CLIENT_PROXY_TYPE GetClientProxyType()
    {
        return m_ClientProxyType; 
    }

	CLIENT_PROXY_MODE GetClientProxyMode()
    {
        return m_ClientProxyMode; 
    }

	//virtual void OnStartUp()=0;
	//virtual void OnClose()=0;
	//virtual void OnRecvData(const CIPAddress& IPAddress, const BYTE * pData, UINT DataSize) = 0;
	//virtual CSmartPtr<CBaseNetConnectionInterface> CreateConnection(CIPAddress& RemoteAddress)=0;
	//virtual bool DeleteConnection(CSmartPtr<CBaseNetConnectionInterface>  pConnection) = 0;
	

protected:
    void OnAccept(CSmartPtr<CBaseNetConnectionInterface> s )
	{
		m_NetHandler->OnAccept(s);
	}
    void OnCreateConnectACK(CSmartPtr<CBaseNetConnectionInterface> s )
	{
		m_NetHandler->OnCreateConnectACK(s);
	}

    void OnDisConnect(CSmartPtr<CBaseNetConnectionInterface> s )
	{
		m_NetHandler->OnDisConnect(s);
	}

    void DisConnectAck(CSmartPtr<CBaseNetConnectionInterface> s )
	{
		//m_NetHandler->DisConnectAck(s);
	}

    void OnMessage(/*xxx*/)
	{
		//m_NetHandler->OnMessage(); //todo?
	}
    void SendMessageAck(/*xxx*/)
	{
		m_NetHandler->SendMessageAck();
	}

};

inline void CBaseNetService::SetStop()
{
	m_StopFlag = true;;
}

inline bool CBaseNetService::IsStop()
{
	return m_StopFlag;
}

inline CNetSocket * CBaseNetService::GetSocket()
{
	return &m_Socket;
}

inline void CBaseNetService::SetRemoteAddress(const CIPAddress& IPAddress)
{
	m_Socket.SetRemoteAddress(IPAddress);
}
inline void CBaseNetService::SetLocalAddress(const CIPAddress& IPAddress)
{
	m_Socket.SetLocalAddress(IPAddress);
}
inline CIPAddress& CBaseNetService::GetRemoteAddress()
{
	return m_Socket.GetRemoteAddress();
}
inline CIPAddress& CBaseNetService::GetLocalAddress()
{
	return m_Socket.GetLocalAddress();
}
inline void CBaseNetService::SetAddressPair(const CIPAddressPair& AddressPair)
{
	m_Socket.SetAddressPair(AddressPair);
}
inline CIPAddressPair& CBaseNetService::GetAddressPair()
{
	return m_Socket.GetAddressPair();
}


inline bool CBaseNetService::IsWorking()
{
	if(m_Socket.GetState()==CNetSocket::SS_LISTENING||
		m_Socket.GetState()==CNetSocket::SS_CONNECTED)
		return TRUE;
	else
		return FALSE;
}