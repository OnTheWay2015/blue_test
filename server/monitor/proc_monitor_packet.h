#pragma once
 

using namespace server_base;
//using namespace t1;
//using namespace p1;
//DEF_PACKET_REGISTER_RECV(MSG_BASE);

//all -> monitor //����������Ϣ�Ĺ�����, ���� PACKET_FACTORY_REGISTER �Ѵ��������Ϣ�Ļص�����ע�ᵽϵͳ����
DEF_PACKET_REGISTER_RECV(MSG_HEARTBEAT);

DEF_PACKET_REGISTER_RECV(MSG_LEADER_REG);
DEF_PACKET_REGISTER_RECV(MSG_LEADER_SERVER_LIST);

//monitor -> all //����������Ϣ�Ĺ�����
DEF_PACKET_REGISTER_SEND(MSG_HEARTBEAT_RES);
DEF_PACKET_REGISTER_SEND(MSG_LEADER_REG_RES);
DEF_PACKET_REGISTER_SEND(MSG_LEADER_SERVER_LIST_RES);






