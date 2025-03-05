#include "stdafx.h"

bool CDBTransLoadRoleList::OnExecute(IDBConnection* pConnection)
{
	char szSQL[1024];
	CSmartPtr<IDBRecordSet> pRecordSet;
	int DBResult;

	pRecordSet = pConnection->CreateRecordSet(DB_RS_TYPE_DYNAMIC);
	if (pRecordSet == NULL)
	{
		Log("无法创建结果集");
		return false;
	}

	pRecordSet->SetBlobMaxProcessSize(1048576);

	m_CompressBuff.Create(1048576 * 2);


	sprintf_s(szSQL, 1024, "select RoleID,RoleName,unix_timestamp(CreateTime),RoleData "
		"from t_role_info where AccountID=%lld and ManageStatus<>1",
		m_AccountID);


	LogDebug("执行SQL:%s", szSQL);
	DBResult = pConnection->ExecuteSQL(szSQL, 0);

	if (DBResult != DBERR_SUCCEED)
	{
		Log("执行SQL出错:(%u)%s",
			pConnection->GetLastDatabaseErrorCode(),
			pConnection->GetLastDatabaseErrorString());
		Log("出错的SQL:%s", szSQL);
		return false;
	}

	DBResult = pConnection->GetResults(pRecordSet);
	if (DBResult != DBERR_SUCCEED && DBResult != DBERR_NO_RECORDS)
	{
		Log("获取结果集出错:(%u)%s",
			pConnection->GetLastDatabaseErrorCode(),
			pConnection->GetLastDatabaseErrorString());
		Log("出错的SQL:%s", szSQL);
		return false;
	}
	while (!pRecordSet->IsEOF())
	{
		int FieldIndex = 0;
		CSmartStruct Packet;

		bool NeedRepack = false;
		UINT64 RoleID = pRecordSet->GetField(FieldIndex++);
		CEasyString RoleName;
		CDBValue& RoleNameValue = pRecordSet->GetField(FieldIndex++);
		RoleName.SetString((const char*)RoleNameValue, RoleNameValue.GetLength(), CEasyString::STRING_CODE_PAGE_UTF8);
		UINT CreateTime = pRecordSet->GetField(FieldIndex++);
		{
			CDBValue& Data = pRecordSet->GetField(FieldIndex++);

			if (!Data.IsNull())
			{
				lzo_uint OutLen = m_CompressBuff.GetBufferSize();
				int Error = lzo1x_decompress_safe((BYTE*)Data.GetBuffer(), Data.GetLength(),
					(BYTE*)m_CompressBuff.GetBuffer(), &OutLen, NULL);
				if (Error != LZO_E_OK)
				{
					Log("玩家数据解压失败%d", Error);
					return true;
				}
				
				Packet.Attach(m_CompressBuff.GetBuffer(), m_CompressBuff.GetBufferSize(), false);
			}
		}
		UINT64 PacketRoleID = Packet.GetMember(CBaseCharInfo::SST_MII_OBJECT_ID);
		if (RoleID != PacketRoleID)
		{
			//ID不一致，重新设置
			Log("角色ID[0x%llX]和数据包内角色ID[0x%llX]不一致", RoleID, PacketRoleID);
			if (Packet.IsMemberExist(CBaseCharInfo::SST_MII_OBJECT_ID))
			{
				Packet.GetMember(CBaseCharInfo::SST_MII_OBJECT_ID).SetValue(RoleID);
			}
			else
			{
				//需要重新打包
				NeedRepack = true;
				Packet.AddMember(CBaseCharInfo::SST_MII_OBJECT_ID, RoleID);
			}
		}
		
		LPCTSTR PacketRoleName = Packet.GetMember(CBaseCharInfo::SST_MII_NAME);
		if (RoleName != PacketRoleName)
			NeedRepack = true;
		if(NeedRepack)
		{
			//昵称不一致，需要重新打包
			CRoleInfo RoleInfo;
			RoleInfo.ParsePacket(Packet, DOMF_ROLE_INFO_FULL);
			RoleInfo.SetObjectID(RoleID);
			RoleInfo.SetName(RoleName);
			RoleInfo.SetCreateTime(CreateTime);
			CSmartStruct* pPacket = m_RoleList.AddEmpty();
			pPacket->Create(RoleInfo.GetSmartStructSize(DOMF_PLAYER_INFO_FOR_SAVE));
			if (!RoleInfo.MakePacket(*pPacket, DOMF_PLAYER_INFO_FOR_SAVE))
			{
				Log("数据重新打包失败[0x%llX]", RoleID);
				return true;
			}
		}
		else
		{
			if (Packet.IsMemberExist(CRoleInfo::SST_RI_CREATE_TIME))
			{
				Packet.GetMember(CRoleInfo::SST_RI_CREATE_TIME).SetValue(CreateTime);
			}
			else
			{
				Packet.AddMember(CRoleInfo::SST_RI_CREATE_TIME, CreateTime);
			}
			m_RoleList.AddEmpty()->CloneFrom(Packet);
		}
		pRecordSet->MoveNext();
	}
	
	m_Result = COMMON_RESULT_SUCCEED;
	return true;
}
void CDBTransLoadRoleList::OnFinish()
{
	m_pParent->OnDBRetLoadRoleList(m_Result, m_CallerID, m_AccountID, m_RoleList);
}
