/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Disp_Manager.h
  begin     : Wed Aug 21 2002
  copyright : (C) 2002 by Daniel Hawthorn
  _____________________________________________________________________
  This sourcefile is part of the Banna Slug Invasion project, which is
  free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your
  option) any later version.
\***************************************************************************/
/*! \file */
#ifndef __DISP_MANAGER_H__INC
#define __DISP_MANAGER_H__INC

#include "Base_Global.h"
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#include "Gfx_BitBuffer.h"
#include "Game_Manager.h"

/*! \brief Thread-safe display manager
    Checks the c_Game_Manager for various state and display information, and
    updates the screen accordingly.
    On initialization, is passed a mutex for using SDL event routines (to prevent
    the use of multiple calls to event and/or video routines simultaneously).
 */
class c_Disp_Manager
    {
    protected:
        _bool                m_IsInit;
        _bool                m_Quit;
        SDL_mutex           *m_SdlMutex;
        c_Game_Manager      *i_GameManager;

        _int32 Run(void);

    public:
        c_Disp_Manager() : m_IsInit(false), m_Quit(false)
            {
             }
        ~c_Disp_Manager() { };

        static _int32 Begin(void *p);

        _bool Init(c_Game_Manager *gm, SDL_mutex *m);
        void Post_Quit(void) { m_Quit = true; };

     };

#endif
