#include "stdafx.h"
 

void proc_init()
{
	//PACKET_FACTORY_REGISTER(MSG_TEST,666);
	//PACKET_FACTORY_REGISTER(MSG_TEST_RES,667);
	PACKET_FACTORY_REGISTER(MSG_TEST, PACKET_IDS::MSG_TEST);
	PACKET_FACTORY_REGISTER(MSG_TEST_RES,PACKET_IDS::MSG_TEST_RES);
}
bool MSG_TEST_factory::packet_process(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_TEST> msg)
{	
    auto msgstr = msg->DebugString();
    int packet_id = 667;
    auto b = PACKET_CREATE(MSG_TEST_RES, packet_id);
    b->set_name_res("test_name_res");
    auto m = std::make_shared<DOS_SIMPLE_MESSAGE>();
    m->MsgID = packet_id;
    m->MsgLen = b->ByteSize() + sizeof(DOS_SIMPLE_MESSAGE_HEAD);
    m->MSG = b;

    session->SendMsg(m);
    return false;
}
 