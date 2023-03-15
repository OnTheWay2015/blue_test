#include "stdafx.h"



CProxyConnectionGroup::CProxyConnectionGroup()
{
	m_pService = NULL;
	m_Index = 0;
}


CProxyConnectionGroup::~CProxyConnectionGroup()
{
}


void CProxyConnectionGroup::Init()
{

}


bool CProxyConnectionGroup::RemoveConnection(CSmartPtr<CBaseNetConnectionInterface> pConnection)
{
	CAutoLock Lock(m_EasyCriticalSection);
    m_ConnectionPool.erase(pConnection->GetSessionID());
	return true;
}		
bool CProxyConnectionGroup::AddConnection(CSmartPtr<CBaseNetConnectionInterface> pConnection)
{
	CAutoLock Lock(m_EasyCriticalSection);
	m_ConnectionPool.insert({pConnection->GetSessionID(), pConnection});

	return true;
}		

BOOL CProxyConnectionGroup::OnStart()
{
//	m_ThreadPerformanceCounter.Init(GetThreadHandle(), THREAD_CPU_COUNT_TIME);
	return TRUE;
}

BOOL CProxyConnectionGroup::OnRun()
{
	int ProcessCount = 0;
    for (auto it= m_ConnectionPool.begin();it!=m_ConnectionPool.end();)
    {
		//if (!it->second->IsConnected()) //使用自定义移除标识,主动标记移除.防止并发时处理有异常,比如元素已回收,却还有引用到元素的调用 
		if (it->second->IsStop())
		{
			CAutoLock Lock(m_EasyCriticalSection);
			it = m_ConnectionPool.erase(it);
//		m_pService->QueryDestoryConnection(pConnection);
			continue;
		}
		ProcessCount += it->second->Update();
		it++;
    }


	if (ProcessCount == 0)
	{
		DoSleep(DEFAULT_IDLE_SLEEP_TIME);
	}
	return TRUE;
}

void CProxyConnectionGroup::OnTerminate()
{
}

void CProxyConnectionGroup::SendSessionMessage(CSmartPtr<CoreSessionMessage> msg)
{
	auto SID = msg->SID;
	auto s = m_ConnectionPool.find(SID);
	if (s == m_ConnectionPool.end())
	{
		//todo log err
		return;
	}

    auto m = std::make_shared<DOS_SIMPLE_MESSAGE>();
    m->MsgID = msg->Header->MsgID;
    m->MsgLen = msg->Header->MsgLen;
    m->MSG = msg->Msg;
	
	s->second->SendMsg(m);

	
}

