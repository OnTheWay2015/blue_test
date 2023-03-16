#include "stdafx.h"
 
void proc_init()
{
	PACKET_FACTORY_REGISTER(MSG_TEST, PACKET_IDS::MSG_TEST);
	PACKET_FACTORY_REGISTER(MSG_TEST_RES,PACKET_IDS::MSG_TEST_RES);
}
bool MSG_TEST_RES_factory::packet_process(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_TEST_RES> msg)
{	

    printf("packet_process msg:%s",msg->DebugString().c_str()); 
	return false;
}
 