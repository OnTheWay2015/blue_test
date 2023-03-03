/****************************************************************************/
/*                                                                          */
/*      文件名:    LogManager.cpp                                           */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "stdafx.h"



TCHAR LOG_MANAGER_INSTANCE[]=_T("LogManagerInstance");

CLogManager::CLogManager(void)
	:CNameObject()
{
	//m_LogChannels.SetTag(_T("CLogManager"));
	//m_WorkThreadList.SetTag(_T("CLogManager"));

	//m_LogChannels.Create(128, 128, 128);
	//m_LogChannels.Insert(LOG_SYSTEM_CHANNEL, MONITORED_NEW(_T("CLogManager"), CVSOutputLogPrinter));

	//CEasyString ImportLogFileName = CFileTools::GetModuleFilePath(NULL);
	//
	//m_LogChannels.Insert(LOG_IMPORTANT_CHANNEL, MONITORED_NEW(_T("CLogManager"), CFileLogPrinter, ImportLogFileName, FILE_LOG_APPEND | FILE_LOG_SAFE_WRITE));

	//m_WorkThreadList.Resize(DEFAULT_ASYNC_LOG_WORK_THREAD_COUNT);
	//for (UINT i = 0; i < m_WorkThreadList.GetCount(); i++)
	//{
	//	m_WorkThreadList[i].Start();
	//}
	std::string xx = "tttlog.txt";
    m_log = std::make_shared<blue_logskin>(xx);
    //m_log = std::make_shared<blue_logskin>();

}

CLogManager::~CLogManager(void)
{
	//for (UINT i = 0; i < m_WorkThreadList.GetCount(); i++)
	//{
	//	m_WorkThreadList[i].SafeTerminate(5);
	//}

	//void * Pos=m_LogChannels.GetFirstObjectPos();
	//while(Pos)
	//{
	//	UINT Key;
	//	ILogPrinter ** ppLogPrinter=m_LogChannels.GetNextObject(Pos,Key);
	//	if(ppLogPrinter)
	//	{
	//		SAFE_RELEASE(*ppLogPrinter)
	//	}
	//}
	//m_LogChannels.Clear();
}

UINT CLogManager::AddWorkThreadCount(UINT Count)
{
	//if (Count)
	//{
	//	UINT OldCount = (UINT)m_WorkThreadList.GetCount();
	//	m_WorkThreadList.Resize(m_WorkThreadList.GetCount() + Count);
	//	for (UINT i = OldCount; i < m_WorkThreadList.GetCount(); i++)
	//	{
	//		m_WorkThreadList[i].Start();
	//	}
	//}
	//return (UINT)m_WorkThreadList.GetCount();
    return 0;
}

UINT CLogManager::GetChannelCount()
{
	//return m_LogChannels.GetObjectCount();
    return 0;
}

//void CLogManager::AddChannel(UINT ChannelID,ILogPrinter * pLogPrinter)
//{
//	if(pLogPrinter==NULL)
//		return;
//	if(m_LogChannels.Find(ChannelID)!=NULL)
//	{
//		DelChannel(ChannelID);
//	}
//	m_LogChannels.Insert(ChannelID,pLogPrinter);
//	pLogPrinter->AddUseRef();
//	if (pLogPrinter->NeedAsyncUpdate())
//	{
//		CAsyncLogWorkThread * pWorkThread = NULL;
//		UINT MinCount = 0;
//		for (UINT i = 0; i < m_WorkThreadList.GetCount(); i++)
//		{
//			if (pWorkThread == NULL || MinCount>m_WorkThreadList[i].GetLogPrinterCount())
//			{
//				pWorkThread = m_WorkThreadList.GetObject(i);
//				MinCount = pWorkThread->GetLogPrinterCount();
//			}
//		}
//		if (pWorkThread)
//		{
//			pWorkThread->AddLogPrinter(pLogPrinter);
//		}
//		else
//		{
//			PrintImportantLog(_T("无法给%u通道的日志输器分配工作线程"), ChannelID);
//		}
//	}
//	PrintImportantLog(_T("已在%u通道添加日志输出器"), ChannelID);
//}

//ILogPrinter * CLogManager::GetChannel(UINT ChannelID)
//{
//	ILogPrinter ** ppLogPrinter=m_LogChannels.Find(ChannelID);
//	if(ppLogPrinter)
//	{
//		return *ppLogPrinter;
//	}
//	//else
//	//{
//	//	PrintSystemLog(_T("无法找到Log通道%u"), ChannelID);
//	//}
//	return NULL;
//}

bool CLogManager::DelChannel(UINT ChannelID)
{
	return false;
}

bool CLogManager::PrintLogDirect(UINT ChannelID, int Level, LPCTSTR Tag, LPCTSTR Msg)
{
	//*m_log << Msg;
	std::cout << Msg << std::endl;
	return false;
}


static TCHAR m_MsgBuff[4096];
bool CLogManager::PrintLog(UINT ChannelID, int Level, LPCTSTR Tag, LPCTSTR Format, ...)
{
	memset(m_MsgBuff,0,4096);
    va_list	vl;
    va_start(vl, Format);
    _vstprintf_s(m_MsgBuff, 4096, Format, vl);
	va_end( vl);
	
	std::cout << m_MsgBuff<< std::endl;;

	//*m_log << m_MsgBuff;
	return false;
}
bool CLogManager::PrintLogVL(UINT ChannelID, int Level, LPCTSTR Tag, LPCTSTR Format, va_list vl)
{
	memset(m_MsgBuff,0,4096);
    _vstprintf_s(m_MsgBuff, 4096, Format, vl);

	std::cout << m_MsgBuff<< std::endl;;

	//*m_log << m_MsgBuff;
	return false;
}


