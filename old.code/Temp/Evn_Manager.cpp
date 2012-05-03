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
while (!m_Quit)
	{
	if (SDL_PollEvent(&event))
        {
        // Quit message:
		if (event.type == SDL_QUIT)
            {
            i_GameManager->Post_Quit();     // Post quits and get outta here
            break;
             }

        // Key event:
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
            {
            // Key release?  Don't have to do much:
            if (event.key.type == SDL_KEYUP)
                n_System_Input::Set_KeyState(event.key.keysym.sym, false);

            // Otherwise process for a keypress:
            else
                {
                n_System_Input::Set_KeyState(event.key.keysym.sym, true);
                n_System_Input::Enqueue_Press(event.key.keysym.sym, event.key.keysym.mod);
                 }
             }

         }  // end if SDL_POLL_EVENT
	 }

// Signal the display manager to quit:
i_DispManager->Post_Quit();

return 1;
 }

/*! \brief Static method to begin thread.  Calls the run method specified
    in p, which is cast to a c_Evn_Manager. Runs for duration of manager thread.
 */
_int32 c_Evn_Manager::Begin(void *p)
{
if (!p)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Null ptr passed!");
    return 0;
     }
c_Evn_Manager *mngr = reinterpret_cast<c_Evn_Manager *>(p);
return mngr->Run();
 }

/*! \brief Initializes the event manager system
    Pass pointers to other subsystems to which the event manager
    must have access.
 */
bool c_Evn_Manager::Init(c_Game_Manager *gm, c_Disp_Manager *dm, SDL_mutex *m)
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
if (!m)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "NULL mutex passed!");
    return STATUS_FAIL;
     }
m_SdlMutex = m;
i_GameManager = gm;
i_DispManager = dm;
m_IsInit = true;
return STATUS_OK;
 }


