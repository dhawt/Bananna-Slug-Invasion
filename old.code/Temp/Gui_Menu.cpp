/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Evn_Manager.cpp
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
#include "Gui_Menu.h"
#include "Base_Logging.h"

/*! Overloaded ==
 */
_bool c_Gui_Menu::operator==(const c_Gui_Menu &m)
{
if (m_NumItems != m.m_NumItems) return false;
for (_int32 i = 0; i < m_NumItems; i ++)
	if (!(m_Items[i] == m.m_Items[i])) return false;
return true;
 }

/*! Adds the specified item to the menu
 */
_int32 c_Gui_Menu::Add_Item(string name)
{
// Add one more item, check if that's too many:
if (++m_NumItems >= c_MaxMenuItems)
	{
	m_NumItems = c_MaxMenuItems - 1;
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Max items exceeded in menu (%d)", c_MaxMenuItems);
	return STATUS_FAIL;
	 }
m_Items[m_NumItems - 1].m_Name = name;
m_Items[m_NumItems - 1].m_State = ACTIVE;
return STATUS_OK;
 }

/*! Returns the currently selected menu item, zero-length string
	if no item is selected or the selected item is invalid in any way.
 */
string c_Gui_Menu::Get_Selected(void)
{
if (m_SelectedItem < 0 || m_SelectedItem >= m_NumItems) return "";
return m_Items[m_SelectedItem].m_Name;
 }

/*! Selects the next item in the menu.  If at end, wraps to beginning.
	Returns the newly selected item.
 */
string c_Gui_Menu::Inc_Selection(void)
{
++ m_SelectedItem;
if (m_SelectedItem >= m_NumItems) m_SelectedItem = 0;
return Get_Selected();
 }

/*! Selects the previous item in the menu.  If at beginning, wraps to end.
	Returns the newly selected item.
 */
string c_Gui_Menu::Dec_Selection(void)
{
-- m_SelectedItem;
if (m_SelectedItem < 0) m_SelectedItem = m_NumItems - 1;
return Get_Selected();
 }

/*! Selects the specified item in the menu.  Returns the newly selected
	item, or leaves the selection unchanged if the specified item
	doesn't exist in the menu.
 */
string c_Gui_Menu::Select(string name)
{
for (_int32 index = 0; index < m_NumItems; ++ index)
	if (m_Items[index].m_Name == name) m_SelectedItem = index;
return Get_Selected();
 }

/*! Sets the state of the specified item.  If the item doesn't exist,
	does nothing.
 */
void c_Gui_Menu::Set_ItemState(string name, e_MenuItemStateEnum state)
{
for (_int32 index = 0; index < m_NumItems; ++ index)
	if (m_Items[index].m_Name == name) m_Items[index].m_State = state;
 }

/*! Returns the state of the specified item.  If the item doesn't exist,
	returns UNDEF.
 */
e_MenuItemStateEnum c_Gui_Menu::Get_ItemState(string name)
{
for (_int32 index = 0; index < m_NumItems; ++ index)
	if (m_Items[index].m_Name == name) return m_Items[index].m_State;
return UND;
 }
