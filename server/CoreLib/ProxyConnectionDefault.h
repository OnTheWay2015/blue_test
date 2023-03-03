/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSProxyConnection.h                                     */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once




class CProxyServiceDefault;
class CProxyConnectionGroup;

class CProxyConnectionDefault :
	public CNetConnection,
	public  std::enable_shared_from_this<CProxyConnectionDefault>
{
public:

	enum STATUS
	{
		STATUS_NONE,
		STATUS_INITED,
		STATUS_CONNECTED,
		STATUS_DISCONNECTED,
		STATUS_DESTORYED,
	};
protected:
    CBaseNetServiceInterface* m_pService;
    CProxyConnectionGroup* m_pGroup;
    volatile STATUS								m_Status;



	UINT										m_KeepAliveCount;
	UINT										m_MaxKeepAliveCount;
	UINT										m_KeepAliveTime;
	CEasyTimer									m_KeepAliveTimer;
	CEasyTimer									m_UnacceptConnectionKeepTimer;
	bool										m_UseServerInitiativeKeepAlive;
	UINT										m_RecentPingDelay;

	bool										m_NeedDelayClose;
	CEasyTimer									m_DelayCloseTimer;

	CEasyBuffer									m_AssembleBuffer;
	CEasyBuffer *								m_pCompressBuffer;
	CEasyBuffer *								m_pEncyptBuffer;
	char *										m_pLZOCompressWorkBuffer;

	UINT										m_RecvCount;
	UINT										m_RecvFlow;
	CEasyTimer									m_RecvProtectCheckTimer;
	UINT										m_TotalMsgSendCount;
	UINT										m_TotalMsgRecvCount;

	UINT										m_ReleaseTime;

	volatile SESSION_ID								m_BroadcastGroupMask;

public:
	CProxyConnectionDefault(void);
	virtual ~CProxyConnectionDefault(void);


protected:
	virtual void RecvMsg(DOS_SIMPLE_MESSAGE_HEAD* pMsg);

public:
	virtual bool SendMsg(CSmartPtr<DOS_SIMPLE_MESSAGE> msg) override ;
public:
	virtual void OnRecvData(const BYTE * pData, UINT DataSize) override;
	virtual void OnConnection(bool IsSucceed) override;
	virtual void OnDisconnection() override;
	virtual int Update(int ProcessPacketLimit = DEFAULT_SERVER_PROCESS_PACKET_LIMIT) override;
public:
	bool Init();
	void SetService(CBaseNetServiceInterface* pService);



	STATUS GetStatus()
	{
		return m_Status;
	}
	void SetGroup(CProxyConnectionGroup * pGroup)
	{
		m_pGroup = pGroup;
	}
protected:

//	CDOSServer * GetServer();
	
	//virtual void OnKeepAliveMsg(UINT Timestamp, UINT RecentDelay);
	//virtual void OnKeepAliveAckMsg(UINT Timestamp);
	//virtual void SendKeepAliveMsg();
	//virtual void SendProtocolOption();
};
