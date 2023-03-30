/****************************************************************************/
/*                                                                          */
/*      文件名:    NetServiceEpoll.cpp                                      */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "../stdafx.h"


CNetService::CNetService()
{
	m_WantClose=false;
	m_IsEventProcessing = false;
	m_pServer=NULL;
	m_CurProtocol=IPPROTO_TCP;
	m_CurAddressFamily = AF_INET;
	m_IPv6Only = false;
	m_AcceptQueueSize=0;
	m_RecvQueueSize=0;
	m_SendQueueSize=0;
	m_ParallelAcceptCount=0;
	m_ParallelRecvCount=0;
	m_pEpollEventRouter=NULL;
}

CNetService::~CNetService(void)
{
	Destory();
}

bool CNetService::OnEpollEvent(UINT EventID)
{
	PrintNetLog("CNetService::OnEpollEvent(%d)", EventID);
	m_IsEventProcessing = true;
	if(IsWorking())
	{
		if(EventID&(EPOLLERR|EPOLLHUP))
		{
			PrintNetLog("CNetService::Epoll发生错误%d！",errno);
			QueryClose();
			m_IsEventProcessing = false;
			return true;
		}
		if(EventID&EPOLLIN)
		{
			if(m_CurProtocol==IPPROTO_UDP)
			{
				PrintNetLog("CNetService::OnEpollEvent EPOLLIN IPPROTO_UDP");
				DoUDPRecv();
			}
			else
			{
				PrintNetLog("CNetService::OnEpollEvent EPOLLIN IPPROTO_TCP");
				DoAcceptSocket();
			}
		}
		if(EventID&EPOLLOUT)
		{
			PrintNetLog("CNetService::OnEpollEvent EPOLLOUT");
			DoUDPSend();
		}
	}
	else
	{
		PrintNetLog("Service未启用，事件被忽略！");
	}
	m_IsEventProcessing = false;
	return false;
}

bool CNetService::Create(int Protocol, int AcceptQueueSize, int RecvQueueSize, int SendQueueSize, int ParallelAcceptCount, int ParallelRecvCount, bool IsUseListenThread, bool IPv6Only)
{
	if(GetServer()==NULL)
		return false;

	Close();

	if(m_pEpollEventRouter==NULL)
	{
		m_pEpollEventRouter=GetServer()->CreateEventRouter();
		if (m_pEpollEventRouter == NULL)
		{
			PrintNetLog(_T("无法分配到EpollEventRouter"));
			return false;
		}
		m_pEpollEventRouter->Init(this);
	}
	m_CurProtocol = Protocol;
	m_AcceptQueueSize=AcceptQueueSize;
	m_RecvQueueSize=RecvQueueSize;
	m_SendQueueSize=SendQueueSize;
	m_ParallelAcceptCount=ParallelAcceptCount;
	m_ParallelRecvCount=ParallelRecvCount;	
	m_IPv6Only = IPv6Only;
	m_WantClose=false;
	

	m_Socket.EnableBlocking(false);
	return true;
}

void CNetService::Destory()
{
	Close();
}


bool CNetService::StartListen(const CIPAddress& Address)
{
	if (m_pEpollEventRouter == NULL)
		Create();

	if (Address.IsIPv4())
		m_CurAddressFamily = AF_INET;
	else if (Address.IsIPv6())
		m_CurAddressFamily = AF_INET6;

	if (m_CurProtocol == IPPROTO_UDP)
	{
		if (!m_Socket.MakeSocket(m_CurAddressFamily, SOCK_DGRAM, IPPROTO_UDP))
			return false;
		//m_RecvQueue.Create(m_RecvQueueSize);
		//m_SendQueue.Create(m_SendQueueSize);
	}
	else if (m_CurProtocol == IPPROTO_TCP)
	{
		if (!m_Socket.MakeSocket(m_CurAddressFamily, SOCK_STREAM, IPPROTO_TCP))
			return false;
		//m_AcceptQueue.Create(m_AcceptQueueSize);
	}
	else
	{
		PrintNetLog( _T("(%d)Service协议错误%d！"), GetID(), m_CurProtocol);
		return false;
	}

	if (!m_IPv6Only)
	{
		//启用同时侦听IPv6和IPv4
		DWORD Value = 0;
		socklen_t Len = sizeof(Value);
		m_Socket.SetOption(IPPROTO_IPV6, IPV6_V6ONLY, (char *)&Value, Len);
	}
	

	if(m_Socket.GetState()==CNetSocket::SS_UNINITED)
	{
		if(!Create())
			return false;
	}

	m_pEpollEventRouter->Init(this);

	if(!m_Socket.EnableBlocking(false))
	{
		PrintNetLog("(%d)Service开始非阻塞模式失败！",GetID());
		return false;
	}

	if(m_CurProtocol==IPPROTO_UDP)
	{
		if(!m_Socket.Bind(Address))
		{
			PrintNetLog("(%d)Service绑定失败！",GetID());
			return false;
		}
		m_Socket.SetState(CNetSocket::SS_CONNECTED);
	}
	else
	{
		if(!m_Socket.Listen(Address))
		{
			PrintNetLog("(%d)Service侦听失败！",GetID());
			return false;
		}
	}


	if (!GetServer()->BindSocket(m_Socket.GetSocket(), m_pEpollEventRouter, false))
	//if (!GetServer()->BindSocket(m_Socket.GetSocket(), m_pEpollEventRouter, true))
	{
		PrintNetLog("(%d)Service绑定Socket失败！", GetID());
		return false;
	}

	OnStartUp();
	return true;
}
void CNetService::Close()
{
	if (m_Socket.GetState() != CNetSocket::SS_UNINITED)
		OnClose();

	m_Socket.Close();

	m_WantClose = false;

	while (m_IsEventProcessing)
	{
		DoSleep(1);
	}

	if (m_pEpollEventRouter)
	{
		m_pEpollEventRouter->SetEventHander(NULL);
		GetServer()->DeleteEventRouter(m_pEpollEventRouter);
		m_pEpollEventRouter = NULL;
	}

	SOCKET Scoket;
	//while (m_AcceptQueue.PopFront(&Scoket))
	//{
	//	closesocket(Scoket);
	//}

	while(m_AcceptQueue.size()>0)
	{	
		auto Scokdet= m_AcceptQueue.front();
		m_AcceptQueue.pop_front();
		closesocket(Scoket);
	}	




	m_RecvQueue.clear();
	m_SendQueue.clear();
	m_AcceptQueue.clear();
}
void CNetService::QueryClose()
{
	m_WantClose=true;
}
void CNetService::OnStartUp()
{
}
void CNetService::OnClose()
{
}

int CNetService::Update(int ProcessPacketLimit)
{
	//处理UDP接收
	int PacketCount=0;
	int Limit = ProcessPacketLimit;
	while (m_RecvQueue.size()>0)
	{
		auto pEpollEventObject = m_RecvQueue.front();
		if (pEpollEventObject->GetType() == IO_RECV)
		{
			OnRecvData(pEpollEventObject->GetAddress(),
				(BYTE *)pEpollEventObject->GetDataBuff()->GetBuffer(), pEpollEventObject->GetDataBuff()->GetUsedSize());
		}
		else
		{
			PrintNetLog("(%d)Servicec收到不明类型的OverLapped！",GetID());
		}
		m_RecvQueue.pop_front();
		PacketCount++;
		Limit--;
		if (Limit <= 0)
			break;
	}

	//处理Accept
	while (m_AcceptQueue.size()>0)
	{
		SOCKET Scoket = m_AcceptQueue.front();
		if (!AcceptSocket(Scoket))
		{
			PrintNetLog("(%d)AcceptSocket失败！", GetID());
		}		
		m_AcceptQueue.pop_front();
		PacketCount++;
		Limit--;
		if (Limit <= 0)
			break;
	}

	//处理关闭
	if(m_WantClose)
	{
		Close();
	}
	return PacketCount;
}

CSmartPtr<CBaseNetConnectionInterface> CNetService::CreateConnection(CIPAddress& RemoteAddress)
{
	return NULL;
}

bool CNetService::DeleteConnection(CBaseNetConnection * pConnection)
{
	return false;
}

bool CNetService::QueryUDPSend(const CIPAddress& IPAddress,LPCVOID pData,int Size)
{
	CAutoLock Lock(&m_SendLock);

	//CEpollEventObject * pEpollEventObject = m_SendQueue.GetFreeTail();
	//if(pEpollEventObject==NULL)
	//{
	//	PrintNetLog("(%d)Service创建UDPSend用OverLappedObject失败！",GetID());
	//	return false;
	//}

	//pEpollEventObject->SetAddress(IPAddress);

	//pEpollEventObject->SetType(IO_SEND);
	//pEpollEventObject->GetDataBuff()->SetUsedSize(0);
	//pEpollEventObject->SetParentID(GetID());

	//if(!pEpollEventObject->GetDataBuff()->PushBack(pData,Size))
	//{		
	//	PrintNetLog("(%d)Service要发送的数据包过大！",GetID());
	//	return false;
	//}

	//m_SendQueue.PushBack(NULL);
	//DoUDPSend();
	return true;	
}

void CNetService::OnRecvData(const CIPAddress& IPAddress, const BYTE * pData, UINT DataSize)
{
}




bool CNetService::AcceptSocket(SOCKET Socket)
{

	CIPAddress LocalAddress;
	CIPAddress RemoteAddress;

	socklen_t LocalAddressLen = sizeof(CIPAddress);
	socklen_t RemoteAddressLen = sizeof(CIPAddress);

	getsockname(Socket,
		LocalAddress.GetSockAddrPtr(),
		&LocalAddressLen);

	getpeername(Socket,
		RemoteAddress.GetSockAddrPtr(),
		&RemoteAddressLen);

	auto pConnection=CreateConnection(RemoteAddress);
	if(pConnection)
	{
		PrintNetLog("(%d)CreateConnection OK ！",GetID());
		if(pConnection->Create(Socket,m_RecvQueueSize,m_SendQueueSize))
		{
			PrintNetLog("(%d) Connection Create OK ！",GetID());
			pConnection->SetRemoteAddress(RemoteAddress);
			pConnection->SetLocalAddress(LocalAddress);

			if(pConnection->StartWork())
			{
				PrintNetLog("(%d) Connection StartWork OK ！",GetID());
				OnAccept(pConnection);	
				return true;
			}
			else
				PrintNetLog("(%d)Service启动Connection失败！",GetID());
		}
		else
		{
			PrintNetLog("(%d)Service初始化Connection失败！",GetID());
			closesocket(Socket);
		}
		//DeleteConnection(pConnection.get());
	}
	else
	{
		PrintNetLog("(%d)Servicec创建Connection失败！",GetID());
		closesocket(Socket);
	}
	return false;
}

void CNetService::DoAcceptSocket()
{
	CIPAddress Address;
			
	PrintNetLog("CNetService::DoAcceptSocket");

	while(true)
	{
		//socklen_t AddressLen = sizeof(CIPAddress);
		socklen_t AddressLen = sizeof(sockaddr_in );
		SOCKET AcceptSocket = accept(m_Socket.GetSocket(), Address.GetSockAddrPtr(), &AddressLen);
		if(AcceptSocket==INVALID_SOCKET)
		{
			int ErrorCode=errno;
			//if(ErrorCode==EAGAIN)// EAGAIN, EWOULDBLOCK(11)
			//{
			//	PrintNetLog("CNetService::DoAcceptSocket EAGAIN, continue");
			//	DoSleep(1);
			//	continue;
			//}
			
			PrintNetLog("Accept fail, errno(%u)  acceptQueue_size(%u)", ErrorCode, m_AcceptQueue.size());
			break;
		}

		m_AcceptQueue.push_back(AcceptSocket);
		PrintNetLog("Accept OK socketid[%lu] acceptQueue_size(%u)", AcceptSocket, m_AcceptQueue.size());

		//{
		//	closesocket(AcceptSocket);
		//	PrintNetLog("CNetService创建Accept用EpollEventObject失败(%u)！", m_AcceptQueue.size());
		//}
	}
}

void CNetService::DoUDPRecv()
{
	//while(true)
	//{
	//	CEpollEventObject * pEpollEventObject = m_RecvQueue.GetFreeTail();
	//	int RecvSize=0;
	//	if(pEpollEventObject)
	//	{
	//		pEpollEventObject->SetType(IO_RECV);
	//		pEpollEventObject->SetParentID(GetID());

	//		 RecvSize=recvfrom(
	//			m_Socket.GetSocket(),
	//			pEpollEventObject->GetDataBuff()->GetBuffer(),
	//			pEpollEventObject->GetDataBuff()->GetBufferSize(),
	//			0,
	//			pEpollEventObject->GetAddress().GetSockAddrPtr(),
	//			&(pEpollEventObject->GetAddressLen()));
	//		 
	//	}
	//	else
	//	{
	//		PrintNetLog("CNetService创建Recv用EpollEventObject失败,数据将被丢弃！");

	//		static char RecvBuffer[NET_DATA_BLOCK_SIZE];
	//		static CIPAddress FromAddr;
	//		static socklen_t FromAddrLen;

	//		FromAddrLen=sizeof(FromAddr);
	//		RecvSize=recvfrom(
	//			m_Socket.GetSocket(),
	//			RecvBuffer,
	//			NET_DATA_BLOCK_SIZE,
	//			0,
	//			FromAddr.GetSockAddrPtr(),
	//			&FromAddrLen);
	//	}

	//	if (RecvSize>0)
	//	{
	//		GetServer()->AddUDPRecvBytes(RecvSize);
	//		if(pEpollEventObject)
	//		{
	//			pEpollEventObject->GetDataBuff()->SetUsedSize(RecvSize);
	//			m_RecvQueue.PushBack(NULL);
	//		}
	//	}
	//	else
	//	{
	//		int ErrorCode=errno;
	//		switch(ErrorCode)
	//		{
	//		case EAGAIN:
	//			return;
	//		case EINTR:
	//			PrintNetLog("Recv被系统中断");
	//			break;
	//		default:
	//			PrintNetLog("Recv失败(%u),Socket关闭",ErrorCode);
	//			QueryClose();
	//			return;
	//		}
	//	}
	//}
}

void CNetService::DoUDPSend()
{
	//CAutoLock Lock(&m_SendLock);

	//CEpollEventObject * pEpollEventObject = m_SendQueue.GetUsedTop();
	//while (pEpollEventObject)
	//{
	//	int SendSize=sendto(
	//		m_Socket.GetSocket(),
	//		pEpollEventObject->GetDataBuff()->GetBuffer(),
	//		pEpollEventObject->GetDataBuff()->GetUsedSize(),
	//		0,
	//		pEpollEventObject->GetAddress().GetSockAddrPtr(),
	//		sizeof(CIPAddress));



	//	if(SendSize>=0)
	//	{
	//		GetServer()->AddUDPSendBytes(SendSize);
	//		if (SendSize < (int)pEpollEventObject->GetDataBuff()->GetUsedSize())
	//		{
	//			PrintNetLog("Send没有完成，数据被部分丢弃");
	//		}
	//		m_SendQueue.PopFront(NULL);
	//	}
	//	else
	//	{
	//		int ErrorCode=errno;
	//		if(ErrorCode!=EAGAIN)
	//		{
	//			PrintNetLog("Send失败(%u)",ErrorCode);
	//		}
	//		return;
	//	}
	//	pEpollEventObject = m_SendQueue.GetUsedTop();
	//}
}
