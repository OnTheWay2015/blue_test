
#pragma once
class BridgeInterface;
class MonitorCoreHandler 
    :public CoreHandlerInterface

{
struct SERVER_INFO 
{
    UINT svrid;  //服务器ID
    CSmartPtr<CBaseNetConnectionInterface> session;
    UINT state; // =0 none ; =1,connected; =2,registed ; =3,closed
};


public:
    MonitorCoreHandler(BridgeInterface* bridge);
    ~MonitorCoreHandler();
public:
    void AddServiceInfo(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_LEADER_REG> msg);
    void RmvServiceInfo(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_LEADER_REG> msg);
    void GetSerivceInfos(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<MSG_LEADER_SERVER_LIST> msg);
public:
    virtual bool Init();
    virtual bool Start();
    virtual void Update(float tm) override;
    virtual void OnNetMessage(CSmartPtr<CoreSessionMessage> msg) override; 

private:
    BridgeInterface* m_pBridge;
    std::unordered_map<UINT,SERVER_INFO> m_Servers; 
    std::unordered_map<SERVICE_TYPE,std::vector<SERVICE_INFO>> m_ServiceList; 
}; 



