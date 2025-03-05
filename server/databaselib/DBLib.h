/****************************************************************************/
/*                                                                          */
/*      文件名:    DBLib.h                                                  */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

#define DBLIB_VERSION	"2.0.0.0"


enum DB_TYPE
{
	DB_TYPE_MSSQL,
	DB_TYPE_MYSQL,
};
struct DB_TABLE_CONFIG
{
	CEasyString	Name;
	CEasyString	Define;
};
struct DB_CONNECT_CONFIG
{
	CEasyString					Name;
	int							DBType;
	CEasyString					DBConnectString;
	UINT						ThreadCount;
	UINT						QueueSize;
	UINT						RunLimit;
	bool						IsLogPerformance;
	CEasyString					DBName;
	CEasyArray<DB_TABLE_CONFIG>	TableList;
	DB_CONNECT_CONFIG()
	{
		DBType = DB_TYPE_MYSQL;
		ThreadCount = 2;
		QueueSize = 128;
		RunLimit = 64;
		IsLogPerformance = true;
	}
};


struct COMMON_CONFIG
{
	struct SERVER_CONFIG
	{
		//CClassifiedID		MinVersion;
		//CClassifiedID		CurVersion;
		CEasyString			Env;
		UINT				ServerID;
		CEasyArray<UINT>	ServerAreaList;
		SERVER_CONFIG()
		{
			Env = _T("unknow");
			ServerID = 0;
		}
		bool IsValidServerArea(UINT ServerArea) const
		{
			for (UINT i = 0; i < ServerAreaList.size(); i++)
			{
				if (ServerAreaList[i] == ServerArea)
					return true;
			}
			return false;
		}
	};


	struct EXTERNAL_MAIL_SYSTEM_CONFIG
	{
		CEasyString		RequestURL;
		CEasyString		EncodeKey;
	};

	SERVER_CONFIG					ServerConfig;
	EXTERNAL_MAIL_SYSTEM_CONFIG		MailConfig;
	CEasyArray<DB_CONNECT_CONFIG>	DBConfigList;

	const DB_CONNECT_CONFIG * GetDBConfig(LPCTSTR Name) const
	{
		for (const DB_CONNECT_CONFIG& Config : DBConfigList)
		{
			if (Config.DBName == Name)
				return &Config;
		}
		return NULL;
	}
};



struct INSTANCE_CONFIG
{
	UINT				HashMask;
	UINT				IDSeedStep;
	UINT				IDSeedIndex;
	DB_CONNECT_CONFIG	DBConfig;
};





//typedef unsigned long ulong;

inline BOOL PrintDBLogWithTag(LPCTSTR Tag, LPCTSTR Format, ...)
{
	//va_list vl;
	//va_start(vl, Format);
	//BOOL ret = CLogManager::GetInstance()->PrintLogVL(LOG_DB_ERROR_CHANNEL, ILogPrinter::LOG_LEVEL_NORMAL, Tag, Format, vl);
	//va_end(vl);
	//return ret;

	return false;
}

inline BOOL PrintDBDebugLogWithTag(LPCTSTR Tag, LPCTSTR Format, ...)
{
	//va_list vl;
	//va_start(vl, Format);
	//BOOL ret = CLogManager::GetInstance()->PrintLogVL(LOG_DB_ERROR_CHANNEL, ILogPrinter::LOG_LEVEL_DEBUG, Tag, Format, vl);
	//va_end(vl);
	//return ret;
	return false;
}

#define PrintDBLog(_Format, ...)	PrintDBLogWithTag(__FUNCTION__, _Format, ##__VA_ARGS__)
#define PrintDBDebugLog(_Format, ...)	PrintDBDebugLogWithTag(__FUNCTION__, _Format, ##__VA_ARGS__)

#include "DBTypes.h"
#include "DBValue.h"
#include "DBInterfaces.h"

#include "DBParameterSet.h"
#include "DBStaticRecordSet.h"
#include "DBFakeRecordSet.h"


#ifdef WIN32

//#include <sql.h>
//#include <sqlext.h>
//#include <WinSock2.h>
//#include <MSWSock.h>


#ifdef _WIN64

#include "../3rds/include/win/mysql/mysql.h"

#else

#include "../OtherInclude/MySQL/x86/win/mysql.h"

#endif

#else

#ifdef __x86_64__

#include "../OtherInclude/MySQL/x64/linux/mysql.h"


#else

#include "../OtherInclude/MySQL/x86/linux/mysql.h"

#endif

#endif






#include "MySQLDynamicRecordSet.h"
#include "MySQLRecordSet.h"
#include "MySQLConnection.h"
#include "MySQLDatabase.h"

#include "DBTransaction.h"
#include "DBTransationWorkThread.h"
#include "DBTransationManager.h"


using namespace DBLib;


