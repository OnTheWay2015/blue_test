﻿/****************************************************************************/
/*                                                                          */
/*      文件名:    LinuxFileAccessor.cpp                                    */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "../stdafx.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


CLinuxFileAccessor::CLinuxFileAccessor(void)
{
	m_FileDescriptor=INVALID_HANDLE_VALUE;
	m_IsWriteFlush=false;
}

CLinuxFileAccessor::~CLinuxFileAccessor(void)
{
	Close();
}


BOOL CLinuxFileAccessor::Open(LPCTSTR FileName,int OpenMode)
{
	if(m_FileDescriptor!=INVALID_HANDLE_VALUE)
		Close();

	int Flag=0;

	switch(OpenMode&modeAccessMask)
	{
	case modeRead:
		Flag=O_RDONLY;
		break;
	case modeWrite:
		Flag=O_WRONLY;
		break;
	case modeReadWrite:
		Flag=O_RDWR;
		break;
	}

	switch(OpenMode&modeCreationMask)
	{
	case modeOpenAlways:
		Flag|=O_CREAT;
		break;
	case modeCreate:
		Flag|=O_EXCL|O_CREAT|O_TRUNC;
		break;
	case modeCreateAlways:
		Flag|=O_CREAT|O_TRUNC;
		break;
	case modeTruncate:
		Flag|=O_TRUNC;
		break;
	case modeAppend:
		Flag|=O_CREAT|O_APPEND;
		break;
	}

	if(OpenMode&(osWriteThrough|osNoBuffer))
		m_IsWriteFlush=true;
	else
		m_IsWriteFlush=false;

	m_FileDescriptor=open(FileName,Flag,S_IRWXU);

	if(m_FileDescriptor==INVALID_HANDLE_VALUE)
	{
		printf(_T("File Create Failed:%d\r\n"),errno);
		return false;
	}
	else
	{
		if ((OpenMode&KeepOnExec) == 0)
		{
			fcntl(m_FileDescriptor, F_SETFD, FD_CLOEXEC);
		}
		m_FileInfo.FetchFileInfo(FileName);
		return true;
	}

}
void CLinuxFileAccessor::Close()
{
	if(m_FileDescriptor!=INVALID_HANDLE_VALUE)
	{
		close(m_FileDescriptor);
		m_FileDescriptor=INVALID_HANDLE_VALUE;
	}
}

UINT64 CLinuxFileAccessor::GetSize()
{
	if(m_FileDescriptor!=INVALID_HANDLE_VALUE)
	{
		off_t CurPos=lseek(m_FileDescriptor,0,SEEK_CUR);
		off_t FileSize=lseek(m_FileDescriptor,0,SEEK_END);
		lseek(m_FileDescriptor,CurPos,SEEK_SET);

		return FileSize;

	}
	else
	{
		return 0;
	}
}

UINT64 CLinuxFileAccessor::Read(LPVOID pBuff,UINT64 Size)
{
	if(m_FileDescriptor!=INVALID_HANDLE_VALUE)
	{
		INT64 ReadSize=read(m_FileDescriptor,pBuff,(size_t)Size);
		if(ReadSize!=-1)
		{
			return (UINT64)ReadSize;
		}
		return 0;
	}
	else
	{
		return 0;
	}
}

UINT64 CLinuxFileAccessor::Write(LPCVOID pBuff,UINT64 Size)
{
	if(m_FileDescriptor!=INVALID_HANDLE_VALUE)
	{
		INT64 WriteSize=write(m_FileDescriptor,pBuff,(size_t)Size);
		if (WriteSize != -1)
		{
			if (m_IsWriteFlush)
			{
				fsync(m_FileDescriptor);
			}
			return (UINT64)WriteSize;
		}
		return 0;
	}
	else
	{
		return 0;
	}
}


BOOL CLinuxFileAccessor::IsEOF()
{
	return GetCurPos()==GetSize();
}

BOOL CLinuxFileAccessor::Seek(INT64 Offset,int SeekMode)
{
	if(m_FileDescriptor!=INVALID_HANDLE_VALUE)
	{
		int Whence=SEEK_SET;
		switch(SeekMode)
		{
		case seekBegin:
			Whence=SEEK_SET;
			break;
		case seekCurrent:
			Whence=SEEK_CUR;
			break;
		case seekEnd:
			Whence=SEEK_END;
			break;
		}
		if(lseek(m_FileDescriptor,(off_t)Offset,Whence)==-1)
		{
			return FALSE;
		}
		return TRUE;

	}
	else
	{
		return FALSE;
	}
}
UINT64 CLinuxFileAccessor::GetCurPos()
{
	if(m_FileDescriptor!=INVALID_HANDLE_VALUE)
	{
		return lseek(m_FileDescriptor,0,SEEK_CUR);
	}
	else
	{
		return 0;
	}
}

BOOL CLinuxFileAccessor::SetCreateTime(const CEasyTime& Time)
{
	return FALSE;
}
BOOL CLinuxFileAccessor::GetCreateTime(CEasyTime& Time)
{
	if (m_FileInfo.IsOK())
	{
		Time = m_FileInfo.GetCreateTime();
		return TRUE;
	}
	return FALSE;
}

BOOL CLinuxFileAccessor::SetLastAccessTime(const CEasyTime& Time)
{
	if (m_FileInfo.SetLastAccessTime(Time))
	{
		return true;
	}
	return FALSE;
}
BOOL CLinuxFileAccessor::GetLastAccessTime(CEasyTime& Time)
{
	if (m_FileInfo.IsOK())
	{
		Time = m_FileInfo.GetLastAccessTime();
		return TRUE;
	}
	return FALSE;
}

BOOL CLinuxFileAccessor::SetLastWriteTime(const CEasyTime& Time)
{
	if (m_FileInfo.SetLastWriteTime(Time))
	{
		return true;
	}
	return FALSE;
}
BOOL CLinuxFileAccessor::GetLastWriteTime(CEasyTime& Time)
{
	if (m_FileInfo.IsOK())
	{
		Time = m_FileInfo.GetLastWriteTime();
		return TRUE;
	}
	return FALSE;
}
