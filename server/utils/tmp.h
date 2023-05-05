#pragma once
//----------------------------------------------------------
#if !defined(__PRETTY_FUNCTION__)
	#ifdef _WIN32
		#define __PRETTY_FUNCTION__ __FUNCSIG__
	#else
		#define __PRETTY_FUNCTION__ __FUNCTION__
	#endif
#endif

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


//----------------------------------------------------------

