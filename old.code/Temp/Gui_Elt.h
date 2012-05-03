/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Gui_Elt.h  -  Thread-safe event manager.  Essentially a wrapper class
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
#ifndef __GUI_ELT__INC
#define __GUI_ELT__INC

#include "Base_Global.h"
#include "Base_NumberedResource.h"

/*! Base class from which dialog element classes can derive.
	Contains various information common to dialog elements, including
	position, (more later...)
	Inherits c_Base_NumberedResource.
 */
class c_Gui_Elt
	{
	private:
		_float m_XPos, m_YPos;

	public:
		c_Gui_Elt(int id = -1) { m_XPos = m_YPos = 0; };
		virtual ~c_Gui_Elt() = 0;
	
		void Set_XPos(_float pos);
		void Set_YPos(_float pos);
		_float Get_XPos(void) { return m_XPos; };
		_float Get_YPos(void) { return m_YPos; };
	 };

#endif
