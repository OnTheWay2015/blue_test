
#include "stdafx.h"



CoreBase::CoreBase()
{
}
CoreBase::~CoreBase()
{
}

bool CoreBase::Init(LPCTSTR FileName)
{

    //m_CoreConfig.LoadConfig("./configD.xml");
    m_CoreConfig.LoadConfig(FileName);
    m_CoreNetManager= std::make_shared<CoreNetManager>();
    m_CoreNetManager->Init(this,m_CoreConfig);

    //m_CoreConfig.GetLeaderConfig();

    return true;
}



void CoreBase::Update()
{
    m_CoreNetManager->Update();

}


void CoreBase::AddHandler(CoreHandlerInterface* h, CLIENT_PROXY_TYPE type)
{
    if (m_CoreHandlers.find(type) != m_CoreHandlers.end())
    {
        //log warning
    }
    m_CoreHandlers[type] = h;
}

CSmartPtr<CBaseNetConnectionInterface> CoreBase::CreateConnect(CIPAddress& remoteAddress,CLIENT_PROXY_TYPE type,CLIENT_PROXY_MODE mode)
{
return  m_CoreNetManager->CreateConnection(remoteAddress,type,mode);
}

void CoreBase::SendSessionMessage(CSmartPtr<CoreSessionMessage> msg)
{
}

void CoreBase::DisConnect(SESSION_ID SessionID)
{
}

void CoreBase::OnNetMessage(CSmartPtr<CoreSessionMessage> msg) 
{
    m_CoreHandlers[msg->ClientProxyType]->OnNetMessage(msg);
    //if (msg->EventType == CORE_EVENT::SESSION_ADD)
    //{
    //}
    //todo
}
void CoreBase::SetParseMessageHandler(MessageHandlerInterface* h)
{
    m_CoreNetManager->SetParseMessageHandler(h);
}




