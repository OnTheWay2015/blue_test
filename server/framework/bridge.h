#pragma once
class BridgeInterface 
{
    public:
        virtual CoreBase* GetCore()=0;
        virtual PacketManager* GetPacketManager() = 0;   
        virtual MonitorCoreHandler* GetMonitorCoreHandler() = 0;   
        virtual MonitorConnecter* GetMonitorConnecter() = 0;   
        virtual MessageHandler* GetMessageHandler() = 0;   


};

