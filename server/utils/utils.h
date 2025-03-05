

#pragma once





//#include <winnt.h>
//#include <tchar.h>










#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <queue>
#include <deque>
#include <unordered_map>
#include <assert.h>


#define CHashMap  std::unordered_map
#define CStaticMap std::map
#define CEasyArray std::vector
#define CIDStorage std::vector

//todo
#define CThreadSafeIDStorage std::vector   
#define CSmartPtr  std::shared_ptr
#define CCycleQueue std::deque

















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

typedef unsigned __int64 SESSION_ID; 

#include "tmp.h"



#include "config.h"
#include "MemoryLeakDetective.h"


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
#include "FileTools.h"

#include "StandardFileAccessor.h"
#include "StandardFileObjectCreator.h"

#include "SettingFile.h"

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

#include "AutoLock.h"
#include "ThreadSafeCycleQueue.h"

#ifdef _WIN32 
#endif 
#ifdef __linux__
#endif 




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
#include "smart/SmartValue.h"
#include "smart/SmartArray.h"
#include "smart/SmartStruct.h"


#include "blue_logskin.h"
#include "ILogPrinter.h"
#include "LogManager.h"


