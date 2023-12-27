#pragma once
 

using namespace server_base;
//using namespace t1;
//using namespace p1;
//DEF_PACKET_REGISTER_RECV(MSG_BASE);

//all -> monitor //声明接收消息的工厂类, 再用 PACKET_FACTORY_REGISTER 把处理接收消息的回调方法注册到系统处理
DEF_PACKET_REGISTER_RECV(MSG_HEARTBEAT);

DEF_PACKET_REGISTER_RECV(MSG_LEADER_REG);
DEF_PACKET_REGISTER_RECV(MSG_LEADER_SERVER_LIST);

//monitor -> all //声明发送消息的工厂类
DEF_PACKET_REGISTER_SEND(MSG_HEARTBEAT_RES);
DEF_PACKET_REGISTER_SEND(MSG_LEADER_REG_RES);
DEF_PACKET_REGISTER_SEND(MSG_LEADER_SERVER_LIST_RES);






