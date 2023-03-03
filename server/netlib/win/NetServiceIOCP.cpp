/****************************************************************************/
/*                                                                          */
/*      文件名:    NetServiceIOCP.cpp                                       */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "stdafx.h"


CNetService::CNetService()
{
	m_WantClose=false;
	m_pServer=NULL;
	m_CurProtocol = IPPROTO_TCP;
	m_CurAddressFamily = AF_INET;
	m_IPv6Only = false;
	m_AcceptQueueSize=0;
	m_RecvQueueSize=0;
	m_SendQueueSize=0;
	m_ParallelAcceptCount=0;
	m_ParallelRecvCount=0;
	m_IsUseListenThread=false;
	m_pIOCPEventRouter=NULL;	
	m_pListenThread=NULL;
	m_AcceptQueue.SetLockMode(false, true);
}

CNetService::~CNetService(void)
{
	Destory();
}

bool CNetService::OnIOCPEvent(int EventID, COverLappedObject * pOverLappedObjectV)
{		
	if (IsStop())
	{
		return false;
	}
	CSmartPtr<COverLappedObject> pOverLappedObject = pOverLappedObjectV->shared_from_this();

	if(IsWorking())
	{
		if(pOverLappedObject->GetType()==IO_ACCEPT)
		{			
			if(!QueryAccept())
			{
				PrintNetLog(_T("无法发出更多的Accept请求！"));
				QueryClose();
			}


			if(EventID==IOE_PACKET)
			{	
				SOCKET ListenSocket=m_Socket.GetSocket();
				if(setsockopt( pOverLappedObject->GetAcceptSocket(),
					SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, 
					(char *)&ListenSocket, sizeof(ListenSocket))!=SOCKET_ERROR)
				{
					m_AcceptQueue.PushBack(&pOverLappedObject);
					return true;
				}				
				else
				{
					PrintNetLog(_T("更新AcceptScoket状态失败！"));
				}
			}
			else
			{
				PrintNetLog(_T("Accept返回错误！"));
			}

			closesocket(pOverLappedObject->GetAcceptSocket());
		}
		else if(pOverLappedObject->GetType()==IO_ACCEPT2)
		{
			if(EventID==IOE_PACKET)
			{
				if(m_AcceptQueue.PushBack(&pOverLappedObject))
				{							
					return true;
				}
				else
					PrintNetLog(_T("Accept队列已满！"));
				closesocket(pOverLappedObject->GetAcceptSocket());
			}			
			else
			{
				PrintNetLog(_T("侦听失败，关闭侦听！"));
				QueryClose();
			}
		}
		else if(pOverLappedObject->GetType()==IO_RECV)
		{			
			if(!QueryUDPRecv())
			{
				PrintNetLog(_T("无法发出更多的UDPRecv请求！"));
				QueryClose();
			}

			if(EventID==IOE_PACKET)
			{
				GetServer()->AddUDPRecvBytes(pOverLappedObject->GetDataBuff()->GetUsedSize());

				if(m_AcceptQueue.PushBack(&pOverLappedObject))
				{						
					return true;
				}
				else
					PrintNetLog(_T("UDP数据接收队列已满！"));		
			}
			else
			{
				PrintNetLog(_T("接收失败！"));
			}				
		}
		else if(pOverLappedObject->GetType()==IO_SEND)
		{
			if(EventID==IOE_PACKET)
			{
				GetServer()->AddUDPSendBytes(pOverLappedObject->GetDataBuff()->GetUsedSize());
				ReleaseOverLappedObject(pOverLappedObject, 1);
				return true;
			}
			else
				PrintNetLog(_T("发送失败！"));
		}			
		else
			PrintNetLog(_T("Service收到非法IOCP包(%u)！"), pOverLappedObject->GetType());
	}
	else
		PrintNetLog(_T("Service未启用，IOCP包被忽略！"));
	ReleaseOverLappedObject(pOverLappedObject, 2);


	return false;
}

bool CNetService::Create(int Protocol, UINT AcceptQueueSize, UINT RecvQueueSize, UINT SendQueueSize, UINT ParallelAcceptCount, UINT ParallelRecvCount, bool IsUseListenThread, bool IPv6Only)
{	
	

	if(GetServer()==NULL)
		return false;

	Close();

	if(m_pIOCPEventRouter==NULL)
	{
		m_pIOCPEventRouter=GetServer()->CreateEventRouter();
		if (m_pIOCPEventRouter == NULL)
		{
			PrintNetLog(_T("无法分配到IOCPEventRouter"));
			return false;
		}
		m_pIOCPEventRouter->Init(this);
	}

	

	m_CurProtocol = Protocol;
	m_AcceptQueueSize=AcceptQueueSize;
	m_RecvQueueSize=RecvQueueSize;
	m_SendQueueSize=SendQueueSize;
	m_ParallelAcceptCount=ParallelAcceptCount;
	m_ParallelRecvCount=ParallelRecvCount;
	m_IsUseListenThread=IsUseListenThread;
	m_IPv6Only = IPv6Only;	
	m_WantClose = false;	
	m_StopFlag = false;
	return true;
}

void CNetService::Destory()
{
	Close();
}

bool CNetService::StartListen(const CIPAddress& Address)
{
	if (m_pIOCPEventRouter == NULL)
		Create();

	if (Address.IsIPv4())
		m_CurAddressFamily = AF_INET;
	else if (Address.IsIPv6())
		m_CurAddressFamily = AF_INET6;

	if (m_CurProtocol == IPPROTO_UDP)
	{

		if (!m_Socket.MakeSocket(m_CurAddressFamily, SOCK_DGRAM, IPPROTO_UDP))
			return false;
		m_AcceptQueue.Create(m_RecvQueueSize);
	}
	else if (m_CurProtocol == IPPROTO_TCP)
	{
		if (!m_Socket.MakeSocket(m_CurAddressFamily, SOCK_STREAM, IPPROTO_TCP))
			return false;
		m_AcceptQueue.Create(m_AcceptQueueSize);
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
	
	
	
	m_pIOCPEventRouter->Init(this);


	if(!m_IsUseListenThread||m_CurProtocol==IPPROTO_UDP)
	{
	
		if(!GetServer()->BindSocket(m_Socket.GetSocket()))
		{
			PrintNetLog(_T("(%d)Service绑定IOCP失败！"),GetID());
			Close();
			return false;
		}	
	}
	
	
	if(m_CurProtocol==IPPROTO_UDP)
	{		
		if(!m_Socket.Bind(Address))
		{
			PrintNetLog(_T("(%d)Service绑定失败！"),GetID());
			return false;
		}

		m_Socket.SetState(CNetSocket::SS_CONNECTED);

		for (UINT i = 0; i<m_ParallelRecvCount; i++)
		{
			if(!QueryUDPRecv())
			{
				PrintNetLog(_T("(%d)Service发出UDPRecv请求失败！"),GetID());
				Close();
				return false;
			}
		}
	}
	else
	{
		if(!m_Socket.Listen(Address))
		{
			PrintNetLog(_T("(%d)Service侦听失败！"),GetID());
			return false;
		}

		if(m_IsUseListenThread)
		{

			PrintNetLog(_T("(%d)Service启用线程侦听模式！ port[%d] GetClientProxyType[%d] GetClientProxyMode[%d]"),GetID(),Address.GetPort(),GetClientProxyType(),GetClientProxyMode());
			if (m_pListenThread == NULL)
				m_pListenThread = MONITORED_NEW(_T("CNetService"), CIOCPListenThread);
			m_pListenThread->Init(this,m_Socket.GetSocket());
			m_pListenThread->Start();
		}
		else
		{		
			PrintNetLog(_T("(%d)Service启用IOCP侦听模式！ port[%d] GetClientProxyType[%d] GetClientProxyMode[%d]"),GetID(),Address.GetPort(),GetClientProxyType(),GetClientProxyMode());
			for (UINT i = 0; i<m_ParallelAcceptCount; i++)
			{
				if(!QueryAccept())
				{
					PrintNetLog(_T("(%d)Service发出Accpet请求失败！"),GetID());
					Close();
					return false;
				}
			}
		}
	}	
	OnStartUp();
	return true;
}



void CNetService::Close()
{
	if (m_Socket.GetState()!=CNetSocket::SS_UNINITED)
		OnClose();

	m_Socket.Close();

	SAFE_RELEASE(m_pListenThread);

	m_WantClose = false;

	SetStop();

	if (m_pIOCPEventRouter)
	{
		m_pIOCPEventRouter->SetEventHander(NULL);
		GetServer()->DeleteEventRouter(m_pIOCPEventRouter);
		m_pIOCPEventRouter = NULL;
	}


	while(m_AcceptQueue.size()>0)
	{	
		auto pOverLappedObject = m_AcceptQueue.front();
		m_AcceptQueue.pop_front();
		ReleaseOverLappedObject(pOverLappedObject, 3);
	}	

	
}
void CNetService::QueryClose()
{
	m_WantClose = true;	
}
void CNetService::OnStartUp()
{
}
void CNetService::OnClose()
{
}

int CNetService::Update(int ProcessPacketLimit)
{	
	
	//处理Accept
	int PacketCount=0;
	while(m_AcceptQueue.size()>0)
	{
		auto pOverLappedObject = m_AcceptQueue.front();
		m_AcceptQueue.pop_front();
		if(pOverLappedObject->GetType()==IO_ACCEPT)
		{
			if(!AcceptSocketEx(pOverLappedObject->GetAcceptSocket(),pOverLappedObject->GetDataBuff()))
			{
				PrintNetLog(_T("(%d)AcceptSocketEx失败！"),GetID());
			}
		}
		else if(pOverLappedObject->GetType()==IO_ACCEPT2)
		{					
			if(!AcceptSocket(pOverLappedObject->GetAcceptSocket()))
			{
				PrintNetLog(_T("(%d)AcceptSocket失败！"),GetID());
			}
		}
		else if(pOverLappedObject->GetType()==IO_RECV)
		{				
			OnRecvData(pOverLappedObject->GetAddress(),
				(const BYTE *)pOverLappedObject->GetDataBuff()->GetBuffer(), pOverLappedObject->GetDataBuff()->GetUsedSize());
		}
		else
		{
			PrintNetLog(_T("(%d)Servicec收到不明类型(%d)的OverLapped！"), GetID(), pOverLappedObject->GetType());
		}
		ReleaseOverLappedObject(pOverLappedObject, 4);
		PacketCount++;
		if(PacketCount>=ProcessPacketLimit)
			break;
	}	

	//处理关闭
	if(m_WantClose)
	{
		Close();		
	}		
	return PacketCount;
}

//CSmartPtr<CBaseNetConnection> CNetService::CreateConnection(CIPAddress& RemoteAddress)
//{
//	return NULL;
//}

//bool CNetService::DeleteConnection(CBaseNetConnection * pConnection)
//{
//	return false;
//}


bool CNetService::QueryAccept()
{		

	//PrintNetLog(_T("(%d)发出AcceptEx请求！"),GetID());

	SOCKET	AcceptSocket=0;
	AcceptSocket = socket(m_CurAddressFamily, SOCK_STREAM, IPPROTO_TCP);
	if(AcceptSocket==INVALID_SOCKET)
	{
		PrintNetLog(_T("(%d)创建AcceptSocket失败(%u)！"),GetID(),GetLastError());
		return false;
	}

	auto pOverLappedObject = AllocOverLappedObject(IO_ACCEPT);
	if(pOverLappedObject==NULL)
	{
		PrintNetLog(_T("(%d)Service创建OverLappedObject失败！"),GetID());
		return false;
	}

	pOverLappedObject->SetType(IO_ACCEPT);
	pOverLappedObject->SetAcceptSocket(AcceptSocket);
	pOverLappedObject->SetIOCPEventRouter(m_pIOCPEventRouter);
	pOverLappedObject->SetParentID(GetID());
	
	static DWORD NumberOfBytes;	
		
	if(m_Socket.AcceptOverlapped(pOverLappedObject->GetAcceptSocket(),
		pOverLappedObject->GetDataBuff()->GetBuffer(),
		0,
		NumberOfBytes,pOverLappedObject->GetOverlapped()))
	{
		return true;
	}
	PrintNetLog(_T("发出Accept请求失败！"));
	closesocket(AcceptSocket);
	ReleaseOverLappedObject(pOverLappedObject, 5);

	return false;
	
}
bool CNetService::QueryUDPRecv()
{	

	auto pOverLappedObject = AllocOverLappedObject(IO_RECV);
	if(pOverLappedObject==NULL)
	{
		PrintNetLog(_T("(%d)Service创建热UDPRecv用OverLappedObject失败！"),GetID());
		return false;
	}

    pOverLappedObject->SetType(IO_RECV);
    pOverLappedObject->GetDataBuff()->SetUsedSize(0);
    pOverLappedObject->SetIOCPEventRouter(m_pIOCPEventRouter);
    pOverLappedObject->SetParentID(GetID());

	static DWORD NumberOfBytes;
	static DWORD Flag;	

	NumberOfBytes=0;
	Flag=0;

	if(m_Socket.RecvFromOverlapped(pOverLappedObject->GetAddress(),
		pOverLappedObject->GetAddressLen(),
		pOverLappedObject->GetDataBuff()->GetBuffer(),
		pOverLappedObject->GetDataBuff()->GetBufferSize(),
		NumberOfBytes,Flag,
		pOverLappedObject->GetOverlapped()))
	{
		return true;
	}
	PrintNetLog(_T("Service发出UDPRecv请求失败！"));	
	ReleaseOverLappedObject(pOverLappedObject, 6);
	return false;
}

bool CNetService::QueryUDPSend(const CIPAddress& IPAddress, LPCVOID pData, int Size)
{	
	
	auto pOverLappedObject = AllocOverLappedObject(IO_SEND);
	if(pOverLappedObject==NULL)
	{
		PrintNetLog(_T("创建UDPSend用OverLappedObject失败！"));
		return false;
	}

	pOverLappedObject->SetAddress(IPAddress);

	pOverLappedObject->SetType(IO_SEND);	
	pOverLappedObject->SetIOCPEventRouter(m_pIOCPEventRouter);
	pOverLappedObject->GetDataBuff()->SetUsedSize(0);
	pOverLappedObject->SetParentID(GetID());

	if(!pOverLappedObject->GetDataBuff()->PushBack(pData,Size))
	{
		ReleaseOverLappedObject(pOverLappedObject, 7);
		PrintNetLog(_T("要发送的数据包过大！"));
		return false;
	}
	

	static DWORD NumberOfBytes;
	static DWORD Flag;

	NumberOfBytes=0;
	Flag=0;

	if(m_Socket.SendToOverlapped(pOverLappedObject->GetAddress(),
		pOverLappedObject->GetDataBuff()->GetBuffer(),
		pOverLappedObject->GetDataBuff()->GetUsedSize(),
		NumberOfBytes,Flag,
		pOverLappedObject->GetOverlapped()))
	{		
		return true;
	}
	PrintNetLog(_T("Service发出UDPSend请求失败！"));	
	ReleaseOverLappedObject(pOverLappedObject, 8);
	return false;
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

	auto pConnection= CreateConnection(RemoteAddress);
	if(pConnection)
	{
		if(pConnection->Create(Socket,m_RecvQueueSize,m_SendQueueSize))	
		{					
			pConnection->SetRemoteAddress(RemoteAddress);				
			pConnection->SetLocalAddress(LocalAddress);					

			if(pConnection->StartWork())
			{
				OnAccept(pConnection);	
				return true;	
			}
			else
				PrintNetLog(_T("启动Connection失败！"));
		}
		else
		{
			PrintNetLog(_T("初始化Connection失败！"));
			closesocket(Socket);
		}
		//DeleteConnection(pConnection);
		//OnDisConnect(pConnection);
	}
	else
	{
		PrintNetLog(_T("创建Connection失败！"));	
		closesocket(Socket);
	}
	return false;
}

bool CNetService::AcceptSocketEx(SOCKET Socket,CEasyBuffer * pAcceptData)
{

	sockaddr * pLocalAddress = NULL;
	sockaddr * pRemoteAddress = NULL;
	int LocalAddressLen,RemoteAddressLen;

	GetAcceptExSockaddrs(pAcceptData->GetBuffer(), pAcceptData->GetUsedSize(),
		sizeof(CIPAddress) + 16,
		sizeof(CIPAddress) + 16,
		(sockaddr**)&pLocalAddress, &LocalAddressLen,
		(sockaddr**)&pRemoteAddress, &RemoteAddressLen);


	CIPAddress LocalAddress;
	CIPAddress RemoteAddress;			

	if(pLocalAddress)
	{
		LocalAddress.SetAddress(pLocalAddress, LocalAddressLen);
	}

	if(pRemoteAddress)
	{
		RemoteAddress.SetAddress(pRemoteAddress, RemoteAddressLen);
	}

//CreateConnection 和 DeleteConnection 在子类重写, 比如
//D:\work_def\MMOPrj\Dev\Server\MMOServer\Libs\DOSLib\WebSocketProxyService.cpp
	auto pConnection=CreateConnection(RemoteAddress);
	if(pConnection)
	{
		if(pConnection->Create(Socket,m_RecvQueueSize,m_SendQueueSize))	
		{					
			pConnection->SetRemoteAddress(RemoteAddress);				
			pConnection->SetLocalAddress(LocalAddress);					

			if(pConnection->StartWork())//创建的 pConnection 放入管理器中,在管理器循环调用 pConnection->Update()
			{				
				OnAccept(pConnection);	
				return true;	
			}
			else
				PrintNetLog(_T("启动Connection失败！"));
		}
		else
		{
			PrintNetLog(_T("初始化Connection失败！"));
			closesocket(Socket);
		}
		//DeleteConnection(pConnection);
		//OnDisConnect(pConnection);
	}
	else
	{
		PrintNetLog(_T("创建Connection失败！"));	
		closesocket(Socket);
	}
	return false;
}

CSmartPtr<COverLappedObject>  CNetService::AllocOverLappedObject(int Type)
{
    auto  pObject =m_pServer->AllocOverLappedObject(GetID());
    pObject->SetType(Type);
    pObject->SetStatus(OVERLAPPED_OBJECT_STATUS_USING);
    pObject->GetDataBuff()->SetUsedSize(0);
    pObject->SetIOCPEventRouter(m_pIOCPEventRouter);
    return pObject;
}
bool CNetService::ReleaseOverLappedObject(CSmartPtr<COverLappedObject>  pObject, int Where)
{
    m_pServer->ReleaseOverLappedObject(pObject);
	return true;
}