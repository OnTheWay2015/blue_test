#include "stdafx.h"



extern void proc_init();




int main()
{

    proc_init();

    CoreBase C;
    //C.Init("./configd.xml");
    C.Init("../../../conf/monitor.xml");
    ProtobufParseMessage::GetInstance()->Init();

    MonitorCoreHandler::GetInstance()->Init(); 

    C.SetHandler(MonitorCoreHandler::GetInstance(),SERVICE_TYPE::SERVER_MONITOR);
    C.SetParseMessageHandler(ProtobufParseMessage::GetInstance());


    while(1)
    {
        C.Update();
        DoSleep(10);
    }
    return 0;
}


