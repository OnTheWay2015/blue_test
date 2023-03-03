#include "stdafx.h"
extern void proc_init();
int main()
{
    proc_init();
    
    CoreBase C;
    C.Init("./ConfigClient.xml");
    
    ProtobufParseMessage::GetInstance()->Init();
    MonitorConnecter::GetInstance()->Init(&C); 

    C.SetParseMessageHandler(ProtobufParseMessage::GetInstance());

    while(1)
    {
        C.Update();
        DoSleep(10);
    }
    return 0;
}










