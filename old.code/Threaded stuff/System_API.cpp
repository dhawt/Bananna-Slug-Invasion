/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  System_API.cpp
  begin     : Sun Aug 18 2002
  copyright : (C) 2002 by Daniel Hawthorn
  _____________________________________________________________________
  This sourcefile is part of the Banna Slug Invasion project, which is
  free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your
  option) any later version.
\***************************************************************************/
#include <fstream>
#include <string.h>
#include "Base_Global.h"
#include "System_API.h"
#include "Base_Logging.h"
#include "Base_Status.h"
#include "Gfx_BitBuffer.h"
#include "Base_Time.h"
#include "Disp_Manager.h"
#include "Evn_Manager.h"
#include "Game_Manager.h"
#include "System_Resource.h"
#include "System_Input.h"

// System API namespace:
namespace n_System_API
{

// Module locals:
SDL_mutex *m_SdlMutex;

struct
    {
   	c_Gfx_BitBuffer m_DisplayBuffer;		//!< BitBuffer for screen
   	 } s_Video;

struct						//!< System status flags
   	{
   	_uint32 m_SDL_IsInit:1;
   	_uint32 m_SDL_HaveDisplay:1;
   	_uint32 _unused:14;
     } s_Flags;

// Various interfaces:
c_Game_Manager          i_Game_Manager;         // Various manager interfaces...
c_Disp_Manager          i_Disp_Manager;
c_Evn_Manager           i_Evn_Manager;


/*! Initializes the API
    User of this system must ensure that Init() and Shutdown() are called before
    or after any other methods in the class, respectively.
 */
_bool Init(void)
{
// Init flags:
s_Flags.m_SDL_HaveDisplay = false;

// Initialize SDL video libraries:
if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Initialization of SDL video failed");
	s_Flags.m_SDL_IsInit = false;
	return STATUS_FAIL;
	 }
s_Flags.m_SDL_IsInit = true;

// Create our SDL mutex:
m_SdlMutex = SDL_CreateMutex();

return STATUS_OK;
 }

/*! Shuts down the API
    User of this system must ensure that Init() and Shutdown() are called before
    or after any other methods in the class, respectively.
 */
void Shutdown(void)
{
// Shutdown the display system:
Shutdown_Display();

// Kill our mutex:
SDL_DestroyMutex(m_SdlMutex);

// Shutdown SDL libraries:
SDL_Quit();
 }

/*! Initializes the display system.  Uses values in i_PropertySet for
	various parameters.
 */
_bool Init_Display(void)
{
// Clear everything up first:
Shutdown_Display();

// Set up SDL:
n_System_Resource::s_Disp_Properties dispprops;
n_System_Resource::Get_Disp_Properties(&dispprops);  // Get the current display properties
if (dispprops.fullscreen)
	{
	if (!Check_FullscreenValid(dispprops.xsize, dispprops.ysize, dispprops.bpp))
		{
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Invalid window - %d,%d,%d", dispprops.xsize, dispprops.ysize, dispprops.bpp);
		return STATUS_FAIL;
		 }
	if (Set_Video_Fullscreen(dispprops.xsize, dispprops.ysize, dispprops.bpp) != STATUS_OK)
		{
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Can't init video");
		return STATUS_FAIL;
		 }
	 }
else
	{
	if (!Check_WindowValid(dispprops.xsize, dispprops.ysize, dispprops.bpp))
		{
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Invalid window");
		return STATUS_FAIL;
		 }
	if (Set_Video_Windowed(dispprops.xsize, dispprops.ysize, dispprops.bpp) != STATUS_OK)
		{
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Can't init video");
		return STATUS_FAIL;
		 }
	 }

return STATUS_OK;
 }

/*! Shuts down the display system.
 */
_bool Shutdown_Display(void)
{
// Do nothing for now...
return STATUS_OK;
 }

/*! Returns a pointer to the display buffer
 */
c_Gfx_BitBuffer *Get_Display_BitBuffer(void)
{
return &s_Video.m_DisplayBuffer;
 }

/*! Returns 0 if the fullscreen mode specified will fail under all bitdepths,
	otherwise the bpp of the closest bitdepth matching the specified resolution.
 */
_int32 Check_FullscreenValid(_int32 w, _int32 h, _int32 bpp)
{
return SDL_VideoModeOK(w, h, bpp, SDL_DOUBLEBUF | SDL_FULLSCREEN);
 }

/*! Returns 0 if the window size specified will fail under all bitdepths,
	otherwise the bpp of the closest bitdepth matching the specified resolution.
 */
_int32 Check_WindowValid(_int32 w, _int32 h, _int32 bpp)
{
return SDL_VideoModeOK(w, h, bpp, SDL_ANYFORMAT);
 }

/*! Updates the display.  Simply calls SDL_UpdateRect.
 */
void Update_Display(void)
{
if (s_Video.m_DisplayBuffer.Get_SDLSurface())
	SDL_UpdateRect(s_Video.m_DisplayBuffer.Get_SDLSurface(), 0, 0, 0, 0);
 }

/*! Sets the display mode to fullscreen with the specified characteristics.
	Returns ok if successful, otherwise failure.
 */
_int32 Set_Video_Fullscreen(_int32 w, _int32 h, _int32 bpp)
{
if (!s_Video.m_DisplayBuffer.Create_FullScreenSurface(w, h, bpp))
	{
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Couldn't create fullscreen surface (%d x %d x %d)", w, h, bpp);
	s_Flags.m_SDL_HaveDisplay = false;
	return STATUS_FAIL;
	 }
s_Flags.m_SDL_HaveDisplay = true;
return STATUS_OK;
 }

/*! Sets the display mode to windowed with the specified characteristics.
	Returns ok if successful, otherwise failure.
 */
_int32 Set_Video_Windowed(_int32 w, _int32 h, _int32 bpp)
{
if (!s_Video.m_DisplayBuffer.Create_WindowedScreenSurface(w, h, bpp))
	{
 	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Couldn't create window surface (%d x %d x %d)", w, h, bpp);
	s_Flags.m_SDL_HaveDisplay = false;
	return STATUS_FAIL;
	 }
s_Flags.m_SDL_HaveDisplay = true;
return STATUS_OK;
 }

/*! Starts the whole shebang running.
 */
void Start(void)
{
// Initialize all subsystems:
if (n_System_Resource::Init(m_SdlMutex) != STATUS_OK)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "System_Resource initialization failed");
    return;
     }
if (n_System_Input::Init() != STATUS_OK)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "System_Input initialization failed");
    return;
     }

// Initialize the display:
if (Init_Display() != STATUS_OK) return;

// Initialize all our managers, one by one:
if (i_Game_Manager.Init(m_SdlMutex) != STATUS_OK)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Game_Manager initialization failed");
    return;
     }
if (i_Evn_Manager.Init(&i_Game_Manager, &i_Disp_Manager, m_SdlMutex) != STATUS_OK)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Game_Manager initialization failed");
    return;
     }
if (i_Disp_Manager.Init(&i_Game_Manager, m_SdlMutex) != STATUS_OK)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Disp_Manager initialization failed");
    return;
     }
// Init sound manager here...

// Set up and start each manager thread:
SDL_Thread *thread_dispmngr = SDL_CreateThread(i_Disp_Manager.Begin, &i_Disp_Manager);
SDL_Thread *thread_gamemngr = SDL_CreateThread(i_Game_Manager.Begin, &i_Game_Manager);
SDL_Thread *thread_evnmngr = SDL_CreateThread(i_Evn_Manager.Begin, &i_Evn_Manager);

// Wait until all threads are done:
_int32 status;
SDL_WaitThread(thread_evnmngr, &status);
SDL_WaitThread(thread_gamemngr, &status);
SDL_WaitThread(thread_dispmngr, &status);

// Shut down our systems:
n_System_Input::Shutdown();
n_System_Resource::Shutdown();

 }

 } // End namespace n_System_API
