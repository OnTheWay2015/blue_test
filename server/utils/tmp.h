#pragma once
//----------------------------------------------------------
#if !defined(__PRETTY_FUNCTION__)
	#ifdef _WIN32
		#define __PRETTY_FUNCTION__ __FUNCSIG__
	#else
		#define __PRETTY_FUNCTION__ __FUNCTION__
	#endif
#endif

extern void Stringsplit(const std::string& str, const char split, std::vector<std::string>& res);
extern void Stringsplit(const std::string& str, const std::string& splits, std::vector<std::string>& res);




//----------------------------------------------------------
class CNameObject
{
public:
    CNameObject():m_UseRef(0) {};
    ~CNameObject() {};

public:
	virtual void Release();
public:
    UINT AddUseRef();
    UINT DecUseRef();
    UINT GetUseRef();
private:
   unsigned int m_UseRef;

public:
	void SetID(UINT id);
	UINT GetID() ;

private:
	UINT m_ID;

};


inline void CNameObject::SetID(UINT id)
{
    m_ID = id;
}

inline UINT CNameObject::GetID()
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

