
#include "stdafx.h"



MonitorConnecter::MonitorConnecter()
{
}
MonitorConnecter::~MonitorConnecter()
{
}

void MonitorConnecter::Init(CoreBase* base)
{
    base->SetHandler(this,SERVICE_TYPE::CONNECT_MONITOR_LOCAL);
    
    m_pCorebase = base;
    CIPAddress  addr;
    auto& coreConf = base->GetCoreConfig();
    addr.SetIP(coreConf.GetLeaderConfig().IP.c_str());
    addr.SetPort(coreConf.GetLeaderConfig().Port);

    //addr.SetIP("192.168.100.157");
    //addr.SetPort(64000);
    auto connect = base->CreateConnect(addr,SERVICE_TYPE::CONNECT_MONITOR_LOCAL,CLIENT_PROXY_MODE::NORMAL);
    

}


void MonitorConnecter::Update()
{
}
void MonitorConnecter::OnNetMessage(CSmartPtr<CoreSessionMessage> msg)
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
    else if (msg->EventType == CORE_EVENT::SESSION_ADD)
    {

        /*
        int packet_id = 666;
        //t1::MSG_TEST b;
        auto b = PACKET_CREATE(t1::MSG_TEST, packet_id );
        b->set_name("test_name");
        auto m = std::make_shared<DOS_SIMPLE_MESSAGE>();
        m->MsgID = packet_id;
        m->MsgLen = b->ByteSize() + sizeof(DOS_SIMPLE_MESSAGE_HEAD) ;
        m->MSG = b;
      
        msg->Session->SendMsg(m);
        */
         
       
 
        int packet_id = MSG_BASE::MsgCase::kMSGTEST;
        auto b = PACKET_CREATE(MSG_TEST, packet_id );
        b->set_name("test_name");
        auto m = std::make_shared<DOS_SIMPLE_MESSAGE>();
        m->MsgID = packet_id;
        m->MsgLen = b->ByteSize() + sizeof(DOS_SIMPLE_MESSAGE_HEAD) ;
        m->MSG = b;
        msg->Session->SendMsg(m);
    }
    else if (msg->EventType == CORE_EVENT::SESSION_REMOVE)
    {
        //todo
         

    }

}

