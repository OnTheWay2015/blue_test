#include "stdafx.h"
#include "../protocol/protocol.h"
#include "google/protobuf/message.h"

CSmartPtr<void> ProtobufParseMessage::ParseMessage(DOS_SIMPLE_MESSAGE_HEAD* h)
{
    //DOS_SIMPLE_MESSAGE_HEAD* pMsg = (DOS_SIMPLE_MESSAGE_HEAD*)m_AssembleBuffer.GetBuffer();
    int msgsize = h->MsgLen - sizeof(DOS_SIMPLE_MESSAGE_HEAD);
    auto msg = CONVERT_POINT(google::protobuf::Message, m_pBridge->GetPacketManager()->create(h->MsgID));
    //auto msg = CONVERT_POINT(google::protobuf::Message, PacketManager::GetInstance()->create(h->MsgID));
    if (!msg)
    {
        //todo log err
        return nullptr;
    }
    if (!msg->ParseFromArray((char*)h + sizeof(DOS_SIMPLE_MESSAGE_HEAD), msgsize))
    {
        //todo log err
        return nullptr;
    }
    PrintCoreDebugLog(_T("---- pb parse:\n%s"), msg->DebugString().c_str());
    return msg;
}
CSmartPtr<std::vector<unsigned char>> ProtobufParseMessage::SerializeToArray(CSmartPtr<void> h)
{
    auto msg = CONVERT_POINT(google::protobuf::Message, h);
    auto vec = std::make_shared<std::vector<unsigned char>>();
    auto sz =  msg->ByteSize();
    if (sz<=0)
    {
        PrintCoreDebugLog(_T("---- pb serialize error size 0"));
        return vec;
    }
    vec->resize(sz);
    if (!msg->SerializeToArray(&(*vec)[0], sz))
    {
        //todo log err
    }
    PrintCoreDebugLog(_T("---- pb serialize:\n%s"), msg->DebugString().c_str());
    return vec;
}

void ProtobufParseMessage::Init()
{
}

