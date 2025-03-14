#pragma once

class MessageParseInterface 
{
public:
    virtual CSmartPtr<void> ParseMessage(DOS_SIMPLE_MESSAGE_HEAD* h)=0;
    virtual CSmartPtr<std::vector<unsigned char>> SerializeToArray(CSmartPtr<void> h)=0;


};
class CoreHandlerInterface 
{
public:
    virtual bool Init()=0;
    virtual bool Start()=0;
    
    virtual void Update(float tm)=0;
    virtual void OnNetMessage(CSmartPtr<CoreSessionMessage> msg)=0; 
};

class CoreBaseInterface 
{

public://NetHandlerInterface
    //virtual void OnConnect(UINT SessionID, UINT TypeID /*客户端,服务器*/) = 0;//接受连接
    //virtual void OnCreateConnectACK(UINT SessionID,UINT TypeID ) = 0;//发起连接返回   对应 CreateConnect

    //virtual void OnDisConnect(UINT SessionID) = 0;//断开连接
    //virtual void DisConnectAck(UINT SessionID) = 0;//发起断开连接返回   对应 DisConnect


    //virtual void OnMessage(/*xxx*/) = 0;//收到消息
    //virtual void SendMessageAck(/*xxx*/) = 0;//发送消息返回  对应 SendMessage
//-------------
public:
    virtual bool Init(LPCTSTR FileName)=0;
    virtual void Update(float tm)=0;
    virtual const CoreConfig& GetCoreConfig()=0; 
    virtual void SetHandler(CoreHandlerInterface* h, SERVICE_TYPE type) = 0;//注册模块(消息处理模块)
    virtual UINT GetServerID()=0;
};

class CoreBaseNetInterface 
{
public:
    virtual CSmartPtr<CBaseNetConnectionInterface> CreateConnect(CIPAddress& remoteAddress,SERVICE_TYPE type,CLIENT_PROXY_MODE mode)=0 ;//发起连接
    virtual void DisConnect(SESSION_ID SessionID) = 0;//发起断开连接
    virtual void SendSessionMessage(CSmartPtr<CoreSessionMessage> msg)=0;
    virtual void OnNetMessage(CSmartPtr<CoreSessionMessage> msg)  =0;//发送消息
};



class CoreBase 
    :public CoreBaseInterface
    ,public CoreBaseNetInterface 
{
public :
    CoreBase();
    ~CoreBase();

private:
	CEasyCriticalSection m_EasyCriticalSection;
    CoreConfig m_CoreConfig; 
   CSmartPtr<CoreNetManager> m_CoreNetManager;
   CHashMap<SERVICE_TYPE ,CoreHandlerInterface* > m_CoreHandlers; 
public:
   void SetParseMessageHandler(MessageParseInterface* h); 
public://CoreBaseInterface 
    virtual bool Init(LPCTSTR FileName) override;
    virtual void Update(float tm) override;
    virtual CoreConfig& GetCoreConfig()override { return m_CoreConfig;} 
    virtual void SetHandler(CoreHandlerInterface* h, SERVICE_TYPE type)  override;//注册模块(消息处理模块)
    virtual UINT GetServerID();
public://CoreBaseNetInterface 
    virtual CSmartPtr<CBaseNetConnectionInterface> CreateConnect(CIPAddress& remoteAddress,SERVICE_TYPE type,CLIENT_PROXY_MODE mode) override ;//发起连接
    virtual void DisConnect(SESSION_ID SessionID)  override;//发起断开连接

    virtual void SendSessionMessage(CSmartPtr<CoreSessionMessage> msg)  override;//发送消息
    virtual void OnNetMessage(CSmartPtr<CoreSessionMessage> msg)  override;//发送消息
};



