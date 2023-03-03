/****************************************************************************/
/*                                                                          */
/*      文件名:    NetServiceIOCP.h                                         */
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
	public CBaseNetService,public IIOCPEventHandler,public CBaseNetServiceInterface 
{
protected:
	friend class CIOCPListenThread;

	volatile bool								m_WantClose;
	CNetServer *								m_pServer;	
	int											m_CurProtocol;
	int											m_CurAddressFamily;
	bool										m_IPv6Only;
	CCycleQueue< CSmartPtr<COverLappedObject> >			m_OverLappedObjectPool;
	CEasyCriticalSection						m_OverLappedObjectPoolLock;
	CThreadSafeCycleQueue< CSmartPtr<COverLappedObject> >	m_AcceptQueue;
	UINT										m_AcceptQueueSize;
	UINT										m_RecvQueueSize;
	UINT										m_SendQueueSize;
	UINT										m_ParallelAcceptCount;
	UINT										m_ParallelRecvCount;
	bool										m_IsUseListenThread;
	CSmartPtr<CIOCPEventRouter> 							m_pIOCPEventRouter;
	CIOCPListenThread *							m_pListenThread;

	

public:
	CNetService();
	virtual ~CNetService(void);

	



	virtual bool OnIOCPEvent(int EventID,COverLappedObject * pOverLappedObject);
	

	virtual bool Create(int Protocol = IPPROTO_TCP,
		UINT AcceptQueueSize = DEFAULT_SERVER_ACCEPT_QUEUE,
		UINT RecvQueueSize = DEFAULT_SERVER_RECV_DATA_QUEUE,
		UINT SendQueueSize = DEFAULT_SERVER_SEND_DATA_QUEUE,
		UINT ParallelAcceptCount = DEFAULT_PARALLEL_ACCEPT,
		UINT ParallelRecvCount = DEFAULT_PARALLEL_RECV,
		bool IsUseListenThread = false,
		bool IPv6Only = false);
	virtual void Destory();
	
	void Close();
	void QueryClose();



	//virtual CBaseNetConnection * CreateConnection(CIPAddress& RemoteAddress);
	

	bool QueryUDPSend(const CIPAddress& IPAddress, LPCVOID pData, int Size);


public: //CBaseNetServiceInterface	
	virtual bool StartListen(const CIPAddress& Address) override;
	virtual void SetServer(CNetServer * pServer) override;
	virtual CNetServer * GetServer() override;
	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT) override;

	virtual void OnStartUp() override;
	virtual void OnClose() override;
	virtual CSmartPtr<CBaseNetConnectionInterface> CreateConnection(CIPAddress& RemoteAddress) override {return nullptr;}; 
	virtual bool DeleteConnection(CSmartPtr<CBaseNetConnectionInterface> pConnection) override { return false;};
	virtual void OnRecvData(const CIPAddress& IPAddress, const BYTE * pData, UINT DataSize) override;
	virtual void OnRecvData(CSmartPtr<CBaseNetConnectionInterface> pConnection, DOS_SIMPLE_MESSAGE_HEAD* pData ) override { };
	

	virtual void OnConnection(CSmartPtr<CBaseNetConnectionInterface> s, bool IsSucceed) override { };
	virtual void OnDisconnection(CSmartPtr<CBaseNetConnectionInterface> s) override { };
	virtual void OnRecvMessage(CSmartPtr<CBaseNetConnectionInterface> s,  DOS_SIMPLE_MESSAGE_HEAD* pMsg) override { }; 
	
	
protected:
	bool QueryAccept();
	bool QueryUDPRecv();
	bool AcceptSocket(SOCKET Socket);
	bool AcceptSocketEx(SOCKET Socket, CEasyBuffer * pAcceptData);
	CSmartPtr<COverLappedObject> AllocOverLappedObject(int Type);
	bool ReleaseOverLappedObject(CSmartPtr<COverLappedObject> pObject, int Where);
};



inline void CNetService::SetServer(CNetServer * pServer)
{
	m_pServer=pServer;
}

inline CNetServer * CNetService::GetServer()
{
	return m_pServer;
}


