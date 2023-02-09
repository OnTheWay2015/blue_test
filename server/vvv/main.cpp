#include "stdafx.h"
#include "../protocol/protocol.h"
int main_svr()
{
    
    CNetServer svr;

    t1::MSG_TEST a;
    a.set_name("test");

    int EventObjectPoolSize = 10;
    int WorkThreadCount = 1;
    int EventRouterPoolSiz = 10;
    int EventObjectPoolGrowSize = 10;
    int EventObjectPoolGrowLimit = 10;
    int EventRouterPoolGrowSize = 10;
    int EventRouterPoolGrowlimit = 10;

    svr.StartUp(EventObjectPoolSize,
        WorkThreadCount,
        EventRouterPoolSiz,
        EventObjectPoolGrowSize,
        EventObjectPoolGrowLimit,
        EventRouterPoolGrowSize,
        EventRouterPoolGrowlimit);//有初始化网络线程, iocp 在网络线程里读取网卡数据

    
    //CNetService ss;
    CProxyServiceDefault ss;
    ss.SetServer(&svr);

    if (!ss.Create(
        IPPROTO_TCP,
        16,//AcceptQueueSize
        1024*5,//RecvBufferSize
        1024*5,//SendBufferSize
        1,//ParallelAcceptCount
        64,//DEFAULT_PARALLEL_RECV
    false //IPv6Only
    ))
    {

		//PrintDOSLog( _T("代理服务[%u]创建失败！"), GetID());
		return 0;
	}
   
    ss.Init();//有初始化工作线程

    CIPAddress  addr;
    addr.SetIP("0");
    //addr.SetIP("127.0.0.1");
    addr.SetPort(61000);
	if (!ss.StartListen(addr)) //m_Config.ListenAddress
	{
		//PrintDOSLog( _T("代理服务[%u]侦听于(%s:%u)失败！"),
		//	GetID(),
		//	m_Config.ListenAddress.GetIPString(),
		//	m_Config.ListenAddress.GetPort());
		return 0;
	}



    while(1)
    {
        ss.Update();//可单独创建一线程处理, 处理Accept事件
    
        DoSleep(100);
    }
    return 0;
}



void main_client()
{
    CNetServer svr;

    int EventObjectPoolSize = 10;
    int WorkThreadCount = 1;
    int EventRouterPoolSiz = 10;
    int EventObjectPoolGrowSize = 10;
    int EventObjectPoolGrowLimit = 10;
    int EventRouterPoolGrowSize = 10;
    int EventRouterPoolGrowlimit = 10;

    svr.StartUp(EventObjectPoolSize,
        WorkThreadCount,
        EventRouterPoolSiz,
        EventObjectPoolGrowSize,
        EventObjectPoolGrowLimit,
        EventRouterPoolGrowSize,
        EventRouterPoolGrowlimit);//有初始化网络线程, iocp 在网络线程里读取网卡数据

    
    CProxyConnectionDefault cc;
    cc.SetServer(&svr);
    cc.Init(); 

    CIPAddress  addr;
    addr.SetIP("127.0.0.1");
    addr.SetPort(61000);
	if (!cc.Connect(addr)) 
    {
        //error 连接失败
        return;
    }


    while(1)
    {
        cc.Update();
    
        DoSleep(100);
    }
}

int main()
{
    main_svr();

    //main_client();
    return 0;
}


