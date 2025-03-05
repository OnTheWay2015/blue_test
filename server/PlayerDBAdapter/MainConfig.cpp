#include "stdafx.h"

CMainConfig::CMainConfig(void)
{
	m_IsCoreShutdown = true;

	m_PlayerDBAdapterRunConfig.Weight=1024;
	m_PlayerDBAdapterRunConfig.MsgProcessLimit=64;

	m_DataCacheConfig.DataClearTime=5000;
	m_DataCacheConfig.DataClearFactor=1;
	m_DataCacheConfig.DataSaveTime=5000;
	m_DataCacheConfig.DataSaveFactor=1;
	m_DataCacheConfig.MaxDataIdleTime=60*60*1000;
}

CMainConfig::~CMainConfig(void)
{
}

bool CMainConfig::LoadConfig(LPCTSTR FileName)
{
	CEasyString ConfileFileName = CFileTools::GetPathDirectory(FileName);
	ConfileFileName += DIR_SLASH;
	ConfileFileName += COMMON_CONFIG_FILE_NAME;
	if (!LoadCommonConfig(m_CommonConfig, ConfileFileName))
	{
		return false;
	}

	xml_parser Parser;

	if(Parser.parse_file(FileName,pug::parse_trim_attribute))
	{
		xml_node Config=Parser.document();
		if(Config.moveto_child("Main"))
		{
			if (Config.has_attribute("IsCoreShutdown"))
			{
				m_IsCoreShutdown = Config.attribute("IsCoreShutdown");
			}
			
			xml_node RunConfig=Config;
			if(RunConfig.moveto_child("RunConfig"))
			{
				xml_node PlayerDBAdapterRunConfig=RunConfig;
				if (PlayerDBAdapterRunConfig.moveto_child("PlayerDBAdapter"))
				{
					ReadObjectRunConfig(PlayerDBAdapterRunConfig, m_PlayerDBAdapterRunConfig);
				}

				xml_node PlayerDBAdapterGroupRunConfig = RunConfig;
				if (PlayerDBAdapterGroupRunConfig.moveto_child("PlayerDBAdapterGroup"))
				{
					ReadObjectRunConfig(PlayerDBAdapterGroupRunConfig, m_PlayerDBAdapterGroupRunConfig);
				}
			}

			xml_node PlayerPoolConfig=Config;
			if (PlayerPoolConfig.moveto_child("PlayerPoolConfig"))
			{
				ReadPoolConfig(PlayerPoolConfig, m_PlayerPoolConfig);
			}

			xml_node RolePoolConfig = Config;
			if (RolePoolConfig.moveto_child("RolePoolConfig"))
			{
				ReadPoolConfig(RolePoolConfig, m_RolePoolConfig);
			}

			xml_node DataCacheConfig=Config;
			if(DataCacheConfig.moveto_child("DataCacheConfig"))
			{
				m_DataCacheConfig.DataClearTime=DataCacheConfig.attribute("DataClearTime");
				m_DataCacheConfig.DataClearFactor=DataCacheConfig.attribute("DataClearFactor");
				m_DataCacheConfig.MaxDataIdleTime=DataCacheConfig.attribute("MaxDataIdleTime");				
				m_DataCacheConfig.DataSaveTime=DataCacheConfig.attribute("DataSaveTime");
				m_DataCacheConfig.DataSaveFactor=DataCacheConfig.attribute("DataSaveFactor");
			}

			xml_node SystemConfig = Config;
			if (SystemConfig.moveto_child("SystemConfig"))
			{
				if (SystemConfig.has_attribute("InstanceShift"))
					m_SystemConfig.InstanceShift = SystemConfig.attribute("InstanceShift");
				if (m_SystemConfig.InstanceShift > 5)
				{
					Log("InstanceShift最大为5");
					m_SystemConfig.InstanceShift = 5;
				}
			}
		}
		else
		{
			Log("配置文件[%s]不合法",FileName);
			return false;
		}

	}
	else
	{
		Log("无法打开配置文件[%s]",FileName);
		return false;
	}

	return true;
}


