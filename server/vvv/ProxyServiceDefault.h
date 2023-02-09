/****************************************************************************/
/*                                                                          */
/*      文件名:    ProxyService.h                                  */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

struct IP_INFO
{
    UINT		ExpireTime;
    CIPAddress	IP;
};
class CProxyServiceDefault :
    public CNetService
{

protected:


	CEasyCriticalSection												m_EasyCriticalSection;

	CGuardThread														m_GuardThread;


	CIDStorage<CSmartPtr<CProxyConnectionDefault>>			m_ConnectionPool;
	//CThreadSafeIDStorage<CProxyConnectionDefault *>			m_DestoryConnectionList;
	CEasyArray<CSmartPtr<CProxyConnectionGroup>>							m_ConnectionGroups;

	//CHashMap<CIPAddress, IP_INFO>										m_IPBlackList;
	//CHashMap<CIPAddress, IP_INFO>										m_RecvProtectedIPList;
	CEasyArray<IP_INFO>													m_PrepareIPBlackList;
	CEasyCriticalSection												m_BlackListCriticalSection;
	CEasyTimer															m_BlackListUpdateTimer;

public:
	CProxyServiceDefault(void);
	virtual ~CProxyServiceDefault(void);
	
public:

	virtual int Update(int ProcessPacketLimit = DEFAULT_SERVER_PROCESS_PACKET_LIMIT) override;
	virtual CSmartPtr<CBaseNetConnection> CreateConnection(CIPAddress& RemoteAddress) override;
	virtual bool DeleteConnection(CBaseNetConnection * pConnection) override;
	virtual CProxyConnectionDefault* GetConnection(UINT ID);
	virtual LPVOID GetFirstConnectionPos();
	virtual CProxyConnectionDefault* GetNextConnection(LPVOID& Pos);
public:

	void Init();
	void AcceptConnection(CSmartPtr<CProxyConnectionDefault >  pConnection);
	void QueryDestoryConnection(CProxyConnectionDefault * pConnection);


	//CDOSServer * GetServer();

	bool AddBlackList(CIPAddress IP, UINT Duration);
	bool OnRecvProtected(CIPAddress IP);
};



