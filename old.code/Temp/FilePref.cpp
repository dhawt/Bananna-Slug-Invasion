/////////////////////////////////////////////////////////////////////////////
//      created:        1/24/2002
//      filename:       FilePref.cpp
//      author:         Eli Pulsifer
//
//      purpose:        

#include "stdafx.h"
#include "defines.h"
#include "FilePref.h"
#include <sys/io.h>
#include <string.h>

using namespace PreferenceStore;

IMPLEMENT_PREFIX_TABLE(CFilePref)
IMPLEMENT_POSTFIX_TABLE(CFilePref)
IMPLEMENT_PARSER_TABLE(CFilePref)


///////////////////////////////////////////////////////////////////////////////////////////////////

CFilePref::CFilePref()
	: m_hFile(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

CFilePref::~CFilePref()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CFilePref::LoadFromFile
// Description	    : Loads preference data from a file
// Return type		: void 
// Argument         : FILE * hFile
//
bool CFilePref::Load( const std::string & strPathName )
{
	bool bRet = false;
	m_hFile = fopen(strPathName.c_str(), "r");
	if(m_hFile)
	{
		//size_t nSize = _filelength(m_hFile->_file);
        size_t temp = ftell(m_hFile); fseek(m_hFile, 0, SEEK_END);
        size_t nSize = ftell(m_hFile); fseek(m_hFile, temp, SEEK_SET);
		char * pBuffer = new char[nSize + 1];

		fseek(m_hFile, 0, 0);

		nSize = fread(pBuffer, sizeof(char), nSize, m_hFile);
//	pad with a linefeed incase there is no linefeed at the end of the file
		pBuffer[nSize] = et_lf_endl;

		LoadPreffDataToTree(pBuffer, nSize + 1);
		SetKey("");
		fclose(m_hFile);
		m_hFile = NULL;
		delete pBuffer;
		bRet = true;
	}
	return(bRet);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CFilePref::SaveToFile
// Description	    : Loads preference data to a file
// Return type		: void 
// Argument         : FILE * hFile
//
bool CFilePref::Save( const std::string & strPathName )
{
	bool bRet = false;

	m_hFile = fopen( strPathName.c_str(), "w");
	if(m_hFile)
	{
		SaveTree(RootKey());
		fclose(m_hFile);
		m_hFile = NULL;
		bRet = true;
	}
	return(bRet);
}

///////////////////////////////////////////////////////////////////////////////
// Function name : CFilePref::Save
// Description   : 
// Return type   : bool 
// Argument      : const std::string & strPathName
// Argument      : IPreferenceStore * pPreferences
//
bool CFilePref::Save( const std::string & strPathName, IPreferenceStore * pPreferences )
{
	bool bRet = false;

	m_hFile = fopen( strPathName.c_str(), "w");
	if(m_hFile)
	{
		SaveTree(pPreferences->CurrentKey());
		fclose(m_hFile);
		m_hFile = NULL;
		bRet = true;
	}
	return(bRet);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CFilePref::OnSaveKey
// Description	    : 
// Return type		: bool 
// Argument         : const std::string strPath
// Argument         : const SKey & refKey
//
bool CFilePref::OnSaveKey( const std::string strPath, const PreferenceStore::SKey & refKey ) const
{
	static std::string strBuffer;
	bool bRet = true;

	if(!refKey.Destroy())
	{
		strBuffer = ((!strPath.empty()) ? "\n" : "");
		strBuffer += "[" + strPath + "]\n";
		bRet = strBuffer.size() == fwrite(strBuffer.c_str(), sizeof(char), strBuffer.size(), m_hFile);
	}

	return(bRet);
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: CFilePref::OnSaveValue
// Description	    : 
// Return type		: bool 
// Argument         : const std::string strPath
// Argument         : const SKey & refKey
// Argument         : const SPreferenceValue & refValueName
// Argument         : const SPreferenceValue & refValue
//
bool CFilePref::OnSaveValue( const std::string strPath, const PreferenceStore::SKey & refKey,
		const PreferenceStore::SPreferenceValue & refValueName, const PreferenceStore::SPreferenceValue & refValue ) const
{
	static std::string strBuffer;
	bool bRet = true;

	if(!refValue.Destroy())
	{
		strBuffer = '"' + refValueName.GetDataAsText() + '"';
		strBuffer += "=";
		strBuffer += GET_PREFIX(refValue.GetType());
		strBuffer += refValue.GetDataAsText();
		strBuffer += GET_POSTFIX(refValue.GetType());
		strBuffer += "\n";
		bRet = strBuffer.size() == fwrite(strBuffer.c_str(), sizeof(char), strBuffer.size(), m_hFile);
	}

	return(bRet);
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CFilePref::LoadPreffDataToTree
// Description	    : 
// Return type		: void 
// Argument         : char * pSourceData
// Argument         : uint32 nSourceLength
//
void CFilePref::LoadPreffDataToTree( char * pSourceData, size_t nSourceLength )
{
	uint32			nPos = 0;
	parser_state	theState = et_seeking_key_name;
	SKey			*pKey = NULL;
	std::string		m_strKeyName;
	std::string		strValueName;
	std::string		strValue;
	bool			bComment = false;

	while(nPos < nSourceLength)
	{
		switch(theState)
		{
		case et_comment:
			switch(sm_mapCode[pSourceData[nPos]])
			{

			case et_cr_endl:
			case et_lf_endl:
				theState = theState.LastState();
			break;
			default:
				break;
			}
			break;
		case et_seeking_key_name:
//	search for the start of a key name
			if(et_key_name_delim_start == sm_mapCode[pSourceData[nPos]])
			{
				m_strKeyName.erase();
				theState = et_reading_key_name;
			}
			else if(et_comment_delim == pSourceData[nPos])
			{
				theState = et_comment;
			}
			break;

		case et_reading_key_name:
//	read the key name
			switch(sm_mapCode[pSourceData[nPos]])
			{
			case et_cr_endl:
			case et_lf_endl:
			case et_key_name_delim_start:
			case et_value_delim:
				assert(false);
				break;
			case et_key_name_delim_end:
				theState = et_seeking_value_name;
				Trim(m_strKeyName);
				if(!CreateKeys(pKey, m_strKeyName, RootKey(), false))
					assert(false);
				break;
			case et_print_char:
				m_strKeyName += pSourceData[nPos];
			default:
				break;
			}
			break;

		case et_seeking_value_name:
//	search for the next value or key name
			switch(sm_mapCode[pSourceData[nPos]])
			{
			case et_cr_endl:
			case et_lf_endl:
				break;
			case et_key_name_delim_start:
				m_strKeyName.erase();
				theState = et_reading_key_name;
				break;
			case et_value_delim:
				strValueName.erase();
				strValue.erase();
				theState = et_reading_value;
				break;
			default:
				if(et_comment_delim == pSourceData[nPos])
				{
					theState = et_comment;
				}
				else
				{
//	dont lose the first character
					nPos--;
					strValueName.erase();
					theState = et_reading_value_name;
				}
				break;
			}
			break;

		case et_reading_value_name:
//	reade the value name
			switch(sm_mapCode[pSourceData[nPos]])
			{
			case et_cr_endl:
			case et_lf_endl:
				theState = et_seeking_value_name;
				break;
			case et_key_name_delim_start:
			case et_key_name_delim_end:
				assert(false);
				break;
			case et_print_char:
				strValueName += pSourceData[nPos];
				break;
			case et_value_delim:
				Trim(strValueName);
				Trim(strValueName, "\"");
				theState = et_reading_value;
				break;
			default:
				break;
			}
			break;

		case et_reading_value:
//	read data
			switch(sm_mapCode[pSourceData[nPos]])
			{
			case et_cr_endl:
			case et_lf_endl:
				{
					SPreferenceValue objValue;
					
					Trim(strValue);
					ConvertToValue(objValue, strValue);
					pKey->AddValue(strValueName, objValue);
					theState = et_seeking_value_name;
					strValue.erase();
				}
				break;
			case et_key_name_delim_start:
			case et_key_name_delim_end:
			case et_value_delim:
			case et_print_char:
				strValue += pSourceData[nPos];
				break;
			default:
				break;
			}
			break;
		};
		nPos++;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: CFilePref::ConvertToValue
// Description	    : Converts the data in strData to an SPreferenceValue object
// Return type		: void 
// Argument         : SPreferenceValue & refValue
// Argument         : const std::string & strData
//
void CFilePref::ConvertToValue( PreferenceStore::SPreferenceValue & refValue,
							   const std::string & strData )
{
	static std::string strBuffer;

	if(!strData.empty())
	{
		if(strData[0] == '\"')
		{
			strBuffer = strData;
			Trim(strBuffer, "\"");
			refValue.SetData(strBuffer);
		}
		else
		{
			char		*pBuffer = new char[strData.size() + 1];
			char		*pTemp;
			std::string	strToken;
			std::string	strValue;
			SPreferenceValue::ValueType	eType = SPreferenceValue::kUndefinedType;

			strcpy(pBuffer, strData.c_str());

			pTemp = strtok(pBuffer, ":");
			//strlwr(pTemp);
			strToken = pTemp;
			strToken += ':';

			if(0 == strncmp(strToken.c_str(), GET_PREFIX(SPreferenceValue::kStringType), strToken.size()))
				eType = SPreferenceValue::kStringType;
			else if(0 == strcmp(strToken.c_str(), GET_PREFIX(SPreferenceValue::kUnsignedIntegerType)))
				eType = SPreferenceValue::kUnsignedIntegerType;
			else if(0 == strcmp(strToken.c_str(), GET_PREFIX(SPreferenceValue::kBinaryType)))
				eType = SPreferenceValue::kBinaryType;
			else if(0 == strcmp(strToken.c_str(), GET_PREFIX(SPreferenceValue::kSignedIntegerType)))
				eType = SPreferenceValue::kSignedIntegerType;
			else if(0 == strcmp(strToken.c_str(), GET_PREFIX(SPreferenceValue::kFloatType)))
				eType = SPreferenceValue::kFloatType;
			else if(0 == strcmp(strToken.c_str(), GET_PREFIX(SPreferenceValue::kVector3Type)))
				eType = SPreferenceValue::kVector3Type;
			else if(0 == strcmp(strToken.c_str(), GET_PREFIX(SPreferenceValue::kVector4Type)))
				eType = SPreferenceValue::kVector4Type;
			else if(0 == strcmp(strToken.c_str(), GET_PREFIX(SPreferenceValue::kRGBAColorType)))
				eType = SPreferenceValue::kRGBAColorType;
			
			if(eType == SPreferenceValue::kUndefinedType)
			{
				eType = SPreferenceValue::kStringType;
				strValue = strData.c_str();
			}
			else
			{
				strValue = strData.substr(strToken.size());
			}


			if(eType == SPreferenceValue::kStringType)
				Trim(strValue, "\"");

			refValue.SetRawData(strValue.empty() ? "" : strValue, eType);
			delete[] pBuffer;
		}
	}
	refValue.SetDirty(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CFilePref::Trim
// Description	    : Removes any leading and trailing values in strRemove from strToTrim
// Return type		: void 
// Argument         : std::string & strToTrim
// Argument         : const std::string & strRemove
//
void CFilePref::Trim( std::string & strToTrim, const std::string & strRemove /*= " "*/ )
{
	std::string::size_type nBegin = strToTrim.find_first_not_of(strRemove);
	std::string::size_type nEnd = strToTrim.find_last_not_of(strRemove);
	if(std::string::npos == nBegin)
	{
		strToTrim.erase();
	}
	else
	{
		nEnd = std::string::npos == nEnd ? 0 : nEnd;
		strToTrim = strToTrim.substr( nBegin, nEnd - nBegin + 1);
	}
}
