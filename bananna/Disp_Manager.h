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
#include "Gfx_BitBuffer.h"
#include "Game_Manager.h"

/*! \brief Display manager
    Checks the c_Game_Manager for various state and display information, and
    updates the screen accordingly.
 */
class c_Disp_Manager
    {
    protected:
        _bool                m_IsInit;
        c_Game_Manager      *i_GameManager;

    public:
        c_Disp_Manager() : m_IsInit(false)
            {
             }
        ~c_Disp_Manager() { };

        _bool Init(c_Game_Manager *gm);

        _int32 Run(void);

     };

#endif
