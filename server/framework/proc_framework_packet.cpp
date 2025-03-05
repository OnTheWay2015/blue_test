#include "stdafx.h"
extern BridgeInterface* g_pWorker; 
void proc_init()
{
//send
    auto pm = g_pWorker->GetPacketManager(); 
    pm->regedit_object(server_base::MSG_SVR_BASE::MsgCase::kMSGLEADERREG,PACKET_FACTORY(MSG_LEADER_REG));
    pm->regedit_object(server_base::MSG_SVR_BASE::MsgCase::kMSGLEADERSERVERLIST,PACKET_FACTORY(MSG_LEADER_SERVER_LIST));
   
 

//recv 
    pm->regedit_object(server_base::MSG_SVR_BASE::MsgCase::kMSGLEADERREGRES,PACKET_FACTORY(MSG_LEADER_REG_RES));

    pm->regedit_object(server_base::MSG_SVR_BASE::MsgCase::kMSGLEADERSERVERLISTRES,PACKET_FACTORY(MSG_LEADER_SERVER_LIST_RES));


//send and recv
    pm->regedit_object( server_base::MSG_SVR_BASE::MsgCase::kMSGSVRREG,PACKET_FACTORY(MSG_SVR_REG));
    pm->regedit_object( server_base::MSG_SVR_BASE::MsgCase::kMSGSVRREGRES,PACKET_FACTORY(MSG_SVR_REG_RES));
}

bool MSG_LEADER_REG_RES_factory::packet_process(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_LEADER_REG_RES> msg)
{	

	return false;
}
 
bool MSG_LEADER_SERVER_LIST_RES_factory::packet_process(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_LEADER_SERVER_LIST_RES> msg)
{	
    g_pWorker->GetMessageHandler()->OnGetServiceList(session,msg);
	return true;
}


bool MSG_SVR_REG_factory::packet_process(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_SVR_REG> msg)
{	

    g_pWorker->GetMessageHandler()->OnReg(session,msg);
	return true;
}

bool MSG_SVR_REG_RES_factory::packet_process(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_SVR_REG_RES> msg)
{	
    g_pWorker->GetMessageHandler()->OnRegRes(session,msg);
	return true;
}


