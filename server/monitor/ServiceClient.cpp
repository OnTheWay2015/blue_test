/****************************************************************************/
/*                                                                          */
/*      文件名:    DOSObjectProxyService.cpp                                */
/*      创建日期:  2009年10月23日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "stdafx.h"


CDOSObjectProxyServiceDefault::CDOSObjectProxyServiceDefault(void)
{
}

CDOSObjectProxyServiceDefault::~CDOSObjectProxyServiceDefault(void)
{
}

void CDOSObjectProxyServiceDefault::Init()
{
//test
    for (int i=0;i<5;i++)
    {
		auto p = std::make_shared<CDOSObjectProxyConnectionGroup>();
		p->SetID(i);
		p->Init();
		m_ConnectionGroups.push_back(p);
        p->Start();
    }

}


int CDOSObjectProxyServiceDefault::Update(int ProcessPacketLimit)
{
	int ProcessCount = CNetService::Update(ProcessPacketLimit);

	return ProcessCount;
}


CSmartPtr<CBaseNetConnection> CDOSObjectProxyServiceDefault::CreateConnection(CIPAddress& RemoteAddress)
{
	auto pConnection = std::make_shared<CDOSObjectProxyConnectionDefault>();
	pConnection->Init();
	pConnection->SetService(this);
	return pConnection;
}

bool CDOSObjectProxyServiceDefault::DeleteConnection(CBaseNetConnection * pConnection)
{

    for (auto it = m_ConnectionPool.begin(); it != m_ConnectionPool.end(); it++)
    {
        if ( (*it).get() == pConnection)
        {
            m_ConnectionPool.erase(it);
            return true;
        }
    }
    return false;
}

CDOSObjectProxyConnectionDefault* CDOSObjectProxyServiceDefault::GetConnection(UINT IDX)
{
	if (IDX >= m_ConnectionPool.size())
	{
		return nullptr;
	}

	return m_ConnectionPool[IDX].get(); //todo
}
LPVOID CDOSObjectProxyServiceDefault::GetFirstConnectionPos()
{
	return nullptr;
}
CDOSObjectProxyConnectionDefault* CDOSObjectProxyServiceDefault::GetNextConnection(LPVOID& Pos)
{
	return nullptr;
}

void CDOSObjectProxyServiceDefault::AcceptConnection(CSmartPtr<CDOSObjectProxyConnectionDefault > pConnection)
{
    if (m_ConnectionGroups.size() > 0)
    {
        UINT Index = pConnection->GetID() % m_ConnectionGroups.size();
        m_ConnectionGroups[Index]->AddConnection(pConnection);
    }
    else
    {
        pConnection->Disconnect();
        //PrintDOSLog(_T("未能找到合适的连接组"));
    }
}


void CDOSObjectProxyServiceDefault::QueryDestoryConnection(CDOSObjectProxyConnectionDefault * pConnection)
{
}

bool CDOSObjectProxyServiceDefault::OnRecvProtected(CIPAddress IP)
{
	return true;
}

bool CDOSObjectProxyServiceDefault::AddBlackList(CIPAddress IP, UINT Duration)
{
    return true;
}


