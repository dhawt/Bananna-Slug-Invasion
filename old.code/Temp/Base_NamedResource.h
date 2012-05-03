/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Base_NamedResource.h  -  Abstract class for named resources.  Provides
                           name information functionality to derived
                           classes.
  begin     : 8/18/02
  copyright : (C) 2002 by Daniel Hawthorn
  _____________________________________________________________________
  This sourcefile is part of the Banna Slug Invasion project, which is
  free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your
  option) any later version.
\***************************************************************************/
/*! \file */
#ifndef __BASE_NAMEDRESOURCE__INC
#define __BASE_NAMEDRESOURCE__INC

#include "Base_Global.h"

/*! \class c_Base_NamedResource
	\brief Abstract base class for named resources

	Provides name information functionality to derived classes.
 */
class c_Base_NamedResource
	{
	private:
		_bool m_Expired;
		std::string m_Name;

	public:
		c_Base_NamedResource() { m_Expired = false; };
		virtual ~c_Base_NamedResource() { };
		
        void Set_Name(std::string newname) { m_Name = newname; };
		std::string Get_Name(void) const { return m_Name; };
		_bool Is(const std::string cmp) { return (m_Name == cmp); };
		_bool Is(const c_Base_NamedResource &cmp) { return (m_Name == cmp.m_Name); };

		_bool operator<(const c_Base_NamedResource &n) { return m_Name < n.m_Name; };
		_bool operator==(const c_Base_NamedResource &n) { return Is(n); };
		_bool operator==(const std::string n) { return Is(n); };

		void Set_Expired(_bool flag) { m_Expired = flag; };
		_bool Is_Expired(void) { return m_Expired; };
	 };

#endif
