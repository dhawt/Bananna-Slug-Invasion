/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Gui_Elt.cpp
  begin     : Sun Aug 18 2002
  copyright : (C) 2002 by Daniel Hawthorn
  _____________________________________________________________________
  This sourcefile is part of the Banna Slug Invasion project, which is
  free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your
  option) any later version.
\***************************************************************************/

#include "Base_Global.h"
#include "Gui_Elt.h"

/*! Sets the X position of the dialog element. (0 < X < 1)
 */
void c_Gui_Elt::Set_XPos(_float pos)
{
if (pos < 0) m_XPos = 0;
else if (pos > 1) m_XPos = 1;
else m_XPos = pos;
 }

/*! Sets the Y position of the dialog element. (0 < X < 1)
 */
void c_Gui_Elt::Set_YPos(_float pos)
{
if (pos < 0) m_YPos = 0;
else if (pos > 1) m_YPos = 1;
else m_YPos = pos;
 }
