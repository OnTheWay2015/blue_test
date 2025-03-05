
#include "stdafx.h"



CoreNetManager::CoreNetManager()
{

}
CoreNetManager::~CoreNetManager()
{

}

bool CoreNetManager::Init(CoreBase* base, CoreConfig& config)
{
    m_CoreBase = base;
    m_NetServer = std::make_shared<CNetServer>();
    m_NetServer->SetHandler(shared_from_this());


    //t1::MSG_TEST a;
    //a.set_name("test");

    int EventObjectPoolSize = 10;
    int WorkThreadCount = 1;
    int EventRouterPoolSiz = 10;
    int EventObjectPoolGrowSize = 10;
    int EventObjectPoolGrowLimit = 10;
    int EventRouterPoolGrowSize = 10;
    int EventRouterPoolGrowlimit = 10;

    m_NetServer->StartUp(EventObjectPoolSize,
        WorkThreadCount,
        EventRouterPoolSiz,
        EventObjectPoolGrowSize,
        EventObjectPoolGrowLimit,
        EventRouterPoolGrowSize,
        EventRouterPoolGrowlimit);//有初始化网络线程, iocp 在网络线程里读取网卡数据


    auto& servicesConfig = config.GetServiceList();
    for (auto& sconf: servicesConfig )
	{
		auto& clientProxys = sconf.m_ClientProxys;
		for (auto& conf : clientProxys)
		{
			auto ss = std::make_shared<CProxyServiceDefault >();
			ss->SetServer(m_NetServer.get());
			ss->SetClientProxy(conf.ProxyMode);
            ss->SetService((SERVICE_TYPE)sconf.ServiceType,sconf.ServiceID);

			if (!ss->Create(
				IPPROTO_TCP,
				16,       // AcceptQueueSize
				1024 * 5, // RecvBufferSize
				1024 * 5, // SendBufferSize
				1,        // ParallelAcceptCount
				64,       // DEFAULT_PARALLEL_RECV
				false     // IPv6Only
			))
			{

				// PrintDOSLog( _T("代理服务[%u]创建失败！"), GetID());
				continue;
			}


			//ss->Init(); 

			CIPAddress addr;
			addr.SetIP(conf.ListenIP.c_str());
			// addr.SetIP("127.0.0.1");
			addr.SetPort(conf.ListenPort);
			if (!ss->StartListen(addr)) // m_Config.ListenAddress
			{
				// PrintDOSLog( _T("代理服务[%u]侦听于(%s:%u)失败！"),
				//	GetID(),
				//	m_Config.ListenAddress.GetIPString(),
				//	m_Config.ListenAddress.GetPort());
				continue;
			}

			ss->SetHandler(this);
			m_NetServices.push_back(ss);
		}
	}

    
    m_NetServiceActConnect = std::make_shared<ServiceActConnect>();
    m_NetServiceActConnect->Init();
    m_NetServiceActConnect->SetServer(m_NetServer.get());
    m_NetServiceActConnect->SetHandler(this);



    int connectionGroupCount = 5;
    for (int i=0;i<connectionGroupCount;i++)
    {
		auto p = std::make_shared<CProxyConnectionGroup>();
		p->SetID(i);
		p->Init();
		m_ConnectionGroups.push_back(p);
        p->Start();
    }
    return true;
}



void CoreNetManager::Update()
{
    for (auto& s : m_NetServices)
    {
        s->Update(10);
    }
  
    m_NetServiceActConnect->Update(5);

    UpdateProcessMessage(1);
}


//接受连接(服务器)
void CoreNetManager::OnAccept(CSmartPtr<CBaseNetConnectionInterface> s)
{
    AddConnection(s);
}

//主动创建连接使用(客户端)
void CoreNetManager::OnCreateConnectACK(CSmartPtr<CBaseNetConnectionInterface> s)
{
    AddConnection(s);
}

void CoreNetManager::OnDisConnect(CSmartPtr<CBaseNetConnectionInterface> s)
{
    RemoveConnection(s);
}
void CoreNetManager::DisConnectAck(SESSION_ID SessionID)
{
}

void CoreNetManager::OnMessage(CSmartPtr<CBaseNetConnectionInterface> c,  DOS_SIMPLE_MESSAGE_HEAD* Msg) //收到消息
{
    auto m = std::make_shared<CoreSessionMessage>();
    auto h = std::make_shared<DOS_SIMPLE_MESSAGE_HEAD>();
    m->EventType = CORE_EVENT::SESSION_MESSAGE;
    m->SID = c->GetSessionID();
    m->ServiceType = c->GetServiceType();
    m->ClientProxyMode = c->GetClientProxyMode();
    m->Session = c;
    *h = *Msg;
    m->Header= h;
    auto ProtoMsg = m_pMessageHandler->ParseMessage(Msg); //todo pb message
    m->Msg = ProtoMsg;
    PushMessage(m);
}

void CoreNetManager::SendMessageAck(/*xxx*/)
{
}


//CSmartPtr<CBaseNetConnectionInterface> CoreNetManager::CreateConnection(CIPAddress& RemoteAddress,ServiceType type,CLIENT_PROXY_MODE mode)
//{
//	auto pConnection = std::make_shared<CProxyConnectionDefault>();
//	pConnection->Init();
//    pConnection->SetClientProxy(type,mode);
//
//	//pConnection->SetService(this);
//	return pConnection;
//}

CSmartPtr<CBaseNetConnectionInterface> CoreNetManager::CreateConnection(CIPAddress& remoteAddress,SERVICE_TYPE type,CLIENT_PROXY_MODE mode)
{
	return m_NetServiceActConnect->CreateConnection(remoteAddress,type,mode);
}

void CoreNetManager::ProcessQueueMessageHandler(CSmartPtr<CoreSessionMessage> msg)
{
   m_CoreBase->OnNetMessage(msg); 
}


void CoreNetManager::RemoveConnection(CSmartPtr<CBaseNetConnectionInterface> c)
{
    auto m = std::make_shared<CoreSessionMessage>();
    m->EventType = CORE_EVENT::SESSION_REMOVE;
    //m->SID= c->GetSessionID(); 
    m->Session = c;
    m->ServiceType = c->GetServiceType();


    UINT Index = c->GetSessionID() % m_ConnectionGroups.size();
    m_ConnectionGroups[Index]->RemoveConnection(c);


    //if (m_ConnectionGroups.size() > 0) //通过 connection 的 stop 状态,在 group里检测删除
    //{
    //    UINT Index = c->GetSessionID() % m_ConnectionGroups.size();
    //    m_ConnectionGroups[Index]->RemoveConnection(c);
    //}
    //else
    //{
    //    //PrintDOSLog(_T("未能找到合适的连接组"));
    //}

    PushMessage(m);
}

void CoreNetManager::AddConnection(CSmartPtr<CBaseNetConnectionInterface> c)
{
    if (m_ConnectionGroups.size() > 0)
    {
        UINT Index = c->GetSessionID() % m_ConnectionGroups.size();
        m_ConnectionGroups[Index]->AddConnection(c);

        auto m = std::make_shared<CoreSessionMessage>();
        m->EventType = CORE_EVENT::SESSION_ADD;
        m->SID= c->GetSessionID(); 
        m->ServiceType = c->GetServiceType();
        m->ServiceID = c->GetServiceID();
        m->Session = c;
        PushMessage(m);
    }
    else
    {
        //c->Disconnect();
        //PrintDOSLog(_T("未能找到合适的连接组"));
    }
}

void CoreNetManager::SetParseMessageHandler(MessageParseInterface* h)
{
    m_pMessageHandler = h;
}

MessageParseInterface* CoreNetManager::GetParseMessageHandler() 
{
    return m_pMessageHandler ;
}

void CoreNetManager::SendSessionMessage(CSmartPtr<CoreSessionMessage> msg)
{
    if (m_ConnectionGroups.size() <= 0)
    {
        return;
    }

    UINT Index = msg->SID;
    //m_ConnectionGroups[Index]->;


}

CoreBase* CoreNetManager::GetCore()
{
    return m_CoreBase;
}





