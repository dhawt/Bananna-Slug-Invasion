/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Gui_Font.cpp
  begin     : Sun Aug 18 2002
  copyright : (C) 2002 by Daniel Hawthorn
  _____________________________________________________________________
  This sourcefile is part of the Banna Slug Invasion project, which is
  free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your
  option) any later version.
\***************************************************************************/

#include <stdarg.h>
#include <string.h>
#include "Gui_Font.h"
#include "Base_Logging.h"

/*! A constructor
 */
c_Gui_Font::Index::Index(_int32 cwidth, _int32 cheight) : m_CharWidth(0), m_CharHeight(0)
{
Init(cwidth, cheight);
 }

/*! Font destructor.
 */
c_Gui_Font::Index::~Index()
{
 }

/*! Does a deep copy from the specified index.
	Returns STATUS_OK/STATUS_FAIL.
 */
_bool c_Gui_Font::Index::Copy(const Index &idx)
{
if (this == &idx) return STATUS_OK;
if (Init(idx.m_CharWidth, idx.m_CharHeight) != STATUS_OK)
	{
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Init failure on copy");
	return STATUS_FAIL;
	 }
memcpy(m_Indices, idx.m_Indices, 256);
memcpy(m_Kerning, idx.m_Kerning, 256);
return STATUS_OK;
 }

/*! Initializes the index.
	Returns STATUS_OK/STATUS_FAIL.
 */
_bool c_Gui_Font::Index::Init(_int32 cwidth, _int32 cheight)
{
m_CharWidth = cwidth;
m_CharHeight = cheight;
for (_int32 i = 0; i < 256; i ++)
    {
    m_Indices[i] = 0;
    m_Kerning[i] = cwidth;
     }
return STATUS_OK;
 }

/*! Constructor
	Uses the specified bitbuffer for bitmap data.
 */
c_Gui_Font::c_Gui_Font()
{
 }

/*! Destructor
 */
c_Gui_Font::~c_Gui_Font()
{
 }

/*! Blits the specified character at the specified location.
	Returns STATUS_OK/STATUS_FAIL.
 */
_bool c_Gui_Font::Print_Char(_int32 x, _int32 y, c_Gfx_BitBuffer &dest, _char c)
{
if (!m_FontBuff)
	{
#ifdef BUILD_DEBUG
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "NULL bitmap active!");
#endif
	return STATUS_OK;
	 }
_int32 cIdx = m_Index.Get_Index(c);
if (cIdx == -1) return STATUS_OK;  // If whitespace, print nothing
_int32 fontx = (cIdx * m_Index.Get_CharWidth()) % m_FontBuff->Get_Width(),
    fonty = (_int32)((_float)(cIdx * m_Index.Get_CharWidth()) / (_float)m_FontBuff->Get_Width()) * m_Index.Get_CharHeight();
//c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Mapped char %c to %d, found at %d,%d", c, cIdx, fontx, fonty);
return m_FontBuff->Blit(fontx, fonty, m_Index.Get_CharWidth(), m_Index.Get_CharHeight(), x, y, &dest);
 }

/*! Prints the specified string to the specified buffer.
	Returns STATUS_OK/STATUS_FAIL.
	TODO: Maybe add support for more interesting formattings, like \n and tabs and such.
 */
_bool c_Gui_Font::Print_Str(_int32 x, _int32 y, c_Gfx_BitBuffer &dest, _char *str)
{
if (!m_FontBuff)
	{
#ifdef BUILD_DEBUG
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "NULL bitmap active!");
#endif
	return STATUS_OK;
	 }

_int32 px = x;
if (!str) return STATUS_OK;			// Ignore NULL string
for (_int32 i = 0; i < static_cast<_int32>(strlen(str)); i ++)
	{
	if (Print_Char(px, y, dest, str[i]) != STATUS_OK)
		{
		c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Failure on Print_Char");
		return STATUS_FAIL;
		 }
	px += m_Index.Get_CharKerning(str[i]); //m_Index.Get_CharWidth();
 	if (px > dest.Get_Width()) break;
	 }
return STATUS_OK;
 }

/*! Prints the printf() style formatted string to the specified buffer.
	Returns STATUS_OK/STATUS_FAIL.
 */
_bool c_Gui_Font::Print(_int32 x, _int32 y, c_Gfx_BitBuffer &dest, _char *str, ...)
{
if (!m_FontBuff)
	{
#ifdef BUILD_DEBUG
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "NULL bitmap active!");
#endif
	return STATUS_OK;
	 }

_char buff[80];		// !!! Could be trouble if we exceed 80 chars...
va_list arglist;

// Put everything into our buffer:
va_start(arglist, str);
vsprintf(buff, str, arglist);

// Print the string and return what we get:
return Print_Str(x, y, dest, buff);
 }

/*! Loads the index data from the chunk.
	Returns STATUS_OK if successful, otherwise STATUS_FAIL.
 */
_bool c_Gui_Font::Load_Descriptor(c_Base_ResourceParser::Chunk *chunk)
{
_int32 numChars, charWidth, charHeight, ch;

// Check our pointer...
if (!chunk)
	{
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Null chunk passed");
	return STATUS_FAIL;
	 }

// Read the data:
numChars = 0; if (chunk->Read_Int16(reinterpret_cast<_int16 *>(&numChars)) != STATUS_OK) return STATUS_FAIL;
charWidth = 0; if (chunk->Read_Int16(reinterpret_cast<_int16 *>(&charWidth)) != STATUS_OK) return STATUS_FAIL;
charHeight = 0; if (chunk->Read_Int16(reinterpret_cast<_int16 *>(&charHeight)) != STATUS_OK) return STATUS_FAIL;

// Reinit the font:
if (m_Index.Init(charWidth, charHeight) != STATUS_OK)
	{
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Couldn't init font on load");
	return STATUS_FAIL;
	 }

// Now read all the data:
for (_int32 i = 0; i < numChars; i ++)
	{
	ch = 0; if (chunk->Read_Byte(reinterpret_cast<_uchar *>(&ch)) != STATUS_OK) return STATUS_FAIL;
	m_Index.Set_Index(ch, i);
	 }

// And return ok:
return STATUS_OK;
 }

/*! Sets the internal pointer to the bitbuffer this font will use to render
    text.
 */
void c_Gui_Font::Set_BitBuffer(c_Gfx_BitBuffer *buff, _int32 kernSpacing)
{
// Set the buffer:
m_FontBuff = buff;
if (!m_FontBuff)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Null bitmap passed");
    return;
     }

// Lock the bitmap surface:
_char *data = reinterpret_cast<_char *>(m_FontBuff->Lock());
_int32 bpp = m_FontBuff->Get_SDLSurface()->format->BytesPerPixel;

// Get the top-left pixel:
_uint32 cNone;
if (bpp == 1) cNone = (_uint32)m_FontBuff->Get_Pixel<unsigned char>(data, 0, 0);
else if (bpp == 2) cNone = (_uint32)m_FontBuff->Get_Pixel<_uint16>(data, 0, 0);
else if (bpp == 3 || bpp == 4) cNone = (_uint32)m_FontBuff->Get_Pixel<_uint32>(data, 0, 0);

// Cheat for whitespace:
m_Index.Set_CharKerning(' ', kernSpacing * 3);

// Now loop through each cell and scan for kerning info:
_int32 fontx, fonty;
_uint32 cCurr;
for (_int32 i = 0; i < Get_MaxCells(); i ++)
    {
    // Get the top-left coord of the cell:
    fontx = (i * m_Index.Get_CharWidth()) % m_FontBuff->Get_Width();
    fonty = (_int32)((_float)(i * m_Index.Get_CharWidth()) / (_float)m_FontBuff->Get_Width()) * m_Index.Get_CharHeight();

    // Scan for the rightmost pixel in the char:
    _int32 x;
    for (x = m_Index.Get_CharWidth() - 1; x >= 0; x --)
        for (_int32 y = 0; y < m_Index.Get_CharHeight(); y ++)
            {
            if (bpp == 1) cCurr = (_uint32)m_FontBuff->Get_Pixel<unsigned char>(data, fontx + x, fonty + y);
            else if (bpp == 2) cCurr = (_uint32)m_FontBuff->Get_Pixel<_uint16>(data, fontx + x, fonty + y);
            else if (bpp == 3 | bpp == 4) cCurr = (_uint32)m_FontBuff->Get_Pixel<_uint32>(data, fontx + x, fonty + y);
            else cCurr = 5;

            // If this pixel isn't transparent, set the kerning to this width
            // and stop scanning:
            if (cCurr != cNone)
                {
                m_Index.Set_CellKerning(i, x + kernSpacing);
                x = -2;
                break;
                 }
             }
    if (x == -2) m_Index.Set_CellKerning(i, kernSpacing);
     }

// Unlock the bitmap surface:
m_FontBuff->Unlock();
 }
