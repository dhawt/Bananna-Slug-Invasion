/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  System_Input.h
  begin     : Thu Aug 22 2002
  copyright : (C) 2002 by Daniel Hawthorn
  _____________________________________________________________________
  This sourcefile is part of the Banna Slug Invasion project, which is
  free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your
  option) any later version.
\***************************************************************************/
/*! \file Module encapsulating the thread-safe input management system.
    Storage of various input flags and status is maintained here, including
    key states, mouse position, button status, etc.  All info is obtained by
    messages pased to the system by c_Evn_Manager, which recieves input
    messages from the OS via SDL.
 */
#ifndef __SYSTEM_INPUT_H__INC
#define __SYSTEM_INPUT_H__INC

#include "Base_Global.h"
#include "SDL/SDL.h"

// Input system namespace:
namespace n_System_Input
    {
    // Prototypes:
    _bool Init(void);
    void Shutdown(void);
    void Handle_Key(SDL_KeyboardEvent *key);
    _bool Get_KeyState(SDLKey k);
    _bool Dequeue_Press(SDL_keysym *k);
    void Clear_Queue(void);
     }

#endif
