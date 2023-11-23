#include "stdafx.h"
 
void proc_init()
{
	//PACKET_FACTORY_REGISTER(MSG_TEST, PACKET_IDS::MSG_TEST);
	//PACKET_FACTORY_REGISTER(MSG_TEST_RES,PACKET_IDS::MSG_TEST_RES);
	PACKET_FACTORY_REGISTER(MSG_TEST, MSG_BASE::MsgCase::kMSGTEST);
	PACKET_FACTORY_REGISTER(MSG_TEST_RES,MSG_BASE::MsgCase::kMSGTESTRES);
}
bool MSG_TEST_RES_factory::packet_process(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_TEST_RES> msg)
{	

	return false;
}
 