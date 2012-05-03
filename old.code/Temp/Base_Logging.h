/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Base_Logging.h  -  Logging subsystem.  Provides logfile output, as
                     well as a log history via c_Base_MsgBuffer.
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
#ifndef __BASE_LOGGING__INC
#define __BASE_LOGGING__INC

#include <fstream>
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#include "Base_Global.h"
#include "Base_MsgBuffer.h"

/*! \class c_Base_Logging
	\brief Thread-safe logging system class providing logfile output.
	
	Utilizes c_Base_MsgBuffer for providing log history to
	client app.

 */
class c_Base_Logging
	{
	private:
        static SDL_mutex *m_mutex;      // So we only log one line at a time...
		static ofstream m_File;
		static _bool m_Active;
		static _char *m_FileName;
		static c_Base_MsgBuffer i_MsgBuffer;
	
	public:
	   	static _uint32 Get_MsgBufferSize(void) { return i_MsgBuffer.Get_BufferSize(); };
		static void Set_MsgBufferSize(_uint32 size) { i_MsgBuffer.Set_BufferSize(size); };
		static std::string Get_BufferedMsg(_uint32 i) { return i_MsgBuffer.Get_Msg(i); };

		static _bool Begin(_char *filename);
		static _bool End(void);
		static void Log_Line(_char *msg, ...);
		static void Log_Msg(_int32 line, _char *module, _char *msg, ...);
	 };

#endif
