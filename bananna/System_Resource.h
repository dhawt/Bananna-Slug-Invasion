/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  System_Resource.h
  begin     : Wed Aug 21 2002
  copyright : (C) 2002 by Daniel Hawthorn
  _____________________________________________________________________
  This sourcefile is part of the Banna Slug Invasion project, which is
  free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your
  option) any later version.
\***************************************************************************/
/*! \file Resource management system module
 */
#ifndef __SYSTEM_RESOURCE_H__INC
#define __SYSTEM_RESOURCE_H__INC

#include <iostream.h>
#include <stdlib.h>
#include <string>
#include "SDL/SDL.h"
#include "Base_Global.h"
#include "Base_Logging.h"
#include "Base_ResourceMngr.h"
#include "Gfx_BitBuffer.h"
#include "Gui_Font.h"

namespace n_System_Resource
	{
    /*! Display properties structure
     */
    struct s_Disp_Properties
        {
        _int32 xsize,
               ysize,
               bpp,
               fullscreen;
         };

    _bool Init(void);
    void Shutdown(void);

    void Get_Disp_Properties(s_Disp_Properties *props);
    void Set_Disp_Properties(s_Disp_Properties *props);

    c_Gfx_BitBuffer *Create_Resource_BitBuffer(std::string name);
	_bool  	        Delete_Resource_BitBuffer(std::string name);
	c_Gfx_BitBuffer *Get_Resource_BitBuffer(std::string name);
	c_Gfx_BitBuffer *Load_Resource_BitBuffer(std::string name);

	c_Gui_Font      *Create_Resource_Font(std::string name);
	_bool	    	Delete_Resource_Font(std::string name);
	c_Gui_Font      *Get_Resource_Font(std::string name);
	c_Gui_Font      *Load_Resource_Font(std::string name);

     }

#endif
