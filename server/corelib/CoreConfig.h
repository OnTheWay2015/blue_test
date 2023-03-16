﻿#pragma once

class CoreConfig :
	public CStaticObject4<CoreConfig>
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
	struct MONITOR_CONFIG
	{
		UINT					Port;
		MONITOR_CONFIG()
		{
			Port= 0;
		}
	};

	struct PLUGIN_INFO
	{
		UINT PluginType;
		std::string Name;
	};

	struct CLIENT_PROXY_CONFIG
	{
		CLIENT_PROXY_TYPE ProxyType;
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



	
protected:
	CEasyString m_ServiceName;
	CEasyString m_ServiceDesc;
	UINT m_ServiceID;
	UINT m_ServerType; // 服务分类 monitor/gate/game...

	NET_CONFIG								m_NetConfig;
	MONITOR_CONFIG							m_MonitorConfig;
	CEasyArray<CLIENT_PROXY_CONFIG> m_ClientProxys;

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
		return m_ServiceName;
	}
	LPCTSTR GetServiceDesc()
	{
		return m_ServiceDesc;
	}
	bool HaveServiceName()
	{
		return !(m_ServiceName.IsEmpty()||m_ServiceDesc.IsEmpty());
	}

	const NET_CONFIG& GetNetConfig()
	{
		return m_NetConfig;
	}

	const CEasyArray<CLIENT_PROXY_CONFIG>& GetClientProxyList()
	{
		return m_ClientProxys;
	}
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