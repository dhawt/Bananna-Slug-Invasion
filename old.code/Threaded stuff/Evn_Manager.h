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
#include "SDL/SDL_thread.h"
#include "Game_Manager.h"
#include "Disp_Manager.h"
#include "System_API.h"

/*! \brief Thread-safe event manager
    Handles all incoming SDL events and dispatches to various other subsystems.
    On initialization, is passed a mutex for using SDL event routines (to prevent
    the use of multiple calls to event and/or video routines simultaneously).
 */
class c_Evn_Manager
    {
    private:
        _bool                m_IsInit;
        SDL_mutex           *m_SdlMutex;
        c_Game_Manager      *i_GameManager;
        c_Disp_Manager      *i_DispManager;

        _int32 Run(void);

    public:
        c_Evn_Manager() : m_IsInit(false)
            {
             }
        ~c_Evn_Manager() { };

        static _int32 Begin(void *p);
        _bool Init(c_Game_Manager *gm, c_Disp_Manager *dm, SDL_mutex *m);

     };


#endif
