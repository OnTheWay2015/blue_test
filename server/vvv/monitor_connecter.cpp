
#include "stdafx.h"



MonitorConnecter::MonitorConnecter()
{
}
MonitorConnecter::~MonitorConnecter()
{
}

void MonitorConnecter::Init(CoreBase* base)
{
    m_pCorebase = base;
    
    CIPAddress  addr;
    //addr.SetIP("127.0.0.1");
    //addr.SetPort(64000);

    addr.SetIP("192.168.168.100");
    addr.SetPort(64000);
    auto connect = base->CreateConnect(addr,CLIENT_PROXY_TYPE::CONNECT_MONITOR,CLIENT_PROXY_MODE::NORMAL);
    base->AddHandler(this,CLIENT_PROXY_TYPE::CONNECT_MONITOR);

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
        int packet_id = 666;
        //t1::MSG_TEST b;
        auto b = PACKET_CREATE(t1::MSG_TEST, packet_id );
        b->set_name("test_name");
        auto m = std::make_shared<DOS_SIMPLE_MESSAGE>();
        m->MsgID = packet_id;
        m->MsgLen = b->ByteSize() + sizeof(DOS_SIMPLE_MESSAGE_HEAD) ;
        m->MSG = b;
      
        msg->Session->SendMsg(m);
         
       //SendMsg(m.get());
         

	    //PrintImportantLog("开始载入配置文件%s", FileName);
        auto xx =b->DebugString();
        printf("SendMsg:%s",xx.c_str()); 
    }
    else if (msg->EventType == CORE_EVENT::SESSION_REMOVE)
    {
        //todo
         

    }

}

