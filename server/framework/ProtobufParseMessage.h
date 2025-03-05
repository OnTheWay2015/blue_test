#pragma once

class BridgeInterface;

class ProtobufParseMessage 
    :public MessageParseInterface 
{
public:
    ProtobufParseMessage(BridgeInterface* bridge):m_pBridge(bridge) {}
    ~ProtobufParseMessage() {}
private:
    BridgeInterface* m_pBridge;
public:
    void Init();
    virtual CSmartPtr<void> ParseMessage(DOS_SIMPLE_MESSAGE_HEAD* h) override;
    virtual CSmartPtr<std::vector<unsigned char>> SerializeToArray(CSmartPtr<void> h) override;
    
};

