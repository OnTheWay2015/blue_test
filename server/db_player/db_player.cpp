#include "stdafx.h"


int main()
{

    CEasyArray<INSTANCE_CONFIG>				m_InstanceConfigList;

    auto m_pDatabase = MONITORED_NEW(_T("CPlayerDBAdapterGroup"), CMySQLDatabase);
    auto m_pMainConfig = MONITORED_NEW(_T("CMainConfig "), CMainConfig);
    m_pMainConfig->LoadConfig(_T("../../../conf/db_player.xml"));
    const auto& CommonConfig = m_pMainConfig->GetCommonConfig();
    const DB_CONNECT_CONFIG* pDBConfig = CommonConfig.GetDBConfig("test");
    auto m_pDBTransManager = MONITORED_NEW(_T("CPlayerDBAdapterGroup"), CDBTransationManager);
    if (!m_pDBTransManager->Init(m_pDatabase,
        pDBConfig->DBConnectString,
        1, 16, 0))
    {
        //Log("数据库初始化失败");
        return 1;
    }


    UINT InstanceShift = m_pMainConfig->GetSystemConfig().InstanceShift;
    UINT InstanceCount = 1 << InstanceShift;
    for (UINT i = 0; i < InstanceCount; i++)
    {
        INSTANCE_CONFIG InstanceConfig;

        InstanceConfig.DBConfig = *pDBConfig;
        InstanceConfig.DBConfig.DBName.Format("%s_%u", (LPCTSTR)pDBConfig->DBName, i);
        InstanceConfig.DBConfig.DBConnectString.Format("DATABASE=%s;", (LPCTSTR)InstanceConfig.DBConfig.DBName);
        InstanceConfig.DBConfig.DBConnectString += pDBConfig->DBConnectString;

        InstanceConfig.IDSeedStep = InstanceCount;
        InstanceConfig.IDSeedIndex = i;
        InstanceConfig.HashMask = 0;
        for (UINT j = 0; j < 32 / InstanceCount; j++)
        {
            InstanceConfig.HashMask |= ((UINT)1) << (i + j * InstanceCount);
        }

        m_InstanceConfigList.emplace_back(InstanceConfig);
    }


    auto m_pDBHolder = MONITORED_NEW(_T("CPlayerDBAdapter"), CPlayerDBAdapter, m_InstanceConfigList[0], m_pDatabase);


    CSmartStruct info;
    CDBTransCreatePlayer* pDBTrans = MONITORED_NEW(_T("PlayerDBAdapterDBTrans"), CDBTransCreatePlayer, m_pDBHolder, info);
    if (!m_pDBTransManager->AddTransaction(pDBTrans))
    {
        //Log("提交数据库事务失败");
        SAFE_RELEASE(pDBTrans);
        //MsgCaller.DBCreatePlayerAck(COMMON_RESULT_FAILED, CSmartStruct());
    }
    else
    {
        //Log("账户ID不一致[0x%llX,0x%llX]", DataAccountID, AccountID);
        //MsgCaller.DBCreatePlayerAck(COMMON_RESULT_FAILED, CSmartStruct());
    }



    while (1)
    {
        auto tm = 1.0;
        m_pDBTransManager->Update(tm);
        DoSleep(5000);
    }

    return 0;
}


