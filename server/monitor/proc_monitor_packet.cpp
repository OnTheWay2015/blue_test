#include "stdafx.h"
 

void proc_init()
{
	//PACKET_FACTORY_REGISTER(MSG_TEST,666);
	//PACKET_FACTORY_REGISTER(MSG_TEST_RES,667);
	//PACKET_FACTORY_REGISTER(MSG_TEST, PACKET_IDS::MSG_TEST);
	//PACKET_FACTORY_REGISTER(MSG_TEST_RES,PACKET_IDS::MSG_TEST_RES);


	//PACKET_FACTORY_REGISTER(MSG_TEST, MSG_BASE::MsgCase::kMSGTEST);
	//PACKET_FACTORY_REGISTER(MSG_TEST_RES,MSG_BASE::MsgCase::kMSGTESTRES);
	
    //PACKET_FACTORY_REGISTER(MSG_BASE,0);

    //server_base::MSG_SVR_BASE::MsgCase::kMSGHEARTBEAT 
    PACKET_FACTORY_REGISTER(MSG_HEARTBEAT,server_base::MSG_SVR_BASE::MsgCase::kMSGHEARTBEAT);


    PACKET_FACTORY_REGISTER(MSG_LEADER_REG,server_base::MSG_SVR_BASE::MsgCase::kMSGLEADERREG);

    PACKET_FACTORY_REGISTER(MSG_LEADER_SERVER_LIST,server_base::MSG_SVR_BASE::MsgCase::kMSGLEADERSERVERLIST);



    //PACKET_FACTORY_REGISTER(MSG_TEST_RES,MSG_BASE::MsgCase::kMSGLEADERBROADCASTADD);
    //PACKET_FACTORY_REGISTER(MSG_TEST_RES,MSG_BASE::MsgCase::kMSGLEADERBROADCASTREMOVE);


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
    return false;
}


bool  MSG_LEADER_SERVER_LIST_factory::packet_process(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_LEADER_SERVER_LIST> msg)
{	
    return false;
}








 