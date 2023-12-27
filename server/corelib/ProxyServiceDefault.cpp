/****************************************************************************/
/*                                                                          */
/*      文件名:    ProxyService.cpp                                */
/*      创建日期:  2009年10月23日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "stdafx.h"


CProxyServiceDefault::CProxyServiceDefault(void)
{
}

CProxyServiceDefault::~CProxyServiceDefault(void)
{
}

void CProxyServiceDefault::Init()
{
//test
    //for (int i=0;i<5;i++)
    //{
	//	auto p = std::make_shared<CProxyConnectionGroup>();
	//	p->SetID(i);
	//	p->Init();
	//	m_ConnectionGroups.push_back(p);
    //    p->Start();
    //}

}


int CProxyServiceDefault::Update(int ProcessPacketLimit)
{
	int ProcessCount = CNetService::Update(ProcessPacketLimit);

	return ProcessCount;
}


CSmartPtr<CBaseNetConnectionInterface> CProxyServiceDefault::CreateConnection(CIPAddress& RemoteAddress)
{
	//auto pConnection = std::make_shared<CProxyConnectionDefault>();
	//pConnection->Init();
	//pConnection->SetService(this);
	//return pConnection;

	auto pConnection = std::make_shared<CProxyConnectionDefault>();
	pConnection->Init();
    pConnection->SetClientProxy(GetServiceType(),GetClientProxyMode());

	pConnection->SetService(this);
	return pConnection;

}

bool CProxyServiceDefault::DeleteConnection(CSmartPtr<CBaseNetConnectionInterface>  pConnection)
{

    //for (auto it = m_ConnectionPool.begin(); it != m_ConnectionPool.end(); it++)
    //{
    //    if ( (*it).get() == pConnection)
    //    {
    //        m_ConnectionPool.erase(it);
    //        return true;
    //    }
    //}
    return false;
}

CProxyConnectionDefault* CProxyServiceDefault::GetConnection(UINT IDX)
{
	return nullptr;
	//if (IDX >= m_ConnectionPool.size())
	//{
	//	return nullptr;
	//}

	//return m_ConnectionPool[IDX].get(); //todo
}
LPVOID CProxyServiceDefault::GetFirstConnectionPos()
{
	return nullptr;
}
CProxyConnectionDefault* CProxyServiceDefault::GetNextConnection(LPVOID& Pos)
{
	return nullptr;
}

//void CProxyServiceDefault::AcceptConnection(CSmartPtr<CProxyConnectionDefault > pConnection)
//{
//    //if (m_ConnectionGroups.size() > 0)
//    //{
//    //    UINT Index = pConnection->GetID() % m_ConnectionGroups.size();
//    //    m_ConnectionGroups[Index]->AddConnection(pConnection);
//    //}
//    //else
//    //{
//    //    pConnection->Disconnect();
//    //    //PrintDOSLog(_T("未能找到合适的连接组"));
//    //}
//}


void CProxyServiceDefault::QueryDestoryConnection(CProxyConnectionDefault * pConnection)
{
}

bool CProxyServiceDefault::OnRecvProtected(CIPAddress IP)
{
	return true;
}

bool CProxyServiceDefault::AddBlackList(CIPAddress IP, UINT Duration)
{
    return true;
}

void CProxyServiceDefault::OnRecvData(CSmartPtr<CBaseNetConnectionInterface> pConnection, DOS_SIMPLE_MESSAGE_HEAD* pData)
{

}


void CProxyServiceDefault::OnConnection(CSmartPtr<CBaseNetConnectionInterface> c, bool IsSucceed)
{
    if (IsSucceed)
    {
        m_NetHandler->OnAccept(c);
    }
    else
    {
        //todo log err
    }
}

void CProxyServiceDefault::OnDisconnection(CSmartPtr<CBaseNetConnectionInterface> s)
{
    m_NetHandler->OnDisConnect(s);
}
void CProxyServiceDefault::OnRecvMessage(CSmartPtr<CBaseNetConnectionInterface> s, DOS_SIMPLE_MESSAGE_HEAD* pMsg)
{
    m_NetHandler->OnMessage(s,pMsg);
}




