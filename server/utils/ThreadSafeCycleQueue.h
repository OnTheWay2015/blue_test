#pragma once

template < class T ,class _Alloc = std::allocator<T>>
class CThreadSafeCycleQueue :
	public CCycleQueue<T,_Alloc>
{
protected:
	CEasyCriticalSection		m_FrontLock;
	CEasyCriticalSection		m_BackLock;
	bool						m_IsLockFront;
	bool						m_IsLockBack;
public:
	CThreadSafeCycleQueue(LPCTSTR Tag = _T("CThreadSafeCycleQueue"))
	{
		m_IsLockFront = true;
		m_IsLockBack = true;
	}
	~CThreadSafeCycleQueue()
	{

	}

	bool Create(UINT Size)
	{
		//CAutoLockEx FrontLock;
		//if (m_IsLockFront)
		//{
		//	FrontLock.Lock(m_FrontLock);
		//}
		//CAutoLockEx BackLock;
		//if (m_IsLockBack)
		//{
		//	BackLock.Lock(m_BackLock);
		//}
		//return CCycleQueue<T>::Create(Size);
		return true;
	}
	void Destory()
	{
		CAutoLockEx FrontLock;
		if (m_IsLockFront)
		{
			FrontLock.Lock(m_FrontLock);
		}
		CAutoLockEx BackLock;
		if (m_IsLockBack)
		{
			BackLock.Lock(m_BackLock);
		}
		CCycleQueue<T>::Destory();
	}
	void Clear()
	{
		CAutoLockEx FrontLock;
		if (m_IsLockFront)
		{
			FrontLock.Lock(m_FrontLock);
		}
		CAutoLockEx BackLock;
		if (m_IsLockBack)
		{
			BackLock.Lock(m_BackLock);
		}
		CCycleQueue<T>::Clear();
	}

	void SetLockMode(bool IsLockFront, bool IsLockBack)
	{
		m_IsLockFront = IsLockFront;
		m_IsLockBack = IsLockBack;
	}

	bool PushBack(T * pValue)
	{
		CAutoLockEx BackLock;
		if (m_IsLockBack)
		{
			BackLock.Lock(m_BackLock);
		}
		CCycleQueue<T>::push_back(*pValue);
		return true;
	}

	bool PopFront(T * pValue)
	{
		CAutoLockEx FrontLock;
		if (m_IsLockFront)
		{
			FrontLock.Lock(m_FrontLock);
		}
		//return CCycleQueue<T>::PopFront(pValue);
        if (this->size() <= 0)
        {
            return false;
        }
		
		*pValue = this->front();
		this->pop_front();
		//CCycleQueue<T>::pop_front();	
		return true;
	}
};

