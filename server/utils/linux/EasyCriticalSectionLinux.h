/****************************************************************************/
/*                                                                          */
/*      文件名:    EasyCriticalSectionLinux.h                               */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


class CEasyCriticalSection
{
protected:
	pthread_mutex_t		m_Mutex;
public:
	CEasyCriticalSection(void)
	{

/*
PTHREAD_MUTEX_TIMED_NP,这是缺省值,也就是普通锁。当一个线程加锁以后,其余请求锁的线程将形成一个等待队列,并在解锁后按优先级获得锁。这种锁策略保证了资源分配的公平性。
PTHREAD_MUTEX_RECURSIVE_NP,嵌套锁,允许同一个线程对同一个锁成功获得多次,并通过多次unlock解锁。如果是不同线程请求,则在加锁线程解锁时重新竞争。
PTHREAD_MUTEX_ERRORCHECK_NP,检错锁,如果同一个线程请求同一个锁,则返回EDEADLK,否则与PTHREAD_MUTEX_TIMED_NP类型动作相同。这样就保证当不允许多次加锁时不会出现最简单情况下的死锁。
PTHREAD_MUTEX_ADAPTIVE_NP,适应锁,动作最简单的锁类型,仅等待解锁后重新竞争。
PTHREAD_MUTEX_RECURSIVE 表示在同一个线程中可以多次获取同一把锁。并且不会死锁
*/


		//pthread_mutexattr_t MutexAttr;
		//pthread_mutexattr_init(&MutexAttr);
		//pthread_mutexattr_settype(&MutexAttr,PTHREAD_MUTEX_TIMED_NP);
		////pthread_mutexattr_settype(&MutexAttr,PTHREAD_MUTEX_RECURSIVE);
		//pthread_mutex_init(&m_Mutex,&MutexAttr);
		//pthread_mutexattr_destroy(&MutexAttr);
	
	
		pthread_mutex_init(&m_Mutex,nullptr);
	
	}
	~CEasyCriticalSection(void)
	{
		pthread_mutex_destroy(&m_Mutex);
	}
	void Lock()
	{
		//printf("act lock(%p)  __lock[%u] __count[%u], __owner[%u], __nusers[%u], __kind[%u], __spins[%u]\n"
		//,&m_Mutex
		//,m_Mutex.__data.__lock
		//,m_Mutex.__data.__count
		//,m_Mutex.__data.__owner
		//,m_Mutex.__data.__nusers
		//,m_Mutex.__data.__kind
		//,m_Mutex.__data.__spins
		//);
		pthread_mutex_lock(&m_Mutex);
	}
	void Unlock()
	{
		pthread_mutex_unlock(&m_Mutex);
		//printf("act Unlock(%p)  __lock[%u] __count[%u], __owner[%u], __nusers[%u], __kind[%u], __spins[%u]\n"
		//,&m_Mutex
		//,m_Mutex.__data.__lock
		//,m_Mutex.__data.__count
		//,m_Mutex.__data.__owner
		//,m_Mutex.__data.__nusers
		//,m_Mutex.__data.__kind
		//,m_Mutex.__data.__spins
		//);
	}

	BOOL TryLock()
	{
		if(pthread_mutex_trylock(&m_Mutex)==0)
			return TRUE;
		else
			return FALSE;
	}
};
