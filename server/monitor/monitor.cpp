#include "stdafx.h"


Monitor::Monitor()
{
}

Monitor::~Monitor()
{
}

bool Monitor::Init(LPCTSTR FileName)
{
    m_pCore = MONITORED_NEW("CoreBase",CoreBase);  
    m_pMessageParser= MONITORED_NEW("ProtobufParseMessage",ProtobufParseMessage,this);  
    m_pPacketManager = MONITORED_NEW("PacketManager",PacketManager);  
    m_pMonitorCoreHandler= MONITORED_NEW("MonitorCoreHandler",MonitorCoreHandler,this);  

    m_pCore->Init(FileName);
    m_pMessageParser->Init();
    m_pMonitorCoreHandler->Init();
    m_pCore->SetParseMessageHandler(m_pMessageParser);
    return 0;
}


CoreBase* Monitor::GetCore()
{
    return m_pCore;
}

PacketManager* Monitor::GetPacketManager()
{
    return m_pPacketManager ;
}

MonitorCoreHandler* Monitor::GetMonitorCoreHandler()
{
    return m_pMonitorCoreHandler;
}



void Monitor::Update(float tm)
{
    m_pCore->Update(tm);
    m_pMonitorCoreHandler->Update(tm);
}

