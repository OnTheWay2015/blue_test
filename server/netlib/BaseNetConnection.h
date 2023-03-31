/****************************************************************************/
/*                                                                          */
/*      文件名:    BaseTCPConnection.h                                      */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CBaseNetConnectionInterface 
{
public:
	virtual bool Create(UINT RecvQueueSize, UINT SendQueueSize) = 0;
	virtual bool Create(SOCKET Socket, UINT RecvQueueSize, UINT SendQueueSize) = 0;

	virtual bool StartWork() = 0;

	virtual void OnConnection(bool IsSucceed) = 0;
	virtual void OnDisconnection()=0;

	virtual void OnRecvData(const BYTE * pData, UINT DataSize) = 0;

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT)=0;



	virtual CNetSocket * GetSocket()=0;
	virtual void SetRemoteAddress(const CIPAddress& IPAddress)=0;
	virtual void SetLocalAddress(const CIPAddress& IPAddress)=0;
	virtual CIPAddress& GetRemoteAddress()=0;
	virtual CIPAddress& GetLocalAddress()=0;
	virtual void SetAddressPair(const CIPAddressPair& AddressPair)=0;
	virtual CIPAddressPair& GetAddressPair()=0;
	
	virtual void SetStop() =0;
	virtual bool IsStop() =0;

	virtual void SetService(CBaseNetServiceInterface* pService )=0;
	virtual void SetClientProxy(CLIENT_PROXY_TYPE Type, CLIENT_PROXY_MODE Mode)=0;
	virtual CLIENT_PROXY_TYPE GetClientProxyType()=0;
	virtual CLIENT_PROXY_MODE GetClientProxyMode()=0;
	virtual SESSION_ID GetSessionID()=0;
	virtual bool SendMsg(CSmartPtr<DOS_SIMPLE_MESSAGE> msg) = 0;

}; 
class CBaseNetConnection :
	public CBaseNetConnectionInterface 
	,public CNameObject
{
protected:
	CNetSocket								m_Socket;
	bool m_StopFlag;
    CLIENT_PROXY_TYPE	m_ClientProxyType;
    CLIENT_PROXY_MODE	m_ClientProxyMode;
    CBaseNetServiceInterface* m_pService;
public:
	CBaseNetConnection(void);
	virtual ~CBaseNetConnection(void);

public:
	bool IsConnected();
	bool IsDisconnected();
	bool PrintConnectionLogWithTag(LPCTSTR Tag, LPCTSTR Format, ...);

	bool PrintConnectionDebugLogWithTag(LPCTSTR Tag, LPCTSTR Format, ...);
public: //CBaseNetConnectionInterface 
	virtual void SetService(CBaseNetServiceInterface* pService ) override { m_pService =pService; };
	virtual CNetSocket * GetSocket() override ;
	virtual void SetRemoteAddress(const CIPAddress& IPAddress) override ;
	virtual void SetLocalAddress(const CIPAddress& IPAddress) override ;
	virtual CIPAddress& GetRemoteAddress() override ;
	virtual CIPAddress& GetLocalAddress() override ;
	virtual void SetAddressPair(const CIPAddressPair& AddressPair) override ;
	virtual CIPAddressPair& GetAddressPair() override ;

	virtual void SetStop() override ;
	virtual bool IsStop() override ;

	virtual bool SendMsg(CSmartPtr<DOS_SIMPLE_MESSAGE> msg) override { return false;};
	virtual bool Create(UINT RecvQueueSize, UINT SendQueueSize)  override { return false;};
	virtual bool Create(SOCKET Socket, UINT RecvQueueSize, UINT SendQueueSize) override  {return false;};

	virtual bool StartWork()  override {return false;};

	virtual void OnConnection(bool IsSucceed) override  {};
	virtual void OnDisconnection() override {};

	virtual void OnRecvData(const BYTE * pData, UINT DataSize) override  {};

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT){return 0;};


	virtual void SetClientProxy(CLIENT_PROXY_TYPE Type, CLIENT_PROXY_MODE Mode)
    {
        m_ClientProxyType = Type;
        m_ClientProxyMode = Mode;
    }

	virtual CLIENT_PROXY_TYPE GetClientProxyType()
    {
        return m_ClientProxyType; 
    }

	virtual CLIENT_PROXY_MODE GetClientProxyMode()
    {
        return m_ClientProxyMode; 
    }

	virtual SESSION_ID GetSessionID()
	{
		return m_Socket.GetSocket();
	}

};

inline void CBaseNetConnection::SetStop()
{
	m_StopFlag = true;;
}

inline bool CBaseNetConnection::IsStop()
{
	return m_StopFlag;
}


inline CNetSocket * CBaseNetConnection::GetSocket()
{
	return &m_Socket;
}

inline void CBaseNetConnection::SetRemoteAddress(const CIPAddress& IPAddress)
{
	m_Socket.SetRemoteAddress(IPAddress);
}
inline void CBaseNetConnection::SetLocalAddress(const CIPAddress& IPAddress)
{
	m_Socket.SetLocalAddress(IPAddress);
}
inline CIPAddress& CBaseNetConnection::GetRemoteAddress()
{
	return m_Socket.GetRemoteAddress();
}
inline CIPAddress& CBaseNetConnection::GetLocalAddress()
{
	return m_Socket.GetLocalAddress();
}
inline void CBaseNetConnection::SetAddressPair(const CIPAddressPair& AddressPair)
{
	m_Socket.SetAddressPair(AddressPair);
}
inline CIPAddressPair& CBaseNetConnection::GetAddressPair()
{
	return m_Socket.GetAddressPair();
}

inline bool CBaseNetConnection::IsConnected()
{
	return m_Socket.IsConnected();
}

inline bool CBaseNetConnection::IsDisconnected()
{
	return m_Socket.IsDisconnected();
}

inline bool CBaseNetConnection::PrintConnectionLogWithTag(LPCTSTR szFunction, LPCTSTR Format, ...)
{
	//TCHAR Tag[256];
	//_stprintf_s(Tag, 256, _T("%s(%u)"), szFunction, GetID());

	//va_list vl;
	//va_start(vl, Format);
	//bool ret = CLogManager::GetInstance()->PrintLogVL(LOG_NET_CHANNEL, ILogPrinter::LOG_LEVEL_NORMAL, Tag, Format, vl);
	//va_end(vl);
	//return ret;
	//return false;

	va_list vl;
	va_start(vl,Format);
	BOOL ret = CLogManager::GetInstance()->PrintLogVL(LOG_NET_CHANNEL, ILogPrinter::LOG_LEVEL_NORMAL, szFunction, Format, vl);
	va_end(vl);
	return true;
}

inline bool CBaseNetConnection::PrintConnectionDebugLogWithTag(LPCTSTR szFunction, LPCTSTR Format, ...)
{
	//TCHAR Tag[256];
	//_stprintf_s(Tag, 256, _T("%s(%u)"), szFunction, GetID());

	//va_list vl;
	//va_start(vl, Format);
	//bool ret = CLogManager::GetInstance()->PrintLogVL(LOG_NET_CHANNEL, ILogPrinter::LOG_LEVEL_DEBUG, Tag, Format, vl);
	//va_end(vl);
	//return ret;
	va_list vl;
	va_start(vl,Format);
	BOOL ret = CLogManager::GetInstance()->PrintLogVL(LOG_NET_CHANNEL, ILogPrinter::LOG_LEVEL_NORMAL, szFunction, Format, vl);
	va_end(vl);

    return false;
}