/****************************************************************************/
/*                                                                          */
/*      文件名:    DBInterfaces.cpp                                         */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "stdafx.h"

namespace DBLib
{

IDatabase::IDatabase():CNameObject()
{
}

IDatabase::~IDatabase()
{
}

IDBConnection::IDBConnection():CNameObject()
{
}

IDBConnection::~IDBConnection()
{
}

IDBRecordSet::IDBRecordSet():CNameObject()
{
}

IDBRecordSet::~IDBRecordSet()
{
}

IDBParameterSet::IDBParameterSet():CNameObject()
{
}

IDBParameterSet::~IDBParameterSet()
{
}

//IMPLEMENT_CLASS_INFO_STATIC(CDBValue,CNameObject);
//CDBValue::CDBValue():CNameObject()
//{
//}
//
//CDBValue::~CDBValue()
//{
//}

}