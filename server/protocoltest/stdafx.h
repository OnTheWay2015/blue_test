
#pragma once

#include "../protocol/protocol.h"
class NObject
{
public:
    NObject(){};
    virtual ~NObject() {};


};

class BaseNetConnectionInterface 
{
public:
	virtual bool Create(int RecvQueueSize, int SendQueueSize) = 0;
}; 
class BaseNetConnection :
	public BaseNetConnectionInterface 
	,public NObject
{
protected:
	bool m_StopFlag;
public:
	BaseNetConnection(void) {}
	virtual ~BaseNetConnection(void){}

	virtual bool Create(int RecvQueueSize, int SendQueueSize) override {} 

};

class NetConnection :
	public BaseNetConnection
{
protected:
	bool										m_UseSafeDisconnect;

public:
	NetConnection(void) {}
	virtual ~NetConnection(void) {}


};


class ConnectionDefault :
	public NetConnection
{
protected:
    volatile  int m_Status;


public:
	ConnectionDefault(void) {}
	virtual ~ConnectionDefault(void) {}


};

