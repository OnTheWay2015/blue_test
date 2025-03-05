/****************************************************************************/
/*                                                                          */
/*      文件名:    SmartStruct.h                                            */
/*      创建日期:  2022年07月30日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "stdafx.h"

CSmartStruct CSmartArray::PrepareSubStruct()
{
	CSmartStruct Struct;
	BYTE ElementType = GetElementType();
	if ((ElementType == CSmartValue::VT_UNKNOWN) || (ElementType == CSmartValue::VT_STRUCT))
	{
		BYTE* pTail = m_pData + GetDataLen();
		UINT FreeSize = GetFreeLen();
		Struct.Attach(pTail, FreeSize, true);
	}
	return Struct;
}
CSmartStruct CSmartArray::GetNextStruct(void*& Pos) const
{
	CSmartStruct Value;

	if (m_pData == NULL)
	{
		Pos = NULL;
		return Value;
	}


	if (Pos)
	{
		BYTE* pHead = (BYTE*)Pos;
		BYTE* pTail = m_pData + GetDataLen();

		pHead += sizeof(WORD);

		if (pHead >= pTail)
		{
			Pos = NULL;
		}
		else
		{
			if (!Value.Attach(pHead, (UINT)(pTail - pHead), false))
			{
				Pos = NULL;
				return Value;
			}
			Pos = pHead + Value.GetDataLen();
			if (Pos >= pTail)
			{
				Pos = NULL;
			}
		}
	}
	return Value;
}
bool CSmartArray::AddMember(const CSmartStruct& Data)
{
	if (Data.GetData() == NULL)
		return false;

	UINT NeedSize = Data.GetDataLen();
	if (GetFreeLen() < NeedSize)
	{
		return false;
	}

	BYTE ElementType = GetElementType();
	if ((ElementType != CSmartValue::VT_STRUCT) && (ElementType != CSmartValue::VT_UNKNOWN))
		return false;

	BYTE* pFreeBuffer = m_pData + sizeof(BYTE) + sizeof(UINT) + GetLength();
	memcpy(pFreeBuffer, Data.GetData(), Data.GetDataLen());
	*((UINT*)(m_pData + sizeof(BYTE))) += NeedSize;
	return true;
}
bool CSmartArray::GetArray(CEasyArray<CSmartStruct>& Array) const
{
	if (GetLength())
	{
		BYTE ElementType = GetElementType();
		switch (ElementType)
		{
		case CSmartValue::VT_STRUCT:
		{
			const BYTE* pHead = (BYTE*)GetValueData();
			const BYTE* pTail = pHead + GetLength();
			Array.resize(GetArrayLength());
			UINT Index = 0;
			while ((pHead + sizeof(BYTE) <= pTail) && (pHead[0] == ElementType) && (Index < Array.size()))
			{
				Array[Index].Attach((BYTE*)pHead, (UINT)(pTail - pHead), false);
				Index++;
				pHead += Array[Index].GetDataLen();
			}
		}
		break;
		}
		return false;
	}
	else
	{
		return true;
	}
}
bool CSmartArray::AddArray(const CEasyArray<CSmartStruct>& Array)
{
	if (Array.size())
	{
		BYTE ElementType = GetElementType();
		if ((ElementType == CSmartValue::VT_UNKNOWN) ||
			(ElementType == CSmartValue::VT_STRUCT))
		{
			for (const CSmartStruct& Value : Array)
			{
				if (!AddMember(Value))
					return false;
			}
			return true;
		}
	}
	return false;
}
//void CSmartArray::Dump(CStringBuilder& OutBuffer, bool HaveType)
//{
//	OutBuffer.Append(_T("["));
//	bool IsFirst = true;
//	void* Pos = GetFirstMemberPosition();
//	while (Pos)
//	{
//		CSmartValue Member = GetNextMember(Pos);
//		if (IsFirst)
//		{
//			IsFirst = false;
//			OutBuffer.Append(_T(","));
//		}
//		Member.Dump(OutBuffer, HaveType);
//	}
//	OutBuffer.Append(_T("]"));
//}