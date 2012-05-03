/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Gfx_BitBuffer.cpp
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
#include "Gfx_BitBuffer.h"
#include <SDL/SDL_image.h>

/*! Creates a surface with the specified characteristics.
 */
_bool c_Gfx_BitBuffer::Create_RGBSurface(_int32 flags, _int32 w, _int32 h, _int32 depth, _int32 rmask, _int32 gmask, _int32 bmask, _int32 amask)
{
m_SurfaceInfo = SDL_CreateRGBSurface(flags, w, h, depth, rmask, gmask, bmask, amask);
m_Locked = m_IsDisplay = false;
if (!m_SurfaceInfo) return STATUS_FAIL;
return STATUS_OK;
 }

/*! Creates a windowed surface for the screen
 */
_bool c_Gfx_BitBuffer::Create_WindowedScreenSurface(_int32 w, _int32 h, _int32 bpp)
{
//m_SurfaceInfo = SDL_SetVideoMode(w, h, bpp, SDL_ANYFORMAT | SDL_OPENGL | SDL_OPENGLBLIT);
m_SurfaceInfo = SDL_SetVideoMode(w, h, bpp, SDL_ANYFORMAT);
m_Locked = false;
m_IsDisplay = true;
if (!m_SurfaceInfo) return STATUS_FAIL;
return STATUS_OK;
 }

/*! Creates a fullscreen surface for the screen
 */
_bool c_Gfx_BitBuffer::Create_FullScreenSurface(_int32 w, _int32 h, _int32 bpp)
{
//m_SurfaceInfo = SDL_SetVideoMode(w, h, bpp, SDL_DOUBLEBUF | SDL_FULLSCREEN |
//								  			SDL_OPENGL | SDL_OPENGLBLIT);
m_SurfaceInfo = SDL_SetVideoMode(w, h, bpp, SDL_FULLSCREEN);
m_Locked = false;
m_IsDisplay = true;
if (!m_SurfaceInfo) return STATUS_FAIL;
return STATUS_OK;
 }

/*! Locks the surface, and returns a pointer to the pixel data.
	The type of pointer is dependant on the bitdepth of the surface data.
	Returns NULL if the surface can't be locked or hasn't been created.
 */
void *c_Gfx_BitBuffer::Lock(void)
{
if (!m_SurfaceInfo || m_Locked) return NULL;
if (!MustLock()) return m_SurfaceInfo->pixels;
if (SDL_LockSurface(m_SurfaceInfo) == -1) return NULL;
m_Locked = true;
return m_SurfaceInfo->pixels;
 }

/*! Unlocks the surface
 */
void c_Gfx_BitBuffer::Unlock(void)
{
if (!m_SurfaceInfo || !m_Locked) return;
SDL_UnlockSurface(m_SurfaceInfo);
 }

/*! Blits the buffer to the specified destination buffer
	\param dx Destination x position
	\param dy Destination y position
	\param dest Destination buffer to blit to
 */
_bool c_Gfx_BitBuffer::Blit(_int32 dx, _int32 dy, c_Gfx_BitBuffer *dest)
{
if (!m_SurfaceInfo || !dest || !dest->Get_SDLSurface()) return STATUS_FAIL;
SDL_Rect destrect;
destrect.x = dx;
destrect.y = dy;
destrect.w = Get_Width();
destrect.h = Get_Height();
_int32 err = SDL_BlitSurface(m_SurfaceInfo, NULL, dest->Get_SDLSurface(), &destrect);
return !err ? STATUS_OK:STATUS_FAIL;		// Convert to INX style
 }

/*! Blits a portion of the buffer to the specified destination buffer
	\param sx Source x position
	\param sy Source y position
	\param width Width of rectangle to blit
	\param height Height of rectangle to blit
	\param dx Destination x position
	\param dy Destination x position
	\param dest Destination buffer to blit to
 */
_bool c_Gfx_BitBuffer::Blit(_int32 sx, _int32 sy, _int32 width, _int32 height,
							 _int32 dx, _int32 dy, c_Gfx_BitBuffer *dest)
{
if (!m_SurfaceInfo || !dest || !dest->Get_SDLSurface()) return STATUS_FAIL;
SDL_Rect srcrect, destrect;
srcrect.x = sx;
srcrect.y = sy;
srcrect.w = width;
srcrect.h = height;
destrect.x = dx;
destrect.y = dy;
destrect.w = width;
destrect.h = height;
_int32 err = SDL_BlitSurface(m_SurfaceInfo, &srcrect, dest->Get_SDLSurface(), &destrect);
return !err ? STATUS_OK:STATUS_FAIL;		// Convert to INX style
 }

/*! Loads the specified PCX-formatted resource chunk into the bitbuffer.
	Destroys anything currently stored in the buffer.
	Please note this is probably the most inefficient code known to man,
	written a very long time ago.
	Returns STATUS_OK if successful, otherwise STATUS_FAIL.

    NOTE: Depreciated as of 6/2/02; use LoadUsing_SDL instead.
 */
_bool c_Gfx_BitBuffer::Load_PCX(c_Base_ResourceParser::Chunk *chunk)
{
_uchar HeaderByte, ColorByte, Buffer[10], Pal[768];
_uint16 *Data;
_int32 index, Pos, Px, Py, RunLength, Width, Height;
_int16 Temp;

// Check our pointer...
if (!chunk)
	{
	c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Null chunk passed");
	return STATUS_FAIL;
	 }

// Eat some stuff:
if (chunk->Read_Buffer(Buffer, 8) != STATUS_OK) return STATUS_FAIL; //read(PcxFile, Buffer, 8);

// Find the width and the height of the image and ensure it's valid:
Width = 0; if (chunk->Read_Int16(reinterpret_cast<_int16 *>(&Width)) != STATUS_OK) return STATUS_FAIL; // read(PcxFile, &Width, 2);
Height = 0; if (chunk->Read_Int16(reinterpret_cast<_int16 *>(&Height)) != STATUS_OK) return STATUS_FAIL; // read(PcxFile, &Height, 2);
if (Width <= 0 || Height <= 0)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Image size is <= 0!");
    return STATUS_FAIL;
     }

// Go to the palette data and read the palette:
chunk->Seek_Start(chunk->Get_Size() - 768); //lseek(PcxFile, filelength(PcxFile) - 768, SEEK_SET);
if (chunk->Read_Buffer(Pal, 768) != STATUS_OK) return STATUS_FAIL; //read(PcxFile, (void *)Pal, 768);

// Create a new 5550 surface:
Free();					// Kill whatever we have at the moment...
//if (Create_RGBSurface(SDL_SWSURFACE, Width + 1, Height + 1, 16, 0x7c00, 0x3e0, 0x1f, 0x8000) != STATUS_OK) return STATUS_FAIL;
if (Create_RGBSurface(SDL_SWSURFACE, Width + 1, Height + 1,	16, 0x7c00, 0x3e0, 0x1f, 0x8000) != STATUS_OK) return STATUS_FAIL;

// Go to the image data and reset position
chunk->Seek_Start(128); //lseek(PcxFile, 128, SEEK_SET);
Px = Py = 0;
Pos = 0;

// Make sure we have surface data:
Data = reinterpret_cast<_uint16 *>(Lock());
if (!Data)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "No data for img!");
    return STATUS_FAIL;
     }

// Read image:
while (Py < (Height + 3))
    {
    if (chunk->Read_Byte(&HeaderByte) != STATUS_OK) return STATUS_FAIL; //read(PcxFile, &HeaderByte, 1);
    if (HeaderByte >= 193)		// We have a run
        {
        if (chunk->Read_Byte(&ColorByte) != STATUS_OK) return STATUS_FAIL; //read(PcxFile, &ColorByte, 1);
        RunLength = HeaderByte - 193;
        for (index = 0; index < RunLength + 1; index ++)
            {
            Px ++; if (Px > (Width - 1)) { Py ++; Px = 0; }
            Temp = SDL_MapRGB(m_SurfaceInfo->format, Pal[(ColorByte * 3)],
													 Pal[(ColorByte * 3) + 1],
													 Pal[(ColorByte * 3) + 2]);
            Data[Pos ++] = Temp;
             }
         }
    else						// We have just a single pixel
        {
        ColorByte = HeaderByte;
        Px ++; if (Px > Width) { Py ++; Px = 0; }
            Temp = SDL_MapRGB(m_SurfaceInfo->format, Pal[(ColorByte * 3)],
													 Pal[(ColorByte * 3) + 1],
													 Pal[(ColorByte * 3) + 2]);
        Data[Pos ++] = Temp;
         }
     }

// Unlock and return ok:
Unlock();
return STATUS_OK;
 }

/*! Small helper for Load_SGI
 */
void c_Gfx_BitBuffer::Load_SGI_Help(_uchar *p1, _uchar *p2, _uchar *p3, _uchar *p4)
{
if (p1) delete [] p1;			
if (p2) delete [] p2;			
if (p3) delete [] p3;			
if (p4) delete [] p4;
 }

/*! Loads the specified SGI-formatted resource chunk into the bitbuffer.
	Destroys anything currently stored in the buffer.
	Returns STATUS_OK if successful, otherwise STATUS_FAIL.

    NOTE: Depreciated as of 6/2/02; use LoadUsing_SDL instead.
 */
_bool c_Gfx_BitBuffer::Load_SGI(c_Base_ResourceParser::Chunk *chunk)
{
return STATUS_OK;          // No-op, use LoadUsing_SDL instead...
 }

/*! Uses the SDL_image to load the specified imagefile into the buffer.
	Destroys anything currently stored in the buffer.
	Returns STATUS_OK if successful, otherwise STATUS_FAIL.

    Will properly handle any image file that is properly handled by SDL.

 */
_bool c_Gfx_BitBuffer::LoadUsing_SDL(std::string filename)
{
SDL_Surface *newsurf;

// Attempt to load the image:
Free();					// Kill whatever we have at the moment...
newsurf = IMG_Load(filename.c_str());

// If we got an error, log it:
if (!newsurf)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "LoadUsing_SDL failed for file %s; SDL says: %s", filename.c_str(), SDL_GetError());
    return STATUS_FAIL;
     }

// Set up the new surface:
m_SurfaceInfo = newsurf;
m_Locked = m_IsDisplay = false;

// And return ok:
return STATUS_OK;
 }
