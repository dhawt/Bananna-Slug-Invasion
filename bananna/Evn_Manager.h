/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Evn_Manager.h  -  Thread-safe event manager.  Essentially a wrapper class
                    for the SDL message loop.
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
#ifndef __EVN_MANAGER__INC
#define __EVN_MANAGER__INC

#include "Base_Global.h"
#include "Base_Logging.h"
#include "SDL/SDL.h"
#include "Game_Manager.h"
#include "Disp_Manager.h"
#include "System_API.h"

/*! \brief Thread-safe event manager
    Handles all incoming SDL events and dispatches to various other subsystems.
 */
class c_Evn_Manager
    {
    private:
        _bool                m_IsInit;
        c_Game_Manager      *i_GameManager;
        c_Disp_Manager      *i_DispManager;

    public:
        c_Evn_Manager() : m_IsInit(false)
            {
             }
        ~c_Evn_Manager() { };

        _bool Init(c_Game_Manager *gm, c_Disp_Manager *dm);

        _int32 Run(void);
     };


#endif
