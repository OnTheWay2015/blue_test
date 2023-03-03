/****************************************************************************/
/*                                                                          */
/*      文件名:    NetServiceEpoll.h                                        */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


class CNetServer;

class CNetService :	
	public CBaseNetService,public IEpollEventHandler,public CBaseNetServiceInterface 
{
protected:
	volatile bool								m_WantClose;
	volatile bool								m_IsEventProcessing;
	CNetServer *								m_pServer;	
	int											m_CurProtocol;	
	int											m_CurAddressFamily;
	bool										m_IPv6Only;
	CCycleQueue<CSmartPtr<CEpollEventObject> >			m_RecvQueue;
	CCycleQueue<CSmartPtr<CEpollEventObject> >				m_SendQueue;
	CCycleQueue<SOCKET>							m_AcceptQueue;
	CEasyCriticalSection						m_SendLock;
	int											m_AcceptQueueSize;
	int											m_RecvQueueSize;
	int											m_SendQueueSize;
	int											m_ParallelAcceptCount;
	int											m_ParallelRecvCount;
	CSmartPtr<CEpollEventRouter> 				m_pEpollEventRouter;

	

public:
	CNetService();
	virtual ~CNetService(void);

	



	virtual bool OnEpollEvent(UINT EventID);
	

	virtual bool Create(int Protocol = IPPROTO_TCP,
		int AcceptQueueSize=DEFAULT_SERVER_ACCEPT_QUEUE,
		int RecvQueueSize=DEFAULT_SERVER_RECV_DATA_QUEUE,
		int SendQueueSize=DEFAULT_SERVER_SEND_DATA_QUEUE,
		int ParallelAcceptCount=DEFAULT_PARALLEL_ACCEPT,
		int ParallelRecvCount=DEFAULT_PARALLEL_RECV,
		bool IsUseListenThread = false,
		bool IPv6Only = false);
	virtual void Destory();
	
	void Close();
	void QueryClose();

	virtual void OnStartUp();
	virtual void OnClose();

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);

	virtual CSmartPtr<CBaseNetConnection> CreateConnection(CIPAddress& RemoteAddress);
	virtual bool DeleteConnection(CBaseNetConnection * pConnection);
	

	bool QueryUDPSend(const CIPAddress& IPAddress,LPCVOID pData,int Size);

	virtual void OnRecvData(const CIPAddress& IPAddress, const BYTE * pData, UINT DataSize);

	

public:	
	virtual bool StartListen(const CIPAddress& Address);
	virtual void SetServer(CNetServer * pServer);
	virtual CNetServer * GetServer();
	
	
protected:	
	
	bool AcceptSocket(SOCKET Socket);

	void DoAcceptSocket();
	void DoUDPRecv();
	void DoUDPSend();
};



inline void CNetService::SetServer(CNetServer * pServer)
{
	m_pServer=pServer;
}

inline CNetServer * CNetService::GetServer()
{
	return m_pServer;
}


