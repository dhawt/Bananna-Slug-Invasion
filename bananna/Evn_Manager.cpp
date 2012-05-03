/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Evn_Manager.cpp
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
#include "Evn_Manager.h"

/*! \brief Initializes the event manager system
    Pass pointers to other subsystems to which the event manager
    must have access.
 */
bool c_Evn_Manager::Init(c_Game_Manager *gm, c_Disp_Manager *dm)
{
if (!gm)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "NULL manager passed!");
    return STATUS_FAIL;
     }
if (!dm)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "NULL manager passed!");
    return STATUS_FAIL;
     }
i_GameManager = gm;
i_DispManager = dm;
m_IsInit = true;
return STATUS_OK;
 }


/*! \brief Main function for manager thread.
 */
_int32 c_Evn_Manager::Run(void)
{
if (!m_IsInit)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Can't run uninitialized manager!");
    return 0;
     }

SDL_Event event;

if (SDL_PollEvent(&event))
    {
    // Quit message:
    if (event.type == SDL_QUIT)
        i_GameManager->Post_Quit();     // Post a quit to the game manager;
                                        // if the user agrees, Query_Quit will return
                                        // true (above)

    // Key events get passed to the event manager:
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
        n_System_Input::Handle_Key(&event.key);

     }  // end if SDL_POLL_EVENT

return 1;
 }

