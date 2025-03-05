#include "stdafx.h"

bool CDBTransLoadRoleList::OnExecute(IDBConnection* pConnection)
{
	char szSQL[1024];
	CSmartPtr<IDBRecordSet> pRecordSet;
	int DBResult;

	pRecordSet = pConnection->CreateRecordSet(DB_RS_TYPE_DYNAMIC);
	if (pRecordSet == NULL)
	{
		Log("�޷����������");
		return false;
	}

	pRecordSet->SetBlobMaxProcessSize(1048576);

	m_CompressBuff.Create(1048576 * 2);


	sprintf_s(szSQL, 1024, "select RoleID,RoleName,unix_timestamp(CreateTime),RoleData "
		"from t_role_info where AccountID=%lld and ManageStatus<>1",
		m_AccountID);


	LogDebug("ִ��SQL:%s", szSQL);
	DBResult = pConnection->ExecuteSQL(szSQL, 0);

	if (DBResult != DBERR_SUCCEED)
	{
		Log("ִ��SQL����:(%u)%s",
			pConnection->GetLastDatabaseErrorCode(),
			pConnection->GetLastDatabaseErrorString());
		Log("�����SQL:%s", szSQL);
		return false;
	}

	DBResult = pConnection->GetResults(pRecordSet);
	if (DBResult != DBERR_SUCCEED && DBResult != DBERR_NO_RECORDS)
	{
		Log("��ȡ���������:(%u)%s",
			pConnection->GetLastDatabaseErrorCode(),
			pConnection->GetLastDatabaseErrorString());
		Log("�����SQL:%s", szSQL);
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
					Log("������ݽ�ѹʧ��%d", Error);
					return true;
				}
				
				Packet.Attach(m_CompressBuff.GetBuffer(), m_CompressBuff.GetBufferSize(), false);
			}
		}
		UINT64 PacketRoleID = Packet.GetMember(CBaseCharInfo::SST_MII_OBJECT_ID);
		if (RoleID != PacketRoleID)
		{
			//ID��һ�£���������
			Log("��ɫID[0x%llX]�����ݰ��ڽ�ɫID[0x%llX]��һ��", RoleID, PacketRoleID);
			if (Packet.IsMemberExist(CBaseCharInfo::SST_MII_OBJECT_ID))
			{
				Packet.GetMember(CBaseCharInfo::SST_MII_OBJECT_ID).SetValue(RoleID);
			}
			else
			{
				//��Ҫ���´��
				NeedRepack = true;
				Packet.AddMember(CBaseCharInfo::SST_MII_OBJECT_ID, RoleID);
			}
		}
		
		LPCTSTR PacketRoleName = Packet.GetMember(CBaseCharInfo::SST_MII_NAME);
		if (RoleName != PacketRoleName)
			NeedRepack = true;
		if(NeedRepack)
		{
			//�ǳƲ�һ�£���Ҫ���´��
			CRoleInfo RoleInfo;
			RoleInfo.ParsePacket(Packet, DOMF_ROLE_INFO_FULL);
			RoleInfo.SetObjectID(RoleID);
			RoleInfo.SetName(RoleName);
			RoleInfo.SetCreateTime(CreateTime);
			CSmartStruct* pPacket = m_RoleList.AddEmpty();
			pPacket->Create(RoleInfo.GetSmartStructSize(DOMF_PLAYER_INFO_FOR_SAVE));
			if (!RoleInfo.MakePacket(*pPacket, DOMF_PLAYER_INFO_FOR_SAVE))
			{
				Log("�������´��ʧ��[0x%llX]", RoleID);
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
