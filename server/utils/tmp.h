#pragma once
//----------------------------------------------------------

#include <codecvt>

#if !defined(__PRETTY_FUNCTION__)
	#ifdef _WIN32
		#define __PRETTY_FUNCTION__ __FUNCSIG__
	#else
		#define __PRETTY_FUNCTION__ __FUNCTION__
	#endif
#endif


#ifdef UNICODE 
	#define STD_STR std::wstring
	#define STD_CMP(a,b) _wcscmp(a,b)
	#define STD_ICMP(a,b) _wcsicmp(a,b) //不区分大小写

	//#define STD_COUT std::wcout
	#define STD_COUT std::cout
	
#else
	#define STD_STR std::string
	#define STD_CMP(a,b) _strcmp(a,b)
	#define STD_ICMP(a,b) _stricmp(a,b) //不区分大小写
	#define STD_COUT std::cout
#endif

// 将 string 转为 wstring
inline std::wstring s2wstring(const std::string& input)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(input);
}
// 将 wstring 转为 string 
inline std::string w2string(const std::wstring& input)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(input);
}


//inline wchar_t* char2wchar(const char* ch)
//{
//	std::string input(ch);
//	auto ws = s2wstring(input);
//	return ws.c_str();
//}




//_tcscmp()
//_tcsicmp()



//这个 __PRETTY_FUNCTION__ 定义属于 Gcc的 不属于MSVC


extern void Stringsplit(const std::wstring& str, const wchar_t split, std::vector<std::wstring>& res);
extern void Stringsplit(const std::wstring& str, const std::wstring& splits, std::vector<std::wstring>& res);

extern void Stringsplit(const std::string& str, const char split, std::vector<std::string>& res);
extern void Stringsplit(const std::string& str, const std::string& splits, std::vector<std::string>& res);



//----------------------------------------------------------
class CNameObject
{
public:
    CNameObject():m_UseRef(0) {};
    virtual ~CNameObject() {};

public:
	virtual void Release();
public:
    UINT AddUseRef();
    UINT DecUseRef();
    UINT GetUseRef();
private:
   unsigned int m_UseRef;

public:
	void SetID(UINT64 id);
	UINT64 GetID() ;

private:
	UINT64 m_ID;

};


inline void CNameObject::SetID(UINT64 id)
{
    m_ID = id;
}

inline UINT64 CNameObject::GetID()
{
    return m_ID;
}

inline UINT CNameObject::AddUseRef()
{
    return AtomicInc(&m_UseRef);
}
inline UINT CNameObject::DecUseRef()
{
	return AtomicDec(&m_UseRef);
}
inline UINT CNameObject::GetUseRef()
{
	return m_UseRef;
}


#ifndef CONVERT_POINT
#define CONVERT_POINT(dectype, srcptr) std::static_pointer_cast<dectype>(srcptr)
#endif



//----------------------------------------------------------

