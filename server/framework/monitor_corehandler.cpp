
#include "stdafx.h"


extern BridgeInterface* g_pWorker;
MonitorCoreHandler::MonitorCoreHandler(BridgeInterface* bridge)
    :m_pBridge(bridge)
{
}


MonitorCoreHandler::~MonitorCoreHandler()
{
}

bool MonitorCoreHandler::Init()
{
    auto base  = m_pBridge->GetCore();
    base->SetHandler(this,SERVICE_TYPE::SERVER_MONITOR);

    return true;;
}

bool MonitorCoreHandler::Start()
{
    return true;;
}


void MonitorCoreHandler::Update(float tm)
{
}
void MonitorCoreHandler::OnNetMessage(CSmartPtr<CoreSessionMessage> msg)
{
    auto pm = g_pWorker->GetPacketManager(); 
    switch (msg->EventType)
    {
    case CORE_EVENT::SESSION_ADD:
        break;
    case CORE_EVENT::SESSION_REMOVE:
    {
        for (auto svr_it=m_Servers.begin(); svr_it!= m_Servers.end();)
        {
            if (svr_it->second.session != msg->Session)
            {
                svr_it++;
                continue;
            }
            auto svrid = svr_it->second.svrid;

            for (auto& iit : m_ServiceList)
            {
                auto& list = iit.second;
                for (auto it = list.begin(); it != list.end(); )
                {
                    if (it->svrid() == svrid)
                    {
                        PrintLog("RMV svrid[%d] type[%d] on session close", it->svrid(), it->type());
                        it = list.erase(it);

                        //todo broadcast
                    }
                    else
                    {
                        it++;
                    }
                }
            }

            svr_it = m_Servers.erase(svr_it);
        }
    }
        break;
    case CORE_EVENT::SESSION_MESSAGE:
    {
        auto fact =pm->get_factroy(msg->Header->MsgID);
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

void MonitorCoreHandler::RmvServiceInfo(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_LEADER_REG> msg)
{
    PrintLog("act!");
    for (auto info : msg->infos() )
    {
    
        SERVICE_TYPE type = (SERVICE_TYPE )info.type();
        if (m_ServiceList.find(type) == m_ServiceList.end())
        {
            continue;
        }
        auto& list = m_ServiceList[type];
        for (auto it = list.begin(); it != list.end(); it++)
        {
            if (it->svrid() == info.svrid() && it->type() == info.type())
            {
                PrintLog("RMV svrid[%d] type[%d]",it->svrid(),it->type()  );
                list.erase(it);

                //todo broadcast
                break;
            }
        }
    }
}



void MonitorCoreHandler::AddServiceInfo(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_LEADER_REG> msg)
{
    //add server
    UINT svrid =  msg->svrid();
    if ( m_Servers.find(svrid) == m_Servers.end() )
    {
        auto& info = m_Servers[svrid];
        info.session = session;
        info.svrid = svrid;
        info.state = 2;
    }
    else
    {
        auto& info = m_Servers[svrid];
        if (info.session != session)
        {
            info.session->SetStop();
            info.session = session;
        } 
    }


    //add service
    for (auto info : msg->infos() )
    {
        //info.set_sessionid(sid);
        SERVICE_TYPE type = (SERVICE_TYPE )info.type();
        auto& list = m_ServiceList[type];
        auto match = false;
        for (auto it = list.begin(); it != list.end(); it++)
        {
            if (it->svrid() ==svrid && it->type() == info.type())
            {
                PrintLog("RMV svrid[%d] type[%d]",it->svrid(),it->type()  );
                //list.erase(it);
                it->set_ipstr(info.ipstr()); 
                it->set_mode(info.mode());
                it->set_port(info.port());       
                match = true;
                break;
            }
        }
        PrintLog("Add svrid[%d] type[%d]",svrid,info.type());
        if (!match)
        {
            info.set_svrid(svrid);
            list.push_back(info);
        }
    }


    auto pm = g_pWorker->GetPacketManager(); 
    int packet_id = server_base::MSG_SVR_BASE::MsgCase::kMSGLEADERREGRES;
    auto b = pm->CreatePacket<MSG_LEADER_REG_RES>(packet_id );
    b->set_result(1);
    PACK_HEADER(b, packet_id);
    session->SendMsg(M);

}

    


void MonitorCoreHandler::GetSerivceInfos(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_LEADER_SERVER_LIST> msg)
{
    auto pm = g_pWorker->GetPacketManager(); 
    auto msgstr = msg->DebugString();
    int packet_id = server_base::MSG_SVR_BASE::MsgCase::kMSGLEADERSERVERLISTRES;
    auto b = pm->CreatePacket<MSG_LEADER_SERVER_LIST_RES>(packet_id );
    for (auto& list : m_ServiceList)
    {
        for (auto it = list.second.begin(); it != list.second.end(); it++)
        {
            auto info = b->add_infos();
            info->set_svrid(it->svrid());
            info->set_type(it->type());
            info->set_ipstr(it->ipstr());
            info->set_port(it->port());
        }

    }
    PACK_HEADER(b,packet_id);
    session->SendMsg(M);
}




