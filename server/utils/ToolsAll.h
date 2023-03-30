/****************************************************************************/
/*                                                                          */
/*      文件名:    ToolsAll.h                                               */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


#define IS_TYPE_OF(pObject,Type)	((dynamic_cast<Type *>(pObject))!=NULL)

enum BOM_HEADER_TYPE
{
	BMT_UNICODE = 0xFEFF,
	BMT_UNICODE_BIG_ENDIAN = 0xFFFE,
	BMT_UTF_8 = 0xBFBBEF,
};

//inline size_t strnlen(const char * pStr,size_t BufferLen)
//{
//	size_t Len=0;
//	while(*pStr&&BufferLen)
//	{
//		pStr++;
//		Len++;
//		BufferLen--;
//	}
//	return Len;
//}

template<typename T>
inline T Max(T a, T b)
{
	return a > b ? a : b;
}

template<typename T>
inline T Min(T a, T b)
{
	return a < b ? a : b;
}

inline void Swap(FLOAT& v1,FLOAT& v2)
{
	FLOAT temp=v1;
	v1=v2;
	v2=temp;
}


template<typename T>
inline T Saturate(T Value, T Min, T Max)
{
	if (Value<Min)
		return Min;
	if (Value>Max)
		return Max;
	return Value;
}

template<typename T>
inline T Saturate(T Value, T Min = 0)
{
	if (Value < Min)
		return Min;
	return Value;
}


inline int GetRand(int Min,int Max)
{
	float RandValue = (float)rand();
	if (Min > Max)
		return (int)round(RandValue *((float)Min - (float)Max) / (float)RAND_MAX + Max);
	else
		return (int)round(RandValue *((float)Max - (float)Min) / (float)RAND_MAX + Min);
}

inline UINT GetRand(UINT Min, UINT Max)
{
	float RandValue = (float)rand();
	if (Min > Max)
		return (UINT)round(RandValue *((float)Min - (float)Max) / (float)RAND_MAX + Max);
	else
		return (UINT)round(RandValue *((float)Max - (float)Min) / (float)RAND_MAX + Min);
}

inline float GetRand(float Min,float Max)
{
	return (((float)rand())/RAND_MAX)*(Max-Min)+Min;
}

inline float GetRandGaussf(float Min,float Max)
{
	float sigma=(Max-Min)/2.0f;
	float mu=(Max+Min)/2.0f;
	return Saturate((float)(mu + ((rand() % 2) ? -1.0 : 1.0)*sigma*pow(-log(0.99999f*((double)rand() / RAND_MAX)), 0.5)),Min,Max);
}

inline CEasyString FormatSplitNumber(UINT Number)
{
	CEasyString Str;

	Str.Format(_T("%u"), Number);
	int i = (int)Str.GetLength() - 3;
	while (i > 0)
	{
		Str.Insert(i, ',');
		i -= 3;
	}
	return Str;
}

inline CEasyString FormatSplitNumber(int Number)
{
	CEasyString Str;

	Str.Format(_T("%d"), Number);
	int i = (int)Str.GetLength() - 3;
	while (i > 0)
	{
		Str.Insert(i, ',');
		i -= 3;
	}
	return Str;
}
inline CEasyString FormatSplitNumber(UINT64 Number)
{
	CEasyString Str;

	Str.Format(_T("%llu"),Number);
	int i=(int)Str.GetLength()-3;
	while(i>0)
	{
		Str.Insert(i,',');
		i-=3;
	}
	return Str;
}

inline CEasyString FormatSplitNumber(INT64 Number)
{
	CEasyString Str;

	Str.Format(_T("%lld"), Number);
	int i = (int)Str.GetLength() - 3;
	while (i > 0)
	{
		Str.Insert(i, ',');
		i -= 3;
	}
	return Str;
}

inline CEasyString FormatNumberWords(UINT64 Number,bool IsTiny=false)
{
	CEasyString temp;
	

	if(Number==0) return _T("0");
	if(Number<=1024)
	{
		temp.Format(_T("%llu"),Number);
	}
	else if(Number<=1048576)
	{
		float size=Number/1024.0f;
		temp.Format(_T("%.2fK"),size);
	}
	else if (Number <= 1073741824)
	{
		float size = Number / 1048576.0f;
		temp.Format(_T("%.2fM"), size);
	}
	else
	{
		float size = Number / 1073741824.0f;
		temp.Format(_T("%.2fG"), size);
	}
	
	if(IsTiny)
		return temp;
	else
		return temp+_T(" Byte");
}

inline CEasyString FormatNumberWordsFloat(float Number,bool IsTiny=false)
{
	CEasyString temp;


	if(Number==0.0f)
	{
		temp=_T("0");
	}
	else if(Number<=1024.0f)
	{
		temp.Format(_T("%.2f"),Number);
	}else if(Number<=1048576.0f)
	{
		float size=Number/1024.0f;
		temp.Format(_T("%.2fK"),size);
	}else
	{
		float size=Number/1048576.0f;
		temp.Format(_T("%.2fM"),size);
	}
	if(IsTiny)
		return temp;
	else
		return temp+_T(" Byte");
}

inline CEasyString FormatTimeStr(UINT Time)
{
	CEasyString TimeStr;

	TimeStr.Format(_T("%u:%02u:%02u"), Time / 3600, (Time % 3600) / 60, Time % 60);

	return TimeStr;
}

inline UINT GetBomHeader(LPVOID pData, UINT DataLen)
{
	BYTE * pByte=(BYTE *)pData;
	if (DataLen >= 2 && pByte[0] == 0xFF && pByte[1] == 0xFE)
		return BMT_UNICODE;
	if (DataLen >= 2 && pByte[0] == 0xFE && pByte[1] == 0xFF)
		return BMT_UNICODE_BIG_ENDIAN;
	if (DataLen >= 3 && pByte[0] == 0xEF && pByte[1] == 0xBB && pByte[2] == 0xBF)
		return BMT_UTF_8;
	return 0;
}


inline CEasyString BinToString(const BYTE * pData, size_t Len, bool IsLowCase = true)
{
	CEasyString BinString,temp;

	for (size_t i = 0; i < Len; i++)
	{
		if (IsLowCase)
			temp.Format(_T("%02x"), pData[i]);
		else
			temp.Format(_T("%02X"), pData[i]);
		BinString+=temp;
	}
	return BinString;
}

inline bool StringToBin(LPCTSTR szStr, size_t StrLen, BYTE * pData, size_t BuffLen)
{
	TCHAR ConterBuff[3];
	ConterBuff[2] = 0;

	if (StrLen == 0)
		StrLen = _tcslen(szStr);
	size_t OutPtr = 0;
	while (StrLen >= 2 && OutPtr < BuffLen)
	{
		ConterBuff[0] = szStr[0];
		ConterBuff[1] = szStr[1];
		szStr += 2;
		StrLen -= 2;
		pData[OutPtr] = (BYTE)_tcstol(ConterBuff, NULL, 16);
		OutPtr++;
	}
	return true;
}

inline bool IsHostBigEndian()
{
	short Temp = 1;
	return ((BYTE *)&Temp)[0] == 0;
}
inline UINT64 __htonll(UINT64 Value)
{
	if (IsHostBigEndian())
	{
		return Value;
	}
	else
	{
		UINT64 Temp;
		BYTE * pInput = (BYTE *)&Value;
		BYTE * pOutput = (BYTE *)&Temp;
		for (int i = 0; i < 8; i++)
		{
			pOutput[i] = pInput[7 - i];
		}
		return Temp;
	}
}

inline UINT __htonf(float Value)
{
	if (IsHostBigEndian())
	{		
		return *((UINT *)&Value);
	}
	else
	{
		UINT Temp;
		BYTE * pInput = (BYTE *)&Value;
		BYTE * pOutput = (BYTE *)&Temp;
		for (int i = 0; i < 4; i++)
		{
			pOutput[i] = pInput[3 - i];
		}
		return Temp;
	}
}

inline UINT64 __htond(double Value)
{
	if (IsHostBigEndian())
	{
		return *((UINT64 *)&Value);
	}
	else
	{
		UINT64 Temp;
		BYTE * pInput = (BYTE *)&Value;
		BYTE * pOutput = (BYTE *)&Temp;
		for (int i = 0; i < 8; i++)
		{
			pOutput[i] = pInput[7 - i];
		}
		return Temp;
	}
}

inline UINT64 __ntohll(UINT64 Value)
{
	if (IsHostBigEndian())
	{
		return Value;
	}
	else
	{
		UINT64 Temp;
		BYTE * pInput = (BYTE *)&Value;
		BYTE * pOutput = (BYTE *)&Temp;
		for (int i = 0; i < 8; i++)
		{
			pOutput[i] = pInput[7 - i];
		}
		return Temp;
	}
}

inline float __ntohf(UINT Value)
{
	if (IsHostBigEndian())
	{
		return *((float *)&Value);
	}
	else
	{
		float Temp;
		BYTE * pInput = (BYTE *)&Value;
		BYTE * pOutput = (BYTE *)&Temp;
		for (int i = 0; i < 4; i++)
		{
			pOutput[i] = pInput[3 - i];
		}
		return Temp;
	}
}

inline double __ntohd(UINT64 Value)
{
	if (IsHostBigEndian())
	{
		return *((double *)&Value);
	}
	else
	{
		double Temp;
		BYTE * pInput = (BYTE *)&Value;
		BYTE * pOutput = (BYTE *)&Temp;
		for (int i = 0; i < 8; i++)
		{
			pOutput[i] = pInput[7 - i];
		}
		return Temp;
	}
}



inline CEasyString CombineString(CEasyArray<CEasyString>& StrList, TCHAR Separator)
{
	CEasyString ResultStr;
	UINT Len = 0;
	for (UINT i = 0; i < StrList.size(); i++)
	{
		Len += (UINT)StrList[i].GetLength();
	}
	if (Len)
	{
		Len += (UINT)StrList.size() - 1;
		ResultStr.Resize(Len);
		ResultStr.SetLength(Len);
		UINT BuffPtr = 0;
		TCHAR * pBuff = (TCHAR *)ResultStr;
		for (UINT i = 0; i < StrList.size(); i++)
		{
			_tcscpy_s(pBuff + BuffPtr, Len - BuffPtr + 1, StrList[i]);
			BuffPtr += (UINT)StrList[i].GetLength();
			if (i < StrList.size() - 1)
			{
				pBuff[BuffPtr] = Separator;
				BuffPtr++;
			}
		}
	}
	return ResultStr;
}

inline CEasyString UTF8ToLocal(LPCSTR szStr, size_t StrLen = 0)
{
	if (StrLen == 0)
		StrLen = strlen(szStr);
	CEasyString OutStr;
#ifdef UNICODE
	size_t OutLen = UTF8ToUnicode(szStr, StrLen, NULL, 0);
	OutStr.Resize(OutLen);
	UTF8ToUnicode(szStr, StrLen, (TCHAR *)OutStr.GetBuffer(), OutLen);
#else
	size_t OutLen = UTF8ToAnsi(szStr, StrLen, NULL, 0);
	OutStr.Resize(OutLen);
	UTF8ToAnsi(szStr, StrLen, (TCHAR *)OutStr.GetBuffer(), OutLen);
#endif
	OutStr.TrimBuffer();
	return OutStr;
}

inline CEasyStringA LocalToUTF8(LPCTSTR szStr, size_t StrLen = 0)
{
	if (StrLen == 0)
		StrLen = _tcslen(szStr);
	CEasyStringA OutStr;
#ifdef UNICODE
	size_t OutLen = UnicodeToUTF8(szStr, StrLen, NULL, 0);
	OutStr.Resize(OutLen);
	UnicodeToUTF8(szStr, StrLen, (char *)OutStr.GetBuffer(), OutLen);
#else
	size_t OutLen = AnsiToUTF8(szStr, StrLen, NULL, 0);
	OutStr.Resize(OutLen);
	AnsiToUTF8(szStr, StrLen, (char *)OutStr.GetBuffer(), OutLen);
#endif
	OutStr.TrimBuffer();
	return OutStr;
}

inline CEasyStringW AnsiToUnicode(const char* SrcStr, size_t SrcLen = 0)
{
	CEasyStringW DestStr;
	if (SrcLen == 0)
		SrcLen = CEasyString::GetStrLen(SrcStr);
	size_t DestLen = AnsiToUnicode(SrcStr, SrcLen, NULL, 0);
	if (DestLen)
	{
		DestStr.Resize(DestLen, false);
		AnsiToUnicode(SrcStr, SrcLen, DestStr.GetBuffer(), DestLen);
		DestStr.TrimBuffer(DestLen);
	}
	return DestStr;
}

inline CEasyStringA UnicodeToAnsi(const WCHAR* SrcStr, size_t SrcLen = 0)
{
	CEasyStringA DestStr;
	if (SrcLen == 0)
		SrcLen = CEasyString::GetStrLen(SrcStr);
	size_t DestLen = UnicodeToAnsi(SrcStr, SrcLen, NULL, 0);
	if (DestLen)
	{
		DestStr.Resize(DestLen, false);
		UnicodeToAnsi(SrcStr, SrcLen, DestStr.GetBuffer(), DestLen);
		DestStr.TrimBuffer(DestLen);
	}
	return DestStr;
}

inline CEasyStringW UTF8ToUnicode(const char* SrcStr, int SrcLen = -1)
{
	CEasyStringW DestStr;
	if (SrcLen < 0)
		SrcLen = CEasyString::GetStrLen(SrcStr);
	size_t DestLen = UTF8ToUnicode(SrcStr, SrcLen, NULL, 0);
	if (DestLen)
	{
		DestStr.Resize(DestLen, false);
		UTF8ToUnicode(SrcStr, SrcLen, DestStr.GetBuffer(), DestLen);
		DestStr.TrimBuffer(DestLen);
	}
	return DestStr;
}

inline CEasyStringA UnicodeToUTF8(const WCHAR* SrcStr, int SrcLen = -1)
{
	CEasyStringA DestStr;
	if (SrcLen < 0)
		SrcLen = CEasyString::GetStrLen(SrcStr);
	size_t DestLen = UnicodeToUTF8(SrcStr, SrcLen, NULL, 0);
	if (DestLen)
	{
		DestStr.Resize(DestLen, false);
		UnicodeToUTF8(SrcStr, SrcLen, DestStr.GetBuffer(), DestLen);
		DestStr.TrimBuffer(DestLen);
	}
	return DestStr;
}
#ifdef _WIN32
inline CEasyStringA AnsiToUTF8(const char* SrcStr, int SrcLen = -1)
{
	CEasyStringW UnicodeStr = AnsiToUnicode(SrcStr, SrcLen);
	return UnicodeToUTF8(UnicodeStr, (int)UnicodeStr.GetLength());
}

inline CEasyStringA UTF8ToAnsi(const char* SrcStr, int SrcLen = -10)
{
	CEasyStringW UnicodeStr = UTF8ToUnicode(SrcStr, SrcLen);
	return UnicodeToAnsi(UnicodeStr, (int)UnicodeStr.GetLength());
}
#else
inline CEasyStringA AnsiToUTF8(const char* SrcStr, int SrcLen = -1)
{
	CEasyStringA DestStr;
	if (SrcLen < 0)
		SrcLen = CEasyString::GetStrLen(SrcStr);
	size_t DestLen = AnsiToUTF8(SrcStr, SrcLen, NULL, 0);
	if (DestLen)
	{
		DestStr.Resize(DestLen, false);
		AnsiToUTF8(SrcStr, SrcLen, DestStr.GetBuffer(), DestLen);
		DestStr.TrimBuffer(DestLen);
	}
	return DestStr;
}

inline CEasyStringA UTF8ToAnsi(const char* SrcStr, int SrcLen = -1)
{
	CEasyStringA DestStr;
	if (SrcLen < 0)
		SrcLen = CEasyString::GetStrLen(SrcStr);
	size_t DestLen = UTF8ToAnsi(SrcStr, SrcLen, NULL, 0);
	if (DestLen)
	{
		DestStr.Resize(DestLen, false);
		UTF8ToAnsi(SrcStr, SrcLen, DestStr.GetBuffer(), DestLen);
		DestStr.TrimBuffer(DestLen);
	}
	return DestStr;
}
#endif
extern UINT WalkCallStack(void * pFrame, LPVOID * AddressList, UINT MaxStep);
