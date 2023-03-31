﻿#include "stdafx.h"
using namespace pug;

CoreConfig::CoreConfig(void)
{
	m_ExistWhenNoPlugin = false;
}

CoreConfig::~CoreConfig(void)
{
}



bool CoreConfig::ReadProxyConfig(xml_node& XMLContent, CLIENT_PROXY_CONFIG& Config)
{
	if (XMLContent.has_attribute("ProxyType"))
		Config.ProxyType = (CLIENT_PROXY_TYPE)(int)XMLContent.attribute("ProxyType");

	if (XMLContent.has_attribute("ProxyMode"))
		Config.ProxyMode = (CLIENT_PROXY_MODE)((int)XMLContent.attribute("ProxyMode"));

	if (XMLContent.has_attribute("ListenIP"))
		Config.ListenIP = (const TCHAR*)XMLContent.attribute("ListenIP");

	if (XMLContent.has_attribute("ListenPort"))
		Config.ListenPort = (WORD)XMLContent.attribute("ListenPort");

	if (XMLContent.has_attribute("ConnectionGroupCount"))
		Config.ConnectionGroupCount = (UINT)XMLContent.attribute("ConnectionGroupCount");

	if (XMLContent.has_attribute("MsgQueueSize"))
		Config.MsgQueueSize= (UINT)XMLContent.attribute("MsgQueueSize");

	if (XMLContent.has_attribute("KeepAliveTime"))
		Config.KeepAliveTime = (UINT)XMLContent.attribute("KeepAliveTime");


	return true;
}

bool CoreConfig::LoadConfig(LPCTSTR FileName)
{
	xml_parser Parser;

	PrintImportantLog("开始载入配置文件%s", FileName);
	if(Parser.parse_file(FileName,pug::parse_trim_attribute))
	{
		xml_node Config=Parser.document();
		if(Config.moveto_child("Main"))
		{
			if(Config.has_attribute("ServiceName"))
				m_ServiceName=Config.attribute("ServiceName").getvalue();
			if(Config.has_attribute("ServiceDesc"))
				m_ServiceDesc=Config.attribute("ServiceDesc").getvalue();

			if(Config.has_attribute("ServiceID"))
				m_ServiceID=Config.attribute("ServiceID");
			if(Config.has_attribute("ServerType"))
				m_ServerType=Config.attribute("ServerType");


			xml_node Net=Config;
			if(Net.moveto_child("Net"))
			{
				if(Net.has_attribute("NetWorkThreadCount"))
					m_NetConfig.NetWorkThreadCount = Net.attribute("NetWorkThreadCount");
			}

			xml_node Leader=Config;
			if(Leader.moveto_child("Leader"))
			{
				if(Leader.has_attribute("Port"))
					m_LeaderConfig.Port= Leader.attribute("Port");
				if(Leader.has_attribute("IP"))
					m_LeaderConfig.IP = Leader.attribute("IP").getvalue();
			}


			xml_node ClientProxys=Config;
			if(ClientProxys.moveto_child("ClientProxys"))
			{
				for (UINT i = 0; i < ClientProxys.children(); i++)
				{
					xml_node ClientProxy = ClientProxys.child(i);
					if (_stricmp(ClientProxys.name(), "ClientProxy"))
					{
						CLIENT_PROXY_CONFIG ProxyConfig;
						if (ReadProxyConfig(ClientProxy, ProxyConfig))
						{
							m_ClientProxys.push_back(ProxyConfig);
						}
					}
				}
			}



			xml_node Plugins=Config;
			if(Plugins.moveto_child("Plugins"))
			{
				LoadPluginInfo(Plugins);
			}
		}
		else
		{
			PrintImportantLog("配置文件[%s]不合法",FileName);
			return false;
		}

	}
	else
	{
		PrintImportantLog("无法打开配置文件[%s]",FileName);
		return false;
	}
	return true;
}



bool CoreConfig::LoadPluginInfo(xml_node& XMLContent)
{
	if (XMLContent.has_attribute("ExistWhenNoPlugin"))
		m_ExistWhenNoPlugin = XMLContent.attribute("ExistWhenNoPlugin");

	for (UINT i = 0; i < XMLContent.children(); i++)
	{
		xml_node Plugin = XMLContent.child(i);
		if (_stricmp(Plugin.name(), "Plugin") == 0)
		{
			PLUGIN_INFO PluginInfo;
			if (Plugin.has_attribute("Type"))
			{
				CEasyString TypeName = Plugin.attribute("Type").getvalue();
				if (TypeName.CompareNoCase("CSharp") == 0)
					PluginInfo.PluginType = 1;
			}				
			if (Plugin.has_attribute("Name"))
			{
				PluginInfo.Name= (const TCHAR*)Plugin.attribute("Name");
			}

			m_PluginList.push_back(PluginInfo);
		}
	}

	return true;
}