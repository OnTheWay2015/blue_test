#include "stdafx.h"


ServiceActConnect::ServiceActConnect()
{
}
ServiceActConnect::~ServiceActConnect()
{
}
void ServiceActConnect::Init()
{
}
int ServiceActConnect::Update(int count)
{
    //for (auto& v : m_Connections)
    //{
    //   v.second->Update(count); 
    //}
return 0;
}


CSmartPtr<CBaseNetConnectionInterface> ServiceActConnect::CreateConnection(CIPAddress& remoteAddress,SERVICE_TYPE type,CLIENT_PROXY_MODE mode)
{
	auto pConnection = std::make_shared<CProxyConnectionDefault>();
	pConnection->Init();
    pConnection->SetClientProxy(type,mode);
    pConnection->SetServer(m_pServer);
    pConnection->SetService(this);

    if (!pConnection->Connect(remoteAddress))
    {
        return nullptr;
    }

    m_NetHandler->OnCreateConnectACK(pConnection);
    
    //m_Connections[pConnection->GetSessionID()] = pConnection;

    return pConnection;
}


void ServiceActConnect::OnRecvData(CSmartPtr<CBaseNetConnectionInterface> pConnection, DOS_SIMPLE_MESSAGE_HEAD* pData)
{
}


void ServiceActConnect::SetServer(CNetServer* pServer)
{
    m_pServer = pServer;
}
CNetServer* ServiceActConnect::GetServer()
{
    return m_pServer;
}

void ServiceActConnect::OnRecvMessage(CSmartPtr<CBaseNetConnectionInterface> s,  DOS_SIMPLE_MESSAGE_HEAD* pMsg) 
{
    m_NetHandler->OnMessage(s,pMsg);
}


void ServiceActConnect::OnConnection(CSmartPtr<CBaseNetConnectionInterface> c, bool IsSucceed)
{
   // m_NetHandler->OnCreateConnectACK(c);
    //if (IsSucceed)
    //{
    //    if (m_ConnectionGroups.size() > 0)
    //    {
    //        UINT Index = c->GetSessionID() % m_ConnectionGroups.size();
    //        m_ConnectionGroups[Index]->AddConnection(c);
    //    }
    //    else
    //    {
    //        //c->Disconnect();
    //        //PrintDOSLog(_T("未能找到合适的连接组"));
    //    }
    //}
    //else
    //{

    //}
}

void ServiceActConnect::OnDisconnection(CSmartPtr<CBaseNetConnectionInterface> s)
{
    m_NetHandler->OnDisConnect(s);
}
