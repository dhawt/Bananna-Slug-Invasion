///////////////////////////////////////////////////////////////////////////////
//      created:        8/13/2001
//      filename:       PreferenceValue.cpp
//      author:         Eli Pulsifer
//

#include "stdafx.h"
#include "defines.h"
#include "PreferenceValue.h"
#include "assert.h"

using namespace PreferenceStore;

SPreferenceValue::SPreferenceValue(const SPreferenceValue& value)
	: m_eType(value.m_eType)
	, m_pRawData(NULL)
	, m_nLength(0)
	, m_bDirty(value.m_bDirty)
	, m_bDestroy(value.m_bDestroy)
{
	// copy the value data, if necessary
	if (value.m_nLength > 0)
	{
		AllocData(value.m_nLength);
		memcpy(m_pRawData, value.m_pRawData, m_nLength);
	}
}

///////////////////////////////////////////////////////////////////////////////

SPreferenceValue::SPreferenceValue(bool isDirty)
	: m_eType(kUndefinedType)
	, m_pRawData(NULL)
	, m_nLength(0)
	, m_bDirty(isDirty)
	, m_bDestroy(false)
{
}

///////////////////////////////////////////////////////////////////////////////

SPreferenceValue::SPreferenceValue(const std::string& stringData, bool isDirty)
	: m_pRawData(NULL)
	, m_nLength(0)
	, m_bDirty(isDirty)
	, m_bDestroy(false)
{
	SetData(stringData);
}
///////////////////////////////////////////////////////////////////////////////
SPreferenceValue::SPreferenceValue(const void* buffer, uint32 bufferLength, bool isDirty)
	: m_pRawData(NULL)
	, m_nLength(0)
	, m_bDirty(isDirty)
	, m_bDestroy(false)
{
	SetData(buffer, bufferLength);
}

///////////////////////////////////////////////////////////////////////////////

SPreferenceValue::SPreferenceValue(uint32 integerValue, bool isDirty)
	: m_pRawData(NULL)
	, m_nLength(0)
	, m_bDirty(isDirty)
	, m_bDestroy(false)
{
	SetData(integerValue);
}

///////////////////////////////////////////////////////////////////////////////

SPreferenceValue::SPreferenceValue(int32 integerValue, bool isDirty)
	: m_pRawData(NULL)
	, m_nLength(0)
	, m_bDirty(isDirty)
	, m_bDestroy(false)
{
	SetData(integerValue);
}

///////////////////////////////////////////////////////////////////////////////

SPreferenceValue::SPreferenceValue(float floatValue, bool isDirty)
	: m_pRawData(NULL)
	, m_nLength(0)
	, m_bDirty(isDirty)
	, m_bDestroy(false)
{
	SetData(floatValue);
}

///////////////////////////////////////////////////////////////////////////////

#if defined(DIRECT3D_VERSION)
SPreferenceValue::SPreferenceValue(const D3DXVECTOR3 &vecData, bool isDirty)
	: m_pRawData(NULL)
	, m_nLength(0)
	, m_bDirty(isDirty)
	, m_bDestroy(false)
{
	SetData(vecData);
}
///////////////////////////////////////////////////////////////////////////////
SPreferenceValue::SPreferenceValue(const D3DXVECTOR4 &vecValue, bool isDirty)
	: m_pRawData(NULL)
	, m_nLength(0)
	, m_bDirty(isDirty)
	, m_bDestroy(false)
{
	SetData(vecValue);
}
SPreferenceValue::SPreferenceValue(const D3DXCOLOR &colorData, bool isDirty)
	: m_pRawData(NULL)
	, m_nLength(0)
	, m_bDirty(isDirty)
	, m_bDestroy(false)
{
	SetData(colorData);
}
#endif	//	defined(DIRECT3D_VERSION)

///////////////////////////////////////////////////////////////////////////////

SPreferenceValue::SPreferenceValue(const std::string& data, ValueType dataType, bool isDirty)
	: m_pRawData(NULL)
	, m_nLength(0)
	, m_bDirty(isDirty)
	, m_bDestroy(false)
{
	SetRawData(data, dataType);
}

///////////////////////////////////////////////////////////////////////////////

SPreferenceValue::~SPreferenceValue()
{
	FreeData();
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::GetData
// Description	    : Retrieves std::string data if present
// Return type		: bool 
// Argument         : std::string & strData
//
bool SPreferenceValue::GetData(std::string& stringData) const
{
	bool isValid = (kStringType == m_eType);

	if (isValid)
		stringData = reinterpret_cast<char*>(m_pRawData);

	return isValid;
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::SetData
// Description	    : Sets type to std::string and applies std::string data
// Return type		: void 
// Argument         : const std::string & strData
//
void SPreferenceValue::SetData(const std::string& stringData)
{
	FreeData();

	m_eType = kStringType;
	AllocData(stringData.size() + 1);
	strcpy(reinterpret_cast<char*>(m_pRawData), stringData.c_str());
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: char*
// Description		: 
// Return type		: SPreferenceValue::operator const 
//
SPreferenceValue::operator const char*() const
{
	return reinterpret_cast<char*>(m_pRawData);
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::GetData
// Description	    : Retrieves uint32 data if present
// Return type		: bool 
// Argument         : uint32 & dwData
//
bool SPreferenceValue::GetData(uint32& integerData) const
{
	bool isValid = (kUnsignedIntegerType == m_eType);

	if (isValid)
		integerData = *reinterpret_cast<uint32*>(m_pRawData);

	return isValid;
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::SetData
// Description	    : Sets type to uint32 and applies uint32 data
// Return type		: void 
// Argument         : uint32 dwData
//
void SPreferenceValue::SetData(uint32 integerData)
{
	FreeData();

	m_eType = kUnsignedIntegerType;
	AllocData(sizeof(uint32));
	*reinterpret_cast<uint32*>(m_pRawData) = integerData;
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::GetData
// Description	    : Retrieves int32 data if present
// Return type		: bool 
// Argument         : uint32 & dwData
//
bool SPreferenceValue::GetData(int32& integerData) const
{
	bool isValid = (kSignedIntegerType == m_eType);

	if (isValid)
		integerData = *reinterpret_cast<int32*>(m_pRawData);

	return isValid;
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: int32
// Description		: 
// Return type		: SPreferenceValue::operator 
//
SPreferenceValue::operator int32() const
{
	assert(kFloatType == m_eType);
	return *reinterpret_cast<int32*>(m_pRawData);
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::SetData
// Description	    : Sets type to uint32 and applies uint32 data
// Return type		: void 
// Argument         : uint32 dwData
//
void SPreferenceValue::SetData(float floatData)
{
	FreeData();

	m_eType = kFloatType;
	AllocData(sizeof(float));
	*reinterpret_cast<float*>(m_pRawData) = floatData;
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::GetData
// Description	    : Retrieves int32 data if present
// Return type		: bool 
// Argument         : uint32 & dwData
//
bool SPreferenceValue::GetData(float & floatData) const
{
	bool isValid = (kFloatType == m_eType);

	if (isValid)
		floatData = *reinterpret_cast<float*>(m_pRawData);

	return isValid;
}

//////////////////////////////////////////////////////////////////////
// Function name	: float32
// Description		: 
// Return type		: SPreferenceValue::operator 
//
SPreferenceValue::operator float32() const
{
	assert(kFloatType == m_eType);
	return *reinterpret_cast<float32*>(m_pRawData);
}

#if defined(DIRECT3D_VERSION)
///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::GetData
// Description	    : Retrieves D3DXVECTOR3 data if present
// Return type		: bool 
// Argument         : D3DXVECTOR3 & vecData
//
bool SPreferenceValue::GetData(D3DXVECTOR3 &vecData) const
{
	bool isValid = (kVector3Type == m_eType);

	if (isValid)
		vecData = *reinterpret_cast<D3DXVECTOR3*>(m_pRawData);

	return isValid;
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::SetData
// Description	    : Sets type to D3DXVECTOR3 and applies D3DXVECTOR3 data
// Return type		: void 
// Argument         : const D3DXVECTOR3 & vecData
//
void SPreferenceValue::SetData(const D3DXVECTOR3 & vecData)
{
	FreeData();

	m_eType = kVector3Type;
	AllocData(sizeof(D3DXVECTOR3));
	*reinterpret_cast<D3DXVECTOR3*>(m_pRawData) = vecData;
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::GetData
// Description	    : Retrieves D3DXVECTOR4 data if present
// Return type		: bool 
// Argument         : D3DXVECTOR4 & vecData
//
bool SPreferenceValue::GetData(D3DXVECTOR4 &vecData) const
{
	bool isValid = (kVector4Type == m_eType);

	if (isValid)
		vecData = *reinterpret_cast<D3DXVECTOR4*>(m_pRawData);

	return isValid;
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::SetData
// Description	    : Sets type to D3DXVECTOR4 and applies D3DXVECTOR4 data
// Return type		: void 
// Argument         : const D3DXVECTOR4 & vecData
//
void SPreferenceValue::SetData(const D3DXVECTOR4 & vecData)
{
	FreeData();

	m_eType = kVector4Type;
	AllocData(sizeof(D3DXVECTOR4));
	*reinterpret_cast<D3DXVECTOR4*>(m_pRawData) = vecData;
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: D3DXVECTOR4
// Description		: 
// Return type		: SPreferenceValue::operator 
//
SPreferenceValue::operator D3DXVECTOR4() const
{
	assert(kVector4Type == m_eType);
	return *reinterpret_cast<D3DXVECTOR4*>(m_pRawData);
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::GetData
// Description	    : Retrieves D3DXVECTOR3 data if present
// Return type		: bool 
// Argument         : D3DXVECTOR3 & vecData
//
bool SPreferenceValue::GetData(D3DXCOLOR &colorData) const
{
	bool isValid = (kRGBAColorType == m_eType);

	if (isValid)
		colorData = *reinterpret_cast<D3DXCOLOR*>(m_pRawData);

	return isValid;
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::SetData
// Description	    : Sets type to D3DXVECTOR3 and applies D3DXVECTOR3 data
// Return type		: void 
// Argument         : const D3DXVECTOR3 & vecData
//
void SPreferenceValue::SetData(const D3DXCOLOR &colorData)
{
	FreeData();

	m_eType = kRGBAColorType;
	AllocData(sizeof(D3DXCOLOR));
	*reinterpret_cast<D3DXCOLOR*>(m_pRawData) = colorData;
}
#endif	//	defined(DIRECT3D_VERSION)

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::SetData
// Description	    : Sets type to int32 and applies uint32 data
// Return type		: void 
// Argument         : uint32 dwData
//
void SPreferenceValue::SetData(int32 integerData)
{
	FreeData();

	m_eType = kSignedIntegerType;
	AllocData(sizeof(int32));
	*reinterpret_cast<int32*>(m_pRawData) = integerData;
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::GetData
// Description	    : Retrieves binary data if present
// Return type		: bool 
// Argument         :  void * pData
// Argument         : uint32 nLength
//
bool SPreferenceValue::GetData(void* buffer, uint32& bufferLength) const
{
	bool isValid = (kBinaryType == m_eType);

	if (isValid)
	{
		uint32 nLengthToGet = m_nLength > bufferLength ? bufferLength : m_nLength;
		memcpy(buffer, m_pRawData, nLengthToGet);
		bufferLength = nLengthToGet;
	}

	return isValid;
}


///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::SetData
// Description	    : Sets type to binary and applies binary data
// Return type		: void 
// Argument         :  const void * pData
// Argument         : uint32 nLength
//
void SPreferenceValue::SetData(const void* buffer, uint32 bufferLength)
{
	FreeData();

	m_eType = kBinaryType;
	AllocData(bufferLength);
	memcpy(m_pRawData, buffer, bufferLength);
}


///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::GetDataAsText
// Description	    : Retrieves the contained data as a formated std::string
// Return type		: std::string 
// Argument         : void
//
std::string SPreferenceValue::GetDataAsText() const
{
	std::string stringData;

	switch (m_eType)
	{
		case kStringType:
			GetData(stringData);
			break;

		case kUnsignedIntegerType:
			{
				uint32 integerData;
				char pszBuffer[11];
				GetData(integerData);
				sprintf(pszBuffer, "%1X", integerData);
				stringData = pszBuffer;
			}
			break;

		case kSignedIntegerType:
			{
				int32 integerData;
				char pszBuffer[11];
				GetData(integerData);
				sprintf(pszBuffer, "%1d", integerData);
				stringData = pszBuffer;
			}
			break;

		case kFloatType:
			{
				float floatData;
				char pszBuffer[43];
				GetData(floatData);
				sprintf(pszBuffer, "%0.4f", floatData);
				stringData = pszBuffer;
			}
			break;

		case kBinaryType:
			ConvertToString(stringData, m_pRawData, m_nLength);
			break;
			
#if defined(DIRECT3D_VERSION)
		case kVector3Type:
			{
				D3DXVECTOR3 vecData;
				char pszBuffer[128];
				GetData(vecData);
				sprintf(pszBuffer, "%f, %f, %f", vecData.x, vecData.y, vecData.z);
				stringData = pszBuffer;
			}
			break;

		case kVector4Type:
			{
				D3DXVECTOR4 vecData;
				char pszBuffer[128];
				GetData(vecData);
				sprintf(pszBuffer, "%f, %f, %f, %f", vecData.x, vecData.y, vecData.z, vecData.w);
				stringData = pszBuffer;
			}
			break;

		case kRGBAColorType:
			{
				D3DXCOLOR colorData;
				char pszBuffer[128];
				GetData(colorData);
				sprintf(pszBuffer, "%f, %f, %f, %f", colorData.r, colorData.g,
					colorData.b, colorData.a);
				stringData = pszBuffer;
			}
			break;
#endif	//	defined(DIRECT3D_VERSION)
			
		case kUndefinedType:
			{
				char* pszBuffer = new char[m_nLength + 1];
				memcpy(pszBuffer, m_pRawData, m_nLength);
				pszBuffer[m_nLength] = '\0';
				stringData = pszBuffer;
				delete pszBuffer;
			}
	}

	return stringData;
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::SetRawData
// Description	    : Sets the type and contained data from a formatted std::string
// Return type		: void 
// Argument         :  const std::string & strData
// Argument         : ValueType eType
//
void SPreferenceValue::SetRawData(const std::string& data, ValueType dataType)
{
	switch (dataType)
	{
		case kStringType:
			SetData(data);
			break;

		case kUnsignedIntegerType:
			SetData((uint32)strtoul(data.c_str(), NULL, 16));
			break;
		case kSignedIntegerType:
			SetData((int32)strtol(data.c_str(), NULL, 10));
			break;
		case kFloatType:
			SetData((float)strtod(data.c_str(), NULL));
			break;

#if defined(DIRECT3D_VERSION)
		case kVector3Type:
			{
				D3DXVECTOR3 vecData;
				ConvertToD3DXVECTOR3(vecData, data);
				SetData(vecData);
			}
			break;
		case kVector4Type:
			{
				D3DXVECTOR4 vecData;
				ConvertToD3DXVECTOR4(vecData, data);
				SetData(vecData);
			}
			break;
		case kRGBAColorType:
			{
				D3DXCOLOR colorData;
				ConvertToD3DXCOLOR(colorData, data);
				SetData(colorData);
			}
			break;
#endif	//	defined(DIRECT3D_VERSION)

		case kBinaryType:
			{
				int nByteCount = 1;
				int nPos;
				char* pszBuffer = _strdup(data.c_str());
				
				// count the bytes
				for(nPos = 0; pszBuffer[nPos] != '\0' ; ++nPos)
				{
					if (',' == pszBuffer[nPos])
						nByteCount++;
				}

				FreeData();
				m_eType = dataType;
				AllocData(nByteCount);

				char* pszToken = strtok(pszBuffer, ",");

				nPos = 0;

				// read each byte, convert to bin and set the dest memory value
				while (pszToken != NULL)
				{
					m_pRawData[nPos] = pszToken[0] - 0x30;
					m_pRawData[nPos] <<= 4;
					m_pRawData[nPos] = pszToken[1] - 0x30;
					pszToken = strtok(NULL, ",");
				}
				//	return the number of bytes set in the buffer
			}
			break;

		case kUndefinedType:
			{
				FreeData();
				AllocData(data.size() + 1);
				strcpy(reinterpret_cast<char*>(m_pRawData), data.c_str());
			}
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////

SPreferenceValue::ValueType SPreferenceValue::GetType() const
{
	return m_eType;
}

///////////////////////////////////////////////////////////////////////////////

size_t SPreferenceValue::GetSize() const
{
	return m_nLength;
}

///////////////////////////////////////////////////////////////////////////////

void SPreferenceValue::SetDirty(bool isDirty)
{
	m_bDirty = isDirty;
}

///////////////////////////////////////////////////////////////////////////////

bool SPreferenceValue::IsDirty() const
{
	return m_bDirty;
}

///////////////////////////////////////////////////////////////////////////////

void SPreferenceValue::SetDestroy(bool bDestroy)
{
	m_bDirty = m_bDestroy = bDestroy;
}

///////////////////////////////////////////////////////////////////////////////

bool SPreferenceValue::Destroy() const
{
	return m_bDestroy;
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::operator < 
// Description	    : Binary less than compare
// Return type		: bool SPreferenceValue::operator 
// Argument         : const SPreferenceValue & refValue
//
bool SPreferenceValue::operator < (const SPreferenceValue& rhs) const
{
	bool bRet;

	if(m_nLength < rhs.m_nLength)
		bRet = true;
	else if(m_nLength > rhs.m_nLength)
		bRet = false;
	else
		bRet = 0 > memcmp(m_pRawData, rhs.m_pRawData, m_nLength);

	return bRet;
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::operator = 
// Description	    : Assignment 
// Return type		: const SPreferenceValue 
// Argument         : const SPreferenceValue & refValue
//
const SPreferenceValue& SPreferenceValue::operator = (const SPreferenceValue& rhs)
{
	FreeData();
	m_eType = rhs.m_eType;
	AllocData(rhs.m_nLength);
	memcpy(m_pRawData, rhs.m_pRawData, m_nLength);
	m_bDirty = rhs.m_bDirty;
	m_bDestroy = rhs.m_bDestroy;
	return *this;
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::ConvertToString
// Description	    : Converts bytes pointed to by pValue to a hex std::string representation
// Return type		: void 
// Argument         : std::string & strValue
// Argument         : const void * pValue
// Argument         : uint32 nLength
//
void SPreferenceValue::ConvertToString(std::string& stringValue, const void* buffer,
									   size_t bufferLength) const
{
	static char pszBuffer[3];
	uint32 nPos;

	stringValue.erase();

	if(bufferLength)
	{
		stringValue.reserve((bufferLength * 2) + bufferLength - 1);

		for (nPos = 0; nPos < bufferLength; ++nPos)
		{
			_itoa((reinterpret_cast<unsigned char*>(const_cast<void*>(buffer)))[nPos],
				pszBuffer, 16);
			stringValue += pszBuffer;
			stringValue += ',';
		}
		stringValue.resize(stringValue.size() - 1);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Function name   : ConvertToFloatArray
// Description     : 
// Return type     : void 
// Argument        : float *pVals
// Argument        : uint32 nVals
// Argument        : const std::string& stringValue
//
void ConvertToFloatArray(float *pVals, uint32 nVals, const std::string& stringValue)
{
	char* pszBuffer = _strdup(stringValue.c_str());
	char* pszVal = NULL;
	uint32 iVal = 0;

	pszVal = strtok(pszBuffer, ",");
	while(pszVal && iVal < nVals)
	{
		pVals[iVal] = static_cast<float>(atof(pszVal));
		iVal++;
		pszVal = strtok(NULL, ",");
	}
	
	free(pszBuffer);
}

#if defined(DIRECT3D_VERSION)

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::ConvertToD3DXVECTOR3
// Description	    : 
// Return type		: void 
// Argument         : D3DXVECTOR3 &vecData
// Argument         : const std::string& stringValue
//
void SPreferenceValue::ConvertToD3DXVECTOR3(D3DXVECTOR3 &vecData, const std::string& stringValue) const
{
	ConvertToFloatArray(&vecData.x, 3, stringValue);
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::ConvertToD3DXVECTOR4
// Description	   : 
// Return type		: void 
// Argument       : D3DXVECTOR4 &vecData
// Argument       : const std::string& stringValue
//
void SPreferenceValue::ConvertToD3DXVECTOR4(D3DXVECTOR4 &vecData, const std::string& stringValue) const
{
	ConvertToFloatArray(&vecData.x, 4, stringValue);
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::ConvertToD3DXCOLOR
// Description	    : 
// Return type		: void 
// Argument         : D3DXCOLOE &colorData
// Argument         : const std::string& stringValue
//
void SPreferenceValue::ConvertToD3DXCOLOR(D3DXCOLOR &colorData, const std::string& stringValue) const
{
	ConvertToFloatArray(&colorData.r, 4, stringValue);
}
#endif	//	defined(DIRECT3D_VERSION)

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::FreeData
// Description	    : 
// Return type		: void 
//
void SPreferenceValue::FreeData()
{
	delete m_pRawData;
	m_pRawData = NULL;
	m_nLength = 0;
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: SPreferenceValue::AllocData
// Description	    : 
// Return type		: void 
// Argument         : uint32 dataSize
//
void SPreferenceValue::AllocData(size_t dataSize)
{
	m_nLength = dataSize;
	m_pRawData = new uint8[m_nLength];
}
