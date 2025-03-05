#include "stdafx.h"

blue_logskin::blue_logskin(std::string filename)
{
	//todo linux
	auto xx = CFileTools::GetModulePath(NULL);
	//std::string xxx(xx.GetBuffer());
	//xxx = xxx+ filename;
	//	m_hfile= CreateFile(
	//		filename.c_str()	
	//		, GENERIC_WRITE | GENERIC_READ
	//		, FILE_SHARE_READ | FILE_SHARE_WRITE
	//		, NULL
	//		, OPEN_ALWAYS
	//		, FILE_ATTRIBUTE_NORMAL |FILE_FLAG_NO_BUFFERING //FILE_FLAG_NO_BUFFERING  todo 用上buffer
	//		//, FILE_ATTRIBUTE_NORMAL //FILE_FLAG_NO_BUFFERING  todo 用上buffer
	//		, 0);
	//	
	//	if (INVALID_HANDLE_VALUE == m_hfile)
	//	{
	//		assert(false);
	//	}
}

blue_logskin::~blue_logskin()
{
	//todo linux
	//::CloseHandle(m_hfile);
}

blue_logskin& operator <<(blue_logskin& log, const char* s)
{
	return log;
	//todo linux
	//auto str = s;
	//size_t len= strlen(str);
	//if (len <= 0)
	//{
	//	return log;
	//}

	//::SetFilePointer(log.m_hfile, NULL, NULL, FILE_END);
	//DWORD dwBytesWriten = 0;
	//size_t contentlen = len + 2;
	//size_t writeSec = 512;
	//int r = contentlen % writeSec;
	//int secs = contentlen / writeSec;
	//if (r > 0)
	//{
	//	secs += 1;
	//}
	//size_t tlen = secs * writeSec;
	//void* wmem = malloc(tlen);
	//memset(wmem, ' ', tlen);
	//memcpy(wmem, str, len);
	//char end[2]= {'\r','\n'};
	//memcpy((char*)wmem+tlen-2, end, 2);

	//auto nRet = ::WriteFile(log.m_hfile, wmem, tlen, &dwBytesWriten, NULL);
	//if (nRet <= 0)
	//{
	//	auto e = ::GetLastError();
	//	int aa = 123;
	//}

	//free(wmem);
	//return log;
}



