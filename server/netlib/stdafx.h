/****************************************************************************/
/*                                                                          */
/*      文件名:    stdafx.h                                                 */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

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

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
#include "afx.h"

#define USE_CRT_DETAIL_NEW

#endif


#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <queue>
#include <deque>



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
    
    #include "iconv/miniconv.h"
    #include "utils/linux/LinuxAddOn.h"
#endif 



#include "utils/tmp.h"

#include "utils/config.h"


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
#include "utils/CycleBuffer.h"
#include "utils/ToolsAll.h"
#ifdef _WIN32 
#include "utils/win/ToolsWin.h"
#endif 
#ifdef __linux__
#include "utils/linux/ToolsLinux.h"
#endif 



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
#include "FileInfo.h"
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














#include "NetLib.h"


