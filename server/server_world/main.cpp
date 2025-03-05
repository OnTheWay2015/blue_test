#include "stdafx.h"
extern void proc_init();

BridgeInterface* g_pWorker;
int main()
{
    auto pWorker = MONITORED_NEW("World",World);
    g_pWorker = (BridgeInterface*)pWorker;
    pWorker->Init(_T("../../../conf/world.xml"));
    
    proc_init();
 
    pWorker->Start();
 
    while(1)
    {
        auto tm = 1.0;
        pWorker->Update(tm);
        DoSleep(5000);
    }
    return 0;
}










