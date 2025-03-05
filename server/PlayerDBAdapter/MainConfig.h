#pragma once



class CMainConfig :
	public CStaticObject4<CMainConfig>
{
public:

	struct DATA_CACHE_CONFIG
	{
		UINT			DataClearTime;
		UINT			DataClearFactor;
		UINT			MaxDataIdleTime;
		UINT			DataSaveTime;
		UINT			DataSaveFactor;		
	};
	struct SYSTEM_CONFIG
	{
		UINT	InstanceShift;
		SYSTEM_CONFIG()
		{
			InstanceShift = 0;
		}
	};
protected:
	COMMON_CONFIG					m_CommonConfig;
	bool							m_IsCoreShutdown;
	STORAGE_POOL_SETTING			m_PlayerPoolConfig;
	STORAGE_POOL_SETTING			m_RolePoolConfig;
	OBJECT_RUN_CONFIG				m_PlayerDBAdapterRunConfig;
	OBJECT_RUN_CONFIG				m_PlayerDBAdapterGroupRunConfig;
	DATA_CACHE_CONFIG				m_DataCacheConfig;
	SYSTEM_CONFIG					m_SystemConfig;
public:
	CMainConfig(void);
	~CMainConfig(void);

	bool LoadConfig(LPCTSTR FileName);

	UINT GetServerID()
	{
		return m_CommonConfig.ServerConfig.ServerID;
	}
	void SetServerID(UINT ServerID)
	{
		m_CommonConfig.ServerConfig.ServerID = ServerID;
	}
	const COMMON_CONFIG& GetCommonConfig()
	{
		return m_CommonConfig;
	}
	bool IsCoreShutdown()
	{
		return m_IsCoreShutdown;
	}	
	const STORAGE_POOL_SETTING& GetPlayerPoolConfig()
	{
		return m_PlayerPoolConfig;
	}
	const STORAGE_POOL_SETTING& GetRolePoolConfig()
	{
		return m_RolePoolConfig;
	}
	const OBJECT_RUN_CONFIG& GetPlayerDBAdapterRunConfig()
	{
		return m_PlayerDBAdapterRunConfig;
	}
	const OBJECT_RUN_CONFIG& GetPlayerDBAdapterGroupRunConfig()
	{
		return m_PlayerDBAdapterGroupRunConfig;
	}
	const DATA_CACHE_CONFIG& GetDataCacheConfig()
	{
		return m_DataCacheConfig;
	}
	const SYSTEM_CONFIG& GetSystemConfig()
	{
		return m_SystemConfig;
	}
protected:
};
