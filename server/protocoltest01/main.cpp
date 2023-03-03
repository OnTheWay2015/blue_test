#include "stdafx.h"



/*
* ����protobuf3 Ĭ��ֵ����
     * ���ȣ�pb�����get������Զ������null�����û�����ã������ɶ�Ӧ��Ĭ��ֵ
     * �������ֵ��һ��message����ôpb��Ϊ�����������һ��hasXXX��������������������ж��Ƿ��������������
     * �����������ԣ�����string��uint32�ȵȣ����û�����ã�get�����Ľ����ǿ�ֵ������string����""��uint32����0����ʱ�޷�������û�����û���������Ĭ��ֵ����Ҫҵ������Լ�������紫һ��"-1"�������ֵ��ʶ
*/

/*
�� Descriptor ��Ҫ�Ƕ� Message �������������� message �����֡������ֶε�������ԭʼ proto �ļ����ݵȣ�������ܸ����а����ĺ�����
�����ǻ�ȡ������Ϣ�ĺ�����

const std::string & name() const; // ��ȡmessage��������
int field_count() const; // ��ȡ��message���ж����ֶ�
const FileDescriptor* file() const; // The .proto file in which this message type was defined. Never nullptr.

���� Descriptor �У�����ͨ�����·�����ȡ�� FieldDescriptor��

const FieldDescriptor* field(int index) const; // ���ݶ���˳��������ȡ������0��ʼ����������Ŀ
const FieldDescriptor* FindFieldByNumber(int number) const; // ���ݶ����message�����˳��ֵ��ȡ��option string name=3��3��Ϊnumber��
const FieldDescriptor* FindFieldByName(const string& name) const; // ����field name��ȡ
const FieldDescriptor* Descriptor::FindFieldByLowercaseName(const std::string & lowercase_name)const; // ����Сд��field name��ȡ
const FieldDescriptor* Descriptor::FindFieldByCamelcaseName(const std::string & camelcase_name) const; // �����շ��field name��ȡ

����FieldDescriptor* field(int index)��FieldDescriptor* FindFieldByNumber(int number)���������index��number�ĺ����ǲ�һ����

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
    if (ref->HasField(a, field)) // set_name ���ú�Ϊ true
    {
        if (type == ::google::protobuf::FieldDescriptor::CppType::CPPTYPE_STRING) 
        {
            auto ss = ref->GetString(a,field);
        }
    }

    return 0;
}

int main()
{
    pb_001();
    return 0;
}

