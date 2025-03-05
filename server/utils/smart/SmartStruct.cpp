/****************************************************************************/
/*                                                                          */
/*      文件名:    SmartStruct.cpp                                          */
/*      创建日期:  2009年09月11日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "stdafx.h"






//void CSmartStruct::Dump(CStringBuilder& OutBuffer, bool HaveType)
//{
//	OutBuffer.Append(_T("{"));
//	bool IsFirst = true;
//	void* Pos = GetFirstMemberPosition();
//	while (Pos)
//	{
//		WORD MemberID = 0;
//		CSmartValue Member = GetNextMember(Pos, MemberID);
//		if (IsFirst)
//		{
//			IsFirst = false;
//			OutBuffer.Append(_T(","));
//		}
//		OutBuffer.Append(_T("\"%u\":"), MemberID);
//		Member.Dump(OutBuffer, HaveType);
//	}
//	OutBuffer.Append(_T("}"));
//}