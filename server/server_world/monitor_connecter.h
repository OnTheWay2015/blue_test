
#pragma once

class MonitorConnecter
    :public CoreHandlerInterface
	,public CStaticObject4<MonitorConnecter>

{
public:
    MonitorConnecter();
    ~MonitorConnecter();
private:
    CoreBase* m_pCorebase;
public:
    void Init(CoreBase* base);
public:
    virtual void Update() override;
    virtual void OnNetMessage(CSmartPtr<CoreSessionMessage> msg) override; 

}; 



