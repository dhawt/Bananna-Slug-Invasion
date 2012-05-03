/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Base_Logging.cpp
  begin     : 8/18/02
  copyright : (C) 2002 by Daniel Hawthorn
  _____________________________________________________________________
  This sourcefile is part of the Banna Slug Invasion project, which is
  free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your
  option) any later version.
\***************************************************************************/
#include <fstream>
#include <stdarg.h>
#include <string.h>
#include "Base_Global.h"
#include "Base_Logging.h"

SDL_mutex *c_Base_Logging::m_mutex;
ofstream c_Base_Logging::m_File;
_bool c_Base_Logging::m_Active = false;
_char *c_Base_Logging::m_FileName = NULL;
c_Base_MsgBuffer c_Base_Logging::i_MsgBuffer;

/*! Begins logging to the specified file
 */
_bool c_Base_Logging::Begin(_char *filename)
{
// Check if we're already logging:
if (m_Active)
	{
	Log_Msg(__LINE__, __FILE__, "Log already active on attempt to log to %s!", filename);
    return STATUS_FAIL;
	 }

// Create our mutex:
m_mutex = SDL_CreateMutex();

// Attempt to open the file:
m_File.open(filename, ios::out);
	//m_File = open(filename, O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);

// If we can't, disable logging:
if (!m_File)
	//if (m_File == -1)
    {
	m_Active = false;
	if (m_FileName) delete [] m_FileName;
	m_FileName = NULL;
	return STATUS_FAIL;
	 }

// Setup flags:
m_Active = true;
m_FileName = new _char[strlen(filename) + 1];
strcpy(m_FileName, filename);

// Write a message to the log file:
Log_Line("--- Begin logfile %s ---\n\n", m_FileName);

return STATUS_OK;
 }

/*!
  Closes the logfile
 */
_bool c_Base_Logging::End(void)
{
// Check if logging is enabled:
if (m_Active)
    {
    // Write a final message:
    Log_Line("\n\n--- End logfile %s ---", m_FileName);

    // Close the log file:
    m_File.close();
     }

// Setup flags:
m_Active = false;
//m_File = 0;
if (m_FileName) delete [] m_FileName;
m_FileName = NULL;

SDL_DestroyMutex(m_mutex);

// And return ok:
return STATUS_OK;
 }

/*!
  Writes a line to the log file.
 */
void c_Base_Logging::Log_Line(_char *msg, ...)
{
SDL_mutexP(m_mutex);        // Lock our mutex...

_char Buffer[151];
va_list arglist;

// Make sure logging to file is enabled:
if (m_Active)
    {
    if (strlen(msg) > 150) msg[150] = 0;
    va_start(arglist, msg);
    vsprintf(Buffer, msg, arglist);
    va_end(arglist);
	m_File << Buffer << endl;
	//cout << Buffer << endl;
	//write(m_File, Buffer, strlen(Buffer));
	 }

// Now append this to our message buffer:
i_MsgBuffer.Add_Msg(Buffer);

SDL_mutexV(m_mutex);        // And resume
 }

/*!
	Writes a debug message to the file w/ line and module info
 */
void c_Base_Logging::Log_Msg(_int32 line, _char *mod, _char *msg, ...)
{
_char buffer[151];

// Make sure we have a message:
if (!msg) return;

// Write the message to the log if the file is open:
if (m_Active && m_File)
	{
	va_list arglist;				// !!! TODO: add check for long messages

	va_start(arglist, msg);
	vsprintf(buffer, msg, arglist);
	va_end(arglist);

	// Now write the entry to the file:
	Log_Line("%s (%d): %s", mod, line, buffer);
	 }

// And append this to our message buffer:
i_MsgBuffer.Add_Msg(buffer);
 }
