/****************************************************************************/
/*                                                                          */
/*      文件名:    BaseServer.h                                             */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
//enum class SERVER_TYPE 
//{
//	MONITOR=1,
//	GATE,
//	GAME,
//};


class CoreBase;

enum class SERVICE_TYPE // 核心提供的服务分类
{
	NONE=0,
	SERVER_MONITOR=1,
	SERVER_GAME,
	SERVER_GATE,
	CONNECT_MONITOR,
	CONNECT_MONITOR_LOCAL, //本地发起连接
};

//enum class  CLIENT_PROXY_TYPE//connection 连接服务的类型,对应配置 Services.Service.ClientProxys.ClientProxy.ProxyType 同一个 server 不可重复 
//{
//	SERVER_GAME=1,
//	SERVER_MONITOR,
//	SERVER_GATE,
//	CONNECT_MONITOR,
//	CONNECT_MONITOR_LOCAL, //本地发起连接
//};

enum class CLIENT_PROXY_MODE
{
	NORMAL=1,
	WEB_SOCKET,
};

typedef unsigned int	MSG_LEN_TYPE;
typedef unsigned int	MSG_ID_TYPE;
struct DOS_SIMPLE_MESSAGE_HEAD  
{
    MSG_LEN_TYPE	MsgLen;
    MSG_ID_TYPE		MsgID;	//also use for sync id?
    WORD			MsgFlag;
    WORD			CRC;
};

struct DOS_SIMPLE_MESSAGE : DOS_SIMPLE_MESSAGE_HEAD
{
	CSmartPtr<void> MSG;
};


#define PACK_HEADER(m,packet_id) \
    auto M= std::make_shared<DOS_SIMPLE_MESSAGE>();\
    M->MsgID = packet_id;\
    M->MsgLen = m->ByteSize() + sizeof(DOS_SIMPLE_MESSAGE_HEAD);\
    M->MSG = m;\

 


class CBaseNetConnectionInterface;
class NetHandlerInterface
{
public:

    virtual void OnAccept(CSmartPtr<CBaseNetConnectionInterface> s) =0;//接受连接
    virtual void OnCreateConnectACK(CSmartPtr<CBaseNetConnectionInterface> s) = 0;//发起连接返回

    virtual void OnDisConnect(CSmartPtr<CBaseNetConnectionInterface> s) = 0;//断开连接
    virtual void DisConnectAck(UINT64 SessionID) = 0;//发起断开连接返回


    virtual void OnMessage(CSmartPtr<CBaseNetConnectionInterface> s,  DOS_SIMPLE_MESSAGE_HEAD* pMsg) =0;//收到消息
    virtual void SendMessageAck(/*xxx*/) = 0;//发送消息返回
   

    virtual CSmartPtr<CBaseNetConnectionInterface> CreateConnection(CIPAddress& remoteAddress,SERVICE_TYPE type,CLIENT_PROXY_MODE mode)=0;
    virtual CoreBase* GetCore()=0;
};



class CBaseNetServer :
	protected CEasyThread
{
protected:
	volatile UINT		m_TCPRecvBytes;
	volatile UINT		m_TCPSendBytes;
	volatile UINT		m_UDPRecvBytes;
	volatile UINT		m_UDPSendBytes;

	volatile UINT		m_TCPRecvCount;
	volatile UINT		m_TCPSendCount;
	volatile UINT		m_UDPRecvCount;
	volatile UINT		m_UDPSendCount;

	CSmartPtr<NetHandlerInterface> m_NetHandler;

public:
	CBaseNetServer(void);
	~CBaseNetServer(void);

	void AddTCPRecvBytes(int Count)
	{
		AtomicAdd(&m_TCPRecvBytes,Count);
		AtomicInc(&m_TCPRecvCount);
	}
	void AddTCPSendBytes(int Count)
	{
		AtomicAdd(&m_TCPSendBytes,Count);
		AtomicInc(&m_TCPSendCount);
	}
	void AddUDPRecvBytes(int Count)
	{
		AtomicAdd(&m_UDPRecvBytes,Count);
		AtomicInc(&m_UDPRecvCount);
	}
	void AddUDPSendBytes(int Count)
	{
		AtomicAdd(&m_UDPSendBytes,Count);
		AtomicInc(&m_UDPSendCount);
	}
	void ResetFluxStat()
	{
		m_TCPRecvBytes=0;
		m_TCPSendBytes=0;
		m_UDPRecvBytes=0;
		m_UDPSendBytes=0;

		m_TCPRecvCount=0;
		m_TCPSendCount=0;
		m_UDPRecvCount=0;
		m_UDPSendCount=0;
	}

	virtual bool StartUp(int EventObjectPoolSize = DEFAULT_EVENT_OBJECT_COUNT,
		int WorkThreadCount = DEFAULT_WORK_THREAD_COUNT,
		int EventRouterPoolSize=DEFAULT_EVENT_ROUTER_COUNT,
		int EventObjectPoolGrowSize=DEFAULT_EVENT_OBJECT_POOL_GROW_SIZE,
		int EventObjectPoolGrowLimit=DEFAULT_EVENT_OBJECT_POOL_GROW_LIMIT,
		int EventRouterPoolGrowSize=DEFAULT_EVENT_ROUTER_POOL_GROW_SIZE,
		int EventRouterPoolGrowlimit=DEFAULT_EVENT_ROUTER_POOL_GROW_LIMIT)=0;
	//bool StartUp(const STORAGE_POOL_SETTING& EventObjectPoolSetting, const STORAGE_POOL_SETTING& EventRouterPoolSetting, int WorkThreadCount)
	//{
	//	return StartUp(EventObjectPoolSetting.StartSize, WorkThreadCount, EventRouterPoolSetting.StartSize,
	//		EventObjectPoolSetting.GrowSize, EventObjectPoolSetting.GrowLimit,
	//		EventRouterPoolSetting.GrowSize, EventRouterPoolSetting.GrowLimit);
	//}
	virtual void ShutDown(DWORD Milliseconds=DEFAULT_THREAD_TERMINATE_TIME)=0;

	UINT GetThreadID()
	{
		return CEasyThread::GetThreadID();
	}

public:
	void SetHandler(CSmartPtr<NetHandlerInterface> h)
	{
		m_NetHandler = h;
	}

	CSmartPtr<NetHandlerInterface> GetHandler()
	{
		return m_NetHandler;
	}
protected:
	virtual bool OnStartUp() = 0;
	virtual void OnShutDown()=0;
	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT)=0;

};
