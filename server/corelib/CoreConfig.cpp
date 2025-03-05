#include "stdafx.h"
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
	//if (XMLContent.has_attribute(_T("ProxyType")))
	//	Config.ProxyType = (SERVICE_TYPE )(int)XMLContent.attribute(_T("ProxyType"));

	if (XMLContent.has_attribute(_T("ProxyMode")))
		Config.ProxyMode = (CLIENT_PROXY_MODE)((int)XMLContent.attribute(_T("ProxyMode")));

	if (XMLContent.has_attribute(_T("ListenIP")))
	{
		auto attr = XMLContent.attribute(_T("ListenIP"));
		auto sz = attr.value_size();
		Config.ListenIP = STD_STR(attr.getvalue(),attr.value_size()); //name_size()
		//Config.ListenIP = (const TCHAR*)XMLContent.attribute(_T("ListenIP"));
	}

	if (XMLContent.has_attribute(_T("ListenPort")))
		Config.ListenPort = (WORD)XMLContent.attribute(_T("ListenPort"));

	if (XMLContent.has_attribute(_T("ConnectionGroupCount")))
		Config.ConnectionGroupCount = (UINT)XMLContent.attribute(_T("ConnectionGroupCount"));

	if (XMLContent.has_attribute(_T("MsgQueueSize")))
		Config.MsgQueueSize= (UINT)XMLContent.attribute(_T("MsgQueueSize"));

	if (XMLContent.has_attribute(_T("KeepAliveTime")))
		Config.KeepAliveTime = (UINT)XMLContent.attribute(_T("KeepAliveTime"));


	return true;
}

bool CoreConfig::OnLoaded(pug::xml_node& root) 
{
//to be override by sub
    return true;
}


bool CoreConfig::LoadConfig(LPCTSTR FileName)
{
    xml_parser Parser;

    //PrintImportantLog(_T(_T("开始载入配置文件%s")), _T(FileName));
    //PrintImportantLog(_T(_T("开始载入配置文件")));
    PrintImportantLog(_T("不能立即完成"));
    if (!Parser.parse_file(FileName, pug::parse_trim_attribute))
    {
        PrintImportantLog(_T("无法打开配置文件[%s]"), FileName);
        return false;
    }
    xml_node Config = Parser.document();
    if (!LoadCommonConfig(Config,FileName))
    {
        PrintImportantLog(_T("无法加载 common 配置"));
        return false;
    }

    if (Config.moveto_child(_T("Main")))
    {

        xml_node Net = Config;
        if (Net.moveto_child(_T("Net")))
        {
            if (Net.has_attribute(_T("NetWorkThreadCount")))
                m_NetConfig.NetWorkThreadCount = Net.attribute(_T("NetWorkThreadCount"));
        }

        xml_node Leader = Config;
        if (Leader.moveto_child(_T("Leader")))
        {
            if (Leader.has_attribute(_T("Port")))
                m_LeaderConfig.Port = Leader.attribute(_T("Port"));
            if (Leader.has_attribute(_T("IP")))
                m_LeaderConfig.IP = Leader.attribute(_T("IP")).getvalue();
        }



        xml_node ActConnects = Config;
        if (ActConnects.moveto_child(_T("ActConnects")))
        {
            for (UINT i = 0; i < ActConnects.children(); i++)
            {
                xml_node cc = ActConnects.child(i);
                if (STD_ICMP(cc.name(), _T("Connect")) == 0)
                {

                    CONNECT_CONFIG ccConfig;
                    if (cc.has_attribute(_T("ServerID")))
                        ccConfig.ServerID = cc.attribute(_T("ServerID"));
                    if (cc.has_attribute(_T("ServiceType")))
                        ccConfig.ServiceType = cc.attribute(_T("ServiceType"));
                    m_ActConnects.push_back(ccConfig);
                }
            }
        }


        xml_node Services = Config;
        if (Services.moveto_child(_T("Services")))
        {
            for (UINT i = 0; i < Services.children(); i++)
            {
                xml_node Service = Services.child(i);
                if (STD_ICMP(Service.name(), _T("Service")) == 0)
                {

                    SERVICE_CONFIG ServiceConfig;
                    if (Service.has_attribute(_T("Name")))
                        ServiceConfig.Name = Service.attribute(_T("Name")).getvalue();
                    if (Service.has_attribute(_T("Desc")))
                        ServiceConfig.Desc = Service.attribute(_T("Desc")).getvalue();
                    if (Service.has_attribute(_T("ServiceType")))
                        ServiceConfig.ServiceType = Service.attribute(_T("ServiceType"));

                    for (UINT i = 0; i < Service.children(); i++)
                    {
                        xml_node ClientProxy = Service.child(i);
                        if (STD_ICMP(ClientProxy.name(), _T("ClientProxy")) == 0)
                        {
                            CLIENT_PROXY_CONFIG ProxyConfig;
                            if (ReadProxyConfig(ClientProxy, ProxyConfig))
                            {
                                ServiceConfig.m_ClientProxys.push_back(ProxyConfig);
                            }
                        }
                    }

                    m_Services.push_back(ServiceConfig);
                }
            }


        }

        xml_node Plugins = Config;
        if (Plugins.moveto_child(_T("Plugins")))
        {
            LoadPluginInfo(Plugins);
        }
    }
    else
    {
        PrintImportantLog(_T("配置文件[%s]不合法"), FileName);
        return false;
    }

    
    return OnLoaded( Config );
}

bool CoreConfig::ReadDBConfig(xml_node& XMLContent, DB_CONNECT_CONFIG& Config, LPCTSTR SearchDir)
{
	Config.Name = XMLContent.get_name();
	Config.Name = XMLContent.get_name();
	CEasyString DBType = XMLContent.attribute(_T("DBType")).getvalue();
	DBType.Trim();
	DBType.MakeUpper();
	if (DBType == _T("MSSQL"))
		Config.DBType = DB_TYPE_MSSQL;
	else
		Config.DBType = DB_TYPE_MYSQL;

	Config.DBConnectString = XMLContent.attribute(_T("DBConnectString")).getvalue();

	if (XMLContent.has_attribute(_T("ThreadCount")))
		Config.ThreadCount = XMLContent.attribute(_T("ThreadCount"));
	if (XMLContent.has_attribute(_T("QueueSize")))
		Config.QueueSize = XMLContent.attribute(_T("QueueSize"));
	if (XMLContent.has_attribute(_T("IsLogPerformance")))
		Config.IsLogPerformance = XMLContent.attribute(_T("IsLogPerformance"));
	if (XMLContent.has_attribute(_T("RunLimit")))
		Config.RunLimit = XMLContent.attribute(_T("RunLimit"));
	if (XMLContent.has_attribute(_T("DBName")))
		Config.DBName = XMLContent.attribute(_T("DBName")).getvalue();

	for (UINT i = 0; i < XMLContent.children(); i++)
	{
		xml_node TableDefine = XMLContent.child(i);
		if (TableDefine.get_name() == _T("TableDefine"))
		{
			DB_TABLE_CONFIG Table;
			Table.Name = TableDefine.attribute(_T("Name")).getvalue();
			if (!Table.Name.IsEmpty())
			{
				CEasyString Define = TableDefine.attribute(_T("Define")).getvalue();
				if (!CFileTools::IsAbsolutePath(Table.Define))
				{
					Define = DIR_SLASH + Define;
					Define = SearchDir + Define;
					Table.Define = CFileTools::MakeFullPath(Define);
				}
				else
				{
					Table.Define = CFileTools::MakeFullPath(Define);
				}
				CStringFile DefineFile;
				if (DefineFile.LoadFile(Table.Define, false))
				{
					Table.Define = DefineFile.GetData();
					if (!Table.Define.IsEmpty())
					{
						Config.TableList.emplace_back(Table);
					}
				}
				else
				{
					//Log(_T("无法打开文件[%s]"), Table.Define);
				}
			}
		}
	}

	if ((!Config.Name.IsEmpty()) && (!Config.DBConnectString.IsEmpty()) && (!Config.DBName.IsEmpty()))
		return true;
	else
		return false;
}

bool CoreConfig::LoadCommonConfig( pug::xml_node& root, LPCTSTR ConfigFileName)
{
    xml_node Config = root;
    if (!Config.moveto_child(_T("Main")))
    {
        return false;
    }


    xml_node Server = Config;
    if (Server.moveto_child(_T("Server")))
    {
        //m_CommonConfig.ServerConfig.MinVersion = (LPCTSTR)Server.attribute(_T("MinVersion")).getvalue();
        //m_CommonConfig.ServerConfig.CurVersion = (LPCTSTR)Server.attribute(_T("CurVersion")).getvalue();
        //m_CommonConfig.ServerConfig.Env = (LPCTSTR)Server.attribute(_T("Env")).getvalue();
        m_CommonConfig.ServerConfig.ServerID = Server.attribute(_T("ServerID"));
        CEasyString ServerAreaList = Server.attribute(_T("ServerAreaList")).getvalue();


        //if (Config.has_attribute(_T("ServerName")))
        //    m_ServerName = Config.attribute(_T("ServerName")).getvalue();
        //if (Config.has_attribute(_T("ServerDesc")))
        //    m_ServerDesc = Config.attribute(_T("ServerDesc")).getvalue();

        //if (Config.has_attribute(_T("ServerID")))
        //    m_ServerID = Config.attribute(_T("ServerID"));
        ////if(Config.has_attribute(_T("ServiceType")))
        ////	m_ServiceType=Config.attribute(_T("ServiceType"));





        std::vector<STD_STR> Splitter;
        Stringsplit(STD_STR(ServerAreaList), _T('|'), Splitter);

        for (int i = 0; i < (int)Splitter.size(); i++)
        {
            m_CommonConfig.ServerConfig.ServerAreaList.push_back(_tcstol(Splitter[i].c_str(), NULL, 10));
        }
    }

    xml_node ExternalMailSystemConfig = Config;
    if (ExternalMailSystemConfig.moveto_child(_T("ExternalMailSystemConfig")))
    {
        if (ExternalMailSystemConfig.has_attribute(_T("RequestURL")))
            m_CommonConfig.MailConfig.RequestURL = ExternalMailSystemConfig.attribute(_T("RequestURL")).getvalue();

        if (ExternalMailSystemConfig.has_attribute(_T("EncodeKey")))
            m_CommonConfig.MailConfig.EncodeKey = ExternalMailSystemConfig.attribute(_T("EncodeKey")).getvalue();
    }

    xml_node DBConfig = Config;
    if (DBConfig.moveto_child(_T("DBConfig")))
    {
        for (UINT i = 0; i < DBConfig.children(); i++)
        {
            DB_CONNECT_CONFIG Config;
            auto cc = DBConfig.child(i);
            if (ReadDBConfig(cc, Config, CFileTools::GetPathDirectory(ConfigFileName)))
            {
                m_CommonConfig.DBConfigList.emplace_back(Config);
            }
        }
    }
    //LogDebug(_T("MinVersion=%s,CurVersion=%s,ServerID=%u"),
    //    CommonConfig.ServerConfig.MinVersion.ToStr(), CommonConfig.ServerConfig.CurVersion.ToStr(), CommonConfig.ServerConfig.ServerID);

    return true;
}




bool CoreConfig::LoadPluginInfo(xml_node& XMLContent)
{
	if (XMLContent.has_attribute(_T("ExistWhenNoPlugin")))
		m_ExistWhenNoPlugin = XMLContent.attribute(_T("ExistWhenNoPlugin"));

	for (UINT i = 0; i < XMLContent.children(); i++)
	{
		xml_node Plugin = XMLContent.child(i);
		if (STD_ICMP(Plugin.name(), _T("Plugin")) == 0)
		{
			PLUGIN_INFO PluginInfo;
			if (Plugin.has_attribute(_T("Type")))
			{
				CEasyString TypeName = Plugin.attribute(_T("Type")).getvalue();
				if (TypeName.CompareNoCase(_T("CSharp")) == 0)
					PluginInfo.PluginType = 1;
			}				
			if (Plugin.has_attribute(_T("Name")))
			{
				PluginInfo.Name= (const TCHAR*)Plugin.attribute(_T("Name"));
			}

			m_PluginList.push_back(PluginInfo);
		}
	}

	return true;
}