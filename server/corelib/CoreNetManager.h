#pragma once

class CoreBase;
class MessageHandlerInterface; 
enum class CORE_EVENT
{
    SESSION_ADD=1,
    SESSION_REMOVE,
    SESSION_MESSAGE,
};

struct CoreMessage
{
   CORE_EVENT EventType; 
};

struct CoreSessionMessage : CoreMessage
{
    CSmartPtr<CBaseNetConnectionInterface> Session;
    CSmartPtr<DOS_SIMPLE_MESSAGE_HEAD> Header;
    CSmartPtr<void> Msg;
    SESSION_ID SID;
    //SERVICE_TYPE ServiceType;
    CLIENT_PROXY_MODE ClientProxyMode;
    UINT ServiceID;
    SERVICE_TYPE ServiceType;

};



class CoreNetManager 
    :public NetHandlerInterface
    ,public CoreMessageQueue<CoreSessionMessage>
    ,public std::enable_shared_from_this<CoreNetManager>
{
public:
    CoreNetManager();
    ~CoreNetManager();
private:
    CoreBase* m_CoreBase;
    CSmartPtr<CNetServer> m_NetServer;
    CSmartPtr<ServiceActConnect> m_NetServiceActConnect;
    std::vector<CSmartPtr<CBaseNetServiceInterface>> m_NetServices; //CNetService
	CEasyArray<CSmartPtr<CProxyConnectionGroup>> m_ConnectionGroups;
    MessageHandlerInterface* m_pMessageHandler;
public:
    void SendSessionMessage(CSmartPtr<CoreSessionMessage> msg);

public:
    //bool InitClientProxy(CoreConfig::CLIENT_PROXY_CONFIG& Config);
    bool Init(CoreBase* base, CoreConfig& config) ;
    void Update() ;

    void AddConnection(CSmartPtr<CBaseNetConnectionInterface> c);
    void RemoveConnection(CSmartPtr<CBaseNetConnectionInterface> c);
   void SetParseMessageHandler(MessageHandlerInterface* h); 
public://NetHandlerInterface
    virtual void OnAccept(CSmartPtr<CBaseNetConnectionInterface> s) override;
    virtual void OnCreateConnectACK(CSmartPtr<CBaseNetConnectionInterface> s)  override ;//发起连接返回

    virtual void OnDisConnect(CSmartPtr<CBaseNetConnectionInterface> s)  override ; //断开连接
    virtual void DisConnectAck(SESSION_ID SessionID)  override ;//发起断开连接返回
    
    virtual void OnMessage(CSmartPtr<CBaseNetConnectionInterface> s,  DOS_SIMPLE_MESSAGE_HEAD* pMsg)  override ;//收到消息
    virtual void SendMessageAck(/*xxx*/)  override ;//发送消息返回

    virtual CSmartPtr<CBaseNetConnectionInterface> CreateConnection(CIPAddress& RemoteAddress,SERVICE_TYPE type,CLIENT_PROXY_MODE mode) override ;;
public://CoreMessageQueue
    virtual void ProcessQueueMessageHandler(CSmartPtr<CoreSessionMessage> msg) override ;
};
