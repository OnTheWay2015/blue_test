/****************************************************************************/
/*                                                                          */
/*      文件名:    OverLappedObject.h                                       */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class COverLappedObject;

struct OVERLAPPED_EX:public OVERLAPPED
{	
	COverLappedObject *	pOverLappedObject;
};

enum OVERLAPPED_OBJECT_TYPE
{
	IO_NOTSET,
	IO_RECV,
	IO_SEND,
	IO_ACCEPT,
	IO_ACCEPT2,
	IO_FILE_READ,
	IO_FILE_WRITE,
};

enum OVERLAPPED_OBJECT_STATUS
{
	OVERLAPPED_OBJECT_STATUS_FREE,
	OVERLAPPED_OBJECT_STATUS_IDLE,
	OVERLAPPED_OBJECT_STATUS_USING,
};

class CNetServer;

class COverLappedObject :public std::enable_shared_from_this<COverLappedObject>
{
protected:
	OVERLAPPED_EX 				m_OverLapped;
	UINT						m_ID;	
	OVERLAPPED_OBJECT_STATUS	m_Status;  //标识当前对象相关重复使用的状态
	SOCKET						m_AcceptSocket;	// WSAAccept 连接成功时创建的 socket_fd 
	CIPAddress					m_Address;  //ip,port
	socklen_t					m_AddressLen;
	CEasyBuffer					m_DataBuff;	
	int							m_TypeID;// OVERLAPPED_OBJECT_TYPE  功能分类.  Send/Recv/ACCEPT... 	
	UINT						m_RouterSession;
	UINT						m_ParentID;		//绑定 NetConnection  id 	
	UINT						m_ErrorCode;
	UINT						m_EventID;  //IOCP_EVENT_ID  , 当前只有两种 IOE_PACKET/IOE_ERROR, 
	
	CNetServer *				m_pCreator;	
	CSmartPtr<CIOCPEventRouter>			m_pIOCPEventRouter; //绑定事件操作实例,调用 OnIOCPEvent  
										/*
										==> COverLappedObject::Process()	
											==> m_pIOCPEventRouter->OnIOCPEvent(EventID,this); 
												[1] ==>AcceptSocket()  //CNetService 
														==> CNetService::CreateConnection() 子类重写这个方法		
												[2] ==> IO_RECV / 当收到的buf长度是0,说明断开连接, 有数据则放入队例, m_RecvDataQueue.push_back() //CNetConnection
														...> 循环里处理收到的数据
													==> IO_SEND ,IOCP通过 overlappedobject 发送完数据后触发事件,在这里回收  overlappedobject 
										*/
		
	


	
public:
	COverLappedObject(void);
	~COverLappedObject(void);

	void SetID(UINT ID);
	UINT GetID();

	void SetStatus(OVERLAPPED_OBJECT_STATUS Status);
	OVERLAPPED_OBJECT_STATUS GetStatus();

	BOOL Create(CNetServer * pCreator);


	void Clear();

	OVERLAPPED_EX * GetOverlapped();	
	CEasyBuffer * GetDataBuff();	

	void SetIOCPEventRouter( CSmartPtr<CIOCPEventRouter>  pIOCPEventRouter);
	void Process(int EventID);

	void SetType(int Type);
	int GetType();

	void SetAcceptSocket(SOCKET Socket);
	SOCKET GetAcceptSocket();

	void SetAddress(const CIPAddress& Address);
	CIPAddress& GetAddress();
	socklen_t& GetAddressLen();

	void SetRouterSession(UINT Session);
	UINT GetRouterSession();

	void SetParentID(UINT ID);
	UINT GetParentID();	

	void SetErrorCode(UINT ErrorCode);
	UINT GetErrorCode();

	void SetEventID(UINT EventID);
	UINT GetEventID();

	void Release();
};

inline void COverLappedObject::SetID(UINT ID)
{
	m_ID=ID;
}
inline UINT COverLappedObject::GetID()
{
	return m_ID;
}

inline void COverLappedObject::SetStatus(OVERLAPPED_OBJECT_STATUS Status)
{
	m_Status = Status;
}
inline OVERLAPPED_OBJECT_STATUS COverLappedObject::GetStatus()
{
	return m_Status;
}

inline OVERLAPPED_EX * COverLappedObject::GetOverlapped()
{
	return &m_OverLapped;
}

inline CEasyBuffer * COverLappedObject::GetDataBuff()
{
	return &m_DataBuff;
}

inline void COverLappedObject::SetIOCPEventRouter(CSmartPtr<CIOCPEventRouter> pIOCPEventRouter)
{
	m_pIOCPEventRouter=pIOCPEventRouter;
	if(m_pIOCPEventRouter)
		m_RouterSession=m_pIOCPEventRouter->GetSessionID();
	else
		m_RouterSession=0;
}

inline void COverLappedObject::Process(int EventID)
{
	if(m_pIOCPEventRouter)
	{			
		m_pIOCPEventRouter->OnIOCPEvent(EventID,this);			
		
	}
	else
	{
		//PrintNetLog( _T("IOCP包(%u)没有EventRouter，被忽略"), GetParentID());
		Release();
	}
}

inline void COverLappedObject::SetType(int Type)
{
	m_TypeID=Type;
}
inline int COverLappedObject::GetType()
{
	return m_TypeID;
}
inline void COverLappedObject::SetAcceptSocket(SOCKET Socket)
{
	m_AcceptSocket=Socket;
}
inline SOCKET COverLappedObject::GetAcceptSocket()
{
	return m_AcceptSocket;
}
inline void COverLappedObject::SetAddress(const CIPAddress& Address)
{
	m_Address=Address;
}
inline CIPAddress& COverLappedObject::GetAddress()
{
	return m_Address;
}

inline socklen_t& COverLappedObject::GetAddressLen()
{
	return m_AddressLen;
}



inline void COverLappedObject::SetRouterSession(UINT Session)
{
	m_RouterSession=Session;
}
inline UINT COverLappedObject::GetRouterSession()
{
	return m_RouterSession;
}

inline void COverLappedObject::SetParentID(UINT ID)
{
	m_ParentID=ID;
}
inline UINT COverLappedObject::GetParentID()
{
	return m_ParentID;
}

inline void COverLappedObject::SetErrorCode(UINT ErrorCode)
{
	m_ErrorCode=ErrorCode;
}

inline UINT COverLappedObject::GetErrorCode()
{
	return m_ErrorCode;
}

inline void COverLappedObject::SetEventID(UINT EventID)
{
	m_EventID=EventID;
}
inline UINT COverLappedObject::GetEventID()
{
	return m_EventID;
}



class IOverLappedObjectCreater 
{

};

