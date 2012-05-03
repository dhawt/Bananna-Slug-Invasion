/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Base_NumberedResource.h  -  Abstract class for numbered resources.
                              Provides ID information functionality to
                              derived classes.
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
#ifndef __BASE_NUMBEREDRESOURCE__INC
#define __BASE_NUMBEREDRESOURCE__INC

#include "Base_Global.h"

/*! \class c_Base_NumberedResource
	\brief Abstract base class for numbered resources

	Provides numeric ID information functionality to derived classes.
 */
class c_Base_NumberedResource
	{
	private:
		_bool m_Expired;
		_int32 m_Id;

	public:
		c_Base_NumberedResource() { m_Id = -1; m_Expired = false; };
		virtual ~c_Base_NumberedResource() { };
		
        void Set_Id(int newid) { m_Id = newid; };
		_int32 Get_Id(void) const { return m_Id; };
		_bool Is(const _int32 cmp) { return (m_Id == cmp); };
		_bool Is(const c_Base_NumberedResource &cmp) { return (m_Id == cmp.m_Id); };

		_bool operator<(const c_Base_NumberedResource &n) { return m_Id < n.m_Id; };
		_bool operator==(const c_Base_NumberedResource &n) { return Is(n); };
		_bool operator==(const _int32 n) { return Is(n); };

		void Set_Expired(_bool flag) { m_Expired = flag; };
		_bool Is_Expired(void) { return m_Expired; };

	 };

#endif
