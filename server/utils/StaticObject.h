#pragma once




template <class T>
class CStaticObject
{
public:
	static T * GetInstance()
	{
		return &m_Instance;
	}
protected:
	static T m_Instance;
};
template <class T>
T  CStaticObject<T>::m_Instance;







template <class T>
class CStaticObject3
{
public:
	static T * GetInstance()
	{
		static T s_Instance;
		return &s_Instance;
	}
};



template <class T>
class CStaticObject4
{
public:
	static T * GetInstance()
	{
		if(m_pInstance==NULL)
		{
			m_pInstance=new T;
		}
		return m_pInstance;
	}
	static void ReleaseInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}
protected:
	static T * m_pInstance;
};
template <class T>
T  * CStaticObject4<T>::m_pInstance=NULL;