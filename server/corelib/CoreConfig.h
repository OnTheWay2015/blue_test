#pragma once

class CoreConfig 
{
public:
	struct NET_CONFIG
	{
		UINT					NetWorkThreadCount;
		NET_CONFIG()
		{
			NetWorkThreadCount = 2;
		}
	};
	struct LEADER_CONFIG
	{
		UINT					Port;
		STD_STR IP;
		LEADER_CONFIG()
		{
			Port= 0;
			IP= _T("");
		}
	};

	struct PLUGIN_INFO
	{
		UINT PluginType;
		STD_STR Name;
	};

	struct CLIENT_PROXY_CONFIG
	{
		CLIENT_PROXY_MODE ProxyMode;
		UINT ListenPort;
		UINT ConnectionGroupCount;
		UINT KeepAliveTime;
		UINT MsgQueueSize;
		STD_STR ListenIP;
		CLIENT_PROXY_CONFIG()
		{
		}
	};
	
	struct CONNECT_CONFIG
	{
		UINT ServerID;
		UINT ServiceType;
	};
	struct SERVICE_CONFIG
	{
		UINT ServiceType;
		UINT ServiceID;
		STD_STR Desc;
		STD_STR Name;
		CEasyArray<CLIENT_PROXY_CONFIG> m_ClientProxys;
		SERVICE_CONFIG()
		{
		}
	};


	
protected:
	//CEasyString m_ServerName;
	//CEasyString m_ServerDesc;
	//UINT m_ServerID;
	//UINT m_ServiceType; // 服务分类 monitor/gate/game...
	COMMON_CONFIG					m_CommonConfig;

	NET_CONFIG								m_NetConfig;
	LEADER_CONFIG							m_LeaderConfig;
	CEasyArray<SERVICE_CONFIG> m_Services;
	CEasyArray<CONNECT_CONFIG> m_ActConnects;


	//DOS_CONFIG								m_DOSConfig;
	//MONO_CONFIG								m_MonoConfig;
	//STORAGE_POOL_SETTING					m_PluginObjectPoolConfig;
	//CEasyArray<PLUGIN_INFO>					m_PluginList;
	//CEasyArray<LIB_INFO>					m_LibList;
	//CEasyArray<CLIENT_PROXY_PLUGIN_INFO>	m_ProxyPluginList;
	bool									m_ExistWhenNoPlugin;
	CEasyArray<PLUGIN_INFO> m_PluginList;
public:
	CoreConfig(void);
	~CoreConfig(void);

	bool LoadConfig(LPCTSTR FileName);
	bool LoadCommonConfig( pug::xml_node& root, LPCTSTR ConfigFileName);
	bool ReadDBConfig(pug::xml_node& XMLContent, DB_CONNECT_CONFIG& Config, LPCTSTR SearchDir);

	//LPCTSTR GetServiceName()
	//{
	//	return m_ServerName;
	//}
	
	UINT GetServerID()
	{
		//return m_ServerID;
		return m_CommonConfig.ServerConfig.ServerID;
	}
	//LPCTSTR GetServerDesc()
	//{
	//	return m_ServerDesc;
	//}
	//bool HaveServerName()
	//{
	//	return !(m_ServerName.IsEmpty()||m_ServerDesc.IsEmpty());
	//}

	const LEADER_CONFIG& GetLeaderConfig()
	{
		return m_LeaderConfig;
	}

	const NET_CONFIG& GetNetConfig()
	{
		return m_NetConfig;
	}

	const CEasyArray<SERVICE_CONFIG>& GetServiceList()
	{
		return m_Services;
	}

	const CEasyArray<CONNECT_CONFIG>& GetActConnects()
	{
		return m_ActConnects;
	}



	//const CEasyArray<CLIENT_PROXY_CONFIG>& GetClientProxyList()
	//{
	//	return m_ClientProxys;
	//}
	const CEasyArray<PLUGIN_INFO>& GetPluginList()
	{
		return m_PluginList;
	}
	bool ExistWhenNoPlugin()
	{
		return m_ExistWhenNoPlugin;
	}
	COMMON_CONFIG& GetCommonConfig()
	{
		return m_CommonConfig;
	}

protected:
	virtual bool OnLoaded(pug::xml_node& root); 
	
private:
	bool ReadProxyConfig(pug::xml_node &XMLContent, CLIENT_PROXY_CONFIG &Config);
	bool LoadPluginInfo(pug::xml_node& XMLContent);
};
