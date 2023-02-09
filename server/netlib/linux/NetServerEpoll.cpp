/****************************************************************************/
/*                                                                          */
/*      文件名:    NetServerEpoll.cpp                                       */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "../stdafx.h"


CNetServer::CNetServer(void):CBaseNetServer()
{
	m_EventObjectPoolSize=DEFAULT_EVENT_OBJECT_COUNT;
	m_EventObjectPoolGrowSize=DEFAULT_EVENT_OBJECT_POOL_GROW_SIZE;
	m_EventObjectPoolGrowLimit=DEFAULT_EVENT_OBJECT_POOL_GROW_LIMIT;
	m_EventRouterPoolSize=DEFAULT_EVENT_ROUTER_COUNT;
	m_EventRouterPoolGrowSize=DEFAULT_EVENT_ROUTER_POOL_GROW_SIZE;
	m_EventRouterPoolGrowLimit=DEFAULT_EVENT_ROUTER_POOL_GROW_LIMIT;
	m_EpollWordThreadCount = DEFAULT_WORK_THREAD_COUNT;
}

CNetServer::~CNetServer(void)
{
	ShutDown();
}

bool CNetServer::StartUp(int EventObjectPoolSize,
						int WorkThreadCount,
						int EventRouterPoolSiz,
						int EventObjectPoolGrowSize,
						int EventObjectPoolGrowLimit,
						int EventRouterPoolGrowSize,
						int EventRouterPoolGrowlimit)
{
	m_EventObjectPoolSize=EventObjectPoolSize;
	m_EventRouterPoolSize=EventRouterPoolSiz;
	m_EpollWordThreadCount = WorkThreadCount;
	m_EventObjectPoolGrowSize=EventObjectPoolGrowSize;
	m_EventObjectPoolGrowLimit=EventObjectPoolGrowLimit;
	m_EventRouterPoolGrowSize=EventRouterPoolGrowSize;
	m_EventRouterPoolGrowLimit=EventRouterPoolGrowlimit;
	return Start() != FALSE;
}

void CNetServer::ShutDown(DWORD Milliseconds)
{
	SafeTerminate(Milliseconds);
}

BOOL CNetServer::OnStart()
{
	if(!CBaseNetServer::OnStart())
		return false;






	m_EpollThreadList.resize(m_EpollWordThreadCount);

	for (UINT i = 0; i < m_EpollThreadList.size(); i++)
	{
		m_EpollThreadList[i].Init(this);
		m_EpollThreadList[i].Start();
	}
	PrintNetLog("已创建%u个epoll工作线程", (UINT)m_EpollThreadList.size());
	if(!OnStartUp())
	{
		return false;
	}
	return true;
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
	for (UINT i = 0; i < m_EpollThreadList.size(); i++)
	{
		m_EpollThreadList[i].SafeTerminate();
	}
	m_EpollThreadList.clear();

	OnShutDown();



	m_EventRouterPool.clear();
}


CSmartPtr<CEpollEventRouter > CNetServer::CreateEventRouter()
{
	CSmartPtr<CEpollEventRouter> pEventRouter = std::make_shared<CEpollEventRouter>();
	static UINT ID = 0;
	ID++;

	pEventRouter->SetID(ID);
	m_EventRouterPool.push_back(pEventRouter);
	return pEventRouter;
	return NULL;
}

CSmartPtr<CEpollEventRouter> CNetServer::GetEventRouter(UINT ID)
{
	for (auto& v : m_EventRouterPool)
	{
		if (v->GetID() == ID)
		{
			return v;
		}
	}
	return nullptr;
}

bool CNetServer::DeleteEventRouter(CSmartPtr<CEpollEventRouter > pEventRouter)
{

	pEventRouter->Destory();
	for (auto it=m_EventRouterPool.begin();it!=m_EventRouterPool.end();it++)
	{
		if (*it == pEventRouter)
		{
			m_EventRouterPool.erase(it);
			return true;
		}
	}
	return false;
}


bool CNetServer::BindSocket(SOCKET Socket, CSmartPtr<CEpollEventRouter> pEpollEventRouter, bool UseLTMode)
{
	CEpollThread* pEpollThread = nullptr;
	int BindCout = 0;
	for (UINT i = 0; i < m_EpollThreadList.size(); i++)
	{
		if (pEpollThread)
		{
			int TestCount = (int)pEpollThread->GetBindCount();
			if (TestCount < 0)
			{
				PrintNetLog( "(%d)异常线程绑定的端口数是负数！", GetID());
			}
			if (TestCount < BindCout)
			{
				pEpollThread = &m_EpollThreadList[i];
				BindCout = TestCount;
			}
		}
		else
		{
			pEpollThread = &m_EpollThreadList[i];
			BindCout = (int)pEpollThread->GetBindCount();
		}
	}

	if (pEpollThread)
	{
		if (pEpollThread->BindSocket(Socket, pEpollEventRouter, UseLTMode))
		{
			pEpollEventRouter->SetEpollThread(pEpollThread);
			return true;
		}
	}
	return false;
}


bool CNetServer::UnbindSocket(SOCKET Socket, CSmartPtr<CEpollEventRouter> pEpollEventRouter)
{
	if(pEpollEventRouter)
	{
		if (pEpollEventRouter->GetEpollThread())
		{
			if (pEpollEventRouter->GetEpollThread()->UnbindSocket(Socket))
			{
				pEpollEventRouter->SetEpollThread(NULL);
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			PrintNetLog("(%d)EpollEventRouter没有设置EpollThread,无法解绑Epoll事件！", GetID());
			return false;
		}
	}
	else
	{
		return false;
	}
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
	return ProcessCount;
}


void CNetServer::PrintObjectStatus()
{

	//int AcceptCount = 0;
	//int RecvCount = 0;
	//int SendCount = 0;
	//int OtherCount = 0;

	//LPVOID Pos = m_EpollEventObjectPool.GetFirstObjectPos();
	//while (Pos)
	//{
	//	CEpollEventObject * pObject = m_EpollEventObjectPool.GetNextObject(Pos);
	//	switch (pObject->GetType())
	//	{
	//	case IO_RECV:
	//		RecvCount++;
	//		break;
	//	case IO_SEND:
	//		SendCount++;
	//		break;
	//	case IO_ACCEPT:
	//		AcceptCount++;
	//		break;
	//	default:
	//		OtherCount++;
	//	}
	//}

	//UINT UsedCount = 0, FreeCount = 0;

	//m_EpollEventObjectPool.Verfy(UsedCount, FreeCount);

	//PrintNetLog("有%d(%u,%u,%u)个EpollEventObject对象使用中,其中Accept=%d,Recv=%d,Send=%d,Other=%d",
	//	m_EpollEventObjectPool.GetObjectCount(),
	//	UsedCount, FreeCount, UsedCount + FreeCount,
	//	AcceptCount,
	//	RecvCount,
	//	SendCount,
	//	OtherCount);
	//PrintNetLog("有%d个EventRouter使用中", m_EventRouterPool.GetObjectCount());
}
