/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Base_ResourceParser.cpp
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
#include "Base_Global.h"
#include "Base_ResourceParser.h"
#include "Base_Status.h"
#include "Base_Logging.h"

/*! A happy little destructor.
  	Doesn't delete anything!
 */		
c_Base_ResourceParser::Chunk::~Chunk()
{
//_char *data = reinterpret_cast<_char *>(m_Data);
//if (data)
//	{
//	delete data;
//	m_Data = NULL;
// 	 }
 }

/*! A happy little assignment operator.
 */		
void c_Base_ResourceParser::Chunk::operator=(const c_Base_ResourceParser::Chunk &c)
{
if (this != &c)
	{
	m_Id = c.m_Id;
	m_Data = c.m_Data;
	m_Size = c.m_Size;
	m_Index = c.m_Index;
	 }
 }

/*! Allocates the data buffer in the chunk for data.
	Returns STATUS_OK if successful, otherwise STATUS_FAIL.
 */		
bool c_Base_ResourceParser::Chunk::Create(_int32 size)
{
if (size < 0)
	{
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "size < 0 passed for Chunk::Create()");
	return STATUS_FAIL;
	 }
//m_Data = reinterpret_cast<void *>(new _char[size]);
m_Data = new _char[size];
m_Data = reinterpret_cast<void *>(m_Data);
if (!m_Data)
	{
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Mem alloc failed for chunk buffer");
	return STATUS_FAIL;
	 }
m_Size = size;
m_Index = 0;
return STATUS_OK;
 }

/*! Deletes all data currently allocated by the chunk.
 */		
void c_Base_ResourceParser::Chunk::Kill()
{
_char *data = reinterpret_cast<_char *>(m_Data);
if (data)
	{
	delete [] data;
	m_Data = NULL;
 	 }
 }

/*! Seeks to the specified offset (in bytes) from the start of the chunk
	(offset 0).
	Returns STATUS_OK if successful, otherwise STATUS_FAIL if the specified
	offset is invalid.
 */		
bool c_Base_ResourceParser::Chunk::Seek_Start(_int32 offs)
{
if (offs < 0 || offs >= Get_Size()) return STATUS_FAIL;
m_Index = offs;
return STATUS_OK;
 }

/*! Seeks to the specified offset (in bytes) from the current poisition
	in the chunk.
	Returns STATUS_OK if successful, otherwise STATUS_FAIL if the specified
	offset is invalid.
 */		
bool c_Base_ResourceParser::Chunk::Seek_Curr(_int32 offs)
{
if ( (m_Index + offs) < 0 || (m_Index + offs) >= Get_Size()) return STATUS_FAIL;
m_Index += offs;
return STATUS_OK;
 }

/*! Reads a 32-bit float from the chunk.
	Returns STATUS_OK if successful, otherwise STATUS_FAIL if the data index
	is invalid (i.e., at the end of the chunk).
*/
bool c_Base_ResourceParser::Chunk::Read_Float32(float *d)
{
if (!d)
	{
#ifdef BUILD_DEBUG
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "NULL param passed");
#endif
	return STATUS_FAIL;
	 }		
if (m_Index < 0 || m_Index >= (Get_Size() - 4) || !m_Data)
	{
#ifdef BUILD_DEBUG
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "No data reading chunk at index %d", m_Index);
#endif
	return STATUS_FAIL;
	 }
_uchar *bytedata = reinterpret_cast<_uchar *>(m_Data);
float *data = reinterpret_cast<float *>(&bytedata[m_Index]); // Get float pointer to index
*d = *data;
m_Index += 4;
return STATUS_OK;
 }

/*! Reads a little-endian 32-bit float from the chunk and reverses the byte order.
	Returns STATUS_OK if successful, otherwise STATUS_FAIL if the data index
	is invalid (i.e., at the end of the chunk).
*/
bool c_Base_ResourceParser::Chunk::Read_Float32_LE(float *d)
{
if (!d)
	{
#ifdef BUILD_DEBUG
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "NULL param passed");
#endif
	return STATUS_FAIL;
	 }		
if (m_Index < 0 || m_Index >= (Get_Size() - 4) || !m_Data)
	{
#ifdef BUILD_DEBUG
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "No data reading chunk at index %d", m_Index);
#endif
	return STATUS_FAIL;
	 }
_uchar *bytedata = reinterpret_cast<_uchar *>(m_Data);
*d = static_cast<float>((bytedata[m_Index] << 24) +
			  		 	(bytedata[m_Index + 1] << 16) +
						(bytedata[m_Index + 2] << 8) +
						(bytedata[m_Index + 3]));
m_Index += 4;
return STATUS_OK;
 }

/*! Reads a 16-bit integer from the chunk.
	Returns STATUS_OK if successful, otherwise STATUS_FAIL if the data index
	is invalid (i.e., at the end of the chunk).
*/
bool c_Base_ResourceParser::Chunk::Read_Int16(_int16 *d)
{
if (!d)
	{
#ifdef BUILD_DEBUG
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "NULL param passed");
#endif
	return STATUS_FAIL;
	 }		
if (m_Index < 0 || m_Index >= (Get_Size() - 2) || !m_Data)
	{
#ifdef BUILD_DEBUG
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "No data reading chunk at index %d", m_Index);
#endif
	return STATUS_FAIL;
	 }
_uchar *bytedata = reinterpret_cast<_uchar *>(m_Data);
_int16 *shrt = reinterpret_cast<_int16 *>(&bytedata[m_Index]);
*d = *shrt; // Get _int16 pointer to index
m_Index += 2;
return STATUS_OK;
 }

/*! Reads a little-endian 16-bit float from the chunk and reverses the byte order.
	Returns STATUS_OK if successful, otherwise STATUS_FAIL if the data index
	is invalid (i.e., at the end of the chunk).
*/
bool c_Base_ResourceParser::Chunk::Read_Int16_LE(_int16 *d)
{
if (!d)
	{
#ifdef BUILD_DEBUG
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "NULL param passed");
#endif
	return STATUS_FAIL;
	 }		
if (m_Index < 0 || m_Index >= (Get_Size() - 4) || !m_Data)
	{
#ifdef BUILD_DEBUG
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "No data reading chunk at index %d", m_Index);
#endif
	return STATUS_FAIL;
	 }
_uchar *bytedata = reinterpret_cast<_uchar *>(m_Data);
*d = static_cast<_int16>((bytedata[m_Index] << 8) +
						(bytedata[m_Index + 1]));
m_Index += 2;
return STATUS_OK;
 }

/*! Reads a 32-bit integer from the chunk.
	Returns STATUS_OK if successful, otherwise STATUS_FAIL if the data index
	is invalid (i.e., at the end of the chunk).
*/
bool c_Base_ResourceParser::Chunk::Read_Int32(_int32 *d)
{
if (!d)
	{
#ifdef BUILD_DEBUG
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "NULL param passed");
#endif
	return STATUS_FAIL;
	 }		
if (m_Index < 0 || m_Index >= (Get_Size() - 4) || !m_Data)
	{
#ifdef BUILD_DEBUG
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "No data reading chunk at index %d", m_Index);
#endif
	return STATUS_FAIL;
	 }
_uchar *bytedata = reinterpret_cast<_uchar *>(m_Data);
_int32 *data = reinterpret_cast<_int32 *>(&bytedata[m_Index]); // Get _int16 pointer to index
*d = static_cast<_int32>(*data);
m_Index += 4;
return STATUS_OK;
 }

/*! Reads a little-endian 32-bit float from the chunk and reverses the byte order.
	Returns STATUS_OK if successful, otherwise STATUS_FAIL if the data index
	is invalid (i.e., at the end of the chunk).
*/
bool c_Base_ResourceParser::Chunk::Read_Int32_LE(_int32 *d)
{
if (!d)
	{
#ifdef BUILD_DEBUG
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "NULL param passed");
#endif
	return STATUS_FAIL;
	 }		
if (m_Index < 0 || m_Index >= (Get_Size() - 4) || !m_Data)
	{
#ifdef BUILD_DEBUG
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "No data reading chunk at index %d", m_Index);
#endif
	return STATUS_FAIL;
	 }
_uchar *bytedata = reinterpret_cast<_uchar *>(m_Data);
*d = static_cast<_int32>((bytedata[m_Index + 3] << 24) +
						(bytedata[m_Index + 2] << 16) +
						(bytedata[m_Index + 1] << 8) +
						(bytedata[m_Index]));
m_Index += 2;
return STATUS_OK;
 }

/*! Reads a byte from the chunk and casts to an int.  Useful to read byte-sized
	data to 32-bit (or whatever) integer values.
	Returns STATUS_OK if successful, otherwise STATUS_FAIL if the data index
	is invalid (i.e., at the end of the chunk).
*/
bool c_Base_ResourceParser::Chunk::Read_Byte_Alt(_int32 *d)
{
if (!d)
	{
#ifdef BUILD_DEBUG
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "NULL param passed");
#endif
	return STATUS_FAIL;
	 }		
if (m_Index < 0 || m_Index >= Get_Size() || !m_Data)
	{
#ifdef BUILD_DEBUG
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "No data reading chunk at index %d", m_Index);
#endif
	return STATUS_FAIL;
	 }
_uchar *bytedata = reinterpret_cast<_uchar *>(m_Data);
*d = static_cast<_int32>(bytedata[m_Index]);
m_Index += 1;
return STATUS_OK;
 }

/*! Reads a byte from the chunk.
	Returns STATUS_OK if successful, otherwise STATUS_FAIL if the data index
	is invalid (i.e., at the end of the chunk).
*/
bool c_Base_ResourceParser::Chunk::Read_Byte(_uchar *d)
{
if (!d)
	{
#ifdef BUILD_DEBUG
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "NULL param passed");
#endif
	return STATUS_FAIL;
	 }		
if (m_Index < 0 || m_Index >= Get_Size() || !m_Data)
	{
#ifdef BUILD_DEBUG
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "No data reading chunk at index %d", m_Index);
#endif
	return STATUS_FAIL;
	 }
_uchar *bytedata = reinterpret_cast<_uchar *>(m_Data);
*d = bytedata[m_Index];
m_Index += 1;
return STATUS_OK;
 }

/*! Reads the sepcified number of bytes from the chunk into a buffer.
	Returns STATUS_OK if successful, otherwise STATUS_FAIL if the data index
	is invalid (i.e., at the end of the chunk).
*/
bool c_Base_ResourceParser::Chunk::Read_Buffer(_uchar *d, _int32 size)
{
if (!d)
	{
#ifdef BUILD_DEBUG
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "NULL param passed");
#endif
	return STATUS_FAIL;
	 }		
if (m_Index < 0 || m_Index > (Get_Size() - size) || !m_Data)
	{
#ifdef BUILD_DEBUG
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "No data reading chunk at index %d", m_Index);
#endif
	return STATUS_FAIL;
	 }
_uchar *dataoff = reinterpret_cast<_uchar *>(m_Data);
void *voiddat = reinterpret_cast<void *>(&dataoff[m_Index]);
memcpy(reinterpret_cast<void *>(d), voiddat, size);
m_Index += size;
return STATUS_OK;
 }

/*! Reads the sepcified number of bytes from the chunk into a new chunk,
	which can be further parsed.  Useful for nesting files, for instance.
	Returns the chunk if successful, otherwise NULL if the data index
	is invalid (i.e., at the end of the chunk).
*/
c_Base_ResourceParser::Chunk *c_Base_ResourceParser::Chunk::Read_Chunk(_int32 size)
{
if (m_Index < 0 || m_Index > (Get_Size() - size) || !m_Data)
	{
#ifdef BUILD_DEBUG
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "No data reading chunk at index %d", m_Index);
#endif
	return NULL;
	 }
c_Base_ResourceParser::Chunk *newchunk;
newchunk = new c_Base_ResourceParser::Chunk;
if (newchunk->Create(size) != STATUS_OK)			// Create a new chunk
	{
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Can't create new chunk.");
	return NULL;
 	 }
if (Read_Buffer(reinterpret_cast<_uchar *>(newchunk->Get_Data()), size) != STATUS_OK)  // Read the buffer into the new chunk
	{
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Couldn't read data.");
	delete newchunk;
	return NULL;
	 }
return newchunk;
 }

/*! Opens the specified resource and parses the header (if one exists).
	Returns STATUS_FAIL if anything goes wrong, otherwise STATUS_OK.
*/
bool c_Base_ResourceParser::Open(std::string filename)
{
_char buffer[81], newchar;
_int32 bytesread;

// Close and clean up:
Close();

// Attempt to open the resource:
//if ( (m_File = fopen(filename.c_str(), "r")) == NULL)
m_File.open(filename.c_str(), ios::binary);
if (!m_File.good())
    {
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Couldn't open resource \"%s\"", filename.c_str());
    return STATUS_FAIL;
     }

// Check for an INX identifier:
buffer[8] = '\0';
m_File.read(buffer, 8);
bytesread = m_File.gcount();
if (bytesread == 8 &&
	!strcasecmp(buffer, "INXRESRC"))		// Formatted resource
	{
	// Read the resource ID string:
	newchar = 1;
	buffer[80] = '\0';
	m_File.get(buffer, 80, '\0');
	m_File.get();		// Eat the delimeter that was left in the stream
	if (m_File.eof())
		{
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Premature EOF encountered while parsing resource header");
		Close();
		return STATUS_FAIL;
		 }
	m_ResourceID = static_cast<std::string>(buffer); // new _char[strlen(buffer) + 1];
	//strcpy(m_ResourceID, buffer);

	// Flag this is a formatted resource:
	m_IsFormatted = true;
	 }
else										// Unformatted
	{
	//err = fseek(m_File, 0, SEEK_SET);
	m_File.seekg(0);							// Go to the beginning of the file	
	m_IsFormatted = false;						// Flag not formatted
	 }

// And return ok:
return STATUS_OK;
 }

/*! Closes the currently open resource.
	Returns STATUS_OK if the file was closed successfully, otherwise
	STATUS_FAIL. (If no file was open, for instance.)
*/
bool c_Base_ResourceParser::Close(void)
{
m_IsFormatted = m_AtFileEnd = false;
if (m_File)
	{
	m_File.close();
	return STATUS_OK;
	 }
return STATUS_FAIL;
 }

/*! Returns true if all chunks have been read from the file.
*/
bool c_Base_ResourceParser::AtEnd(void)
{
return m_AtFileEnd;
 }

/*! Reads the next chunk from the file.
	Returns NULL if there are no further chunks to be read from the resource.
	\warning Chunk is dynamically allocated, so it must be killed by a call
	to Kill_Chunk()!
*/
c_Base_ResourceParser::Chunk *c_Base_ResourceParser::Get_NextChunk(void)
{
_int32 chunklen, blocksread;
_char buffer[81], newchar;

// Create a new chunk:
c_Base_ResourceParser::Chunk *newchunk;
newchunk = new c_Base_ResourceParser::Chunk;
if (!newchunk)
	{
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Couldn't allocate new chunk!");
	return NULL;
 	 }

// Make sure the file is open and there is data to read:
if (!m_File)
    {
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Resource not open");
	Kill_Chunk(newchunk);
    return NULL;
     }

// Check if this is a formatted resource:
if (m_IsFormatted)
	{
	// Read the chunk size:
	m_File.read(reinterpret_cast<_uchar *>(&chunklen), 4);
	blocksread = m_File.gcount();
	//blocksread = fread(reinterpret_cast<void *>(&chunklen), 4, 1, m_File);
	if (blocksread < 4)
		{
#ifdef BUILD_DEBUG
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Bad resource - premature EOF in chunk header");
		Close();
#endif
		m_AtFileEnd = true;
		Kill_Chunk(newchunk);
    return NULL;
		 }

	// Read the chunk ID string:
	newchar = 1;
	buffer[80] = '\0';
	m_File.get(buffer, 80, '\0');
	m_File.get();		// Eat the delimeter that was left in the stream
	if (m_File.eof())
		{
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Bad resource - premature EOF in chunk header");
		Close();
		Kill_Chunk(newchunk);
    return NULL;
		 }

	// Set up the chunk:
	if (newchunk->Create(chunklen) != STATUS_OK)
		{
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Failed to create chunk buffer");
		Close();
		Kill_Chunk(newchunk);
    return NULL;
		 }
	newchunk->Set_Id(static_cast<string>(buffer));

	// Now read the chunk data:
	m_File.read(reinterpret_cast<_uchar *>(newchunk->Get_Data()), chunklen);
	blocksread = m_File.gcount(); 	//blocksread = fread(newchunk->Get_Data(), 1, chunklen, m_File);
	if (blocksread < chunklen)
		{
		m_AtFileEnd = 1;
#ifdef BUILD_DEBUG
			c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Premature EOF encountered in chunk data (nonfatal)");
#endif
		 }
	 }
// Non-formatted resource:
else
	{
	// Chunk length is the size of the file:
	m_File.seekg(0, ios::end); 	//fseek(m_File, 0, SEEK_END);
	chunklen = m_File.tellg(); 	//fgetpos(m_File, reinterpret_cast<fpos_t *>(&chunklen));
	m_File.seekg(0); 	//rewind(m_File);

	// Allocate a buffer:
	if (newchunk->Create(chunklen) != STATUS_OK)
		{
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Failed to create chunk buffer");
		Close();
		Kill_Chunk(newchunk);
    return NULL;
		 }
	newchunk->Set_Id("UNFORMATTED");

	// Read our data:		
	m_File.read(reinterpret_cast<_uchar *>(newchunk->Get_Data()), chunklen);
	blocksread = m_File.gcount(); 	//blocksread = fread(newchunk->Get_Data(), 1, chunklen, m_File);
	if (blocksread < chunklen)
		{
		m_AtFileEnd = 1;
#ifdef BUILD_DEBUG
			c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Premature EOF encountered in chunk data (nonfatal)");
#endif
		 }
	 }

// And return our chunk:
return newchunk;
 }
