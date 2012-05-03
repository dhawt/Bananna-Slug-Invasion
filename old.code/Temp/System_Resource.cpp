/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  System_Resource.cpp
  begin     : Wed Aug 21 2002
  copyright : (C) 2002 by Daniel Hawthorn
  _____________________________________________________________________
  This sourcefile is part of the Banna Slug Invasion project, which is
  free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your
  option) any later version.
\***************************************************************************/
#include <iostream.h>
#include <stdlib.h>
#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#include "FilePref.h"           // Include first so it doesn't use the memory manager
#include "System_Resource.h"

namespace n_System_Resource
{

// Module locals:
SDL_mutex *m_mutex;      // Basically one person at a time can grab a resource
c_Base_ResourceMngr<c_Gfx_BitBuffer, std::string> i_Resources_BitBuffer;
c_Base_ResourceMngr<c_Gui_Font, std::string> i_Resources_Font;
std::string Find_ResourceFile(std::string fname);
CFilePref i_FilePref;


/*! Initializes the system
    User of this system must ensure that Init() and Shutdown() are called before
    or after any other methods in the class, respectively.
 */
_bool Init(SDL_mutex *s)
{
m_mutex = s;

// Now attempt to load the config file.  If we can't, create some new prefs:
if (!i_FilePref.Load("config.reg"))
    {
c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Creating config");

    i_FilePref.CreateKey("Config");

    // Display settings:
    i_FilePref.CreateKey("Display");
    i_FilePref.SetKey("Config\\Display");
    i_FilePref.CreateValue(640, "xres");
    i_FilePref.CreateValue(480, "yres");
    i_FilePref.CreateValue(16, "bpp");
    i_FilePref.CreateValue(false, "fullscreen");

    // Resource directories:
    i_FilePref.CreateKey("Resource");
    i_FilePref.SetKey("Config\\Resource");
    i_FilePref.CreateValue("data\\", "respath1");
    i_FilePref.CreateValue("data\\font\\", "respath2");
     }

return STATUS_OK;
 }

/*! Shuts down the system
    User of this system must ensure that Init() and Shutdown() are called before
    or after any other methods in the class, respectively.
 */
void Shutdown(void)
{
 }

/*! Fills the passed property structure with the requested data.
 */
void Get_Disp_Properties(s_Disp_Properties *props)
{
if (!props)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Null ptr passed");
    return;
     }

i_FilePref.SetKey("Config\\Display");
i_FilePref.GetValue(props->xsize, "xsize");
i_FilePref.GetValue(props->ysize, "ysize");
i_FilePref.GetValue(props->bpp, "bpp");
i_FilePref.GetValue(props->fullscreen, "fullscreen");

// !!! Hacked here...
props->xsize = 640;
props->ysize = 480;
props->bpp = 16;
props->fullscreen = false;
 }

/*! Stores the passed properties.
 */
void Set_Disp_Properties(s_Disp_Properties *props)
{
if (!props)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Null ptr passed");
    return;
     }
i_FilePref.SetKey("Config\\Display");
i_FilePref.SetValue(props->xsize, "xsize");
i_FilePref.SetValue(props->ysize, "ysize");
i_FilePref.SetValue(props->bpp, "bpp");
i_FilePref.SetValue(props->fullscreen, "fullscreen");
 }

/*! Returns the path and filename to the specified resourcefile if it is
	found, otherwise returns "_DNE" for "does not exist".
	FIXME: Test if this works with multiple directories...
 */
std::string Find_ResourceFile(std::string fname)
{
std::string fpath;
ifstream file;

// Loop through each resource directory and attempt to find the file:
i_FilePref.SetKey("Config\\Resource");
for (CFilePref::ValueIterator itr = i_FilePref.ValueBegin(); itr != i_FilePref.ValueEnd(); itr ++)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Checking: %s", static_cast<std::string>(itr).c_str());
    i_FilePref.GetValue(fpath, static_cast<std::string>(itr));
	file.open(fpath.c_str(), ifstream::in);	// Open the file...
	if (file.good())	// Does this file exist?
		{
		file.close();		// Close the file and return the location
		return fpath;
		 }
     }

//return "_DNE";
fpath = "data//" + fname;
return fpath.c_str();        // !!! Hacked here..
 }

/*! Creates a new BitBuffer resource.  Returns a pointer to the resource,
	NULL if failure.
 */
c_Gfx_BitBuffer *Create_Resource_BitBuffer(std::string name)
{
SDL_mutexP(m_mutex);        // Lock our mutex...
c_Gfx_BitBuffer newRes;
newRes.Set_Name(name);
if (i_Resources_BitBuffer.Add_Resource(newRes) != STATUS_OK)
	{
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Alloc failed on resource addition %s", name.c_str());
    SDL_mutexV(m_mutex);        // Unlock our mutex
	return NULL;
	 }
SDL_mutexV(m_mutex);        // Unlock our mutex
return i_Resources_BitBuffer.Get_Resource(name);
 }

/*! Deletes the specified BitBuffer resource.
	Returns STATUS_OK / STATUS_FAIL.
 */
_bool Delete_Resource_BitBuffer(std::string name)
{
SDL_mutexP(m_mutex);
_bool val = i_Resources_BitBuffer.Delete_Resource(name);
SDL_mutexV(m_mutex);
return val;
 }

/*! Returns a pointer to the specified bitmap resource.
	Returns NULL if it's not loaded in memory.
 */
c_Gfx_BitBuffer *Get_Resource_BitBuffer(std::string name)
{
return i_Resources_BitBuffer.Get_Resource(name);
 }

/*! If the specified BitBuffer hasn't been loaded into memory,
	searches the disk for the resource within the data subdirectories
	specified in i_PropertySet.
	Returns a pointer to the resource if found, otherwise NULL.
 */
c_Gfx_BitBuffer *Load_Resource_BitBuffer(std::string name)
{
SDL_mutexP(m_mutex);

c_Gfx_BitBuffer *res;
std::string fname;
c_Base_ResourceParser	i_ResourceParser;

// Check if the resource already exists; if it does, just return a pointer,
// otherwise try and load it out of our resources:
res = i_Resources_BitBuffer.Get_Resource(name);
if (!res)									// The resource doesn't exist yet
	{
	// Look for the resoucefile:
	fname = Find_ResourceFile(name);
	if (!fname.compare("_DNE"))					// If we couldn't find it...
		{
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Couldnt find resource %s", name.c_str());
        SDL_mutexV(m_mutex);
		return NULL;
		 }

	// Create a new resource:
	res = Create_Resource_BitBuffer(name);
	if (!res)
		{
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Couldnt create resouce %s", name.c_str());
        SDL_mutexV(m_mutex);
		return NULL;
		 }

    // Attempt to load the image file:
    if (res->LoadUsing_SDL(fname) != STATUS_OK)
        {
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Couldnt load resouce %s", name.c_str());
        SDL_mutexV(m_mutex);
		Delete_Resource_BitBuffer(name);
		return NULL;
		 }
 	 }

// Close the file and return the resource:
i_ResourceParser.Close();
SDL_mutexV(m_mutex);
return res;
 }

/*! Creates a new Font resource.  Returns a pointer to the resource,
	NULL if failure.
 */
c_Gui_Font *Create_Resource_Font(std::string name)
{
SDL_mutexP(m_mutex);
c_Gui_Font newRes;
newRes.Set_Name(name);
if (i_Resources_Font.Add_Resource(newRes) != STATUS_OK)
	{
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Alloc failed on resource addition %s", name.c_str());
    SDL_mutexV(m_mutex);
	return NULL;
	 }
SDL_mutexV(m_mutex);
return i_Resources_Font.Get_Resource(name);
 }

/*! Deletes the specified Font resource.
	Returns STATUS_OK / STATUS_FAIL.
 */
_bool Delete_Resource_Font(std::string name)
{
SDL_mutexP(m_mutex);
_bool err = i_Resources_Font.Delete_Resource(name);
SDL_mutexV(m_mutex);
return err;
 }

/*! Returns a pointer to the specified resource.
	Returns NULL if it's not loaded in memory.
 */
c_Gui_Font *Get_Resource_Font(std::string name)
{
return i_Resources_Font.Get_Resource(name);
 }

/*! If the specified Font hasn't been loaded into memory,
	searches the disk for the resource within the data subdirectories
	specified in i_PropertySet.
	Returns a pointer to the resource if found, otherwise NULL.
 */
c_Gui_Font *Load_Resource_Font(std::string name)
{
SDL_mutexP(m_mutex);

c_Gui_Font *res;
std::string fname;
c_Base_ResourceParser	i_ResourceParser;

// Check if the resource already exists; if it does, just return a pointer,
// otherwise try and load it out of our resources:
res = i_Resources_Font.Get_Resource(name);
if (!res)									// The resource doesn't exist yet
	{
	// Look for the resoucefile:
	fname = Find_ResourceFile(name);
	if (fname == "_DNE")					// If we couldn't find it...
		{
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Couldnt find resource %s", name.c_str());
        SDL_mutexV(m_mutex);
		return NULL;
		 }

	// Create a new resource:
	res = Create_Resource_Font(name);
	if (!res)
		{
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Couldnt create resouce %s", name.c_str());
        SDL_mutexV(m_mutex);
		return NULL;
		 }

    // Open the file:
	if (i_ResourceParser.Open(fname) != STATUS_OK)
		{
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Couldnt load resouce %s", name.c_str());
        SDL_mutexV(m_mutex);
		Delete_Resource_Font(name);
		return NULL;
		 }

    // Load the resource:
	c_Base_ResourceParser::Chunk *chunk = i_ResourceParser.Get_NextChunk();
	if (!chunk)
		{
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "No data for resource %s", name.c_str());
        SDL_mutexV(m_mutex);
		Delete_Resource_Font(name);
	    res = NULL;
		 }
	if (res->Load_Descriptor(chunk) != STATUS_OK)
		{
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Couldnt load descriptor for resource %s", name.c_str());
        SDL_mutexV(m_mutex);
		Delete_Resource_Font(name);
	    res = NULL;
		 }

    // Kill the chunk:
	i_ResourceParser.Kill_Chunk(chunk);
 	 }

// Close the file and return the resource:
i_ResourceParser.Close();
SDL_mutexV(m_mutex);
return res;
 }


 } // End namespace n_System_Resource
