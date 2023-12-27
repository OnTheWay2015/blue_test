
#include "stdafx.h"



MonitorCoreHandler::MonitorCoreHandler()
{
}
MonitorCoreHandler::~MonitorCoreHandler()
{
}

void MonitorCoreHandler::Init()
{
}


void MonitorCoreHandler::Update()
{
}
void MonitorCoreHandler::OnNetMessage(CSmartPtr<CoreSessionMessage> msg)
{
    switch (msg->EventType)
    {
    case CORE_EVENT::SESSION_ADD:
        break;
    case CORE_EVENT::SESSION_REMOVE:
        break;
    case CORE_EVENT::SESSION_MESSAGE:
    {
        auto fact = PacketManager::GetInstance()->get_factroy(msg->Header->MsgID);
        if (fact)
        {
            fact->packet_process(msg->Session, msg->Msg);
        }
        else
        {
            // log err
        }
    }
    break;
    default:
        ;
    }

}
