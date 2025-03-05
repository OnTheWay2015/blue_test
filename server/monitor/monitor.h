#pragma once


class Monitor :public BridgeInterface
{
private:
    CoreBase* m_pCore;
    MonitorCoreHandler* m_pMonitorCoreHandler;
    ProtobufParseMessage* m_pMessageParser;
    PacketManager* m_pPacketManager ;
public:
    Monitor();
    ~Monitor();
public:
    bool Init(LPCTSTR FileName);
    void Update(float tm);

public://BridgeInterface
        virtual CoreBase* GetCore();
        virtual PacketManager* GetPacketManager() ;   
        virtual MonitorCoreHandler* GetMonitorCoreHandler();   
        virtual MonitorConnecter* GetMonitorConnecter() {return nullptr;}   
        virtual MessageHandler* GetMessageHandler() {return nullptr;}
};


