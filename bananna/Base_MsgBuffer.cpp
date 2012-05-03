/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Base_MsgBuffer.cpp
  begin     : 8/18/02
  copyright : (C) 2002 by Daniel Hawthorn
  _____________________________________________________________________
  This sourcefile is part of the Banna Slug Invasion project, which is
  free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your
  option) any later version.
\***************************************************************************/
#include <vector>
#include "Base_Global.h"
#include "Base_MsgBuffer.h"

/*!
  Adds the specified message to the buffer.
  If there are too many, deletes the oldest (first) one
 */
_bool c_Base_MsgBuffer::Add_Msg(const std::string &msg)
{
m_Buffer.push_back(msg);
if (m_Buffer.size() > m_BufferSize) m_Buffer.erase(m_Buffer.begin());
return STATUS_OK;
 }

/*!
  Returns the message at the specified index from the buffer.
  If index is out of bounds, returns an empty string.
 */
std::string c_Base_MsgBuffer::Get_Msg(_uint32 i) const
{
if (i < 0 || i >= m_BufferSize) return "";
return m_Buffer[i];
 }
