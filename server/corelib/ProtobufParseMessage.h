#pragma once

class ProtobufParseMessage 
    :public MessageHandlerInterface 
	,public CStaticObject4<ProtobufParseMessage>
{
public:
    ProtobufParseMessage() {}
    ~ProtobufParseMessage() {}
public:
    void Init();
    virtual CSmartPtr<void> ParseMessage(DOS_SIMPLE_MESSAGE_HEAD* h) override;
    virtual CSmartPtr<std::vector<unsigned char>> SerializeToArray(CSmartPtr<void> h) override;
};
