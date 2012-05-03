/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Base_ResourceParser.h  -  Resource file parser.  Provides lots of great
                            functionality for parsing resourcefiles.
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
#ifndef __BASE_RESOURCEPARSER_H__INC
#define __BASE_RESOURCEPARSER_H__INC

#include <iostream>
#include <fstream>
#include "Base_Global.h"
#include "Base_Logging.h"

/*! \brief External (file) resource parser class
	
	Class encapsulating basic resourcefile input parsing functionality.
	Can handle INX resource files with structured 'chunk' formatting.
	Non-INX structured resources are just treated as a INX file with a single
	chunk and all parsing is left to be done from within that chunk.
 */
class c_Base_ResourceParser
	{
	public:
		//! INX Resouce File chunk information structure
		class Chunk
			{
			protected:
				std::string m_Id;
				void *m_Data;
				_int32 m_Size, m_Index;

			public:
				Chunk(const Chunk &c) { m_Id = c.m_Id; m_Data = c.m_Data; m_Size = c.m_Size; m_Index = c.m_Index; };
				Chunk() { m_Id = ""; m_Data = NULL; m_Size = m_Index = 0; };
				~Chunk();
				void operator=(const Chunk &c);

				_bool Create(_int32 size);
				void Kill(void);
				void *Get_Data(void) { return m_Data; }; //!< Returns a void* to chunk data

				_bool Seek_Start(_int32 offs);
			    _bool Seek_Curr(_int32 offs);
				_int32 Get_Pos(void) { return m_Index; }; //!< Returns the current chunk position offset in bytes
				_int32 Get_Size(void) { return m_Size; }; //!< Returns the size of the chunk in bytes

				_bool Read_Float32(float *d);
				_bool Read_Float32_LE(float *d);
        		_bool Read_Int16(_int16 *d);
        		_bool Read_Int16_LE(_int16 *d);
				_bool Read_Int32(_int32 *d);
				_bool Read_Int32_LE(_int32 *d);
				_bool Read_Byte_Alt(_int32 *d);
				_bool Read_Byte(_uchar *d);
				_bool Read_Buffer(_uchar *d, _int32 size);
				Chunk *Read_Chunk(_int32 size);
		     	
				void Set_Id(std::string stn) { m_Id = stn; }; //!< Set the ID string for the resource chunk
				const _char *Get_Id(void) const { return m_Id.c_str(); }; //!< Returns the ID string of the chunk
				_uint32 Get_Size(void) const { return m_Size; }; //!< Returns the size of the chunk in bytes
				_uint32 Get_Index(void) const { return m_Index; }; //!< Returns the data index for reading from the chunk
				void Rewind(void) { m_Index = 0; }; //!< Rewinds the read index to the beginning of the buffer
			 };

	protected:
		c_Base_Logging i_SystemLog;				//!< System logging interface
		_bool m_IsFormatted,
			 m_AtFileEnd;
		ifstream m_File;
		std::string m_ResourceID;
		
	public:
		c_Base_ResourceParser() { m_IsFormatted = m_AtFileEnd = false; };
		~c_Base_ResourceParser() { Close(); };

		_bool Open(std::string filename);
		_bool Close(void);
		_bool AtEnd(void);
		Chunk *Get_NextChunk(void);
		void Kill_Chunk(Chunk *c) { if (c != NULL) { c->Kill(); delete c; } };
		//_bool Seek_Chunk(_char *id);
	 };

#endif
