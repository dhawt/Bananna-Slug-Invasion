/////////////////////////////////////////////////////////////////////////////
//      created:        1/24/2002
//      filename:       PreferenceStore.h
//      author:         Eli Pulsifer
//
//      purpose:        

#if !defined _PREFERENCESTORE_H
#define _PREFERENCESTORE_H

#if _MSC_VER > 1000
#pragma once
#pragma warning ( disable : 4786 )
#endif // _MSC_VER > 1000

#include "PreferenceValue.h"
#include <map>


namespace PreferenceStore
{
	class CPreferenceStore;
	struct SKey;




	typedef std::map<SPreferenceValue, SPreferenceValue>		valuemap;
	typedef std::map<std::string, SKey*>	keymap;

	struct SKey
	{
		friend CPreferenceStore;
		SKey(SKey* pParent, const std::string& strName, bool bDirty);

		const std::string & Name() const;
		bool Dirty() const;
		bool Destroy() const;
		void AddValue(const std::string& strValueName, const SPreferenceValue& refValue, bool bNameDirty = false);
		size_t NumSubKeys() const;
		size_t NumValues() const;

	protected:
		void SetDirty(bool bDirty = true);
		void SetDestroy(bool bDestroy = true);

		valuemap	m_mapData;
		keymap		m_mapSubKeys;
		std::string	m_strKeyName;
		SKey*		m_pParentKey;
		bool		m_bDirty;
		bool		m_bDestroy;
	};
}

struct IPreferenceStore
{
	struct KeyIterator
	{
		KeyIterator();
		KeyIterator(PreferenceStore::keymap::iterator iKey);
		operator std::string() const;
		const KeyIterator& operator ++( int );
		bool operator == (const KeyIterator &iKey) const;
		bool operator != (const KeyIterator &iKey) const;
	private:
		PreferenceStore::keymap::iterator m_iKey;
	};

	struct ValueIterator
	{
		ValueIterator();
		ValueIterator(PreferenceStore::valuemap::iterator iValue);
		operator std::string() const;
		operator PreferenceStore::SPreferenceValue() const;
		const ValueIterator& operator ++( int );
		bool operator == (const ValueIterator &iValue) const;
		bool operator != (const ValueIterator &iValue) const;
	private:
		PreferenceStore::valuemap::iterator m_iValue;
	};
	
	virtual ~IPreferenceStore(){}


//	Key manipulation functions
	virtual bool GetKey(std::string& m_strKeyName) const = 0;
	virtual bool SetKey(const std::string& m_strKeyName) = 0;
	virtual bool PushKey(const std::string& m_strKeyName) = 0;
	virtual bool PopKey(void) = 0;
	virtual bool IsChildKey(const std::string& m_strKeyName) = 0;
	virtual bool CreateKey(const std::string& m_strKeyName) = 0;
	virtual bool DestroyKey(const std::string& m_strKeyName) = 0;
	virtual void Purge() = 0;
	virtual size_t NumSubKeys() const = 0;
	virtual size_t NumValues() const = 0;
	virtual KeyIterator KeyBegin() const = 0;
	virtual KeyIterator KeyEnd() const = 0;
	virtual bool Insert(IPreferenceStore &ref) = 0;

//	Value manipulation functions
	virtual bool GetValue(std::string& strValue, const std::string& strValueName) const = 0;
	virtual bool SetValue(const std::string& strValue, const std::string& strValueName) = 0;
	virtual bool CreateValue(const std::string& strValue, const std::string& strValueName) = 0;

	virtual bool GetValue(uint32& dwValue, const std::string& strValueName) const = 0;
	virtual bool SetValue(uint32 dwValue, const std::string& strValueName) = 0;
	virtual bool CreateValue(uint32 dwValue, const std::string& strValueName) = 0;

	virtual bool GetValue(int32& nValue, const std::string& strValueName) const = 0;
	virtual bool SetValue(int32 nValue, const std::string& strValueName) = 0;
	virtual bool CreateValue(int32 nValue, const std::string& strValueName) = 0;

	virtual bool GetValue(float32& fValue, const std::string& strValueName) const = 0;
	virtual bool SetValue(float32 fValue, const std::string& strValueName) = 0;
	virtual bool CreateValue(float32 fValue, const std::string& strValueName) = 0;
	
	virtual bool GetValue(PreferenceStore::SPreferenceValue& refValue, const std::string& strValueName) const = 0;
	virtual bool SetValue(PreferenceStore::SPreferenceValue& refValue, const std::string& strValueName) = 0;
	virtual bool CreateValue(PreferenceStore::SPreferenceValue& refValue, const std::string& strValueName) = 0;

//	DirectX math and color types
#if defined(DIRECT3D_VERSION)
	virtual bool GetValue(D3DXVECTOR4& vValue, const std::string& strValueName) const = 0;
	virtual bool SetValue(const D3DXVECTOR4 &vValue, const std::string& strValueName) = 0;
	virtual bool CreateValue(const D3DXVECTOR4 &vValue, const std::string& strValueName) = 0;
	
	virtual bool GetValue(D3DXVECTOR3 &vecValue, const std::string& strValueName) const = 0;
	virtual bool SetValue(const D3DXVECTOR3 &vecValue, const std::string& strValueName) = 0;
	virtual bool CreateValue(const D3DXVECTOR3 &vecValue, const std::string& strValueName) = 0;
	
	virtual bool GetValue(D3DXCOLOR &colorValue, const std::string& strValueName) const = 0;
	virtual bool SetValue(const D3DXCOLOR &colorValue, const std::string& strValueName) = 0;
	virtual bool CreateValue(const D3DXCOLOR &colorValue, const std::string& strValueName) = 0;
#endif	//	defined(DIRECT3D_VERSION)

	virtual bool GetValue(void * pValue, uint32& nLength, const std::string& strValueName) const = 0;
	virtual bool SetValue(const void * pValue, uint32 nLength, const std::string& strValueName) = 0;
	virtual bool CreateValue(const void * pValue, uint32 nLength, const std::string& strValueName) = 0;

	virtual bool DestroyValue(const std::string& strValueName) = 0;

	virtual ValueIterator ValueBegin() const = 0;
	virtual ValueIterator ValueEnd() const = 0;

	virtual PreferenceStore::SKey& CurrentKey(void) = 0;
	virtual PreferenceStore::SKey& RootKey(void) = 0;
};

namespace PreferenceStore
{
	class CPreferenceStore: public IPreferenceStore
	{
	public:
		CPreferenceStore();
		CPreferenceStore(PreferenceStore::SKey & rootKey);
		virtual ~CPreferenceStore();

	//	Key manipulation functions
		bool GetKey(std::string& m_strKeyName) const;
		bool SetKey(const std::string& m_strKeyName);
		bool PushKey(const std::string& m_strKeyName);
		bool PopKey(void);
		bool IsChildKey(const std::string& m_strKeyName);
		bool CreateKey(const std::string& m_strKeyName);
		bool DestroyKey(const std::string& m_strKeyName);
		void Purge();
		size_t NumSubKeys() const;
		size_t NumValues() const;
		KeyIterator KeyBegin() const;
		KeyIterator KeyEnd() const;
		bool Insert(IPreferenceStore &ref);

	//	Value manipulation functions
		bool GetValue(std::string& strValue, const std::string& strValueName) const;
		bool SetValue(const std::string& strValue, const std::string& strValueName);
		bool CreateValue(const std::string& strValue, const std::string& strValueName);

		bool GetValue(uint32& dwValue, const std::string& strValueName) const;
		bool SetValue(uint32 dwValue, const std::string& strValueName);
		bool CreateValue(uint32 dwValue, const std::string& strValueName);

		bool GetValue(int32& nValue, const std::string& strValueName) const;
		bool SetValue(int32 nValue, const std::string& strValueName);
		bool CreateValue(int32 nValue, const std::string& strValueName);

		bool GetValue(float32& fValue, const std::string& strValueName) const;
		bool SetValue(float32 fValue, const std::string& strValueName);
		bool CreateValue(float32 fValue, const std::string& strValueName);

	//	DirectX math and color types
	#if defined(DIRECT3D_VERSION)
		bool GetValue(D3DXVECTOR4& vValue, const std::string& strValueName) const;
		bool SetValue(const D3DXVECTOR4 &vValue, const std::string& strValueName);
		bool CreateValue(const D3DXVECTOR4 &vValue, const std::string& strValueName);
		
		bool GetValue(D3DXVECTOR3 &vecValue, const std::string& strValueName) const;
		bool SetValue(const D3DXVECTOR3 &vecValue, const std::string& strValueName);
		bool CreateValue(const D3DXVECTOR3 &vecValue, const std::string& strValueName);

		bool GetValue(D3DXCOLOR &colorValue, const std::string& strValueName) const;
		bool SetValue(const D3DXCOLOR &colorValue, const std::string& strValueName);
		bool CreateValue(const D3DXCOLOR &colorValue, const std::string& strValueName);
	#endif	//	defined(DIRECT3D_VERSION)

		bool GetValue(void * pValue, uint32& nLength, const std::string& strValueName) const;
		bool SetValue(const void * pValue, uint32 nLength, const std::string& strValueName);
		bool CreateValue(const void * pValue, uint32 nLength, const std::string& strValueName);

		bool GetValue(SPreferenceValue& refValue, const std::string& strValueName) const;
		bool SetValue(SPreferenceValue& refValue, const std::string& strValueName);
		bool CreateValue(SPreferenceValue& refValue, const std::string& strValueName);

		bool DestroyValue(const std::string& strValueName);

		virtual ValueIterator ValueBegin() const;
		virtual ValueIterator ValueEnd() const;

	protected:

		bool SaveTree(PreferenceStore::SKey& refKey);
		virtual bool OnSaveKey(const std::string strPath, const PreferenceStore::SKey& refKey) const;
		virtual bool OnSaveValue(const std::string strPath, const PreferenceStore::SKey& refKey,
			const SPreferenceValue& refValueName, const SPreferenceValue& refValue) const;
		void Purge(PreferenceStore::SKey* pKey);
		bool Insert(PreferenceStore::SKey* pKey);


		bool CreateKeys(PreferenceStore::SKey *& pNewKey, const std::string& m_strKeyName,
			PreferenceStore::SKey& refParentKey, bool bDirty = true);
		void GetPathToKey(std::string& strPath, const PreferenceStore::SKey& refKey);
		void BuildPathToKey(std::string& strPath, const PreferenceStore::SKey& refKey);
		void FreeSubkeys(PreferenceStore::SKey& refKey);
		void DestroyValuesAndSubkeys(PreferenceStore::SKey& refKey);
		bool FindKey(PreferenceStore::SKey *& pKey, std::string& strName, const std::string& m_strKeyName,
			PreferenceStore::SKey& refParentKey);

		PreferenceStore::SKey & RootKey(void);
		PreferenceStore::SKey & CurrentKey(void);

	private:
		bool						m_bAutoDelete;
		PreferenceStore::SKey*		m_pRootKey;
		PreferenceStore::SKey*		m_pCurrentKey;
		std::string					m_strCurrentKeyName;
		SPreferenceValue::ValueType	m_etValueNameType;
	};
}
#endif	//	!defined(_PREFERENCESTORE_H)
