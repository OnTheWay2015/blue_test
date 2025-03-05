#include "stdafx.h"

using namespace pug;
CMainConfig::CMainConfig(void)
{
	m_IsCoreShutdown = true;

	m_DataCacheConfig.DataClearTime=5000;
	m_DataCacheConfig.DataClearFactor=1;
	m_DataCacheConfig.DataSaveTime=5000;
	m_DataCacheConfig.DataSaveFactor=1;
	m_DataCacheConfig.MaxDataIdleTime=60*60*1000;
}

CMainConfig::~CMainConfig(void)
{
}

bool CMainConfig::OnLoaded(pug::xml_node& root)
{
    auto Config = root;
    if (Config.moveto_child("Main"))
    {
        if (Config.has_attribute("IsCoreShutdown"))
        {
            m_IsCoreShutdown = Config.attribute("IsCoreShutdown");
        }

        xml_node DataCacheConfig = Config;
        if (DataCacheConfig.moveto_child("DataCacheConfig"))
        {
            m_DataCacheConfig.DataClearTime = DataCacheConfig.attribute("DataClearTime");
            m_DataCacheConfig.DataClearFactor = DataCacheConfig.attribute("DataClearFactor");
            m_DataCacheConfig.MaxDataIdleTime = DataCacheConfig.attribute("MaxDataIdleTime");
            m_DataCacheConfig.DataSaveTime = DataCacheConfig.attribute("DataSaveTime");
            m_DataCacheConfig.DataSaveFactor = DataCacheConfig.attribute("DataSaveFactor");
        }

        xml_node SystemConfig = Config;
        if (SystemConfig.moveto_child("SystemConfig"))
        {
            if (SystemConfig.has_attribute("InstanceShift"))
                m_SystemConfig.InstanceShift = SystemConfig.attribute("InstanceShift");
            if (m_SystemConfig.InstanceShift > 5)
            {
                //Log("InstanceShift最大为5");
                m_SystemConfig.InstanceShift = 5;
            }
        }
    }
    return true;
}


