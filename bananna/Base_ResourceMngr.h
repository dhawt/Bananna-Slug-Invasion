/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Base_ResourceManager.h  -  Templated resource manager.  Provides basic
                             adding/deleting functionality for classes
                             derived from c_Base_NamedResource or
                             c_Base_NumberedResource.
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
#ifndef __BASE_RESOURCEMNGR__INC
#define __BASE_RESOURCEMNGR__INC

#include "Base_Global.h"
#include "Base_Logging.h"

//! Maximum number of resources the manager can contain
const _int32 c_ResourceMngr_MaxResources = 64;

/*! Templated resource manager class.
	Provides memory management of the datatype, as well as provides access
	to a specified resource.
	Data should be derived from c_Base_Named_Resources (or numbered), and
	must provide operator=() functionality.
	Key should be std::string for named entities, or int for numbered
	entities.  Note it must also provide operator<() functionality.
 */
template <class Data, class Key>
class c_Base_ResourceMngr
	{
	private:
		_int32 m_NumResources;
		Data *m_Resources[c_ResourceMngr_MaxResources];

	public:
        c_Base_ResourceMngr()
			{
			m_NumResources = 0;
			for (_int32 index = 0; index < c_ResourceMngr_MaxResources; ++ index)
				m_Resources[index] = NULL;
			 };
		~c_Base_ResourceMngr()
			{
			for (_int32 index = 0; index < m_NumResources; ++ index)
				delete m_Resources[index];
			 };

		_bool Add_Resource(Data &res);
		_bool Delete_Resource(Key key);
		Data *Get_Resource(Key key, _int32 &index);
		Data *Get_Resource(Key key) { _int32 tmp; return Get_Resource(key, tmp); };
	 };

#include "Base_ResourceMngr.cpp"

#endif
