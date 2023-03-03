
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
    if (msg->EventType == CORE_EVENT::SESSION_MESSAGE)
    {
        auto fact = PacketManager::GetInstance()->get_factroy(msg->Header->MsgID);
        if (fact)
        {
            fact->packet_process(msg->Session, msg->Msg);
        }
        else
        {
            //log err
}
    }


}

