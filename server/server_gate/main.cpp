#include "stdafx.h"

extern void proc_init();
extern void proc_gate_init();

BridgeInterface* g_pWorker;
int main()
{
    auto pWorker = MONITORED_NEW("Gate",Gate);
    g_pWorker = (BridgeInterface*)pWorker;
    pWorker->Init(_T("../../../conf/gate.xml"));
    
    proc_init();
    proc_gate_init();
 
    pWorker->Start();
 
    while(1)
    {
        auto tm = 1.0;
        pWorker->Update(tm);
        DoSleep(5000);// 1000 =1s
    }
    return 0;
}












/*
extern int InitFramework();
extern void proc_gate_init();
int main()
{
    InitFramework();
    proc_gate_init();
    
    CoreBase C;
    //C.Init("./configclient.xml");
    C.Init(_T("../../../conf/gate.xml"));
    
    ProtobufParseMessage::GetInstance()->Init();
    MonitorConnecter::GetInstance()->Init(&C); 

    C.SetParseMessageHandler(ProtobufParseMessage::GetInstance());

    while(1)
    {
        auto tm = 1.0;
        C.Update(tm);
        MonitorConnecter::GetInstance()->Update(tm);
        MessageHandler::GetInstance()->Update(tm); 
        DoSleep(10);
    }
    return 0;
}





*/




