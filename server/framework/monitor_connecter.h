
#pragma once

class BridgeInterface;

class MonitorConnecter
    :public CoreHandlerInterface
{
public:
    MonitorConnecter(BridgeInterface* bridge);
    ~MonitorConnecter();
private:
    BridgeInterface* m_pBridge;
    //CoreBase* m_pCorebase;
    CSmartPtr<CBaseNetConnectionInterface> m_session;
    //CoreBase* GetCore();
public:
    void GetServiceList(); 
public:
    virtual bool Init();
    virtual bool Start();
    virtual void Update(float tm) override;
    virtual void OnNetMessage(CSmartPtr<CoreSessionMessage> msg) override; 
}; 



