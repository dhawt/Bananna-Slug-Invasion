/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Base_ResourceMngr.cpp
  begin     : 8/18/02
  copyright : (C) 2002 by Daniel Hawthorn
  _____________________________________________________________________
  This sourcefile is part of the Banna Slug Invasion project, which is
  free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your
  option) any later version.
\***************************************************************************/

/*!	Creates a dynamically allocated copy of the specified resource
	and adds to the list of resources.
	Returns STATUS_OK if succesfull, otherwise STATUS_FAIL.
 */
template <class Data, class Key>
_bool c_Base_ResourceMngr<Data, Key>::Add_Resource(Data &res)
{
// First make sure we have enough room for the resource:
if (m_NumResources == c_ResourceMngr_MaxResources)
	{
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "No more space for resources");
	return STATUS_FAIL;
	 }

// Allocate and copy the resource:
Data *newres;
newres = new Data;
if (!newres)
	{
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Mem allocation failure!");
	return STATUS_FAIL;
	 }
*newres = res;

// Now add it to the list in the proper place:
_int32 rindex, index;
for (rindex = 0; rindex < m_NumResources; rindex ++)
	if (*m_Resources[rindex] < *newres) break;
++ m_NumResources;		// One more resource...
for (index = rindex + 1; index < m_NumResources; index ++)
	m_Resources[index] = m_Resources[index - 1];	// Scoot everyone over...
m_Resources[rindex] = newres;

// And return ok:
return STATUS_OK;
 }	

/*!	Searches for the specified resouce.
	Returns a pointer to the resources if found,
	otherwise NULL.
 */
template <class Data, class Key>
Data *c_Base_ResourceMngr<Data, Key>::Get_Resource(Key key, _int32 &a)
{
for (_int32 index = 0; index < m_NumResources; index ++)
	if (*m_Resources[index] == key)
		{
		a = index;
		return m_Resources[index];
		 }
return NULL;
 }

/*!	Deletes the specified resouce if it exists.
	Returns STATUS_OK / STATUS_FAIL.
 */
template <class Data, class Key>
_bool c_Base_ResourceMngr<Data, Key>::Delete_Resource(Key key)
{
_int32 index;
Data *res = Get_Resource(key, index);
if (!res) return STATUS_FAIL;
delete res;
for (_int32 i = index + 1; i < m_NumResources; i ++) m_Resources[i - 1] = m_Resources[i];
m_NumResources --;
return STATUS_OK;
 }
