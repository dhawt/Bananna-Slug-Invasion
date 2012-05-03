/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Game_Manager.h  -  Main game manager class.  Handles all game logic, i.e.,
                     game state, levels, animation, etc.
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
#ifndef __GAME_MANAGER__INC
#define __GAME_MANAGER__INC

#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#include "Base_Global.h"
#include "Base_Logging.h"
#include "System_Input.h"

/*! \brief Game Manager class
    Game manager class.  Interfaces with c_Inp_ReplayManager for all user input.
 */
class c_Game_Manager
    {
    private:
        _bool m_IsInit;
        _bool m_Quit;

    protected:
        SDL_mutex           *m_SdlMutex;

        _int32 Run(void);

    public:
        c_Game_Manager() : m_IsInit(false), m_Quit(false)
            {
             }
        ~c_Game_Manager() { };

        static _int32 Begin(void *p);

        _bool Init(SDL_mutex *m);

        void Post_Quit(void);
        _bool Query_Quit(void) { return m_Quit; };

     };


#endif
