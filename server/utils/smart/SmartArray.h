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
#pragma once

class CSmartStruct;

class CSmartArray
{
protected:
	BYTE*			m_pData;
	UINT			m_DataLen;
	bool			m_IsSelfData;
	LPCTSTR			m_Tag;
public:	
	enum
	{
		HEAD_SIZE = (sizeof(BYTE) + sizeof(UINT)),
	};
	class iterator
	{
	protected:
		CSmartArray* m_pArray;
		void*		 m_Pos;
	public:
		iterator()
		{
			m_pArray = NULL;
			m_Pos = NULL;
		}
		iterator(CSmartArray* pArray, void* Pos)
		{
			m_pArray = pArray;
			m_Pos = Pos;			
		}
		iterator(const iterator& itr)
		{
			m_pArray = itr.m_pArray;
			m_Pos = itr.m_Pos;
		}
		CSmartValue operator*()
		{
			return m_pArray->GetMemberByPos(m_Pos);
		}
		iterator& operator++()
		{
			m_pArray->GetNextMember(m_Pos);
			return *this;
		}
		iterator& operator=(const iterator& itr)
		{
			m_pArray = itr.m_pArray;
			m_Pos = itr.m_Pos;
			return *this;
		}
		bool operator!=(const iterator& itr)
		{
			return (m_pArray != itr.m_pArray) || (m_Pos != itr.m_Pos);
		}
	};
	CSmartArray(LPCTSTR Tag = _T("CSmartArray"))
	{
		m_pData = NULL;
		m_DataLen = 0;
		m_IsSelfData = true;
		m_Tag = Tag;
	}
	CSmartArray(int BufferLen, LPCTSTR Tag = _T("CSmartArray"))
	{
		m_pData = NULL;
		m_DataLen = 0;
		m_IsSelfData = true;
		m_Tag = Tag;
		Create(BufferLen);
	}
	CSmartArray(LPVOID pData, UINT DataLen, bool IsEmpty, LPCTSTR Tag = _T("CSmartArray"))
	{
		m_pData = NULL;
		m_DataLen = 0;
		m_IsSelfData = true;
		m_Tag = Tag;
		Attach(pData, DataLen, IsEmpty);
	}
	CSmartArray(const CSmartArray& Array, LPCTSTR Tag = _T("CSmartArray"))
	{
		m_pData = NULL;
		m_DataLen = 0;
		m_IsSelfData = true;
		m_Tag = Tag;
		Attach((void*)Array.GetData(), Array.GetBufferLen(), false);
	}
	CSmartArray(const CSmartValue& Value, LPCTSTR Tag = _T("CSmartArray"))
	{
		m_pData = NULL;
		m_DataLen = 0;
		m_IsSelfData = true;
		m_Tag = Tag;
		Attach((void*)Value.GetData(), Value.GetDataLen(), false);
	}
	LPCTSTR GetTag()
	{
		return m_Tag;
	}
	void SetTag(LPCTSTR Tag)
	{
		m_Tag = Tag;
	}
	bool Create(UINT BufferLen)
	{
		Destory();
		m_DataLen = sizeof(BYTE) + sizeof(UINT) + BufferLen;
		m_pData = MONITORED_NEW_ARRAY(GetTag(), BYTE, m_DataLen);
		ZeroMemory(m_pData, m_DataLen);
		*m_pData = CSmartValue::VT_ARRAY;
		*((UINT*)(m_pData + sizeof(BYTE))) = 0;
		m_IsSelfData = true;
		return true;
	}
	bool Expand(UINT ExpandSize)
	{
		UINT NewDataLen = 0;
		if (m_DataLen == 0)
		{
			NewDataLen = sizeof(BYTE) + sizeof(UINT) + ExpandSize;
		}
		else
		{
			NewDataLen += ExpandSize;
		}

		BYTE* pNewData = MONITORED_NEW_ARRAY(GetTag(), BYTE, m_DataLen);
		if (m_DataLen == 0)
		{
			ZeroMemory(m_pData, m_DataLen);
			*m_pData = CSmartValue::VT_ARRAY;
			*((UINT*)(m_pData + sizeof(BYTE))) = 0;
		}
		else
		{
			memcpy(pNewData, m_pData, m_DataLen);
		}
		SAFE_DELETE_ARRAY(m_pData);
		m_pData = pNewData;
		m_DataLen = NewDataLen;
		m_IsSelfData = true;
		return true;
	}
	bool Attach(LPVOID pData, UINT DataLen, bool IsEmpty)
	{
		if ((pData == NULL) || (DataLen < sizeof(BYTE) + sizeof(UINT)))
			return false;
		Destory();
		m_IsSelfData = false;
		m_pData = (BYTE*)pData;
		if (IsEmpty)
		{
			m_pData[0] = CSmartValue::VT_ARRAY;
			Clear();
		}
		else
		{
			if (m_pData[0] != CSmartValue::VT_ARRAY)
			{
				Destory();
				return false;
			}
		}
		m_DataLen = DataLen;
		return true;
	}

	bool CloneFrom(const CSmartArray& Value)
	{
		Destory();
		if (Value.GetData() && Value.m_DataLen)
		{
			m_IsSelfData = true;
			m_DataLen = Value.GetDataLen();
			m_pData = MONITORED_NEW_ARRAY(GetTag(), BYTE, m_DataLen);
			memcpy(m_pData, Value.GetData(), Value.GetDataLen());
		}
		return true;
	}

	virtual ~CSmartArray(void)
	{
		Destory();
	}


	void Destory()
	{
		if (m_IsSelfData)
		{
			SAFE_DELETE_ARRAY(m_pData);
		}
		else
		{
			m_pData = NULL;
		}
	}

	void Clear()
	{
		if (m_pData)
		{
			*((UINT*)(m_pData + sizeof(BYTE))) = 0;
		}			
	}

	operator CSmartValue() const
	{
		return CSmartValue((void*)GetData(), GetDataLen());
	}

	void operator=(const CSmartArray& Struct)
	{
		Attach((void*)Struct.GetData(), Struct.GetBufferLen(), false);
	}

	UINT GetArrayLength() const
	{
		if ((m_pData == NULL) || (m_DataLen < sizeof(BYTE) + sizeof(UINT)))
			return 0;
		const BYTE* pArrayData = (const BYTE*)m_pData;
		pArrayData += sizeof(BYTE);
		UINT ArrayDataLen = *((UINT*)pArrayData);
		pArrayData += sizeof(UINT);		
		if (ArrayDataLen + sizeof(BYTE) + sizeof(UINT)> m_DataLen)
			ArrayDataLen = m_DataLen - sizeof(BYTE) - sizeof(UINT);
		if(ArrayDataLen)
		{
			BYTE ElementType = *pArrayData;
			switch (ElementType)
			{
			case CSmartValue::VT_CHAR:
			case CSmartValue::VT_UCHAR:
			case CSmartValue::VT_SHORT:
			case CSmartValue::VT_USHORT:
			case CSmartValue::VT_INT:
			case CSmartValue::VT_UINT:
			case CSmartValue::VT_BIGINT:
			case CSmartValue::VT_UBIGINT:
			case CSmartValue::VT_FLOAT:
			case CSmartValue::VT_DOUBLE:
			case CSmartValue::VT_BOOL:
				{
					//定长元素
					CSmartValue Value;
					if (Value.Attach((void*)pArrayData, ArrayDataLen, CSmartValue::VT_UNKNOWN))
					{
						return ArrayDataLen / Value.GetDataLen();
					}
				}
				break;
			case CSmartValue::VT_STRING_UTF8:			
			case CSmartValue::VT_STRING_ANSI:				
			case CSmartValue::VT_STRING_UCS16:				
			case CSmartValue::VT_BINARY:				
			case CSmartValue::VT_STRUCT:
			case CSmartValue::VT_ARRAY:
				{
					//变长元素
					CSmartValue Value;
					UINT Count = 0;
					const BYTE* pHead = pArrayData;
					const BYTE* pTail = pArrayData + ArrayDataLen;

					while ((pHead + sizeof(BYTE) < pTail) && (pHead[0] == ElementType))
					{					
						if (!Value.Attach((BYTE *)pHead, (UINT)(pTail - pHead), CSmartValue::VT_UNKNOWN))
							break;
						Count++;
						pHead += Value.GetDataLen();
					}
					return Count;
				}
				break;			
			}
		}
		return  0;
	}	
	
	UINT GetLength() const
	{
		if (m_pData == NULL)
			return 0;
		return *((UINT*)(m_pData + sizeof(BYTE)));
	}
	UINT GetDataLen() const
	{
		if (m_pData == NULL)
			return 0;
		UINT DataLen = GetLength() + sizeof(BYTE) + sizeof(UINT);
		if (DataLen > m_DataLen)
			DataLen = m_DataLen;
		return DataLen;
	}
	UINT GetBufferLen() const
	{
		return m_DataLen;
	}
	LPVOID GetData()
	{
		return m_pData;
	}
	LPCVOID GetData() const
	{
		return m_pData;
	}
	LPVOID GetValueData()
	{
		return m_pData + sizeof(BYTE) + sizeof(UINT);
	}
	LPCVOID GetValueData() const
	{
		return m_pData + sizeof(BYTE) + sizeof(UINT);
	}
	BYTE GetElementType() const
	{
		if (GetLength())
		{
			return *(m_pData + sizeof(BYTE) + sizeof(UINT));
		}
		return CSmartValue::VT_UNKNOWN;
	}
	UINT GetElementSize() const
	{
		BYTE ElementType = GetElementType();
		switch (ElementType)
		{
		case CSmartValue::VT_CHAR:
		case CSmartValue::VT_UCHAR:
			return sizeof(BYTE) + sizeof(char);
		case CSmartValue::VT_SHORT:
		case CSmartValue::VT_USHORT:
			return sizeof(BYTE) + sizeof(short);
		case CSmartValue::VT_INT:
		case CSmartValue::VT_UINT:
			return sizeof(BYTE) + sizeof(int);
		case CSmartValue::VT_BIGINT:
		case CSmartValue::VT_UBIGINT:
			return sizeof(BYTE) + sizeof(__int64);
		case CSmartValue::VT_FLOAT:
			return sizeof(BYTE) + sizeof(float);
		case CSmartValue::VT_DOUBLE:
			return sizeof(BYTE) + sizeof(double);
		case CSmartValue::VT_BOOL:
			return sizeof(BYTE) + sizeof(BYTE);
		}
		return 0;
	}
	CSmartValue GetMember(UINT Index) const
	{
		BYTE ElementType = GetElementType();
		UINT ElementSize = GetElementSize();
		if (ElementSize)
		{
			//定长元素
			CSmartValue Value;

			UINT ArrayLen = GetLength() / ElementSize;
			if (Index < ArrayLen)
			{
				Value.Attach((BYTE*)GetValueData() + ElementSize * Index, ElementSize, CSmartValue::VT_UNKNOWN);
				if (Value.GetType() == ElementType)
					return Value;
			}
		}
		else
		{
			//变长元素
			CSmartValue Value;
			const BYTE* pHead = (BYTE*)GetValueData();
			const BYTE* pTail = pHead + GetLength();

			while ((pHead + sizeof(BYTE) <= pTail) && (pHead[0] == ElementType))
			{
				if (!Value.Attach((BYTE*)pHead, (UINT)(pTail - pHead), CSmartValue::VT_UNKNOWN))
					break;
				if (Index == 0)
					return Value;
				Index--;
				pHead += Value.GetDataLen();
			}
		}
		return CSmartValue();
	}
	CSmartValue operator[](UINT Index) const
	{
		return GetMember(Index);
	}
	CSmartValue GetMemberByPos(void* Pos) const
	{
		CSmartValue Value;
		if (Pos)
		{
			BYTE* pHead = (BYTE*)Pos;
			BYTE* pTail = m_pData + GetDataLen();
			if (pHead < pTail)
			{
				Value.Attach(pHead, (UINT)(pTail - pHead), CSmartValue::VT_UNKNOWN);
			}
		}
		return Value;
	}
	LPVOID GetFirstMemberPosition() const
	{
		if (m_pData == NULL)
			return NULL;


		BYTE* pHead = (BYTE*)GetValueData();
		BYTE* pTail = pHead + GetLength();

		if (pHead < pTail)
			return pHead;
		else
			return NULL;
	}

	CSmartValue GetNextMember(void*& Pos) const
	{
		CSmartValue Value;

		if (m_pData == NULL)
		{
			Pos = NULL;
			return Value;
		}


		if (Pos)
		{
			BYTE* pHead = (BYTE*)Pos;
			BYTE* pTail = m_pData + GetDataLen();			

			if (pHead >= pTail)
			{
				Pos = NULL;
			}
			else
			{
				if (!Value.Attach(pHead, (UINT)(pTail - pHead), CSmartValue::VT_UNKNOWN))
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
	CSmartStruct GetNextStruct(void*& Pos) const;
	CSmartArray GetNextArray(void*& Pos) const
	{
		CSmartArray Value;

		if (m_pData == NULL)
		{
			Pos = NULL;
			return Value;
		}


		if (Pos)
		{
			BYTE* pHead = (BYTE*)Pos;
			BYTE* pTail = m_pData + GetDataLen();

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
	template<typename T>
	bool GetArray(CEasyArray<T>& Array) const
	{
		if (GetLength())
		{
			BYTE ElementType = GetElementType();
			UINT ElementSize = GetElementSize();
			if (ElementSize)
			{

				//定长元素
				CSmartValue Value;

				UINT ArrayLen = GetLength() / ElementSize;
				Array.resize(ArrayLen);
				for (UINT i = 0; i < ArrayLen; i++)
				{
					Value.Attach((BYTE*)GetValueData() + ElementSize * i, ElementSize, CSmartValue::VT_UNKNOWN);
					Array[i] = Value;
				}
				return true;
			}

			return false;
		}
		else
		{
			return true;
		}
	}	
	bool GetArray(CEasyArray<CEasyString>& Array) const
	{
		if (GetLength())
		{
			BYTE ElementType = GetElementType();			
			switch (ElementType)
			{
			case CSmartValue::VT_STRING_UTF8:
			case CSmartValue::VT_STRING_UCS16:					
			case CSmartValue::VT_STRING_ANSI:
				{
					CSmartValue Value;
					const BYTE* pHead = (BYTE*)GetValueData();
					const BYTE* pTail = pHead + GetLength();
					Array.resize(GetArrayLength());
					UINT Index = 0;
					while ((pHead + sizeof(BYTE) <= pTail) && (pHead[0] == ElementType) && (Index < Array.size()))
					{
						Value.Attach((BYTE*)pHead, (UINT)(pTail - pHead), CSmartValue::VT_UNKNOWN);
						Value.GetString(Array[Index]);
						Index++;
						pHead += Value.GetDataLen();
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
	bool GetArray(CEasyArray<CEasyBuffer>& Array) const
	{
		if (GetLength())
		{
			BYTE ElementType = GetElementType();
			switch (ElementType)
			{
			case CSmartValue::VT_STRING_UTF8:
			case CSmartValue::VT_STRING_UCS16:
			case CSmartValue::VT_STRING_ANSI:
			case CSmartValue::VT_BINARY:			
				{
					CSmartValue Value;
					const BYTE* pHead = (BYTE*)GetValueData();
					const BYTE* pTail = pHead + GetLength();
					Array.resize(GetArrayLength());
					UINT Index = 0;
					while ((pHead + sizeof(BYTE) <= pTail) && (pHead[0] == ElementType) && (Index < Array.size()))
					{
						Value.Attach((BYTE*)pHead, (UINT)(pTail - pHead), CSmartValue::VT_UNKNOWN);
						Array[Index].Create(Value.GetLength());
						Array[Index].PushBack(Value.GetValueData(), Value.GetLength());
						Index++;
						pHead += Value.GetDataLen();
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
	bool GetArray(CEasyArray<CSmartStruct>& Array) const;

	template<typename T>
	bool AddArray(const CEasyArray<T>& Array)
	{
		if (Array.size())
		{
			BYTE ElementType = GetElementType();
			UINT ElementSize = GetElementSize();
			BYTE ArrayType = CSmartValue::GetTypeByValue(Array[0]);
			
			if (ElementType == CSmartValue::VT_UNKNOWN)
			{
				ElementType = ArrayType;
				ElementSize = sizeof(BYTE) + sizeof(Array[0]);
			}
			
			UINT NeedSize = ElementSize * Array.size();
			if (GetFreeLen() >= NeedSize)
			{
				BYTE* pFreeBuffer = m_pData + sizeof(BYTE) + sizeof(UINT) + GetLength();
				CSmartValue Element;
				for (const T& Value : Array)
				{
					if (Element.Attach(pFreeBuffer, ElementSize, ElementType))
						Element.SetValue(Value);
					else
						return false;
					pFreeBuffer += ElementSize;
				}
				return FinishMember(NeedSize);
			}
		}
		return false;
	}
	bool AddArray(const CEasyArray<CEasyString>& Array)
	{
		if (Array.size())
		{
			BYTE ElementType = GetElementType();
			if ((ElementType == CSmartValue::VT_UNKNOWN) ||
				(ElementType == CSmartValue::VT_STRING_UTF8) ||
				(ElementType == CSmartValue::VT_STRING_UCS16) ||
				(ElementType == CSmartValue::VT_STRING_ANSI))
			{
				for (const CEasyString& Value : Array)
				{
					if (!AddMember(Value))
						return false;
				}
				return true;
			}
		}
		return false;
	}
	bool AddArray(const CEasyArray<CEasyBuffer>& Array)
	{
		if (Array.size())
		{
			BYTE ElementType = GetElementType();
			if ((ElementType == CSmartValue::VT_UNKNOWN) ||
				(ElementType == CSmartValue::VT_BINARY))
			{
				for (const CEasyBuffer& Value : Array)
				{
					if (!AddMember(Value))
						return false;
				}
				return true;
			}
		}
		return false;
	}
	bool AddArray(const CEasyArray<CSmartStruct>& Array);
	iterator begin()
	{
		return iterator(this, GetFirstMemberPosition());
	}

	iterator end()
	{
		return iterator(this, NULL);
	}
	static inline UINT GetDataLenFromData(LPCVOID pData, UINT DataSize)
	{
		if (pData == NULL || DataSize < HEAD_SIZE)
			return 0;
		return *((UINT*)((BYTE*)pData + sizeof(BYTE))) + HEAD_SIZE;
	}
	UINT GetFreeLen()
	{
		return m_DataLen - GetDataLen();
	}
	CSmartStruct PrepareSubStruct();

	CSmartArray PrepareArrayMember()
	{
		CSmartArray Array;
		BYTE ElementType = GetElementType();
		if ((ElementType == CSmartValue::VT_UNKNOWN) || (ElementType == CSmartValue::VT_ARRAY))
		{
			BYTE* pTail = m_pData + GetDataLen();
			UINT FreeSize = GetFreeLen();
			Array.Attach(pTail, FreeSize, true);
		}
		return Array;
	}
	bool FinishMember(UINT MemberSize)
	{
		if (m_pData == NULL)
			return false;
		UINT NeedSize = MemberSize;
		if (GetFreeLen() < NeedSize)
		{
			return false;
		}
		BYTE* pFreeBuffer = m_pData + sizeof(BYTE) + sizeof(UINT) + GetLength();
		*((UINT*)(m_pData + sizeof(BYTE))) += NeedSize;
		return true;
	}
	template<typename T>
	bool AddMember(T Value)
	{
		CSmartValue SmartValue;
		if (PrepareMember(CSmartValue::GetTypeByValue(Value), SmartValue))
		{
			SmartValue.SetValue(Value);
			return FinishMember(SmartValue.GetDataLen());
		}
		return false;
	}	
	bool AddMember(const char* pszStr)
	{
		if (pszStr == NULL)
			return false;
		return AddMember(pszStr, (UINT)strlen(pszStr));
	}
	bool AddMember(const WCHAR* pszStr)
	{
		if (pszStr == NULL)
			return true;
		return AddMember(pszStr, (UINT)wcslen(pszStr));
	}
	bool AddMember(const char* pszStr, UINT nStrLen, CEasyString::STRING_CODE_PAGE CodePage = CEasyString::STRING_CODE_PAGE_AUTO)
	{
		if (m_pData == NULL)
			return false;

		UINT StrRealLen;
		UINT StrByteCount;
		UINT NeedSize;
		BYTE ValueType;
		if (CodePage == CEasyString::STRING_CODE_PAGE_AUTO)
			CodePage = CEasyString::SYSTEM_STRING_CODE_PAGE;
		switch (CodePage)
		{
		case CEasyString::STRING_CODE_PAGE_ANSI:
			switch (CSmartValue::INTERNAL_STRING_CODE_PAGE)
			{
			case CEasyString::STRING_CODE_PAGE_ANSI:
				ValueType = CSmartValue::VT_STRING_ANSI;
				StrRealLen = nStrLen;
				StrByteCount = StrRealLen;
				NeedSize = sizeof(BYTE) + sizeof(UINT) + StrRealLen + 1;
				break;
			case CEasyString::STRING_CODE_PAGE_UTF8:
				ValueType = CSmartValue::VT_STRING_UTF8;
				StrRealLen = (UINT)AnsiToUTF8(pszStr, nStrLen, NULL, 0);
				StrByteCount = StrRealLen;
				NeedSize = sizeof(BYTE) + sizeof(UINT) + StrRealLen + 1;
				break;
			case CEasyString::STRING_CODE_PAGE_UCS16:
				ValueType = CSmartValue::VT_STRING_UCS16;
				StrRealLen = (UINT)AnsiToUnicode(pszStr, nStrLen, NULL, 0);
				StrByteCount = StrRealLen * sizeof(WCHAR);
				NeedSize = sizeof(BYTE) + sizeof(UINT) + StrRealLen * sizeof(WCHAR) + sizeof(WCHAR);
				break;
			default:
				return false;
			}
			break;
		case CEasyString::STRING_CODE_PAGE_UTF8:
			switch (CSmartValue::INTERNAL_STRING_CODE_PAGE)
			{
			case CEasyString::STRING_CODE_PAGE_ANSI:
				ValueType = CSmartValue::VT_STRING_ANSI;
				StrRealLen = (UINT)UTF8ToAnsi(pszStr, nStrLen, NULL, 0);
				StrByteCount = StrRealLen;
				NeedSize = sizeof(BYTE) + sizeof(UINT) + StrRealLen + 1;
				break;
			case CEasyString::STRING_CODE_PAGE_UTF8:
				ValueType = CSmartValue::VT_STRING_UTF8;
				StrRealLen = nStrLen;
				StrByteCount = StrRealLen;
				NeedSize = sizeof(BYTE) + sizeof(UINT) + StrRealLen + 1;
				break;
			case CEasyString::STRING_CODE_PAGE_UCS16:
				ValueType = CSmartValue::VT_STRING_UCS16;
				StrRealLen = (UINT)UTF8ToUnicode(pszStr, nStrLen, NULL, 0);
				StrByteCount = StrRealLen * sizeof(WCHAR);
				NeedSize = sizeof(BYTE) + sizeof(UINT) + StrRealLen * sizeof(WCHAR) + sizeof(WCHAR);
				break;
			default:
				return false;
			}
			break;
		default:
			return false;
		}

		if (GetFreeLen() < NeedSize)
		{
			return false;
		}
		BYTE ElementType = GetElementType();
		if ((ElementType != ValueType) && (ElementType != CSmartValue::VT_UNKNOWN))
			return false;
		
		BYTE* pFreeBuffer = m_pData + sizeof(BYTE) + sizeof(UINT) + GetLength();
		*((BYTE*)pFreeBuffer) = ValueType;
		pFreeBuffer += sizeof(BYTE);
		*((UINT*)pFreeBuffer) = StrByteCount;
		pFreeBuffer += sizeof(UINT);

		if (pszStr && nStrLen)
		{
			switch (CodePage)
			{
			case CEasyString::STRING_CODE_PAGE_ANSI:
				switch (CSmartValue::INTERNAL_STRING_CODE_PAGE)
				{
				case CEasyString::STRING_CODE_PAGE_ANSI:
					memcpy(pFreeBuffer, pszStr, StrRealLen);
					pFreeBuffer[StrRealLen] = 0;
					break;
				case CEasyString::STRING_CODE_PAGE_UTF8:
					AnsiToUTF8(pszStr, nStrLen, (char*)pFreeBuffer, StrRealLen);
					pFreeBuffer[StrRealLen] = 0;
					break;
				case CEasyString::STRING_CODE_PAGE_UCS16:
					AnsiToUnicode(pszStr, nStrLen, (WCHAR*)pFreeBuffer, StrRealLen);
					((WCHAR*)pFreeBuffer)[StrRealLen] = 0;
					break;
				default:
					return false;
				}
				break;
			case CEasyString::STRING_CODE_PAGE_UTF8:
				switch (CSmartValue::INTERNAL_STRING_CODE_PAGE)
				{
				case CEasyString::STRING_CODE_PAGE_ANSI:
					UTF8ToAnsi(pszStr, nStrLen, (char*)pFreeBuffer, StrRealLen);
					pFreeBuffer[StrRealLen] = 0;
					break;
				case CEasyString::STRING_CODE_PAGE_UTF8:
					memcpy(pFreeBuffer, pszStr, StrRealLen);
					pFreeBuffer[StrRealLen] = 0;
					break;
				case CEasyString::STRING_CODE_PAGE_UCS16:
					UTF8ToUnicode(pszStr, nStrLen, (WCHAR*)pFreeBuffer, StrRealLen);
					((WCHAR*)pFreeBuffer)[StrRealLen] = 0;
					break;
				default:
					return false;
				}
				break;
			}
		}
		*((UINT*)(m_pData + sizeof(BYTE))) += NeedSize;
		return true;
	}
	bool AddMember(const WCHAR* pszStr, UINT nStrLen)
	{
		if (m_pData == NULL)
			return false;

		UINT StrRealLen;
		UINT StrByteCount;
		UINT NeedSize;
		BYTE ValueType;

		switch (CSmartValue::INTERNAL_STRING_CODE_PAGE)
		{
		case CEasyString::STRING_CODE_PAGE_ANSI:
			ValueType = CSmartValue::VT_STRING_ANSI;
			StrRealLen = (UINT)UnicodeToAnsi(pszStr, nStrLen, NULL, 0);
			StrByteCount = StrRealLen;
			NeedSize = sizeof(BYTE) + sizeof(UINT) + StrRealLen + 1;
			break;
		case CEasyString::STRING_CODE_PAGE_UTF8:
			ValueType = CSmartValue::VT_STRING_UTF8;
			StrRealLen = (UINT)UnicodeToUTF8(pszStr, nStrLen, NULL, 0);
			StrByteCount = StrRealLen;
			NeedSize = sizeof(BYTE) + sizeof(UINT) + StrRealLen + 1;
			break;
		case CEasyString::STRING_CODE_PAGE_UCS16:
			ValueType = CSmartValue::VT_STRING_UCS16;
			StrRealLen = nStrLen;
			StrByteCount = StrRealLen * sizeof(WCHAR);
			NeedSize = sizeof(BYTE) + sizeof(UINT) + StrRealLen * sizeof(WCHAR) + sizeof(WCHAR);
			break;
		default:
			return false;
		}
		if (GetFreeLen() < NeedSize)
		{
			return false;
		}
		BYTE ElementType = GetElementType();
		if ((ElementType != ValueType) && (ElementType != CSmartValue::VT_UNKNOWN))
			return false;

		BYTE* pFreeBuffer = m_pData + sizeof(BYTE) + sizeof(UINT) + GetLength();
		*((BYTE*)pFreeBuffer) = ValueType;
		pFreeBuffer += sizeof(BYTE);
		*((UINT*)pFreeBuffer) = StrByteCount;
		pFreeBuffer += sizeof(UINT);
		if (pszStr && StrRealLen)
		{
			switch (CSmartValue::INTERNAL_STRING_CODE_PAGE)
			{
			case CEasyString::STRING_CODE_PAGE_ANSI:
				UnicodeToAnsi(pszStr, nStrLen, (char*)pFreeBuffer, StrRealLen);
				pFreeBuffer[StrRealLen] = 0;
				break;
			case CEasyString::STRING_CODE_PAGE_UTF8:
				UnicodeToUTF8(pszStr, nStrLen, (char*)pFreeBuffer, StrRealLen);
				pFreeBuffer[StrRealLen] = 0;
				break;
			case CEasyString::STRING_CODE_PAGE_UCS16:
				memcpy(pFreeBuffer, pszStr, sizeof(WCHAR) * StrRealLen);
				((WCHAR*)pFreeBuffer)[StrRealLen] = 0;
				break;
			default:
				return false;
			}
		}
		*((UINT*)(m_pData + sizeof(BYTE))) += NeedSize;
		return true;
	}
	bool AddMember(const CEasyString& Str)
	{
		return AddMember(Str, (UINT)Str.GetLength());
	}
	bool AddMember(const unsigned char* pData, UINT DataLen)
	{
		if ((DataLen > 0) && (m_pData == NULL))
			return false;

		UINT NeedSize = sizeof(BYTE) + sizeof(UINT) + DataLen;
		if (GetFreeLen() < NeedSize)
		{
			return false;
		}

		BYTE ElementType = GetElementType();
		if ((ElementType != CSmartValue::VT_BINARY) && (ElementType != CSmartValue::VT_UNKNOWN))
			return false;

		BYTE* pFreeBuffer = m_pData + sizeof(BYTE) + sizeof(UINT) + GetLength();
		*((BYTE*)pFreeBuffer) = CSmartValue::VT_BINARY;
		pFreeBuffer += sizeof(BYTE);
		*((UINT*)pFreeBuffer) = DataLen;
		pFreeBuffer += sizeof(UINT);
		memcpy(pFreeBuffer, pData, DataLen);
		*((UINT*)(m_pData + sizeof(BYTE))) += NeedSize;
		return true;
	}
	bool AddMember(const CEasyBuffer& Data)
	{
		return AddMember((const unsigned char*)Data.GetBuffer(), Data.GetUsedSize());
	}
	bool AddMember(const CSmartValue& Value)
	{
		if (m_pData == NULL)
			return false;		
		if (Value.GetDataLen() <= 0)
		{
			return false;
		}
		UINT NeedSize = Value.GetDataLen();
		if (GetFreeLen() < NeedSize)
		{
			return false;
		}
		BYTE ElementType = GetElementType();
		if ((ElementType != Value.GetType()) && (ElementType != CSmartValue::VT_UNKNOWN))
			return false;

		BYTE* pFreeBuffer = m_pData + sizeof(BYTE) + sizeof(UINT) + GetLength();
		memcpy(pFreeBuffer, Value.GetData(), Value.GetDataLen());
		*((UINT*)(m_pData + sizeof(BYTE))) += NeedSize;
		return true;
	}
	bool AddMember(const CSmartStruct& Data);
	bool AddMember(const CSmartArray& Data)
	{
		if (Data.GetData() == NULL)
			return false;

		UINT NeedSize = Data.GetDataLen();
		if (GetFreeLen() < NeedSize)
		{
			return false;
		}

		BYTE ElementType = GetElementType();
		if ((ElementType != CSmartValue::VT_ARRAY) && (ElementType != CSmartValue::VT_UNKNOWN))
			return false;

		BYTE* pFreeBuffer = m_pData + sizeof(BYTE) + sizeof(UINT) + GetLength();
		memcpy(pFreeBuffer, Data.GetData(), Data.GetDataLen());
		*((UINT*)(m_pData + sizeof(BYTE))) += NeedSize;
		return true;
	}
	bool Append(const CSmartArray& Array)
	{
		if (m_pData == NULL)
			return false;
		if (Array.m_pData == NULL)
			return false;
		if (Array.m_pData[0] != CSmartValue::VT_ARRAY)
			return false;
		UINT NeedSize = Array.GetLength();
		if (GetFreeLen() < NeedSize)
		{
			return false;
		}

		BYTE ElementType = GetElementType();
		if ((ElementType != Array.GetElementType()) && (ElementType != CSmartValue::VT_UNKNOWN))
			return false;

		BYTE* pFreeBuffer = m_pData + sizeof(BYTE) + sizeof(UINT) + GetLength();

		memcpy(pFreeBuffer, Array.m_pData + sizeof(BYTE) + sizeof(UINT), NeedSize);
		*((UINT*)(m_pData + sizeof(BYTE))) += NeedSize;
		return true;
	}
	//void Dump(CStringBuilder& OutBuffer, bool HaveType);
	static inline UINT GetFixMemberSize(UINT TypeLen)
	{
		return sizeof(BYTE) + TypeLen;
	}
	static inline UINT GetArrayMemberSize(UINT ArraySize)
	{
		return sizeof(BYTE) + sizeof(UINT) + ArraySize;
	}
	static inline UINT GetStructMemberSize(UINT StructSize)
	{
		return sizeof(BYTE) + sizeof(UINT) + StructSize;
	}
	static inline UINT GetStringMemberSize(const CEasyString& Str)
	{
		return GetStringMemberSize(Str, Str.GetLength());
	}
	static inline UINT GetStringMemberSize(LPCWSTR szStr)
	{
		return GetStringMemberSize(szStr, wcslen(szStr));
	}
	static inline UINT GetStringMemberSize(LPCWSTR szStr, size_t StrLen)
	{
		CEasyString::STRING_CODE_PAGE TargetCodePage = CSmartValue::INTERNAL_STRING_CODE_PAGE;
		if (TargetCodePage == CEasyString::STRING_CODE_PAGE_AUTO)
			TargetCodePage = CEasyString::SYSTEM_STRING_CODE_PAGE;
		switch (TargetCodePage)
		{
		case CEasyString::STRING_CODE_PAGE_UTF8:
			return (szStr ? (UINT)UnicodeToUTF8(szStr, StrLen, NULL, 0) : 0) + sizeof(BYTE) + sizeof(UINT) + sizeof(char);
		case CEasyString::STRING_CODE_PAGE_UCS16:
			return (UINT)StrLen * sizeof(WCHAR) + sizeof(BYTE) + sizeof(UINT) + sizeof(WCHAR);
		default:
			return (szStr ? (UINT)UnicodeToAnsi(szStr, StrLen, NULL, 0) : 0) + sizeof(BYTE) + sizeof(UINT) + sizeof(char);
		}
	}
	static inline UINT GetStringMemberSize(LPCSTR szStr)
	{
		return GetStringMemberSize(szStr, strlen(szStr));
	}
	static inline UINT GetStringMemberSize(LPCSTR szStr, size_t StrLen)
	{
		CEasyString::STRING_CODE_PAGE TargetCodePage = CSmartValue::INTERNAL_STRING_CODE_PAGE;
		if (TargetCodePage == CEasyString::STRING_CODE_PAGE_AUTO)
			TargetCodePage = CEasyString::SYSTEM_STRING_CODE_PAGE;
		switch (CSmartValue::INTERNAL_STRING_CODE_PAGE)
		{
		case CEasyString::STRING_CODE_PAGE_UTF8:
			if (CEasyString::SYSTEM_STRING_CODE_PAGE == CEasyString::STRING_CODE_PAGE_ANSI)
				return (szStr ? (UINT)AnsiToUTF8(szStr, StrLen, NULL, 0) : 0) + sizeof(BYTE) + sizeof(UINT) + sizeof(char);
			else
				return (UINT)StrLen + sizeof(BYTE) + sizeof(UINT) + sizeof(char);
		case CEasyString::STRING_CODE_PAGE_UCS16:
			if (CEasyString::SYSTEM_STRING_CODE_PAGE == CEasyString::STRING_CODE_PAGE_ANSI)
				return (szStr ? (UINT)AnsiToUnicode(szStr, StrLen, NULL, 0) * sizeof(WCHAR) : 0) + sizeof(BYTE) + sizeof(UINT) + sizeof(WCHAR);
			else
				return (szStr ? (UINT)UTF8ToUnicode(szStr, StrLen, NULL, 0) * sizeof(WCHAR) : 0) + sizeof(BYTE) + sizeof(UINT) + sizeof(WCHAR);
		default:
			if (CEasyString::SYSTEM_STRING_CODE_PAGE == CEasyString::STRING_CODE_PAGE_UTF8)
				return (szStr ? (UINT)UTF8ToAnsi(szStr, StrLen, NULL, 0) : 0) + sizeof(BYTE) + sizeof(UINT) + sizeof(char);
			else
				return (UINT)StrLen + sizeof(BYTE) + sizeof(UINT) + sizeof(char);
		}
	}
	static inline UINT GetBinaryMemberSize(UINT DataLen)
	{
		return sizeof(BYTE) + sizeof(UINT) + DataLen;
	}
protected:
	bool PrepareMember(BYTE Type, CSmartValue& SmartValue)
	{
		BYTE ElementType = GetElementType();
		if ((ElementType == CSmartValue::VT_UNKNOWN) || (ElementType == Type))
		{
			BYTE* pFreeBuffer = m_pData + sizeof(BYTE) + sizeof(UINT) + GetLength();
			UINT FreeSize = GetFreeLen();
			if (SmartValue.Attach(pFreeBuffer, FreeSize, Type))
			{
				return true;
			}
		}
		return false;
	}
};

