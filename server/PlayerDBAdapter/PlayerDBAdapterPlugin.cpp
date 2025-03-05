// LoginEntrance.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


extern "C" UTILS_DLL_EXPORT BOOL InitPlugin(IDistributedObjectManager* pObjectManager, UINT PluginID, UINT LogChannel, LPCTSTR ConfigDir, LPCTSTR LogDir);
extern "C" UTILS_DLL_EXPORT bool CheckPluginRelease();

CPlayerDBAdapterGroup * g_pObject;
UINT g_LogChannel=0;
UINT g_PluginID=0;

BOOL InitPlugin(IDistributedObjectManager* pObjectManager, UINT PluginID, UINT LogChannel, LPCTSTR ConfigDir, LPCTSTR LogDir)
{
	//_CrtSetBreakAlloc(21390);
	g_LogChannel=LogChannel;
	g_PluginID=PluginID;

	CEasyString ConfileFileName = ConfigDir;
	ConfileFileName += DIR_SLASH;
	ConfileFileName += CONFIG_FILE_NAME;
	if (!CMainConfig::GetInstance()->LoadConfig(ConfileFileName))
	{
		Log("加载配置文件[%s]失败",CONFIG_FILE_NAME);
		return FALSE;
	}


	DOS_OBJECT_REGISTER_INFO_EX RegisterInfo;
	{
		g_pObject = MONITORED_NEW(_T("CPlayerDBAdapterGroup"), CPlayerDBAdapterGroup);

		RegisterInfo.pObject = g_pObject;
		RegisterInfo.ObjectID.ObjectTypeID=OBJECT_TYPE_PLAYER_DB_ADAPTER_GROUP;
		RegisterInfo.szObjectTypeName = "PlayerDBAdapterGroup";
		RegisterInfo.Weight=CMainConfig::GetInstance()->GetPlayerDBAdapterGroupRunConfig().Weight;
		RegisterInfo.MsgProcessLimit = CMainConfig::GetInstance()->GetPlayerDBAdapterGroupRunConfig().MsgProcessLimit;
		pObjectManager->RegisterObject(RegisterInfo);
	}




	return TRUE;
}

bool CheckPluginRelease()
{
	Log("Check Release");
	if (g_pObject->GetUseRef() > 1)
	{
		return false;
	}
	else
	{
		SAFE_RELEASE(g_pObject);
		CMainConfig::ReleaseInstance();
		return true;
	}

}