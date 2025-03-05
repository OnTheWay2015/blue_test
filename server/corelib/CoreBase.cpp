
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



void CoreBase::Update(float tm)
{
    m_CoreNetManager->Update();

}


//在初始化时完成添加
//void CoreBase::AddHandler(CoreHandlerInterface* h, SERVICE_TYPE type)
void CoreBase::SetHandler(CoreHandlerInterface* h, SERVICE_TYPE type)
{
    //CAutoLock lock(&m_EasyCriticalSection);
    if (m_CoreHandlers.find(type) != m_CoreHandlers.end())
    {
        //log warning
    }
    m_CoreHandlers[type] = h;
}

CSmartPtr<CBaseNetConnectionInterface> CoreBase::CreateConnect(CIPAddress& remoteAddress,SERVICE_TYPE type,CLIENT_PROXY_MODE mode)
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
    auto type = msg->ServiceType;
    if(m_CoreHandlers.find(type) == m_CoreHandlers.end())
    {
        PrintImportantLog(_T("m_CoreHandlers no type: %d,error"),type);
        return;
    }

    //CAutoLock lock(&m_EasyCriticalSection);
    m_CoreHandlers[type]->OnNetMessage(msg);
    //if (msg->EventType == CORE_EVENT::SESSION_ADD)
    //{
    //}
    //todo
}
void CoreBase::SetParseMessageHandler(MessageParseInterface* h)
{
    m_CoreNetManager->SetParseMessageHandler(h);
}


UINT CoreBase::GetServerID()
{
    return m_CoreConfig.GetServerID();
}


