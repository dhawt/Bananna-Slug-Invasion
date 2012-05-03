/////////////////////////////////////////////////////////////////////////////
//      created:        1/24/2002
//      filename:       PreferenceStore.cpp
//      author:         Eli Pulsifer
//
//      purpose:        

#include "stdafx.h"
#include "defines.h"
#include "PreferenceStore.h"
#include <algorithm>

using namespace PreferenceStore;


bool IsValueDestroyed(const std::pair<SPreferenceValue, SPreferenceValue>& p)
{
	return p.second.Destroy();
};
bool IsKeyDestroyed(const std::pair<std::string, SKey*>& p)
{
	return p.second->Destroy();
};

template <class AssociativeContainer, class Predicate>
void remove_if(AssociativeContainer& C, Predicate p)
{
	typedef typename AssociativeContainer::iterator iterator;

	iterator cur = C.begin();
	const iterator last = C.end();

	while ((cur = std::find_if(cur, last, p)) != last)
	{
		iterator tmp = cur++;
		C.erase(tmp);
	}
}

//////////////////////////////////////////////////////////////////////
//	SKey implementation
//////////////////////////////////////////////////////////////////////

SKey::SKey(SKey* pParent, const std::string& strName, bool bDirty)
	: m_pParentKey(pParent)
	, m_bDirty(bDirty)
	, m_bDestroy(false)
	, m_strKeyName(strName)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

const std::string & SKey::Name() const
{
	return m_strKeyName;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SKey::SetDirty(bool bDirty)
{
	m_bDirty = bDirty;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool SKey::Dirty() const
{
	return m_bDirty;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SKey::SetDestroy(bool bDestroy)
{
	bDestroy ? m_bDirty = m_bDestroy = bDestroy : m_bDestroy = bDestroy;
}

bool SKey::Destroy() const
{
	return m_bDestroy;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SKey::AddValue(const std::string& strValueName, const SPreferenceValue& refValue, bool bNameDirty)
{
	SPreferenceValue valueName;

	valueName.SetData(strValueName);
	valueName.SetDirty(bNameDirty);
	m_mapData.insert(valuemap::value_type(strValueName, refValue));
}

//////////////////////////////////////////////////////////////////////
// Function name	: NumSubKeys
// Description		: 
// Return type		: uint32 void 
//
size_t SKey::NumSubKeys() const
{
	return m_mapSubKeys.size();
}

//////////////////////////////////////////////////////////////////////
// Function name	: NumValues
// Description		: 
// Return type		: uint32 void 
//
size_t SKey::NumValues() const
{
	return m_mapData.size();
}

//////////////////////////////////////////////////////////////////////
//	Key Iterator implementation
//////////////////////////////////////////////////////////////////////

IPreferenceStore::KeyIterator::KeyIterator()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

IPreferenceStore::KeyIterator::KeyIterator(keymap::iterator iKey)
{
	m_iKey = iKey;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

IPreferenceStore::KeyIterator::operator std::string() const
{
	return (*m_iKey).second->Name();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

const IPreferenceStore::KeyIterator& IPreferenceStore::KeyIterator::operator ++(int)
{
	m_iKey++;
	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool IPreferenceStore::KeyIterator::operator == (const IPreferenceStore::KeyIterator &iKey) const
{
	return m_iKey == iKey.m_iKey;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool IPreferenceStore::KeyIterator::operator != (const IPreferenceStore::KeyIterator &iKey) const
{
	return m_iKey != iKey.m_iKey;
}

//////////////////////////////////////////////////////////////////////
//	Value Iterator implementation
//////////////////////////////////////////////////////////////////////

IPreferenceStore::ValueIterator::ValueIterator()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

IPreferenceStore::ValueIterator::ValueIterator(valuemap::iterator iValue)
{
	m_iValue = iValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

IPreferenceStore::ValueIterator::operator SPreferenceValue() const
{
	return (*m_iValue).second;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
IPreferenceStore::ValueIterator::operator std::string() const
{
	return (*m_iValue).first.GetDataAsText();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

const IPreferenceStore::ValueIterator& IPreferenceStore::ValueIterator::operator ++(int)
{
	m_iValue++;
	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool IPreferenceStore::ValueIterator::operator == (const IPreferenceStore::ValueIterator &iValue) const
{
	return m_iValue == iValue.m_iValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool IPreferenceStore::ValueIterator::operator != (const IPreferenceStore::ValueIterator &iValue) const
{
	return m_iValue != iValue.m_iValue;
}

//////////////////////////////////////////////////////////////////////
// CPreferenceStore implementation
//////////////////////////////////////////////////////////////////////

CPreferenceStore::CPreferenceStore()
	: m_pRootKey(NULL)
	, m_etValueNameType(SPreferenceValue::kStringType)
	, m_pCurrentKey(NULL)
	, m_bAutoDelete(true)
{
	m_pRootKey = new SKey(NULL, "", false);
	m_pCurrentKey = m_pRootKey;
}

/////////////////////////////////////////////////////////////////////////////

CPreferenceStore::CPreferenceStore(SKey & rootKey)
	: m_pRootKey(NULL)
	, m_etValueNameType(SPreferenceValue::kStringType)
	, m_pCurrentKey(NULL)
	, m_bAutoDelete(false)
{
	m_pRootKey = &rootKey;
	m_pCurrentKey = m_pRootKey;
}

/////////////////////////////////////////////////////////////////////////////

CPreferenceStore::~CPreferenceStore()
{
	if(m_bAutoDelete)
	{
		FreeSubkeys(*m_pRootKey);
		delete m_pRootKey;
	}
	m_pCurrentKey = NULL;
	m_pRootKey = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::FreeSubkeys
// Description	    : Destroyes all the subkeys of pKey
// Return type		: void 
// Argument         : SKey * pKey
//
void CPreferenceStore::FreeSubkeys(PreferenceStore::SKey& refKey)
{
	while(refKey.m_mapSubKeys.begin() != refKey.m_mapSubKeys.end())
	{
		FreeSubkeys(*(*refKey.m_mapSubKeys.begin()).second);
		delete (*refKey.m_mapSubKeys.begin()).second;
		refKey.m_mapSubKeys.erase(refKey.m_mapSubKeys.begin());
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::DestroyValuesAndSubkeys
// Description	    : 
// Return type		: void 
// Argument         : SKey * pKey
//
void CPreferenceStore::DestroyValuesAndSubkeys(PreferenceStore::SKey& refKey)
{
	keymap::const_iterator iK;
	valuemap::iterator iV;

//	mark all sub keys 
	for (iK = refKey.m_mapSubKeys.begin() ; iK != refKey.m_mapSubKeys.end() ; iK++)
	{
		DestroyValuesAndSubkeys(*(*iK).second);

		(*iK).second->SetDirty();
		(*iK).second->SetDestroy();
	}

//	mark all values
	for (iV = refKey.m_mapData.begin() ; iV != refKey.m_mapData.end() ; iV++)
	{
		(*iV).second.SetDestroy();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::CreateKeys
// Description	    : Creates a key tree that matches the privide path std::string using m_pParentKey as the root
// Return type		: SKey * 
// Argument         : const std::string& m_strKeyName
// Argument         : SKey& refParentKey
//
bool CPreferenceStore::CreateKeys(SKey *& pNewKey, const std::string& m_strKeyName, SKey& refParentKey, bool bDirty)
{
	bool		bRet = false;
	std::string	strNewKeyName	= m_strKeyName.substr(0, m_strKeyName.find('\\'));
	SKey		*pCurrentKey = NULL;

	if(!strNewKeyName.empty())
	{
		keymap::iterator i = refParentKey.m_mapSubKeys.find(strNewKeyName);

//	if no key is found create a new one
		if(refParentKey.m_mapSubKeys.end() == i)
		{
			pCurrentKey = new SKey(&refParentKey, strNewKeyName, bDirty);
			refParentKey.m_mapSubKeys.insert(keymap::value_type(strNewKeyName, pCurrentKey));
			pNewKey = pCurrentKey;
			bRet = true;
		}
//	if one exists clear the destroy flag if its set
		else
		{
			pCurrentKey = (*i).second;
			if((*i).second->Destroy())
			{
				pCurrentKey->SetDestroy(false);
				pNewKey = pCurrentKey;
				bRet = true;
			}
		}
//	if this is not the last key, create the sub keys
		if(strNewKeyName != m_strKeyName)
		{
			bRet = CreateKeys(pNewKey, m_strKeyName.substr(m_strKeyName.find('\\') + 1, m_strKeyName.size() - 1), *pCurrentKey);
		}
	}

	return(bRet);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::GetPathToKey
// Description	    : Gets the path to the provided key
// Return type		: void 
// Argument         : std::string& strPath
// Argument         : const SKey& refKey
//
void CPreferenceStore::GetPathToKey(std::string& strPath, const SKey& refKey)
{
	if(refKey.m_pParentKey)
	{
		BuildPathToKey(strPath, *refKey.m_pParentKey);
		strPath += refKey.m_strKeyName;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::BuildPathToKey
// Description	    : Recursively builds the path to the provided key
// Return type		: void 
// Argument         : std::string& strPath
// Argument         : const SKey& refKey
//
void CPreferenceStore::BuildPathToKey(std::string& strPath, const SKey& refKey)
{
	if(refKey.m_pParentKey)
	{
		BuildPathToKey(strPath, *refKey.m_pParentKey);
		strPath += refKey.m_strKeyName + '\\';
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::FindKey
// Description	    : Finds the key that mathces the path / name m_strKeyName
// Return type		: SKey * 
// Argument         : const std::string& m_strKeyName
// Argument         : SKey * m_pParentKey
//
bool CPreferenceStore::FindKey(PreferenceStore::SKey *& pKey, std::string& strName, const std::string& m_strKeyName,
			PreferenceStore::SKey& refParentKey)
{
	std::string	strNewKeyName	= m_strKeyName.substr(0, m_strKeyName.find('\\'));
	bool bResult = false;

	if(!strNewKeyName.empty())
	{
		keymap::iterator i = refParentKey.m_mapSubKeys.find(strNewKeyName);
//	if we find the key and it has not been marked to destroy continue our search
		if(refParentKey.m_mapSubKeys.end() != i && !(*i).second->Destroy())
		{
			if(strNewKeyName != m_strKeyName)
			{
				bResult = FindKey(pKey, strName, m_strKeyName.substr(m_strKeyName.find('\\') + 1, m_strKeyName.size() - 1), *(*i).second);
			}
			else
			{
				pKey = (*i).second;
				strName = m_strKeyName;
				bResult = true;
			}
		}
	}

	return(bResult);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::SetKey
// Description	    : Sets the current working key (location in the key tree)
// Return type		: bool 
// Argument         : const std::string& m_strKeyName
//
bool CPreferenceStore::SetKey(const std::string& m_strKeyName)
{
	std::string strActiveKeyName;
	SKey * pNewActiveKey;
	bool bRet = false;
	
	if(m_strKeyName.empty())
	{
		m_pCurrentKey = m_pRootKey;
		bRet = true;
	}
	else
	{
		if(FindKey(pNewActiveKey, strActiveKeyName, m_strKeyName, *m_pRootKey))
		{
			m_pCurrentKey = pNewActiveKey;
			bRet = true;
		}
	}

	return(bRet);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::PushKey
// Description	    : 
// Return type		: bool 
// Argument         : const std::string& m_strKeyName
//
bool CPreferenceStore::PushKey(const std::string& m_strKeyName)
{
	bool bRet = false;

	keymap::iterator i = m_pCurrentKey->m_mapSubKeys.find(m_strKeyName);
	if(i != m_pCurrentKey->m_mapSubKeys.end() )
	{
		m_pCurrentKey = (*i).second;
		bRet = true;
	}

	return bRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::PopKey
// Description	    : 
// Return type		: bool 
// Argument         : void
//
bool CPreferenceStore::PopKey(void)
{
	bool bRet = false;

	if(m_pCurrentKey != m_pRootKey)
	{
		m_pCurrentKey = m_pCurrentKey->m_pParentKey;
		bRet = true;
	}
	return bRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::IsChildKey
// Description	    : 
// Return type		: bool 
// Argument         : const std::string& m_strKeyName
//
bool CPreferenceStore::IsChildKey(const std::string& m_strKeyName)
{
	bool bRet = false;

	keymap::iterator i = m_pCurrentKey->m_mapSubKeys.find(m_strKeyName);
	bRet = i != m_pCurrentKey->m_mapSubKeys.end();

	return bRet;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::CreateKey
// Description	    : Creates a key tree that matches the privide path std::string using the main root key as the root
// Return type		: bool 
// Argument         : const std::string& m_strKeyName
//
bool CPreferenceStore::CreateKey(const std::string& m_strKeyName)
{
	SKey *pNewKey;
	return(CreateKeys(pNewKey, m_strKeyName, *m_pCurrentKey));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::GetKey
// Description	    : Gets the name of the current working key
// Return type		: bool 
// Argument         : std::string& m_strKeyName
//
bool CPreferenceStore::GetKey(std::string& m_strKeyName) const
{
	if(m_pRootKey != m_pCurrentKey)
		m_strKeyName = m_pCurrentKey->m_strKeyName;

	return m_pRootKey != m_pCurrentKey;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::GetValue
// Description	    : Gets the value associated with strValueName in the current working key
// Return type		: bool 
// Argument         : std::string& strValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::GetValue(std::string& strValue, const std::string& strValueName) const
{
	SPreferenceValue objValue;
	bool bRet = false;
	
	if(GetValue(objValue, strValueName))
	{
		bRet = objValue.GetData(strValue);
	}
	return(bRet);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::SetValue
// Description	    : Sets the value associated with strValueName in the current working key
// Return type		: bool 
// Argument         : const std::string& strValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::SetValue(const std::string& strValue, const std::string& strValueName)
{
	SPreferenceValue objValue(strValue);

	return(SetValue(objValue, strValueName));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::GetValue
// Description	    : Gets the value associated with strValueName in the current working key
// Return type		: bool 
// Argument         : uint32& dwValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::GetValue(uint32& dwValue, const std::string& strValueName) const
{
	SPreferenceValue objValue;
	bool bRet = false;
	
	if(GetValue(objValue, strValueName))
	{
		bRet = objValue.GetData(dwValue);
	}
	return(bRet);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::SetValue
// Description	    : Sets the value associated with strValueName in the current working key
// Return type		: bool 
// Argument         : uint32 dwValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::SetValue(uint32 dwValue, const std::string& strValueName)
{
	SPreferenceValue objValue(dwValue);

	return(SetValue(objValue, strValueName));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::GetValue
// Description	    : Gets the value associated with strValueName in the current working key
// Return type		: bool 
// Argument         : void * pValue
// Argument         : uint32& nLength
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::GetValue(void * pValue, uint32& nLength, const std::string& strValueName) const
{
	SPreferenceValue objValue;
	bool bRet = false;
	
	if(GetValue(objValue, strValueName))
	{
		bRet = objValue.GetData(pValue, nLength);
	}
	return(bRet);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::SetValue
// Description	    : Sets the value associated with strValueName in the current working key
// Return type		: bool 
// Argument         : const void * pValue
// Argument         : uint32 nLength
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::SetValue(const void * pValue, uint32 nLength, const std::string& strValueName)
{
	SPreferenceValue objValue(pValue, nLength);

	return(SetValue(objValue, strValueName));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::GetValue
// Description	    : 
// Return type		: bool 
// Argument         : int32& nValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::GetValue(int32& nValue, const std::string& strValueName) const
{
	SPreferenceValue objValue;
	bool bRet = false;
	
	if(GetValue(objValue, strValueName))
	{
		bRet = objValue.GetData(nValue);
	}
	return(bRet);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::SetValue
// Description	    : 
// Return type		: bool 
// Argument         : int32 nValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::SetValue(int32 nValue, const std::string& strValueName)
{
	SPreferenceValue objValue(nValue);

	return(SetValue(objValue, strValueName));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::GetValue
// Description	    : 
// Return type		: bool 
// Argument         : float32& fValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::GetValue(float32& fValue, const std::string& strValueName) const
{
	SPreferenceValue objValue;
	bool bRet = false;
	
	if(GetValue(objValue, strValueName))
	{
		bRet = objValue.GetData(fValue);
	}
	return(bRet);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::SetValue
// Description	    : 
// Return type		: bool 
// Argument         : float32 fValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::SetValue(float32 fValue, const std::string& strValueName)
{
	SPreferenceValue objValue(fValue);

	return(SetValue(objValue, strValueName));
}



#if defined(DIRECT3D_VERSION)
///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::GetValue
// Description	    : 
// Return type		: bool 
// Argument         : D3DXVECTOR3 &vecValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::GetValue(D3DXVECTOR3 &vecValue, const std::string& strValueName) const
{
	SPreferenceValue objValue;
	bool bRet = false;
	
	if(GetValue(objValue, strValueName))
	{
		bRet = objValue.GetData(vecValue);
	}
	return(bRet);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::SetValue
// Description	    : 
// Return type		: bool 
// Argument         : const D3DXVECTOR3 &vecValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::SetValue(const D3DXVECTOR3 &vecValue, const std::string& strValueName)
{
	SPreferenceValue objValue(vecValue);

	return(SetValue(objValue, strValueName));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::GetValue
// Description	    : 
// Return type		: bool 
// Argument         : D3DXVECTOR4 &vecValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::GetValue(D3DXVECTOR4 &vecValue, const std::string& strValueName) const
{
	SPreferenceValue objValue;
	bool bRet = false;
	
	if(GetValue(objValue, strValueName))
	{
		bRet = objValue.GetData(vecValue);
	}
	return(bRet);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::SetValue
// Description	    : 
// Return type		: bool 
// Argument         : const D3DXVECTOR4 &vecValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::SetValue(const D3DXVECTOR4 &vecValue, const std::string& strValueName)
{
	SPreferenceValue objValue(vecValue);

	return(SetValue(objValue, strValueName));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::GetValue
// Description	    : 
// Return type		: bool 
// Argument         : D3DXCOLOR &colorValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::GetValue(D3DXCOLOR &colorValue, const std::string& strValueName) const
{
	SPreferenceValue objValue;
	bool bRet = false;
	
	if(GetValue(objValue, strValueName))
	{
		bRet = objValue.GetData(colorValue);
	}
	return(bRet);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::SetValue
// Description	    : 
// Return type		: bool 
// Argument         : const D3DXCOLOR &colorValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::SetValue(const D3DXCOLOR &colorValue, const std::string& strValueName)
{
	SPreferenceValue objValue(colorValue);

	return(SetValue(objValue, strValueName));
}
#endif	//	defined(DIRECT3D_VERSION)
///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::CreateValue
// Description	    : Creates a value with the name strValueName in the current working key
// Return type		: bool 
// Argument         : const std::string& strValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::CreateValue(const std::string& strValue, const std::string& strValueName)
{
	SPreferenceValue objNewValue(strValue);

	return(CreateValue(objNewValue, strValueName));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::CreateValue
// Description	    : Creates a value with the name strValueName in the current working key
// Return type		: bool 
// Argument         : uint32 dwValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::CreateValue(uint32 dwValue, const std::string& strValueName)
{
	SPreferenceValue objNewValue(dwValue);

	return(CreateValue(objNewValue, strValueName));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::CreateValue
// Description	    : 
// Return type		: bool 
// Argument         : float32 fValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::CreateValue(float32 fValue, const std::string& strValueName)
{
	SPreferenceValue objNewValue(fValue);

	return(CreateValue(objNewValue, strValueName));
}

#if defined(DIRECT3D_VERSION)
///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::CreateValue
// Description	    : 
// Return type		: bool 
// Argument         : const D3DXVECTOR3 &vecValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::CreateValue(const D3DXVECTOR3 &vecValue, const std::string& strValueName)
{
	SPreferenceValue objNewValue(vecValue);

	return(CreateValue(objNewValue, strValueName));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::CreateValue
// Description	    : 
// Return type		: bool 
// Argument         : const D3DXVECTOR4 &vecValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::CreateValue(const D3DXVECTOR4 &vecValue, const std::string& strValueName)
{
	SPreferenceValue objNewValue(vecValue);

	return(CreateValue(objNewValue, strValueName));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::CreateValue
// Description	    : 
// Return type		: bool 
// Argument         : const D3DXCOLOR &colorValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::CreateValue(const D3DXCOLOR &colorValue, const std::string& strValueName)
{
	SPreferenceValue objNewValue(colorValue);

	return(CreateValue(objNewValue, strValueName));
}
#endif	//	defined(DIRECT3D_VERSION)

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::CreateValue
// Description	    : 
// Return type		: bool 
// Argument         : int32 nValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::CreateValue(int32 nValue, const std::string& strValueName)
{
	SPreferenceValue objNewValue(nValue);

	return(CreateValue(objNewValue, strValueName));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::CreateValue
// Description	    : Creates a value with the name strValueName in the current working key
// Return type		: bool 
// Argument         : const void * pValue
// Argument         : uint32 nLength
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::CreateValue(const void * pValue, uint32 nLength, const std::string& strValueName)
{
	SPreferenceValue objNewValue(pValue, nLength);

	return(CreateValue(objNewValue, strValueName));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::DestroyValue
// Description	    : Destroy the value with the name strValueName in the current working key
// Return type		: bool 
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::DestroyValue(const std::string& strValueName)
{
	bool bRet = false;
	if(m_pRootKey != m_pCurrentKey)
	{
		valuemap::iterator i = m_pCurrentKey->m_mapData.find(strValueName);
//	if the value exists and has not been marked to destroy we can set the destroy flag
		bRet = (i != m_pCurrentKey->m_mapData.end()) && !(*i).second.Destroy();

		if(bRet)
		{
			(*i).second.SetDestroy();
//			m_pCurrentKey->m_mapData.erase(i);
		}
	}
	return(bRet);
}

///////////////////////////////////////////////////////////////////////////////
// Function name : CPreferenceStore::ValueBegin
// Description   : 
// Return type   : IPreferenceStore::ValueIterator 
//
IPreferenceStore::ValueIterator CPreferenceStore::ValueBegin() const
{
	return ValueIterator(m_pCurrentKey->m_mapData.begin());
}

///////////////////////////////////////////////////////////////////////////////
// Function name : CPreferenceStore::ValueEnd
// Description   : 
// Return type   : IPreferenceStore::ValueIterator 
//
IPreferenceStore::ValueIterator CPreferenceStore::ValueEnd() const
{
	return ValueIterator(m_pCurrentKey->m_mapData.end());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::DestroyKey
// Description	    : Destroys the the named key and all sub keys
// Return type		: bool 
// Argument         : const std::string& m_strKeyName
//
bool CPreferenceStore::DestroyKey(const std::string& m_strKeyName)
{
	std::string strKeyToDestroyName;
	SKey * pKeyToDestroy;

//	mark the key to destroy if we can find it and its not the root
	bool bRet = FindKey(pKeyToDestroy, strKeyToDestroyName, m_strKeyName, *m_pRootKey) ? (pKeyToDestroy != m_pRootKey) : false;

	if(bRet)
	{
		pKeyToDestroy->SetDirty();
		pKeyToDestroy->SetDestroy();
		DestroyValuesAndSubkeys(*pKeyToDestroy);
	}

	return(bRet);
}

//////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::NumSubKeys
// Description		: 
// Return type		: uint32 
//
size_t CPreferenceStore::NumSubKeys() const


{
	return m_pCurrentKey->NumSubKeys();
}

//////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::NumValues
// Description		: 
// Return type		: uint32 
//
size_t CPreferenceStore::NumValues() const
{
	return m_pCurrentKey->NumValues();
}

//////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::KeyBegin
// Description		: 
// Return type		: IPreferenceStore::KeyIterator 
//
IPreferenceStore::KeyIterator CPreferenceStore::KeyBegin() const
{
	return KeyIterator(m_pCurrentKey->m_mapSubKeys.begin());
}

//////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::KeyEnd
// Description		: 
// Return type		: IPreferenceStore::KeyIterator 
//
IPreferenceStore::KeyIterator CPreferenceStore::KeyEnd() const
{
	return KeyIterator(m_pCurrentKey->m_mapSubKeys.end());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::SetValue
// Description	    : Sets an SPreferenceValue object in the working key if it exist
// Return type		: bool 
// Argument         : const std::string& strValue
// Argument         : value_type eType
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::SetValue(SPreferenceValue& refValue, const std::string& strValueName)
{
	bool bRet = false;

	if(m_pRootKey != m_pCurrentKey)
	{
		valuemap::iterator i = m_pCurrentKey->m_mapData.find(strValueName);
//	if the value exists and has not been marked to destroy we can set the value
		bRet = (i != m_pCurrentKey->m_mapData.end()) && !(*i).second.Destroy();

		if(bRet)
		{
			(*i).second = refValue;
		}
	}
	return(bRet);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::GetValue
// Description	    : Gets an SPreferenceValue object from the working key if it exist
// Return type		: bool 
// Argument         : std::string& strValue
// Argument         : value_type eType
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::GetValue(SPreferenceValue& refValue, const std::string& strValueName) const
{
	bool bRet = false;

	if(m_pRootKey != m_pCurrentKey)
	{
		valuemap::iterator i = m_pCurrentKey->m_mapData.find(strValueName);
//	if the value exists and has not been marked to destroy we can get the value
		bRet = (i != m_pCurrentKey->m_mapData.end()) && !(*i).second.Destroy();

		if(bRet)
		{
			refValue = (*i).second;
		}
	}
	return(bRet);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::CreateValue
// Description	    : Adds an SPreferenceValue object to the working key if it does not already exist 
// Return type		: bool 
// Argument         : SPreferenceValue& refValue
// Argument         : const std::string& strValueName
//
bool CPreferenceStore::CreateValue(SPreferenceValue& refValue, const std::string& strValueName)
{
	bool bRet = false;

	if(m_pRootKey != m_pCurrentKey)
	{
		valuemap::iterator i = m_pCurrentKey->m_mapData.find(strValueName);
//	if the value does not exist or has been marked to destroy we can add the new value
		bRet = (i == m_pCurrentKey->m_mapData.end()) || (*i).second.Destroy();

		if(bRet)
		{
			m_pCurrentKey->m_mapData[SPreferenceValue(strValueName, m_etValueNameType)] = refValue;

		}
	}
	return(bRet);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::CurrentKey
// Description	    : 
// Return type		: SKey & 
// Argument         : void
//
SKey& CPreferenceStore::CurrentKey()
{
	return(*m_pCurrentKey);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::RootKey
// Description	    : 
// Return type		: SKey & 
// Argument         : void
//
SKey& CPreferenceStore::RootKey()
{
	return(*m_pRootKey);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::SaveTree
// Description	    : 
// Return type		: void 
// Argument         : SKey * pKey
// Argument         : const std::string& m_strKeyName
//
bool CPreferenceStore::SaveTree(SKey& refKey)
{
	bool bRet = true;
	std::string strPath;
	valuemap::iterator iV;
	keymap::iterator iK;

//	if key is marked destroy, dont save
	if(refKey.Destroy())
		return true;
	
	GetPathToKey(strPath, refKey);

//	Save the Key
	if(!strPath.empty())
	{
		bRet = OnSaveKey(strPath, refKey);
	}

	if(bRet)
	{
//	Save the Value name / value pairs
		for (iV = refKey.m_mapData.begin() ; bRet && iV != refKey.m_mapData.end() ; iV++)
		{
			if(!(*iV).second.Destroy())
			{
				bRet = OnSaveValue(strPath, refKey, (*iV).first, (*iV).second);
			}
		}

//	Recurs sub keys
		for (iK = refKey.m_mapSubKeys.begin() ; bRet && iK != refKey.m_mapSubKeys.end() ; iK++)
		{
			bRet = SaveTree(*(*iK).second);
		}
	}
	return(bRet);
}

///////////////////////////////////////////////////////////////////////////////
// Function name   : CPreferenceStore::Purge
// Description     : 
// Return type     : void 
//
void CPreferenceStore::Purge()
{
	Purge(m_pRootKey);
}
void CPreferenceStore::Purge(SKey* pKey)
{
	keymap::iterator iK;

	//	Execute the destroy on marked keys and values
	remove_if(pKey->m_mapSubKeys, IsKeyDestroyed);
	remove_if(pKey->m_mapData, IsValueDestroyed);

	//	Recurs to sub keys
	for (iK = pKey->m_mapSubKeys.begin() ; iK != pKey->m_mapSubKeys.end() ; ++iK)
	{
		Purge((*iK).second);
	}
}
///////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::OnSaveKey
// Description	    : 
// Return type		: bool 
// Argument         : const std::string strPath
// Argument         : const SKey& refKey
//
bool CPreferenceStore::OnSaveKey(const std::string strPath, const SKey& refKey) const
{
	return false;
}

///////////////////////////////////////////////////////////////////////////////
// Function name	: CPreferenceStore::OnSaveValue
// Description	    : 
// Return type		: bool 
// Argument         : const std::string strPath
// Argument         : const SKey& refKey
// Argument         : const SPreferenceValue& refValueName
// Argument         : const SPreferenceValue& refValue
//
bool CPreferenceStore::OnSaveValue(const std::string strPath, const SKey& refKey, const SPreferenceValue& refValueName, const SPreferenceValue& refValue) const
{
	return false;
}

///////////////////////////////////////////////////////////////////////////////
// Function name   : CPreferenceStore::Insert
// Description     : 
// Return type     : bool 
// Argument        : IPreferenceStore &ref
//
bool CPreferenceStore::Insert(IPreferenceStore &ref)
{
	return Insert(&ref.CurrentKey());

}

///////////////////////////////////////////////////////////////////////////////
// Function name   : CPreferenceStore::Insert
// Description     : 
// Return type     : bool 
// Argument        : PreferenceStore::SKey* pKey
//
bool CPreferenceStore::Insert(PreferenceStore::SKey* pKey)
{
	valuemap::iterator iV;
	keymap::iterator iK;

	for(iV = pKey->m_mapData.begin() ; iV != pKey->m_mapData.end() ; ++iV)
	{
		if(!(*iV).second.Destroy())
			CreateValue((*iV).second, static_cast<const char*>((*iV).first));
	}

	for (iK = pKey->m_mapSubKeys.begin() ; iK != pKey->m_mapSubKeys.end() ; ++iK)
	{
		if(!(*iK).second->Destroy())
		{
			CreateKey((*iK).first);
			PushKey((*iK).first);
			Insert((*iK).second);
			PopKey();
		}
	}
	return true;
}
