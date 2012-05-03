/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Gui_Menu.h  -  Simple little class for a menu.
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
#ifndef __GUI_MENU__INC
#define __GUI_MENU__INC

#include "Base_Global.h"
#include "Gui_Elt.h"

//! Maximum number of items in a menu
const _int32 c_MaxMenuItems = 32;

//! Menu item state enum value
typedef enum { UND, ACTIVE, INACTIVE, HIGHLIGHT } e_MenuItemStateEnum;

/*! \brief Dialog element base class
	Base class from which dialog element classes can derive.
	Contains various information common to dialog elements, including
	position, (more later...)
	Inherits c_Gui_Elt which inherits c_Base_NumberedResouce.
 */
class c_Gui_Menu : public c_Gui_Elt
	{
	private:
		struct Item
			{
			string m_Name;
			e_MenuItemStateEnum m_State;
			_bool operator==(const Item &r) { return (m_Name == r.m_Name && m_State == r.m_State); };
			 };				
	private:

		_int32 m_NumItems;
		_int32 m_SelectedItem;
		Item m_Items[c_MaxMenuItems];

    public:
		c_Gui_Menu() { };
		~c_Gui_Menu() { };

		_bool operator==(const c_Gui_Menu &m);

		_int32 Add_Item(string name);
		string Get_Selected(void);
		string Inc_Selection(void);
		string Dec_Selection(void);
		string Select(string name);
		void Set_ItemState(string name, e_MenuItemStateEnum state);		
		e_MenuItemStateEnum Get_ItemState(string name);		
	 };

#endif
