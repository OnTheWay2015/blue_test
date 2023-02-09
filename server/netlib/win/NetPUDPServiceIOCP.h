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
	public CBaseNetService,public IIOCPEventHandler
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
	

	void SetServer(CNetServer * pServer);

	CNetServer * GetServer();

	virtual bool OnIOCPEvent(int EventID, COverLappedObject * pOverLappedObject);
	

	virtual bool Create(UINT ParallelRecvCount = DEFAULT_PARALLEL_RECV);
	virtual void Destory();
	
	bool StartListen(const CIPAddress& Address);
	void Close();	

	virtual void OnStartUp();
	virtual void OnClose();

	bool QueryUDPSend(const CIPAddress& IPAddress, LPCVOID pData, int Size);

	
protected:	
	bool QueryUDPRecv();
	CSmartPtr<COverLappedObject> AllocOverLappedObject();
	bool ReleaseOverLappedObject(CSmartPtr<COverLappedObject> pObject);
};


inline CNetServer * CNetPUDPService::GetServer()
{
	return m_pServer;
}

