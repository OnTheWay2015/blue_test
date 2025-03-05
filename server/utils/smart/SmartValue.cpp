/****************************************************************************/
/*                                                                          */
/*      文件名:    SmartValue.cpp                                           */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "stdafx.h"

#ifdef WIN32
#ifdef UNICODE
CEasyString::STRING_CODE_PAGE CSmartValue::INTERNAL_STRING_CODE_PAGE = CEasyString::STRING_CODE_PAGE_UCS16;
#else
CEasyString::STRING_CODE_PAGE CSmartValue::INTERNAL_STRING_CODE_PAGE = CEasyString::STRING_CODE_PAGE_ANSI;
#endif
#else
CEasyString::STRING_CODE_PAGE CSmartValue::INTERNAL_STRING_CODE_PAGE = CEasyString::STRING_CODE_PAGE_UTF8;
#endif

BYTE CSmartValue::NULL_DATA = VT_NULL;

int CSmartValue::GetTypeFromData(LPCVOID pData, UINT DataSize)
{
	if (pData == NULL || DataSize < sizeof(BYTE))
		return VT_UNKNOWN;

	return *((BYTE *)pData);
}
UINT CSmartValue::GetTypeLen(BYTE Type)
{
	switch (Type)
	{
	case VT_NULL:
		return sizeof(BYTE);
		break;
	case VT_CHAR:
	case VT_UCHAR:
		return sizeof(char) + sizeof(BYTE);
		break;
	case VT_SHORT:
	case VT_USHORT:
		return sizeof(short) + sizeof(BYTE);
		break;
	case VT_INT:
	case VT_UINT:
		return sizeof(int) + sizeof(BYTE);
		break;
	case VT_BIGINT:
	case VT_UBIGINT:
		return sizeof(__int64) + sizeof(BYTE);
		break;
	case VT_FLOAT:
		return sizeof(float) + sizeof(BYTE);
		break;
	case VT_DOUBLE:
		return sizeof(double) + sizeof(BYTE);
		break;
	case VT_STRING_ANSI:
	case VT_STRING_UTF8:
		return sizeof(BYTE) + sizeof(UINT) + sizeof(char);
		break;
	case VT_STRING_UCS16:
		return sizeof(BYTE) + sizeof(UINT) + sizeof(WCHAR);
		break;
	case VT_STRUCT:
	case VT_BINARY:
	case VT_ARRAY:
		return sizeof(BYTE) + sizeof(UINT);
		break;
	case VT_BOOL:
		return sizeof(BYTE) + sizeof(BYTE);
		break;
	}
	return 0;
}

//void CSmartValue::Dump(CStringBuilder& OutBuffer, bool HaveType)
//{
//	switch (GetType())
//	{
//	case VT_NULL:
//		OutBuffer.Append(_T("\"NULL\""));
//		break;
//	case VT_CHAR:
//		if (HaveType)
//			OutBuffer.AppendFormated(_T("(char)"));
//		OutBuffer.AppendFormated(_T("\"%d\""), (char)(*this));
//		break;
//	case VT_UCHAR:
//		if (HaveType)
//			OutBuffer.AppendFormated(_T("(byte)"));
//		OutBuffer.AppendFormated(_T("\"%u\""), (BYTE)(*this));
//		break;
//	case VT_SHORT:
//		if (HaveType)
//			OutBuffer.AppendFormated(_T("(short)"));
//		OutBuffer.AppendFormated(_T("\"%d\""), (short)(*this));
//		break;
//	case VT_USHORT:
//		if (HaveType)
//			OutBuffer.AppendFormated(_T("(ushort)"));
//		OutBuffer.AppendFormated(_T("\"%u\""), (WORD)(*this));
//		break;
//	case VT_INT:
//		if (HaveType)
//			OutBuffer.AppendFormated(_T("(int)"));
//		OutBuffer.AppendFormated(_T("\"%d\""), (int)(*this));
//		break;
//	case VT_UINT:
//		if (HaveType)
//			OutBuffer.AppendFormated(_T("(uint)"));
//		OutBuffer.AppendFormated(_T("\"%u\""), (UINT)(*this));
//		break;
//	case VT_BIGINT:
//		if (HaveType)
//			OutBuffer.AppendFormated(_T("(int64)"));
//		OutBuffer.AppendFormated(_T("\"%lld\""), (int)(*this));
//		break;
//	case VT_UBIGINT:
//		if (HaveType)
//			OutBuffer.AppendFormated(_T("(uint64)"));
//		OutBuffer.AppendFormated(_T("\"%llu\""), (UINT)(*this));
//		break;
//	case VT_FLOAT:
//		if (HaveType)
//			OutBuffer.AppendFormated(_T("(float)"));
//		OutBuffer.AppendFormated(_T("\"%f\""), (float)(*this));
//		break;
//	case VT_DOUBLE:
//		if (HaveType)
//			OutBuffer.AppendFormated(_T("(double)"));
//		OutBuffer.AppendFormated(_T("\"%lf\""), (double)(*this));
//		break;
//	case VT_STRING_ANSI:
//		if (HaveType)
//			OutBuffer.AppendFormated(_T("(ansi)"));
//		OutBuffer.Append(_T("\""));
//		OutBuffer.Append(GetString());
//		OutBuffer.Append(_T("\""));
//		break;
//	case VT_STRING_UTF8:
//		if (HaveType)
//			OutBuffer.AppendFormated(_T("(utf8)"));
//		OutBuffer.Append(_T("\""));
//		OutBuffer.Append(GetString());
//		OutBuffer.Append(_T("\""));
//		break;
//	case VT_STRING_UCS16:
//		if (HaveType)
//			OutBuffer.AppendFormated(_T("(ucs2)"));
//		OutBuffer.Append(_T("\""));
//		OutBuffer.Append(GetString());
//		OutBuffer.Append(_T("\""));
//		break;
//	case VT_STRUCT:
//		{
//			CSmartStruct Data(*this);
//			Data.Dump(OutBuffer, HaveType);
//		}
//		break;
//	case VT_BINARY:
//		if (HaveType)
//			OutBuffer.AppendFormated(_T("(bin)"));
//		OutBuffer.Append(_T("\""));
//		OutBuffer.Append(BinToString((const BYTE*)(*this), GetLength()));
//		OutBuffer.Append(_T("\""));
//		break;
//	case VT_ARRAY:
//		{
//			CSmartArray Data(*this);
//			Data.Dump(OutBuffer, HaveType);
//		}
//		break;
//	case VT_BOOL:
//		OutBuffer.AppendFormated((bool)(*this) ? _T("\"true\"") : _T("\"false\""));
//		break;
//	}
//}