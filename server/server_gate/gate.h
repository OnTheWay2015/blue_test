#pragma once


class Gate :public BridgeInterface
{
private:
    CoreBase* m_pCore;
    MessageHandler* m_pMessageHandler;

    ProtobufParseMessage* m_pMessageParser;
    MonitorConnecter* m_pMonitor;
    PacketManager* m_pPacketManager ;
public:
    Gate();
    ~Gate();
public:
    bool Init(LPCTSTR FileName);
    bool Start();
    void Update(float tm);

public://BridgeInterface
        virtual CoreBase* GetCore();
        virtual PacketManager* GetPacketManager() ;   
        virtual MonitorCoreHandler* GetMonitorCoreHandler(){return nullptr;}   
        virtual MonitorConnecter* GetMonitorConnecter() ;   
        virtual MessageHandler* GetMessageHandler() ;   
};


