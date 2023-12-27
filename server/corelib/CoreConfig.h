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
		std::string IP;
		LEADER_CONFIG()
		{
			Port= 0;
			IP= "";
		}
	};

	struct PLUGIN_INFO
	{
		UINT PluginType;
		std::string Name;
	};

	struct CLIENT_PROXY_CONFIG
	{
		SERVICE_TYPE ProxyType;
		CLIENT_PROXY_MODE ProxyMode;
		UINT ListenPort;
		UINT ConnectionGroupCount;
		UINT KeepAliveTime;
		UINT MsgQueueSize;
		std::string ListenIP;
		CLIENT_PROXY_CONFIG()
		{
		}
	};

	struct SERVICE_CONFIG
	{
		UINT ServiceType;
		UINT ServiceID;
		std::string Desc;
		std::string Name;
		CEasyArray<CLIENT_PROXY_CONFIG> m_ClientProxys;
		SERVICE_CONFIG()
		{
		}
	};


	
protected:
	CEasyString m_ServerName;
	CEasyString m_ServerDesc;
	UINT m_ServerID;
	//UINT m_ServiceID;
	//UINT m_ServiceType; // 服务分类 monitor/gate/game...

	NET_CONFIG								m_NetConfig;
	LEADER_CONFIG							m_LeaderConfig;
	CEasyArray<SERVICE_CONFIG> m_Services;

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

	LPCTSTR GetServiceName()
	{
		return m_ServerName;
	}
	//UINT GetServiceType()
	//{
	//	return m_ServiceType;
	//}
	//UINT GetServiceID()
	//{
	//	return m_ServiceID;
	//}
	LPCTSTR GetServerDesc()
	{
		return m_ServerDesc;
	}
	bool HaveServerName()
	{
		return !(m_ServerName.IsEmpty()||m_ServerDesc.IsEmpty());
	}

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

private:
	bool ReadProxyConfig(pug::xml_node &XMLContent, CLIENT_PROXY_CONFIG &Config);
	bool LoadPluginInfo(pug::xml_node& XMLContent);
};
