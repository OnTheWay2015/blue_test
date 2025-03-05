/****************************************************************************/
/*                                                                          */
/*      文件名:    ProxyConnection.cpp                                   */
/*      创建日期:  2009年10月23日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "stdafx.h"


static void DumpBinData(const BYTE * pData, UINT DataLen, UINT MaxDumpSize)
{
	static UINT Index = 0;

	AtomicInc(&Index);
	if (DataLen > MaxDumpSize)
		DataLen = MaxDumpSize;
	for (UINT i = 0; i < DataLen; i += 8)
	{
		//PrintDOSLog(_T("%02X %02X %02X %02X %02X %02X %02X %02X"),
		//	((i < DataLen) ? pData[i] : 0),
		//	((i + 1 < DataLen) ? pData[i + 1] : 0),
		//	((i + 2 < DataLen) ? pData[i + 2] : 0),
		//	((i + 3 < DataLen) ? pData[i + 3] : 0),
		//	((i + 4 < DataLen) ? pData[i + 4] : 0),
		//	((i + 5 < DataLen) ? pData[i + 5] : 0),
		//	((i + 6 < DataLen) ? pData[i + 6] : 0),
		//	((i + 7 < DataLen) ? pData[i + 7] : 0)
		//);
	}
}




CProxyConnectionDefault::CProxyConnectionDefault(void)
{
	m_RecentPingDelay = 0;
	m_pService = NULL;
	m_pGroup = NULL;
	m_Status = STATUS_NONE;
	m_pCompressBuffer = NULL;
	m_pEncyptBuffer = NULL;
	m_pLZOCompressWorkBuffer = NULL;
	m_RecvCount = 0;
	m_RecvFlow = 0;
	m_ReleaseTime = 0;
	m_TotalMsgSendCount = 0;
	m_TotalMsgRecvCount = 0;
	m_BroadcastGroupMask = 0;
}

CProxyConnectionDefault::~CProxyConnectionDefault(void)
{
}






bool CProxyConnectionDefault::Init()
{
	m_Status = STATUS_INITED;
    if (!m_AssembleBuffer.Create(1024* 1024))
    {
        //PrintDOSLog(_T("创建%u大小的拼包缓冲失败！"),
        //    m_Config.MaxMsgSize * 2);
        return false;
    }
	return true;
}
void CProxyConnectionDefault::SetService(CBaseNetServiceInterface* pService)
{
	m_pService = pService;
	SetServer(pService->GetServer());
}






void CProxyConnectionDefault::OnRecvData(const BYTE * pData, UINT DataSize)// CNetConnection::Update 调用
{
	MSG_LEN_TYPE PacketSize = 0;
	UINT PeekPos = 0;

	//DumpBinData((BYTE*)pData,DataSize, 2048);

	if (!m_AssembleBuffer.PushBack(pData, DataSize))
	{
		//PrintDOSLog(_T("对象代理拼包缓冲溢出(%u/%u)，连接断开！"), m_AssembleBuffer.GetUsedSize(), m_AssembleBuffer.GetBufferSize());
		//Disconnect();		
		QueryDisconnect();	
	}
	else
	{
		if (m_AssembleBuffer.Peek(PeekPos, &PacketSize, sizeof(MSG_LEN_TYPE)))
		{
			while (m_AssembleBuffer.GetUsedSize() >= PacketSize&&PacketSize)
			{
				if (PacketSize < sizeof(DOS_SIMPLE_MESSAGE_HEAD))
				{
					//Disconnect();
					QueryDisconnect();	
					//PrintDOSLog( _T("对象代理收到非法包，连接断开！"));
					break;
				}
				
				DOS_SIMPLE_MESSAGE_HEAD* pMsg = (DOS_SIMPLE_MESSAGE_HEAD*)m_AssembleBuffer.GetBuffer();
				RecvMsg(pMsg);		
			
				//CDOSSimpleMessage * pMsg = (CDOSSimpleMessage *)m_AssembleBuffer.GetBuffer();
				//ProcessClientMsg(pMsg->GetMsgID(), pMsg->GetMsgFlag(), pMsg->GetCRC(), pMsg->GetMsgData(), pMsg->GetDataLength());				
				m_AssembleBuffer.PopFront(NULL, PacketSize);
				PeekPos = 0;
				PacketSize = 0;
				if (!m_AssembleBuffer.Peek(PeekPos, &PacketSize, sizeof(MSG_LEN_TYPE)))
					break;
			}
		}
	}
}

void CProxyConnectionDefault::RecvMsg(DOS_SIMPLE_MESSAGE_HEAD* m)
{
	m_pService->OnRecvMessage(shared_from_this(), m);	

}
bool CProxyConnectionDefault::SendMsg(CSmartPtr<DOS_SIMPLE_MESSAGE> msg) 
{
    //Send(pMsg,pMsg->MsgLen);

    Send(msg.get(), sizeof(DOS_SIMPLE_MESSAGE_HEAD));

	auto pCoreNet = CONVERT_POINT( CoreNetManager  ,this->GetServer()->GetHandler());
	auto h = pCoreNet->GetParseMessageHandler();
    auto buf = h->SerializeToArray(msg->MSG);
    if (buf->size() <= 0)
    {
        PrintCoreLog("SendMsg failed");
        return false;
    }
    Send(&(*buf)[0], buf->size());
    return true;



}


void CProxyConnectionDefault::OnConnection(bool IsSucceed)
{
	m_NeedDelayClose = false;
	m_KeepAliveTimer.SaveTime();
	m_KeepAliveCount = 0;
	m_RecentPingDelay = 0;
	m_RecvCount = 0;
	m_RecvFlow = 0;
	if (IsSucceed)
	{
		//PrintDOSDebugLog(_T("收到代理对象的连接！IP=%s"), GetRemoteAddress().GetIPString());		
		m_Status = STATUS_CONNECTED;
		//SendProtocolOption();
//test
       // auto m = std::make_shared<DOS_SIMPLE_MESSAGE_HEAD>();
       // m->MsgLen = sizeof(DOS_SIMPLE_MESSAGE_HEAD);
       // m->MsgFlag = 123;
       // SendMsg(m.get());

	}
	else
	{
		//PrintDOSLog(_T("连接初始化失败！IP=%s"), GetRemoteAddress().GetIPString());
		m_Status = STATUS_DISCONNECTED;
	}

	//m_pService->OnConnection(shared_from_this(),IsSucceed);


}
void CProxyConnectionDefault::OnDisconnection()
{
	//PrintDOSDebugLog(_T("对象代理的连接断开！IP=%s,MsgRecv=%u,MsgSend=%u"),
	//	GetRemoteAddress().GetIPString(), m_TotalMsgRecvCount, m_TotalMsgSendCount);
	//SendDisconnectNotify();

	m_Status = STATUS_DISCONNECTED;
	m_pService->OnDisconnection(shared_from_this());	
}

int CProxyConnectionDefault::Update(int ProcessPacketLimit)
{
	int PacketLimit = ProcessPacketLimit / 2;
	int PacketCount = 0;

	PacketCount += CNetConnection::Update(PacketLimit);

	//if (m_KeepAliveTimer.IsTimeOut(m_KeepAliveTime))
	//{
	//	m_KeepAliveTimer.SaveTime();
	//	m_KeepAliveCount++;
	//	if (m_UseServerInitiativeKeepAlive)
	//	{
	//		//SendKeepAliveMsg();
	//	}
	//	if (m_KeepAliveCount >= m_MaxKeepAliveCount)
	//	{
	//		//PrintDOSLog(_T("KeepAlive超时！BuffLeft=%u"), m_AssembleBuffer.GetUsedSize());
	//		m_KeepAliveCount = 0;
	//		//Disconnect();
	//		QueryDisconnect();	
	//	}
	//}


	//if (m_MessageMap.GetObjectCount() == 0)
	//{
	//	if (m_UnacceptConnectionKeepTimer.IsTimeOut(m_Config.UnacceptConnectionKeepTime))
	//	{
	//		PrintDOSDebugLog( _T("未确认连接存在时间过长！"));
	//		Disconnect();
	//	}
	//}

	return PacketCount;
}


//inline CDOSServer * CProxyConnectionDefault::GetServer()
//{
//	return (CDOSServer *)CNetConnection::GetServer();
//}



