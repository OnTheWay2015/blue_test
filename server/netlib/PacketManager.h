#pragma once

//#ifdef WIN32
//#ifdef _DEBUG
//#pragma comment(lib, "libprotobuf-gd.lib")
//#else
//#pragma comment(lib, "libprotobuf.lib")
//#endif
//
//#endif


#ifndef CONVERT_POINT
#define CONVERT_POINT(dectype, srcptr) std::static_pointer_cast<dectype>(srcptr)
#endif


template <typename T>
class PacketFactoryRecv :
	public factory_handler_recv<T>
{
public:
	PacketFactoryRecv(){}
	virtual ~PacketFactoryRecv(){}

    bool packet_process(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<void> msg) override
    {
		return packet_process(session, CONVERT_POINT(T, msg));
    }
	bool packet_process(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<T> msg)  override
	{
		return true;
	}
};


class PacketManager
    :public packet_factories
    , public CStaticObject4<PacketManager>
{
public:
	PacketManager(){}
	virtual ~PacketManager(){}

};

#define PACKET_FACTORY_RECV(packet) \
class packet##_factory : public PacketFactoryRecv<packet>\
{\
public:\
	packet##_factory()\
	{\
\
	};\
	virtual ~packet##_factory(){}\
	virtual bool packet_process(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<packet> msg) override;\
};



#define PACKET_FACTORY_SEND(packet) \
class packet##_factory :public factory_handler_send<packet> \
{\
public:\
	packet##_factory()\
	{\
\
	};\
	virtual ~packet##_factory(){}\
};




/*
	static void regedit_factory(int packet_id)\
	{\
			PacketManager::GetInstance()->regedit_object(packet_id, std::make_shared<packet##_factory>());\
	};

*/

#define DEF_PACKET_REGISTER_SEND(packet)  PACKET_FACTORY_SEND(packet) 
#define DEF_PACKET_REGISTER_RECV(packet)  PACKET_FACTORY_RECV(packet) 


#define PACKET_FACTORY_REGISTER(packet,packet_id) \
			PacketManager::GetInstance()->regedit_object((int)packet_id, std::make_shared<packet##_factory>());


#define PACKET_CREATE(packet_type, packet_id) CONVERT_POINT(packet_type, PacketManager::GetInstance()->create(packet_id))










