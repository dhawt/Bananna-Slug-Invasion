/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Disp_Manager.cpp
  begin     : Wed Aug 21 2002
  copyright : (C) 2002 by Daniel Hawthorn
  _____________________________________________________________________
  This sourcefile is part of the Banna Slug Invasion project, which is
  free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your
  option) any later version.
\***************************************************************************/
#include "Base_Global.h"
#include "Disp_Manager.h"

/*! \brief Main function for manager thread.
 */
_int32 c_Disp_Manager::Run(void)
{
if (!m_IsInit)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Can't run uninitialized manager!");
    return 0;
     }

// Update the display until we can't play anymore:
while (!m_Quit)
	{
     }

return 1;
 }

/*! \brief Static method to begin thread.  Calls the run method specified
    in p, which is cast to a c_Disp_Manager. Runs for duration of manager thread.
 */
_int32 c_Disp_Manager::Begin(void *p)
{
if (!p)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Null ptr passed!");
    return 0;
     }
c_Disp_Manager *mngr = reinterpret_cast<c_Disp_Manager *>(p);
return mngr->Run();
 }

/*! \brief Initializes the display manager system
    Pass pointers to other subsystems to which the event manager
    must have access.
 */
_bool c_Disp_Manager::Init(c_Game_Manager *gm, SDL_mutex *m)
{
m_IsInit = true;
if (!gm)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "NULL manager passed!");
    m_IsInit = false;
     }
if (!m)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "NULL mutex passed!");
    m_IsInit = false;
     }
m_SdlMutex = m;
i_GameManager = gm;
return m_IsInit ? STATUS_OK:STATUS_FAIL;
 }
