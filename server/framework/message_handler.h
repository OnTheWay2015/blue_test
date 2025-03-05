
#pragma once

class BridgeInterface;

class MessageHandler
    :public CoreHandlerInterface

{

struct ServiceInfo {
    UINT sid;  //服务器ID
    SERVICE_TYPE type; //服务类型 (全局唯一) 不同的服务器可以配置有一样的服务类型,但在一个服务器里,一个服务类型只能开一个配置 
    //UINT mode; //连接服务方式 对应  ClientProxy.ProxyMode
    CSmartPtr<CBaseNetConnectionInterface> session;
    UINT state; // =0 none ; =1, start regist; =2,registed ; =3,closed
};

public:
    MessageHandler(BridgeInterface* bridge);
    ~MessageHandler();

public:
    bool SendMsg(CSmartPtr<DOS_SIMPLE_MESSAGE> msg,SERVICE_TYPE type, UINT sid=0);

public:
    void OnGetServiceList(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_LEADER_SERVER_LIST_RES> msg);
    void OnReg(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_SVR_REG> msg);
    void OnRegRes(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_SVR_REG_RES> msg);
public:
    virtual bool Init();
    virtual bool Start();
    virtual void Update(float tm) override;
    virtual void OnNetMessage(CSmartPtr<CoreSessionMessage> msg) override; 
private:
    void CheckServiceConnect();
    void AddService(UINT sid, SERVICE_TYPE type);
    void RmvService(UINT sid, SERVICE_TYPE type);
private:
    BridgeInterface* m_pBridge;
    float m_CheckServiceTm;
    float m_CostTM;
    CSmartPtr<MSG_LEADER_SERVER_LIST_RES>  m_pServicelist; //同步 monitor 的数据
    std::unordered_map< SERVICE_TYPE,  CEasyArray<ServiceInfo> > m_ServiceInfos; //保存当前服务间的连接数据. todo 可以存在多个相同的 service_type 分别在不同的 svrid 时的 分发处理 
    CEasyArray<ServiceInfo> m_ServiceCheckState;
}; 



