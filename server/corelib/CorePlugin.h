#pragma once

class CoreBaseInterface;
class CorePluginInterface
{
public:
    virtual bool Init(CoreBaseInterface* Core, std::string DllName )=0;
    virtual void Update()=0;
    virtual int Exit()=0;
    virtual bool IsExit()=0;
};


