
#include "stdafx.h"



extern BridgeInterface* g_pWorker; 


MessageHandler::MessageHandler(BridgeInterface* bridge)
    :m_pServicelist(nullptr)
    , m_pBridge(bridge)
{
    m_CheckServiceTm = ::GetRand(1, 6);
}

MessageHandler::~MessageHandler()
{
}

bool MessageHandler::Init()
{
    //auto pCore = MonitorConnecter::GetInstance()->GetCore(); 
    auto pCore = m_pBridge->GetCore();
    auto localServicelist = pCore->GetCoreConfig().GetServiceList();

    for (auto& conf : localServicelist)
	{
        pCore ->SetHandler(this,(SERVICE_TYPE)conf.ServiceType);//todo gate 单独出来
	}
    return true;
}

bool MessageHandler::Start()
{
    return true;
}

void MessageHandler::Update(float tm)
{
    m_CostTM += tm;

    if (m_CostTM  <= m_CheckServiceTm )
    {
        return;
    }
    m_CostTM  = 0;
    m_pBridge->GetMonitorConnecter()->GetServiceList();
}



void MessageHandler::OnNetMessage(CSmartPtr<CoreSessionMessage> msg)
{
    auto pm = g_pWorker->GetPacketManager(); 
    switch (msg->EventType)
    {
    case CORE_EVENT::SESSION_ADD:
        break;
    case CORE_EVENT::SESSION_REMOVE:
    {
        for (auto&it: m_ServiceInfos)
        {
            auto&list= it.second;
            for (auto iit=list.begin();iit!=list.end();)
            {
                if (iit->session == msg->Session)
                {
                    iit = list.erase(iit);
                }
                else
                {
                    iit++;
                }
            }
        } 

 } 

        break;
    case CORE_EVENT::SESSION_MESSAGE:
    {


        auto MsgID = msg->Header->MsgID;
        auto fact = pm->get_factroy(MsgID);
        if (fact)
        {
            fact->packet_process(msg->Session, msg->Msg);
        }
        else
        {
            PrintLog("no factory error, msgid:%d",MsgID);
        }
    }
    break;
    default:
        ;
    }
}

bool MessageHandler::SendMsg(CSmartPtr<DOS_SIMPLE_MESSAGE> msg,SERVICE_TYPE type, UINT sid)//todo msg queue,thread safe
{
    auto it = m_ServiceInfos.find(type);
    if (it == m_ServiceInfos.end())
    {
        PrintLog("no service type error, type:%u,sid:%u:",(UINT)type,sid);
        return false;
    }
    auto&list = m_ServiceInfos[type];
    if (list.size() <=0 )
    {
        PrintLog("service type list size 0 error, type:%u,sid:%u:",(UINT)type,sid);
        return false;
    }
    list[0].session->SendMsg(msg);
    return true;
}

void MessageHandler::OnReg(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_SVR_REG> msg)
{
    auto pm = g_pWorker->GetPacketManager(); 
    
    int packet_id = server_base::MSG_SVR_BASE::MsgCase::kMSGSVRREGRES;
    auto b = pm->CreatePacket<MSG_SVR_REG_RES>(packet_id);
    auto pCore = m_pBridge->GetCore();
    auto& coreConf = pCore->GetCoreConfig();
    auto svrid = coreConf.GetServerID();

    auto& infos = msg->infos();
    for (auto& o : infos)
    {
        SERVICE_TYPE type = (SERVICE_TYPE)o.type();
        auto& list = m_ServiceInfos[type];
        ServiceInfo v{ o.svrid(), (SERVICE_TYPE)o.type(), session,2};
        list.push_back(v);

        auto info = b->add_infos();
        info->set_type((UINT)type);
        info->set_svrid(svrid);
        info->set_reg_flag(1);
    }

    //send reg_res   必定返回这条消息.    在 CheckServiceConnect 发启 MSG_SVR_REG 请求
    PACK_HEADER(b, packet_id);
    session->SendMsg(M);

}



void MessageHandler::OnRegRes(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_SVR_REG_RES> msg)
{
    auto& infos = msg->infos();
    for (auto& o : infos)
    {
        SERVICE_TYPE type = (SERVICE_TYPE)o.type();
        auto& list = m_ServiceInfos[type];
        ServiceInfo v{ o.svrid(), (SERVICE_TYPE)o.type(), session ,2};
        list.push_back(v);
        
        for (auto it= m_ServiceCheckState.begin();it!=m_ServiceCheckState.end();)
        {
            if (it->sid == o.svrid() && it->type == (SERVICE_TYPE)o.type())
            {
                it = m_ServiceCheckState.erase(it);
            }
            else
            {
                it++;
            }
        }
    }
}

void MessageHandler::OnGetServiceList(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_LEADER_SERVER_LIST_RES> msg)
{
    m_pServicelist = msg;
    CheckServiceConnect();
}

void MessageHandler::CheckServiceConnect()
{
    auto pm = g_pWorker->GetPacketManager(); 
    if (!m_pServicelist)
    {
        return;
    }
    auto& infos = m_pServicelist->infos();
    //auto pCore = MonitorConnecter::GetInstance()->GetCore();
    auto pCore = m_pBridge->GetCore();
    auto& coreConf = pCore->GetCoreConfig();
    auto svrid = coreConf.GetServerID();
    for (auto& o : infos)
    {
        if (o.svrid() == pCore->GetServerID())
        {
            continue;
        }

        auto match = false;
        for (auto& checksv : m_ServiceCheckState)
        {
            if (checksv.sid == o.svrid() && checksv.type == (SERVICE_TYPE)o.type())
            {
                match = true;   
                break; 
            }
        }

        if (match)
        {
            continue;
        }
    
        for (auto& svs : m_ServiceInfos)
        {
            for (auto& sv : svs.second)
            {
                if (sv.sid == o.svrid() && sv.type == (SERVICE_TYPE)o.type())
                {
                    match = true;
                    break;
                }

            }
            if (match)
            {
                break;
            }
        }

        if (match)
        {
            continue;
        }
        auto type = (SERVICE_TYPE)o.type();
        pCore->SetHandler(this, type);

        CIPAddress  addr;
        //auto ws = s2wstring(o.ipstr());
        //addr.SetIP( ws.c_str() );
        addr.SetIP(o.ipstr().c_str());
        addr.SetPort(o.port());
        auto session = pCore->CreateConnect(addr, type, (CLIENT_PROXY_MODE)o.mode());

        if (!session)
        {
            PrintLog("check connect error, type:%u,port:%u,ip:%s", (UINT)type, o.port(), o.ipstr().c_str());
            continue;
        }

        //m_ServiceCheckState 
        ServiceInfo v{ o.svrid(),type,session,1 };
        m_ServiceCheckState.push_back(v);

        int packet_id = server_base::MSG_SVR_BASE::MsgCase::kMSGSVRREG;
        auto b = pm->CreatePacket<MSG_SVR_REG>(packet_id);

        auto info = b->add_infos();
        info->set_svrid(svrid);
        info->set_type((int)type);
        PACK_HEADER(b, packet_id);
        session->SendMsg(M);

    }
}






