/****************************************************************************/
/*                                                                          */
/*      文件名:    NetServerIOCP.cpp                                        */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "../StdAfx.h"


CNetServer::CNetServer(void):CBaseNetServer()
{
	CNetSocket::NetStartup();
	m_hIOCP=NULL;	
	m_pIOCPThreads=NULL;	
	m_IOCPObjectPoolSize=DEFAULT_EVENT_OBJECT_COUNT;
	m_IOCPObjectPoolGrowSize=DEFAULT_EVENT_OBJECT_POOL_GROW_SIZE;
	m_IOCPObjectPoolGrowLimit=DEFAULT_EVENT_OBJECT_POOL_GROW_LIMIT;
	m_EventRouterPoolSize=DEFAULT_EVENT_ROUTER_COUNT;
	m_EventRouterPoolGrowSize=DEFAULT_EVENT_ROUTER_POOL_GROW_SIZE;
	m_EventRouterPoolGrowLimit=DEFAULT_EVENT_ROUTER_POOL_GROW_LIMIT;
	m_IOCPWorkThreadCount = DEFAULT_WORK_THREAD_COUNT;
}

CNetServer::~CNetServer(void)
{
	ShutDown();	
	CNetSocket::NetCleanup();
}

bool CNetServer::StartUp(int EventObjectPoolSize,
						int WorkThreadCount,
						int EventRouterPoolSiz,
						int EventObjectPoolGrowSize,
						int EventObjectPoolGrowLimit,
						int EventRouterPoolGrowSize,
						int EventRouterPoolGrowlimit)
{
	m_IOCPObjectPoolSize=EventObjectPoolSize;
	m_EventRouterPoolSize=EventRouterPoolSiz;
	m_IOCPWorkThreadCount = WorkThreadCount;
	m_IOCPObjectPoolGrowSize=EventObjectPoolGrowSize;
	m_IOCPObjectPoolGrowLimit=EventObjectPoolGrowLimit;
	m_EventRouterPoolGrowSize=EventRouterPoolGrowSize;
	m_EventRouterPoolGrowLimit=EventRouterPoolGrowlimit;
	return Start() != FALSE;;
}

void CNetServer::ShutDown(DWORD Milliseconds)
{	
	SafeTerminate(Milliseconds);	
}

BOOL CNetServer::OnStart()
{
	if(!CBaseNetServer::OnStart())
		return FALSE;

	if(m_hIOCP!=NULL)
		return FALSE;

	CNetSocket::NetStartup();


	m_hIOCP = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );
	if( m_hIOCP == NULL )
	{
		PrintNetLog(_T("(%d)创建完成端口失败(%d)！"),GetID(),GetLastError());		
		return FALSE;
	}

	if (m_IOCPWorkThreadCount <= 0)
	{
		PrintNetLog(_T("非法的IOCP工作线程数量"));
		return FALSE;
	}

	m_pIOCPThreads = MONITORED_NEW_ARRAY(_T("CNetService"), CIOCPThread, m_IOCPWorkThreadCount);

	for (int i = 0; i < m_IOCPWorkThreadCount; i++)
	{
		m_pIOCPThreads[i].SetIOCPHandle(m_hIOCP);
		m_pIOCPThreads[i].Start();
	}	
	PrintNetLog(_T("已创建%d个IOCP工作线程"), m_IOCPWorkThreadCount);
	if(!OnStartUp())
	{		
		return FALSE;
	}
	return TRUE;
}

BOOL CNetServer::OnRun()
{
	if(!CBaseNetServer::OnRun())
		return FALSE;

	if(Update()==0)
	{
		DoSleep(DEFAULT_IDLE_SLEEP_TIME);
	}
	return TRUE;
}

void CNetServer::OnTerminate()
{	
	if( m_pIOCPThreads )
	{
		for (int i = 0; i < m_IOCPWorkThreadCount; i++)
		{			
			m_pIOCPThreads[i].SafeTerminate();
		}
		MONITORED_DELETE_ARRAY(m_pIOCPThreads);
		m_pIOCPThreads = NULL;
	}		

	OnShutDown();

	CloseHandle( m_hIOCP );
	m_hIOCP = NULL;	

	CAutoLock Lock(m_OverLappedObjectPoolLock);

	if(m_OverLappedObjectPool.size())
	{		
		PrintNetLog(_T("(%d)关闭，开始统计OverLappedObject使用状况！"),GetID());
		PrintObjectStatus();
	}

	m_OverLappedObjectPool.clear();	
	m_EventRouterPool.clear();

	CNetSocket::NetCleanup();
}

CSmartPtr<COverLappedObject >CNetServer::AllocOverLappedObject(UINT ParantID)
{
	CAutoLock Lock(m_OverLappedObjectPoolLock);

    auto pOverLappedObject = std::make_shared<COverLappedObject >();
    pOverLappedObject->Create(this);
    pOverLappedObject->SetParentID(ParantID);
    pOverLappedObject->SetStatus(OVERLAPPED_OBJECT_STATUS_IDLE);
    m_OverLappedObjectPool.push_back(pOverLappedObject);
	return pOverLappedObject;
}


bool CNetServer::ReleaseOverLappedObject(UINT ParantID)
{
    CAutoLock Lock(m_OverLappedObjectPoolLock);

    for (auto it = m_OverLappedObjectPool.begin(); it != m_OverLappedObjectPool.end(); )
    {
        if ((*it)->GetParentID() == ParantID)
        {
            it = m_OverLappedObjectPool.erase(it);
        }
        else
        {
            it++;
        }
    }
    return true;
}

bool CNetServer::ReleaseOverLappedObject( CSmartPtr<COverLappedObject> obj)
{
	CAutoLock Lock(m_OverLappedObjectPoolLock);

    for (auto it=m_OverLappedObjectPool.begin();it!=m_OverLappedObjectPool.end();it++)
    {
        if ((*it)== obj)
        {
            m_OverLappedObjectPool.erase(it);
            return true;
        }
    }
	
	return false;
}

CSmartPtr<CIOCPEventRouter> CNetServer::CreateEventRouter()
{
	auto pEventRouter=std::make_shared<CIOCPEventRouter >();
	m_EventRouterPool.push_back(pEventRouter);
	return pEventRouter;
}

bool CNetServer::DeleteEventRouter(CSmartPtr<CIOCPEventRouter >pEventRouter)
{
	//pEventRouter->Destory();
m_EventRouterPoolDel.push_back(pEventRouter); 

/*
CIOCPEventRouter 在 OnIOCPEnent 处理时有可能 Connection 会销毁
	会导致 CIOCPEventRouter 没了引用触发析构 CIOCPEventRouter::Destory 此时和 CIOCPEventRouter::OnIOCPEnent 进入了同一个锁,死锁.
为避免这种情况，把 Destory 的操作放到另一处
 
*/

    //for (auto it=m_EventRouterPool.begin();it!=m_EventRouterPool.end();it++)
    //{
    //    if ( *it == pEventRouter)
    //    {
    //        m_EventRouterPool.erase(it);
    //        return true;
    //    }
    //}
	return TRUE;
}


void CNetServer::DeleteEventRouterProcess()
{
    if (m_EventRouterPoolDel.size() <= 0)
    {
        return;
    }


    for (auto d : m_EventRouterPoolDel)
    {
        for (auto it = m_EventRouterPool.begin(); it != m_EventRouterPool.end();)
        {
            if (*it == d)
            {
                it = m_EventRouterPool.erase(it);
            }
            else
            {
                it++;
            }
        }
    }
    m_EventRouterPoolDel.clear();
}


bool CNetServer::BindSocket(SOCKET Socket)
{
	if(m_hIOCP == NULL)
	{
		PrintNetLog(_T("(%d)完成端口没有初始化,无法绑定Socket！"),GetID());		
		return false;
	}
	if(Socket == INVALID_SOCKET)
	{
		PrintNetLog(_T("(%d)Socket没有初始化,无法绑定Socket！"),GetID());		
		return false;
	}
	HANDLE hPort = CreateIoCompletionPort((HANDLE)Socket, m_hIOCP, 0, 0 );
	return true;
}

bool CNetServer::BindFile(HANDLE FileHandle)
{
	if(m_hIOCP == NULL)
	{
		PrintNetLog(_T("(%d)完成端口没有初始化,无法绑定FileHandle！"),GetID());		
		return false;
	}
	if(FileHandle == INVALID_HANDLE_VALUE)
	{
		PrintNetLog(_T("(%d)FileHandle没有初始化,无法绑定FileHandle！"),GetID());		
		return false;
	}
	HANDLE hPort = CreateIoCompletionPort(FileHandle, m_hIOCP, 0, 0 );
	return true;
}

bool CNetServer::OnStartUp()
{
	return true;
}
void CNetServer::OnShutDown()
{
	
}

int CNetServer::Update(int ProcessPacketLimit)
{
	int ProcessCount=0;	
    DeleteEventRouterProcess();
    return ProcessCount;
}


void CNetServer::PrintObjectStatus()
{
	CAutoLock Lock(m_OverLappedObjectPoolLock);

	int AcceptCount=0;
	int RecvCount=0;
	int SendCount=0;
	int OtherCount=0;
	
	for (auto pObject : m_OverLappedObjectPool)
	{
		switch(pObject->GetType())
		{
		case IO_RECV:
			RecvCount++;			
			break;
		case IO_SEND:
			SendCount++;			
			break;
		case IO_ACCEPT:
			AcceptCount++;			
			break;
		default:
			OtherCount++;			
		}
	}

	UINT UsedCount=0,FreeCount=0;

	//m_OverLappedObjectPool.Verfy(UsedCount,FreeCount);

	PrintNetLog(_T("有%d(%u,%u,%u)个OverLapped对象使用中,其中Accept=%d,Recv=%d,Send=%d,Other=%d"),
		m_OverLappedObjectPool.size(),
		UsedCount,FreeCount,UsedCount+FreeCount,
		AcceptCount,
		RecvCount,
		SendCount,
		OtherCount);
	PrintNetLog(_T("有%d个EventRouter使用中"),m_EventRouterPool.size());
}