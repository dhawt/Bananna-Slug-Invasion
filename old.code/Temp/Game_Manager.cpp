/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Game_Manager.cpp
  begin     : Sun Aug 18 2002
  copyright : (C) 2002 by Daniel Hawthorn
  _____________________________________________________________________
  This sourcefile is part of the Banna Slug Invasion project, which is
  free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your
  option) any later version.
\***************************************************************************/
#include "Base_Global.h"
#include "Game_Manager.h"
#include "System_Resource.h"
#include "System_API.h"
#include "Base_Time.h"

/*! \brief Main function for manager thread.
 */
_int32 c_Game_Manager::Run(void)
{
if (!m_IsInit)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Can't run uninitialized manager!");
    return 0;
     }

// Load and setup some fonts:
//if (!n_System_Resource::Load_Resource_BitBuffer("Font_Verdana.png")) { c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Fail"); goto done; };
//if (!n_System_Resource::Load_Resource_Font("Font_Verdana.rdf")) { c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Fail"); goto done; };
//n_System_Resource::Get_Resource_Font("Font_Verdana.rdf")->Set_BitBuffer(n_System_Resource::Get_Resource_BitBuffer("Font_Verdana.png"), 2);
if (!n_System_Resource::Load_Resource_BitBuffer("Font_VerdanaBig.png")) { c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Fail"); return 0; };
if (!n_System_Resource::Load_Resource_Font("Font_VerdanaBig.rdf")) { c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Fail"); return 0; };
n_System_Resource::Get_Resource_Font("Font_VerdanaBig.rdf")->Set_BitBuffer(n_System_Resource::Get_Resource_BitBuffer("Font_VerdanaBig.png"), 2);

c_Gfx_BitBuffer *disp;
c_Gui_Font *font;
while (!m_Quit)
	{
	disp = n_System_API::Get_Display_BitBuffer();
    //font = Get_Resource_Font("Font_Verdana.rdf");
    //font->Print_Str(10, 50, *disp, "Hello, world.");
    font = n_System_Resource::Get_Resource_Font("Font_VerdanaBig.rdf");
    font->Print(0, 0, *disp, "Hello, world.");
    font->Print(0, 20, *disp, "%d", c_Timebase::Get_Time());
	n_System_API::Update_Display();
	 }

return 1;
 }

/*! \brief Static method to begin thread.  Calls the run method specified
    in p, which is cast to a c_Game_Manager. Runs for duration of manager thread.
 */
_int32 c_Game_Manager::Begin(void *p)
{
if (!p)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Null ptr passed!");
    return 0;
     }
c_Game_Manager *mngr = reinterpret_cast<c_Game_Manager *>(p);
return mngr->Run();
 }

/*! \brief Initializes the game manager system
    Pass pointers to other subsystems to which the manager
    must have access.
 */
_bool c_Game_Manager::Init(SDL_mutex *m)
{
if (!m)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "NULL mutex passed!");
    return STATUS_FAIL;
     }
m_IsInit = true;
m_SdlMutex = m;
return STATUS_OK;
 }

/*! \brief Initializes the game manager system
    Pass pointers to other subsystems to which the manager
    must have access.
 */
_bool c_Game_Manager::Init(SDL_mutex *m)
{
