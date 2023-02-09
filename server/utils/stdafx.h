
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

#include <windows.h>
#endif




//#include <winnt.h>
//#include <tchar.h>





#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <assert.h>



#define CStaticMap std::map
#define CEasyArray std::vector
#define CIDStorage std::vector

#ifdef _WIN32 
    #define UINT unsigned int
    #define BYTE unsigned char 
#endif 





#ifdef _WIN32 
    #include "win/Win32AddOn.h"
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

    #include "iconv/miniconv.h"
    //#include "linux/TCharLinux.h"
    #include "linux/LinuxAddOn.h"
#endif 
#include "tmp.h"

#include "config.h"

#include "EasyString.h"
#include "EasyBuffer.h"
#include "CycleBuffer.h"
#include "CycleQueue.h"
#include "ToolsAll.h"

#ifdef _WIN32 
    #include "win/ToolsWin.h"
#endif 
#ifdef __linux__
    #include "linux/ToolsLinux.h"
#endif 


#include "StaticObject.h"
#include "EasyTime.h"
#include "EasyTimer.h"
#include "EasyTimer64.h"
#include "EasyTimerEx.h"

#include "StringFile.h"
#include "FileInfo.h"
#include "IFileObjectCreator.h"
#include "IFileAccessor.h"

#include "StandardFileAccessor.h"
#include "StandardFileObjectCreator.h"



#ifdef _WIN32 
    #include "win/WinFileAccessorObjectCreator.h"
    #include "win/WinFileAccessor.h"
    #include "win/EasyCriticalSectionWin.h"
#endif 
#ifdef __linux__
    #include "linux/LinuxFileAccessorObjectCreator.h"
    #include "linux/LinuxFileAccessor.h"
    #include "linux/EasyCriticalSectionLinux.h"
#endif 


#ifdef _WIN32 
#endif 
#ifdef __linux__
#endif 



#include "AutoLock.h"

#ifdef _WIN32 
#include "win/EasyReadWriteLockWin.h"
#endif 
#ifdef __linux__
#include "linux/EasyReadWriteLockLinux.h"
#endif 

#include "AutoReadLock.h"
#include "AutoWriteLock.h"

#ifdef _WIN32 
#include "win/EasyThreadWin.h"
#endif 
#ifdef __linux__
#include "linux/EasyThreadLinux.h"
#endif 

#include "GuardThread.h"

#include "FileSystemManager.h"




#include "pugxml.h"

#include "blue_logskin.h"
#include "ILogPrinter.h"
#include "LogManager.h"


