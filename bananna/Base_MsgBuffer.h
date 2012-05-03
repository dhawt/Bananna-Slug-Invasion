/***************************************************************************\
  Filename: Base_MsgBuffer.h
  Created: Wed Dec 26 2001
  _____________________________________________________________________
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
\***************************************************************************/
/*! \file */
#ifndef __BASE_MSGBUFFER__INC
#define __BASE_MSGBUFFER__INC

#include "Base_Global.h"

/*! \class c_Base_MsgBuffer
	\brief FIFO history buffer for messages

	Class providing FIFO history buffer for messages.
	Useful for terminals and such.
 */
class c_Base_MsgBuffer
	{
	private:
		std::vector<std::string> m_Buffer;
		_uint32 m_BufferSize;

	public:
		c_Base_MsgBuffer(_uint32 size = 0) { m_BufferSize = size; };
		~c_Base_MsgBuffer() { Clear(); };

		void Clear(void) { while (m_Buffer.size()) m_Buffer.erase(m_Buffer.begin()); };
		_uint32 Get_BufferSize(void) const { return m_BufferSize; };
		void Set_BufferSize(_uint32 size) { Clear(); m_BufferSize = size; };
		bool Add_Msg(const std::string &msg);
		std::string Get_Msg(_uint32 i) const;
	 };

#endif
