#include "stdafx.h"




BridgeInterface* g_pWorker;
extern void proc_monitor_init();
int main()
{
    auto pWorker = MONITORED_NEW("Monitor",Monitor);
    g_pWorker = (BridgeInterface*)pWorker;
    pWorker->Init(_T("../../../conf/Monitor.xml"));

    proc_monitor_init();

    while(1)
    {
        auto tm = 1.0;
        pWorker->Update(tm);
        DoSleep(10);
    }
    return 0;
}


