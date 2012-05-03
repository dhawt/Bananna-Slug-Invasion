/////////////////////////////////////////////////////////////////////////////
//      created:        1/24/2002
//      filename:       FilePref.h
//      author:         Eli Pulsifer
//
//      purpose:			

#if !defined(_FILEPREF_H)
#define _FILEPREF_H

#if _MSC_VER > 1000
#pragma once

#pragma warning ( disable : 4786 )
#endif // _MSC_VER > 1000

#include "PreferenceStore.h"
#include <iostream.h>


class CFilePref : public PreferenceStore::CPreferenceStore
{
public:
	enum parser_code	
	{
		et_print_char,
		et_nonprint_char,
		et_escape_char = '\\',
		et_key_name_delim_start = '[',
		et_key_name_delim_end = ']',
		et_value_delim = '=',
		et_cr_endl = '\r',
		et_lf_endl = '\n',
		et_comment_delim	=	';'
	};
public:


private:
	enum state	
	{
		et_seeking_key_name,
		et_reading_key_name,
		et_seeking_value_name,
		et_reading_value_name,
		et_reading_value,
		et_comment
	};

	struct parser_state
	{
		parser_state(state eState)
			: m_eState(eState), m_eLastState(eState) {}
		parser_state(const parser_state & theState)
			: m_eState(theState.m_eState), m_eLastState(theState.m_eLastState) {}
		void SetState(state eState)
			{ m_eLastState = m_eState; m_eState = eState; }
		state State(void) const
			{ return m_eState; }
		state LastState(void) const
			{ return m_eLastState; }
		operator state() const
			{ return m_eState; }
		const parser_state & operator = ( state eNewState )
			{ SetState(eNewState); return *this; }
	private:
		state	m_eState;
		state	m_eLastState;
	};
public:

	CFilePref();
	virtual ~CFilePref();

//	Load and save functions
	bool Load( const std::string & strPathName );
	bool Save( const std::string & strPathName );
	bool Save( const std::string & strPathName, IPreferenceStore * pPreferences );


private:
	bool OnSaveKey( const std::string strPath, const PreferenceStore::SKey & refKey ) const;
	bool OnSaveValue( const std::string strPath, const PreferenceStore::SKey & refKey,
		const PreferenceStore::SPreferenceValue & refValueName, const PreferenceStore::SPreferenceValue & refValue ) const;
	void LoadPreffDataToTree( char * pSourceData, size_t m_nSourceLength );
	void Trim( std::string & strToTrim, const std::string & strRemove = " " );
	void ConvertToValue( PreferenceStore::SPreferenceValue & refValue, const std::string & strData );

	DECLARE_PARSER_TABLE();
	DECLARE_PREFIX_TABLE();
	DECLARE_POSTFIX_TABLE();

#if defined( _MSC_VER )
	mutable FILE * m_hFile;
#elif defined( __MWERKS__ )
	mutable std::FILE * m_hFile;
#endif

};

#endif	//	!defined(_FILEPREF_H)