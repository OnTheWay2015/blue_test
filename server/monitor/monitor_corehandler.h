
#pragma once

class MonitorCoreHandler 
    :public CoreHandlerInterface
	,public CStaticObject4<MonitorCoreHandler>

{
public:
    MonitorCoreHandler();
    ~MonitorCoreHandler();
public:
    void Init();
public:
    virtual void Update() override;
    virtual void OnNetMessage(CSmartPtr<CoreSessionMessage> msg) override; 

private:
    std::vector<SERVER_INFO> m_SvrList; 
}; 



