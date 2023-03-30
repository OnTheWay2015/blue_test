#include "stdafx.h"



/*
* 关于protobuf3 默认值问题
     * 首先，pb对象的get方法永远不会有null，如果没有设置，会生成对应的默认值
     * 如果属性值是一个message，那么pb会为这个属性生成一个hasXXX方法，可以用这个方法判断是否设置了这个属性
     * 对于其他属性，比如string，uint32等等，如果没有设置，get出来的将会是空值，比如string就是""，uint32就是0，此时无法区分是没有设置还是设置了默认值，需要业务自行约定，比如传一个"-1"等特殊的值标识
*/

/*
类 Descriptor 主要是对 Message 进行描述，包括 message 的名字、所有字段的描述、原始 proto 文件内容等，下面介绍该类中包含的函数。
首先是获取自身信息的函数：

const std::string & name() const; // 获取message自身名字
int field_count() const; // 获取该message中有多少字段
const FileDescriptor* file() const; // The .proto file in which this message type was defined. Never nullptr.

在类 Descriptor 中，可以通过如下方法获取类 FieldDescriptor：

const FieldDescriptor* field(int index) const; // 根据定义顺序索引获取，即从0开始到最大定义的条目
const FieldDescriptor* FindFieldByNumber(int number) const; // 根据定义的message里面的顺序值获取（option string name=3，3即为number）
const FieldDescriptor* FindFieldByName(const string& name) const; // 根据field name获取
const FieldDescriptor* Descriptor::FindFieldByLowercaseName(const std::string & lowercase_name)const; // 根据小写的field name获取
const FieldDescriptor* Descriptor::FindFieldByCamelcaseName(const std::string & camelcase_name) const; // 根据驼峰的field name获取

其中FieldDescriptor* field(int index)和FieldDescriptor* FindFieldByNumber(int number)这个函数中index和number的含义是不一样的

message Student{
optional string name = 1;
optional string gender = 2;
optional string phone = 5;
}

*/






int pb_001()
{
    t1::MSG_TEST b;
    b.set_name("test");
   
/*
int len = mb.msgbuff.size();	
std::memcpy(pbuf+PACKET_HEAD_SIZE, mb.msgbuff.c_str(), len);
*/

    std::vector<unsigned char> vec(b.ByteSize());
    b.SerializeToArray( &vec[0], b.ByteSize());
    
    t1::MSG_TEST c;
    c.ParseFromArray(&vec[0], b.ByteSize());


    std::string sstr;
    b.SerializeToString(&sstr);

    t1::MSG_TEST a;
    a.ParseFromString(sstr);
 
    //const ::google::protobuf::Descriptor* desc = t1::MSG_TEST::descriptor();
    const ::google::protobuf::Descriptor* desc = a.descriptor();
    const ::google::protobuf::FieldDescriptor* field = desc->FindFieldByLowercaseName("name");

    ::google::protobuf::FieldDescriptor::CppType type = field->cpp_type();
    if (field->has_default_value())
    {
        if (type == ::google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING)  // TYPE_STRING, TYPE_BYTES 
        {
            auto ss = field->default_value_string();
        }
    }

    const ::google::protobuf::Reflection*  ref = a.GetReflection();
    if (ref->HasField(a, field)) // set_name 设置后为 true
    {
        if (type == ::google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING) 
        {
            auto ss = ref->GetString(a,field);
        }
    }

    return 0;
}
void ttt()
{
    std::map<int,int> b{{1,2},{3,4}};

    std::vector<int> a {1,2,3,4};

    int c = 0x123456;

}
/*
class axx
{
    public:
    axx() {
    }
    virtual ~axx(){}
};

class ax
{
    public:
        virtual void axtest() = 0;
};
class ab :public ax , public axx
{
public:
    ab(){
    }
    virtual ~ab(){}
    virtual void axtest() override {}
public:
    bool value_ab;
};


class abc:public ab
{
public:
    abc(){
    }
    virtual ~abc(){}
public:
    bool value_abc;
};

class abcd:
	public abc 
{
protected:
    volatile  int m_Status;


public:
	abcd(void) {}
	virtual ~abcd(void) {}
};
*/

#include <unordered_map>
#include <vector>
#include <map>
#include <memory>


//typedef std::shared_ptr<ax> ABC;


//typedef std::shared_ptr<ConnectionDefault> CCC;
//typedef std::shared_ptr<BaseNetConnectionInterface > CCC;
//void fff( ABC dd)

//std::map<int, CCC> mm;
//void fff(  CCC dd)
//{
//    auto id = dd->GetSessionID();
//    mm[id] = dd;
//
//}




class axx
{
public:
    axx(){
    }
    virtual ~axx() {}


};

class ax 
{
public:
    ax(){
    }
    virtual ~ax() {}
public:
	virtual bool Create(int RecvQueueSize, int SendQueueSize) = 0;
}; 
class ab:
	public ax
	,public axx
{
public:
	bool m_StopFlag;
public:
	ab(void) {

    }
	virtual ~ab(void){}

	virtual bool Create(int RecvQueueSize, int SendQueueSize) override {} 

};



int main()
{
    auto xx = std::make_shared<ConnectionDefault>();
    std::map<int, std::shared_ptr<BaseNetConnectionInterface> > mmm;
    mmm[123] = xx;

    //auto xx = std::make_shared<ab>();
    //std::map<int, std::shared_ptr<ax> > mmm;
    //mmm[123] = xx;

    //std::map<int, BaseNetConnectionInterface* > mmm1;
    
    //BaseNetConnectionInterface* xxx = xx.get(); 
    //mmm1[xx->GetID()] = xxx;

    
    //std::map<int, ax* > mmm1;
    //auto xx = std::make_shared<abc>(123);
    //ax* xxx = xx.get(); 
    //mmm1[123] = xxx;



    //abc a(123);

    //auto xx = std::make_shared<abc>(123);
    //fff(xx);




    //ttt();
    //pb_001();
    return 0;
}

