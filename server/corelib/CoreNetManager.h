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
    CLIENT_PROXY_TYPE ClientProxyType;
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
    virtual void OnCreateConnectACK(CSmartPtr<CBaseNetConnectionInterface> s)  override ;//�������ӷ���

    virtual void OnDisConnect(CSmartPtr<CBaseNetConnectionInterface> s)  override ; //�Ͽ�����
    virtual void DisConnectAck(SESSION_ID SessionID)  override ;//����Ͽ����ӷ���
    
    virtual void OnMessage(CSmartPtr<CBaseNetConnectionInterface> s,  DOS_SIMPLE_MESSAGE_HEAD* pMsg)  override ;//�յ���Ϣ
    virtual void SendMessageAck(/*xxx*/)  override ;//������Ϣ����

    virtual CSmartPtr<CBaseNetConnectionInterface> CreateConnection(CIPAddress& RemoteAddress,CLIENT_PROXY_TYPE type,CLIENT_PROXY_MODE mode) override ;;
public://CoreMessageQueue
    virtual void ProcessQueueMessageHandler(CSmartPtr<CoreSessionMessage> msg) override ;
};