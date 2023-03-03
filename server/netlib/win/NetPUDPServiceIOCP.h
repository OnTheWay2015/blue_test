/****************************************************************************/
/*                                                                          */
/*      文件名:    NetPUDPServiceIOCP.h                                     */
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

class CNetPUDPService :	
	public CBaseNetService,public IIOCPEventHandler,public CBaseNetServiceInterface	
{
protected:
	volatile bool							m_WantClose;
	CNetServer *							m_pServer;
	UINT									m_ParallelRecvCount;
	CSmartPtr<CIOCPEventRouter> 			m_pIOCPEventRouter;

	CCycleQueue<CSmartPtr<COverLappedObject>> m_OverLappedObjectPool;
	CEasyCriticalSection					m_OverLappedObjectPoolLock;
	

public:
	CNetPUDPService();
	virtual ~CNetPUDPService(void);
	



	virtual bool OnIOCPEvent(int EventID, COverLappedObject * pOverLappedObject);
	

	virtual bool Create(UINT ParallelRecvCount = DEFAULT_PARALLEL_RECV);
	virtual void Destory();
	
	//CNetServer * GetServer();
	//void SetServer(CNetServer * pServer);
	//bool StartListen(const CIPAddress& Address);
	//virtual void OnStartUp();
	//virtual void OnClose();
	void Close();	


	bool QueryUDPSend(const CIPAddress& IPAddress, LPCVOID pData, int Size);

public: //CBaseNetServiceInterface	
	virtual bool StartListen(const CIPAddress& Address) override;
	virtual void SetServer(CNetServer * pServer) override;
	virtual CNetServer * GetServer() override;
	virtual void OnStartUp();
	virtual void OnClose();
	virtual void OnRecvData(const CIPAddress& IPAddress, const BYTE * pData, UINT DataSize) override; //todo 

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT) override {return 0;}
	virtual CSmartPtr<CBaseNetConnectionInterface> CreateConnection(CIPAddress& RemoteAddress) override {return nullptr;}; 
	virtual bool DeleteConnection(CSmartPtr<CBaseNetConnectionInterface> pConnection) override { return false;};


	
	virtual void OnConnection(CSmartPtr<CBaseNetConnectionInterface> s, bool IsSucceed) {} //todo;
	virtual void OnDisconnection(CSmartPtr<CBaseNetConnectionInterface> s) {}//todo;
	virtual void OnRecvMessage(CSmartPtr<CBaseNetConnectionInterface> s, const BYTE * pData, UINT DataSize) {}//todo; 
protected:	
	bool QueryUDPRecv();
	CSmartPtr<COverLappedObject> AllocOverLappedObject();
	bool ReleaseOverLappedObject(CSmartPtr<COverLappedObject> pObject);
};


inline CNetServer * CNetPUDPService::GetServer()
{
	return m_pServer;
}

