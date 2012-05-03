/////////////////////////////////////////////////////////////////////////////
//      created:        1/24/2002
//      filename:       PreferenceValue.h
//      author:         Eli Pulsifer
//
//      purpose:        

#if !defined(_PREFERENCEVALUE_H)
#define _PREFERENCEVALUE_H

#if _MSC_VER > 1000
#pragma once
#pragma warning ( disable : 4786 )
#endif // _MSC_VER > 1000

#include "defines.h"
#include <string>

namespace PreferenceStore
{

	struct SPreferenceValue
	{
	public:
		enum ValueType
		{
			kStringType,
			kSignedIntegerType,
			kUnsignedIntegerType,
			kFloatType,
			kVector3Type,
			kVector4Type,
			kRGBAColorType,
			kBinaryType,
			kUndefinedType
		};

		SPreferenceValue(const SPreferenceValue& value);
		SPreferenceValue(bool isDirty = true);
		SPreferenceValue(const std::string& stringData, bool isDirty = true);
		SPreferenceValue(uint32 integerValue, bool isDirty = true);
		SPreferenceValue(int32 integerValue, bool isDirty = true);
		SPreferenceValue(float32 floatValue, bool isDirty = true);
		SPreferenceValue(const void* buffer, uint32 bufferLength, bool isDirty = true);
		SPreferenceValue(const std::string& data, ValueType dataType, bool isDirty = true);

	#if defined(DIRECT3D_VERSION)
		SPreferenceValue(const D3DXVECTOR4 &vecValue, bool isDirty = true);
		SPreferenceValue(const D3DXVECTOR3 &vectorData, bool isDirty = true);
		SPreferenceValue(const D3DXCOLOR &colorData, bool isDirty = true);
	#endif	//	defined(DIRECT3D_VERSION)
		~SPreferenceValue();


		bool	GetData(std::string& stringData) const;
		void	SetData(const std::string& stringData);
		operator const char*() const;

		bool	GetData(int32& integerData) const;
		void	SetData(int32 integerData);
		operator int32() const;

		bool	GetData(uint32& integerData) const;
		void	SetData(uint32 integerData);
		operator uint32() const;

		bool	GetData(float32& floatData) const;
		void	SetData(float32 floatData);
		operator float32() const;

		bool	GetData(void* buffer, uint32& bufferLength) const;
		void	SetData(const void* buffer, uint32 bufferLength);

	#if defined(DIRECT3D_VERSION)
		bool	GetData(D3DXVECTOR4 &vectorData) const;
		void	SetData(const D3DXVECTOR4 &vectorData);
		operator D3DXVECTOR4() const;

		bool	GetData(D3DXVECTOR3 &vectorData) const;
		void	SetData(const D3DXVECTOR3 &vectorData);
		operator D3DXVECTOR3();

		bool	GetData(D3DXCOLOR &colorData) const;
		void	SetData(const D3DXCOLOR &colorData);
		operator D3DXCOLOR();
	#endif	//	defined(DIRECT3D_VERSION)

		std::string	GetDataAsText() const;
		void	SetRawData(const std::string& data, ValueType dataType);

		ValueType	GetType() const;
		size_t	GetSize() const;
		
		void	SetDirty(bool isDirty = true);
		bool	IsDirty() const;
		void	SetDestroy(bool bDestroy = true);
		bool	Destroy() const;

		bool operator < (const SPreferenceValue& rhs) const;
		const SPreferenceValue& operator = (const SPreferenceValue& rhs);

	private:
		void	ConvertToString(std::string& stringValue, const void* buffer, size_t bufferLength) const;
	#if defined(DIRECT3D_VERSION)
		void	ConvertToD3DXVECTOR3(D3DXVECTOR3 &vectorData, const std::string& stringValue) const;
		void	ConvertToD3DXVECTOR4(D3DXVECTOR4 &vecData, const std::string& stringValue) const;
		void	ConvertToD3DXCOLOR(D3DXCOLOR &colorData, const std::string& stringValue) const;
	#endif	//	defined(DIRECT3D_VERSION)

		void	FreeData();
		void	AllocData(size_t dataSize);

		ValueType	m_eType;
		size_t		m_nLength;
		uint8		*m_pRawData;
		bool		m_bDirty;
		bool		m_bDestroy;
	};
}

#endif // !defined(_PREFERENCEVALUE_H)
