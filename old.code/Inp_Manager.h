/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Inp_Manager.h  -  Class to handle all input.  Updates input state based on
                    messages passed from the event manager.
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
#ifndef __INX_INP_MANAGER__INC
#define __INX_INP_MANAGER__INC

#include "Base_Global.h"
#include "Base_Logging.h"
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"

/*! \brief Thread safe input manager/storage class
    Abstracted class for storage of various input flags and status.
    Stores key states, mouse position, button status, etc..
 */
class c_Inp_Manager
    {
    private:
        SDL_mutex  *m_StateMutex;
        _bool       m_IsInit;
        _bool       m_Quit;
        _uint16    *m_KeyStates;

        void Shutdown()
            {
            if (m_IsInit)
                {
                delete [] m_KeyStates;
                SDL_DestroyMutex(m_StateMutex);
                 }
             }

        bool Init()
            {
            if (m_IsInit) return STATUS_OK;
            m_StateMutex = SDL_CreateMutex();
            m_KeyStates = new _uint16[32];            // 512 packed bitflags for key states
            if (m_KeyStates) m_IsInit = true;
            else c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Out of memory on Inp_Manager init! (Trouble for sure)");
            return m_IsInit ? STATUS_OK:STATUS_FAIL;
             }

    public:
        c_Inp_Manager() : m_IsInit(false), m_Quit(false) { Init(); };
        ~c_Inp_Manager() { Shutdown(); };

        // Accessors
        void Set_KeyState(SDLKey k, _bool state);
        _bool Get_KeyState(SDLKey k);

        void Post_Quit(void) { m_Quit = true; };

     };

#endif
