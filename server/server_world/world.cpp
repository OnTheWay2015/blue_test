#include "stdafx.h"


World::World()
{
}

World::~World()
{
}

bool World::Init(LPCTSTR FileName)
{
    //InitFramework();
    //proc_world_init();
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


bool World::Start()
{
    GetMonitorConnecter()->Start(); 
    return true;
}

CoreBase* World::GetCore()
{
    return m_pCore;
}

PacketManager* World::GetPacketManager()
{
    return m_pPacketManager ;
}

MonitorConnecter* World::GetMonitorConnecter()
{
    return m_pMonitor;
}

MessageHandler* World::GetMessageHandler()
{
    return m_pMessageHandler;
}


void World::Update(float tm)
{
    m_pCore->Update(tm);
    m_pMonitor->Update(tm);
    m_pMessageHandler->Update(tm);
}

