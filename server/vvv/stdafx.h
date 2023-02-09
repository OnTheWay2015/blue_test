
#pragma once
#ifdef _WIN32

#ifndef WINVER				// 允许使用 Windows 95 和 Windows NT 4 或更高版本的特定功能。
#define WINVER 0x0600		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_WINNT		// 允许使用 Windows NT 4 或更高版本的特定功能。
#define _WIN32_WINNT 0x0600		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用 Windows 98 或更高版本的特定功能。
#define _WIN32_WINDOWS 0x0600 //为 Windows Me 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_IE			// 允许使用 IE 4.0 或更高版本的特定功能。
#define _WIN32_IE 0x0600	//为 IE 5.0 及更新版本改变为适当的值。
#endif

#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料

//#define USE_CRT_DETAIL_NEW

#endif



#ifdef _WIN32
#include <windows.h>

#include <WinSock2.h>
#include <MSWSock.h>
#include <Ws2tcpip.h>
#include <Iphlpapi.h>

#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Mswsock.lib")
#pragma comment(lib,"Iphlpapi.lib")
#endif



#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <queue>
#include <deque>
#include <unordered_map>


#define CHashMap  std::unordered_map
#define CStaticMap std::map
#define CEasyArray std::vector
#define CIDStorage std::vector

//todo
#define CThreadSafeIDStorage std::vector   
#define CSmartPtr  std::shared_ptr
#define CCycleQueue std::deque



#ifdef _WIN32 
    #include "utils/win/Win32AddOn.h"
#endif 

#ifdef __linux__
    #include <stdarg.h>
    #include <unistd.h>
    #include <math.h>
    #include <utime.h>
    #include <signal.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <sys/dir.h>
    #include <sys/time.h>
    #include <netinet/in.h>
    
    #include "iconv/miniconv.h"
    #include "utils/linux/LinuxAddOn.h"
#endif 


#include "utils/config.h"
#include "utils/tmp.h"


#ifdef _WIN32 
#include "utils/win/EasyCriticalSectionWin.h"
#endif 
#ifdef __linux__
#include "utils/linux/EasyCriticalSectionLinux.h"
#endif 

#include "utils/ClassifiedID.h"


#include "utils/AutoLock.h"

#ifdef _WIN32 
#include "utils/win/EasyReadWriteLockWin.h"
#endif 
#ifdef __linux__
#include "utils/linux/EasyReadWriteLockLinux.h"
#endif 



#include "utils/AutoReadLock.h"
#include "utils/AutoWriteLock.h"


#ifdef _WIN32 
#include "utils/win/EasyThreadWin.h"
#endif 
#ifdef __linux__
#include "utils/linux/EasyThreadLinux.h"
#endif 

#include "utils/EasyString.h"
#include "utils/EasyBuffer.h"
#include "utils/ToolsAll.h"
#ifdef _WIN32 
#include "utils/win/ToolsWin.h"
#endif 
#ifdef __linux__
#include "utils/linux/ToolsLinux.h"
#endif 


#include "utils/CycleBuffer.h"
//#include "utils/CycleQueue.h"
#include "utils/ThreadSafeCycleQueue.h"
#include "utils/StaticObject.h"
#include "utils/EasyTime.h"
#include "utils/EasyTimer.h"
#include "utils/EasyTimer64.h"
#include "utils/EasyTimerEx.h"

#ifdef _WIN32 
#include "utils/win/EasyThreadWin.h"
#endif 
#ifdef __linux__
#include "utils/linux/EasyThreadLinux.h"
#endif 


#include "utils/GuardThread.h"

#include "utils/StringFile.h"
#include "utils/FileInfo.h"
#include "utils/IFileObjectCreator.h"
#include "utils/IFileAccessor.h"

#include "utils/StandardFileAccessor.h"
#include "utils/StandardFileObjectCreator.h"


#ifdef _WIN32 
#include "utils/win/WinFileAccessorObjectCreator.h"
#include "utils/win/WinFileAccessor.h"
#endif 
#ifdef __linux__
#include "utils/linux/LinuxFileAccessorObjectCreator.h"
#include "utils/linux/LinuxFileAccessor.h"
#endif 



#include "utils/FileSystemManager.h"


#include "utils/pugxml.h"
#include "utils/blue_logskin.h"
#include "utils/ILogPrinter.h"
#include "utils/LogManager.h"










/*

#define MAX_LISTEN_BACKLOG						64

#define DEFAULT_WORK_THREAD_COUNT			4
#define DEFAULT_SERVER_ACCEPT_QUEUE				128
#define DEFAULT_SERVER_RECV_DATA_QUEUE			256
#define DEFAULT_SERVER_SEND_DATA_QUEUE			256
#define DEFAULT_PARALLEL_ACCEPT					64
#define DEFAULT_PARALLEL_RECV					64
#define DEFAULT_EVENT_OBJECT_COUNT				4096
#define DEFAULT_EVENT_ROUTER_COUNT				1024
#define DEFAULT_EVENT_OBJECT_POOL_GROW_SIZE		2048
#define DEFAULT_EVENT_OBJECT_POOL_GROW_LIMIT	32
#define DEFAULT_EVENT_ROUTER_POOL_GROW_SIZE		1024
#define DEFAULT_EVENT_ROUTER_POOL_GROW_LIMIT	32
#define NET_DATA_BLOCK_SIZE						4096
//#define EPOLL_DATA_BLOCK_SIZE					16
#define DEFAULT_SERVER_PROCESS_PACKET_LIMIT		32


#define MAX_EASY_NET_LINK_MSG_SIZE			65535
#define MAX_EASY_NET_LINK_DATA_SIZE			(MAX_EASY_NET_LINK_MSG_SIZE-sizeof(EASY_NET_LINK_MSG_HEAD))

#define EASY_LINK_RECONNECT_TIME			(10*1000)


#ifdef UNICODE
#define GetHostName	GetHostNameW
#else
#define GetHostName	gethostname
#endif


#include "netlib/IPAddress.h"
#include "netlib/NetSocket.h"
#include "netlib/BaseNetServer.h"
#include "netlib/BaseNetService.h"
#include "netlib/BaseNetConnection.h"


#ifdef _WIN32 
#include "netlib/win/IOCPThread.h"
#include "netlib/win/IIOCPEventHandler.h"
#include "netlib/win/IOCPEventRouter.h"
#include "netlib/win/OverLappedObject.h"
#include "netlib/win/NetServerIOCP.h"
#include "netlib/win/IOCPListenThread.h"
#include "netlib/win/NetConnectionIOCP.h"
#include "netlib/win/NetServiceIOCP.h"

#endif 
#ifdef __linux__
#include "netlib/linux/EpollEventObject.h"
#include "netlib/linux/IEpollEventHandler.h"
#include "netlib/linux/EpollEventRouter.h"
#include "netlib/linux/EpollThread.h"
#include "netlib/linux/NetServerEpoll.h"
#include "netlib/linux/NetServiceEpoll.h"
#include "netlib/linux/NetConnectionEpoll.h"

#endif 




*/
#include "netlib/NetLib.h"

#include "ProxyConnectionDefault.h"
#include "ProxyConnectionGroup.h"
#include "ProxyServiceDefault.h"

