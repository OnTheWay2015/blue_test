#include "stdafx.h"


Gate::Gate()
{
}

Gate::~Gate()
{
}

bool Gate::Init(LPCTSTR FileName)
{
    //InitFramework();
    //proc_Gate_init();
    m_pCore = MONITORED_NEW("CoreBase",CoreBase);  
    m_pMonitor = MONITORED_NEW("MonitorConnecter",MonitorConnecter,this);  
    m_pMessageHandler = MONITORED_NEW("MessageHandler",MessageHandler,this);  

    m_pMessageParser= MONITORED_NEW("ProtobufParseMessage",ProtobufParseMessage,this);  
    m_pPacketManager = MONITORED_NEW("PacketManager",PacketManager);  


    

    m_pCore->Init(FileName);
    m_pMessageParser->Init();
    m_pMonitor->Init();
    m_pCore->SetParseMessageHandler(m_pMessageParser);



    return 0;

}


bool Gate::Start()
{
    GetMonitorConnecter()->Start(); 
    return true;
}

CoreBase* Gate::GetCore()
{
    return m_pCore;
}

PacketManager* Gate::GetPacketManager()
{
    return m_pPacketManager ;
}

MonitorConnecter* Gate::GetMonitorConnecter()
{
    return m_pMonitor;
}

MessageHandler* Gate::GetMessageHandler()
{
    return m_pMessageHandler;
}


void Gate::Update(float tm)
{
    m_pCore->Update(tm);
    m_pMonitor->Update(tm);
    m_pMessageHandler->Update(tm);
}

