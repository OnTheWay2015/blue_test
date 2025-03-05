
#include "stdafx.h"
extern BridgeInterface* g_pWorker; 

MonitorConnecter::MonitorConnecter(BridgeInterface* bridge)
    :m_pBridge(bridge)
    //m_pCorebase(nullptr)
    ,m_session(nullptr)
{
}
MonitorConnecter::~MonitorConnecter()
{
}
bool MonitorConnecter::Start()
{
    auto base  = m_pBridge->GetCore();
    //m_pCorebase = base;
    CIPAddress  addr;
    auto& coreConf = base->GetCoreConfig();
    addr.SetIP(coreConf.GetLeaderConfig().IP.c_str());
    addr.SetPort(coreConf.GetLeaderConfig().Port);

    //addr.SetIP("192.168.100.157");
    //addr.SetPort(64000);
    m_session = base->CreateConnect(addr,SERVICE_TYPE::CONNECT_MONITOR_LOCAL,CLIENT_PROXY_MODE::NORMAL);
    
    if (!m_session )
    {
	    PrintImportantLog(_T("leader connect error!"));
        //todo error
        return false;
    }
    

    auto pm = g_pWorker->GetPacketManager(); 
    //send reg
    {
        int packet_id = server_base::MSG_SVR_BASE::MsgCase::kMSGLEADERREG;
        auto b = pm->CreatePacket<MSG_LEADER_REG>(packet_id);
        auto svrid =  coreConf.GetServerID();
        b->set_svrid(svrid); 
        auto& list = coreConf.GetServiceList();
        for (auto &o : list)
        {
            auto& proxys = o.m_ClientProxys;
            for (auto& p : proxys)
            {
                auto info = b->add_infos();
                //info->set_svrid(svrid);
                info->set_type(o.ServiceType);
                info->set_port(p.ListenPort);
                //info->set_ipstr( ::w2string(p.ListenIP) );
                info->set_ipstr( p.ListenIP );
                info->set_mode((int)p.ProxyMode);
            }

        }
        
        PACK_HEADER(b, packet_id);
        m_session->SendMsg(M);

    }
    m_pBridge->GetMessageHandler()->Init();
    return true;
}

void MonitorConnecter::GetServiceList()
{
    if (!m_session )
    {
        return;
    }
    auto pm = g_pWorker->GetPacketManager();
    int packet_id = server_base::MSG_SVR_BASE::MsgCase::kMSGLEADERSERVERLIST;
    auto b = pm->CreatePacket<MSG_LEADER_SERVER_LIST>(packet_id);

    PACK_HEADER(b, packet_id);
    m_session->SendMsg(M);
}

bool MonitorConnecter::Init()
{
    auto base  = m_pBridge->GetCore();
    base->SetHandler(this,SERVICE_TYPE::CONNECT_MONITOR_LOCAL);
    return true;
}


void MonitorConnecter::Update(float tm)
{
}
void MonitorConnecter::OnNetMessage(CSmartPtr<CoreSessionMessage> msg)
{
    auto pm = g_pWorker->GetPacketManager(); 
    if (msg->EventType == CORE_EVENT::SESSION_MESSAGE)
    {
        auto fact = pm->get_factroy(msg->Header->MsgID);
        if (fact)
        {
            fact->packet_process(msg->Session, msg->Msg);
        }
        else
        {
	        PrintImportantLog(_T("net essage error!"));
            //log err
        }
    }
    else if (msg->EventType == CORE_EVENT::SESSION_ADD)
    {
        //int packet_id = MSG_BASE::MsgCase::kMSGTEST;
        //auto b = PACKET_CREATE(MSG_TEST, packet_id );
        //b->set_name("test_name");
        //auto m = std::make_shared<DOS_SIMPLE_MESSAGE>();
        //m->MsgID = packet_id;
        //m->MsgLen = b->ByteSize() + sizeof(DOS_SIMPLE_MESSAGE_HEAD) ;
        //m->MSG = b;
        //msg->Session->SendMsg(m);
    }
    else if (msg->EventType == CORE_EVENT::SESSION_REMOVE)
    {
        //todo
         

    }

}

