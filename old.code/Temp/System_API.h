/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  System_API.h
  begin     : 8/18/02
  copyright : (C) 2002 by Daniel Hawthorn
  _____________________________________________________________________
  This sourcefile is part of the Banna Slug Invasion project, which is
  free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your
  option) any later version.
\***************************************************************************/
/*! \file Module encapsuating base-level system functionality, i.e., the API.
    All other classes and modules are built either paralell to or on top of
    the API.
 */
#ifndef __SYSTEM_API__INC
#define __SYSTEM_API__INC

#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#include "Base_Global.h"
#include "Base_Logging.h"
#include "Gfx_BitBuffer.h"

// System API namespace:
namespace n_System_API
    {
    // Prototypes:
    _bool Init(void);
    void Shutdown(void);

    _bool Init_Display(void);		
    _bool Shutdown_Display(void);		

    void Update_Display(void);
    _int32 Set_Video_Fullscreen(_int32 w, _int32 h, _int32 bpp);
    _int32 Set_Video_Windowed(_int32 w, _int32 h, _int32 bpp);
    _int32 Check_FullscreenValid(_int32 w, _int32 h, _int32 bpp);
    _int32 Check_WindowValid(_int32 w, _int32 h, _int32 bpp);

    c_Gfx_BitBuffer *Get_Display_BitBuffer(void);

    void Start(void);
     }

#endif
