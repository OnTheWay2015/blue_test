#include "stdafx.h"
 

extern BridgeInterface* g_pWorker; 
void proc_monitor_init()
{
    auto pm = g_pWorker->GetPacketManager(); 
//send
    pm->regedit_object(server_base::MSG_SVR_BASE::MsgCase::kMSGLEADERREGRES,PACKET_FACTORY(MSG_LEADER_REG_RES));
    pm->regedit_object(server_base::MSG_SVR_BASE::MsgCase::kMSGLEADERSERVERLISTRES,PACKET_FACTORY(MSG_LEADER_SERVER_LIST_RES));

    pm->regedit_object(server_base::MSG_SVR_BASE::MsgCase::kMSGLEADERBROADCASTREMOVE,PACKET_FACTORY(MSG_LEADER_BROADCAST_REMOVE));
    pm->regedit_object(server_base::MSG_SVR_BASE::MsgCase::kMSGLEADERBROADCASTADD,PACKET_FACTORY(MSG_LEADER_BROADCAST_ADD));


//recv
    pm->regedit_object(server_base::MSG_SVR_BASE::MsgCase::kMSGHEARTBEAT,PACKET_FACTORY(MSG_HEARTBEAT));
    pm->regedit_object(server_base::MSG_SVR_BASE::MsgCase::kMSGLEADERREG,PACKET_FACTORY(MSG_LEADER_REG));
    pm->regedit_object(server_base::MSG_SVR_BASE::MsgCase::kMSGLEADERSERVERLIST,PACKET_FACTORY(MSG_LEADER_SERVER_LIST));


}
//bool MSG_BASE_factory::packet_process(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_BASE> msg)
//{
//
//
//    return false;
//}

bool MSG_HEARTBEAT_factory::packet_process(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_HEARTBEAT> msg)
{	
    //auto msgstr = msg->DebugString();
    //int packet_id = 667;
    //auto b = PACKET_CREATE(MSG_TEST_RES, packet_id);
    //b->set_name_res("test_name_res");
    //auto m = std::make_shared<DOS_SIMPLE_MESSAGE>();
    //m->MsgID = packet_id;
    //m->MsgLen = b->ByteSize() + sizeof(DOS_SIMPLE_MESSAGE_HEAD);
    //m->MSG = b;

    //session->SendMsg(m);
    return false;
}


bool MSG_LEADER_REG_factory::packet_process(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_LEADER_REG> msg)
{	
    g_pWorker->GetMonitorCoreHandler()->AddServiceInfo(session,msg);
    return true;
}


bool  MSG_LEADER_SERVER_LIST_factory::packet_process(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_LEADER_SERVER_LIST> msg)
{	
    g_pWorker->GetMonitorCoreHandler()->GetSerivceInfos(session,msg);
    return true;
}








 