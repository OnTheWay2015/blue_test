#pragma once



class CMainConfig:public CoreConfig 
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
	bool							m_IsCoreShutdown;
	DATA_CACHE_CONFIG				m_DataCacheConfig;
	SYSTEM_CONFIG					m_SystemConfig;
public:
	CMainConfig(void);
	~CMainConfig(void);

	bool IsCoreShutdown()
	{
		return m_IsCoreShutdown;
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
	virtual bool OnLoaded(pug::xml_node& root); 
};
