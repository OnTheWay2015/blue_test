#include "stdafx.h"



CDBTransCheckDB::CDBTransCheckDB(CPlayerDBAdapterGroup * pParent, const CEasyArray<INSTANCE_CONFIG>& InstanceConfigList)
{
	m_pParent = pParent;
	m_InstanceConfigList = InstanceConfigList;
}


CDBTransCheckDB::~CDBTransCheckDB()
{
}

bool CDBTransCheckDB::OnExecute(IDBConnection * pConnection)
{
	char szSQL[1024];
	CSmartPtr<IDBRecordSet> pRecordSet;
	CDBParameterSet ParameterSet;
	DB_PARAM_INFO ParamInfo;
	int DBResult;

	pRecordSet = pConnection->CreateRecordSet(DB_RS_TYPE_DYNAMIC);
	if (pRecordSet == NULL)
	{
		Log("无法创建结果集");
		return false;
	}

	for (INSTANCE_CONFIG& InstanceConfig : m_InstanceConfigList)
	{
		CEasyString DBName;

		DBName = InstanceConfig.DBConfig.DBName;

		//检查数据库
		sprintf_s(szSQL, 1024, "SELECT SCHEMA_NAME FROM information_schema.SCHEMATA where SCHEMA_NAME='%s'", (LPCTSTR)DBName);
		DBResult = pConnection->ExecuteSQL(szSQL);
		if (DBResult != DBERR_SUCCEED)
		{
			Log("执行SQL出错:(%u)%s as %s",
				pConnection->GetLastDatabaseErrorCode(),
				pConnection->GetLastDatabaseErrorString(),
				pConnection->GetLastSQL());
			return false;
		}

		DBResult = pConnection->GetResults(pRecordSet);
		if (DBResult != DBERR_SUCCEED && DBResult != DBERR_NO_RECORDS)
		{
			Log("获取结果集出错:(%u)%s at %s",
				pConnection->GetLastDatabaseErrorCode(),
				pConnection->GetLastDatabaseErrorString(),
				pConnection->GetLastSQL());
			return false;
		}

		if (DBResult == DBERR_NO_RECORDS)
		{
			//创建数据库
			sprintf_s(szSQL, 1024, "CREATE DATABASE %s DEFAULT CHARACTER SET utf8", (LPCTSTR)DBName);
			DBResult = pConnection->ExecuteSQL(szSQL);
			if (DBResult != DBERR_SUCCEED)
			{
				Log("执行SQL出错:(%u)%s as %s",
					pConnection->GetLastDatabaseErrorCode(),
					pConnection->GetLastDatabaseErrorString(),
					pConnection->GetLastSQL());
				return false;
			}
		}
		pRecordSet->Destory();

		for (const DB_TABLE_CONFIG& TableDefine : InstanceConfig.DBConfig.TableList)
		{
			//检查表
			sprintf_s(szSQL, 1024, "select TABLE_NAME from INFORMATION_SCHEMA.TABLES where TABLE_SCHEMA = '%s' and TABLE_NAME = '%s'", 
				(LPCTSTR)DBName,
				(LPCTSTR)TableDefine.Name);
			DBResult = pConnection->ExecuteSQL(szSQL);
			if (DBResult != DBERR_SUCCEED)
			{
				Log("执行SQL出错:(%u)%s as %s",
					pConnection->GetLastDatabaseErrorCode(),
					pConnection->GetLastDatabaseErrorString(),
					pConnection->GetLastSQL());
				return false;
			}


			DBResult = pConnection->GetResults(pRecordSet);
			if (DBResult != DBERR_SUCCEED && DBResult != DBERR_NO_RECORDS)
			{
				Log("获取结果集出错:(%u)%s at %s",
					pConnection->GetLastDatabaseErrorCode(),
					pConnection->GetLastDatabaseErrorString(),
					pConnection->GetLastSQL());
				return false;
			}

			if (DBResult == DBERR_NO_RECORDS)
			{
				//创建表

				CEasyString Define = TableDefine.Define;
				CEasyString TableName;
				CEasyString LongTableName;
				TableName.Format("`%s`", (LPCTSTR)TableDefine.Name);
				LongTableName.Format("`%s`.`%s`", (LPCTSTR)DBName, (LPCTSTR)TableDefine.Name);
				Define.Replace(TableName, LongTableName);

				DBResult = pConnection->ExecuteSQL(Define);
				if (DBResult != DBERR_SUCCEED)
				{
					Log("执行SQL出错:(%u)%s as %s",
						pConnection->GetLastDatabaseErrorCode(),
						pConnection->GetLastDatabaseErrorString(),
						pConnection->GetLastSQL());
					return false;
				}
			}

			pRecordSet->Destory();
		}
	}
	return true;
}
void CDBTransCheckDB::OnFinish()
{
	m_pParent->OnDBRetCheckDB(m_IsSucceed);
}