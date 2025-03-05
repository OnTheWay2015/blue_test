// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#ifdef WIN32
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料


#define USE_CRT_DETAIL_NEW
#include <afx.h>

#endif

//#define USE_MONITORED_NEW

#include "../../Libs/Utils/Utils.h"
#include "../../Libs/DBLib/DBLib.h"
#include "../../Libs/DOSLib/DOSMessage.h"
#include "../../Libs/DOSLib/IDistributedObject.h"

#include "../../Libs/NetLib/NetLib.h"

// TODO: 在此处引用程序需要的其他头文件


#define CONFIG_FILE_NAME					"PlayerDBAdapter.xml"
#define CHAR_ID_SEED_FETCH_TIME				(30*1000)
#define SHUTDOWN_CHECK_TIME					(1000)

extern UINT g_LogChannel;
#define OBJECT_FIND_TIME			(5000)
#define CACHE_HIT_FACTOR			1000
#define CACHE_NOT_HIT_FACTOR		1
#define DATA_RESET_TIME				(5*60*1000)
#define CONFIG_DATA_LOAD_TIME		(5000)

inline void LogWithTag(const char * szTag, const char* Format, ...)
{
	va_list vl;

	va_start(vl, Format);
	CLogManager::GetInstance()->PrintLogVL(g_LogChannel, ILogPrinter::LOG_LEVEL_NORMAL, szTag, Format, vl);
	va_end(vl);
}

inline void LogDebugWithTag(const char * szTag, const char* Format, ...)
{
	va_list vl;

	va_start(vl, Format);
	CLogManager::GetInstance()->PrintLogVL(g_LogChannel, ILogPrinter::LOG_LEVEL_DEBUG, szTag, Format, vl);
	va_end(vl);
}

#ifdef WIN32
#define Log(_Format, ...)	LogWithTag(__FUNCTION__, _Format, ##__VA_ARGS__)
#define LogDebug(_Format, ...)	LogDebugWithTag(__FUNCTION__, _Format, ##__VA_ARGS__)
#else
#define Log(_Format, ...)	LogWithTag(__PRETTY_FUNCTION__, _Format, ##__VA_ARGS__)
#define LogDebug(_Format, ...)	LogDebugWithTag(__PRETTY_FUNCTION__, _Format, ##__VA_ARGS__)
#endif


#include "../Protocol/Protocol.h"


enum DATA_STATUS
{
	DATA_STATUS_NONE,
	DATA_STATUS_READY,
	DATA_STATUS_MODIFIED,
	DATA_STATUS_SAVING,
};

enum LOCAL_SST_ID
{
	LOCAL_SST_ID_CALLER_ID=1,
};


struct DB_PLAYER_INFO
{
	UINT64				AccountID;
	int					Status;
	CEasyTimer			StatusTimer;
	CSmartStruct		PlayerInfo;
	CEasyArray<UINT64>	RoleList;
	bool				IsRoleFetched;
	DB_PLAYER_INFO()
	{
		Clear();
	}
	void Clear()
	{
		AccountID=0;
		Status=0;
		PlayerInfo.Clear();	
		RoleList.Clear();
		IsRoleFetched = false;
	}
};

struct DB_ROLE_INFO
{
	UINT64				AccountID;
	UINT64				RoleID;
	int					Status;
	CEasyTimer			StatusTimer;
	CSmartStruct		RoleInfo;
	DB_ROLE_INFO()
	{
		Clear();
	}
	void Clear()
	{
		AccountID = 0;
		RoleID = 0;
		Status = 0;
		RoleInfo.Clear();
	}
};

#include "MainConfig.h"

#include "PlayerDBAdapter.h"
#include "PlayerDBAdapterGroup.h"

#include "DBTransCheckDB.h"
#include "DBTransSavePlayer.h"
#include "DBTransGetPlayerInfo.h"
#include "DBTransCreatePlayer.h"
#include "DBTransDeletePlayer.h"
#include "DBTransPlayerSearch.h"
#include "DBTransSearchPlayerByTID.h"
#include "DBTransGetPlayerCount.h"
#include "DBTransGetIDByTID.h"
#include "DBTransChangeRoleName.h"
#include "DBTransLoadRoleList.h"
#include "DBTransCreateRole.h"
#include "DBTransRemoveRole.h"
#include "DBTransSaveRole.h"
#include "DBTransGetRoleIDSeed.h"