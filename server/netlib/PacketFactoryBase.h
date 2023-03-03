#pragma once
//#include <enable_singleton.h>
//#include <enable_hashmap.h>
//#include <enable_smart_ptr.h>
//#include <exception>
//#include <boost/lexical_cast.hpp>


class packet_factory_base
{
public:
	packet_factory_base(){}
	virtual ~packet_factory_base(){}

	virtual CSmartPtr<void> create(int id)=0;


	virtual bool packet_process(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<void> msg)=0;

};

template <class T>
class factory_handler_send : public packet_factory_base
{
public:
	factory_handler_send (){}
	virtual ~factory_handler_send (){}
	virtual CSmartPtr<void> create(int id) override
	{
		auto m = std::make_shared<T>();
		m->set_packet_id(id);
		return m; 
	}
	virtual bool packet_process(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<void> msg) override {return false;};
};

template <class T>
class factory_handler_recv : public factory_handler_send<T> 
{
public:
	factory_handler_recv (){}
	virtual ~factory_handler_recv (){}

	virtual bool packet_process(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<void> msg) override {return false;};
	virtual bool packet_process(CSmartPtr<CBaseNetConnectionInterface> session, CSmartPtr<T> msg)=0;
};

class packet_factories
{
private:
	CStaticMap<int, CSmartPtr<packet_factory_base>> value_map;
	bool b_release;
public:
	packet_factories(void):b_release(false)
	{}
	virtual ~packet_factories(void)
	{
		release();
	}

	void release()
	{
		value_map.clear();
		b_release = true;
	}

	void regedit_object(int id, CSmartPtr<packet_factory_base> obj)
	{
		auto ret = value_map.insert(std::make_pair(id, obj));
		if(!ret.second)
		{
			//std::string err = "regedit_object error id:" + boost::lexical_cast<std::string>(id);
			throw new std::exception();	
		}
	}

	CSmartPtr<void> create(int id)
	{
		if(b_release) 
			return nullptr;

		auto it = value_map.find(id);
		if(it != value_map.end())
		{
			return it->second->create(id);
		}
		return nullptr;
	}

	CSmartPtr<packet_factory_base> get_factroy(int id)
	{
		if(b_release) 
			return nullptr;

		auto it = value_map.find(id);
		if(it != value_map.end())
			return it->second;

		return nullptr;
	}

	const CStaticMap<int, CSmartPtr<packet_factory_base>>& get_map()
	{
		return value_map;
	}
};

