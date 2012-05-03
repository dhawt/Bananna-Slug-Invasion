/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Gfx_BitBuffer.h  -  BitBuffer class, encapsulating most SDL functionality
                      plus some bells and whistles.
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
#ifndef __GFX_BITBUFFER__INC
#define __GFX_BITBUFFER__INC

#include "Base_Global.h"
#include "SDL/SDL.h"
#include "Base_NamedResource.h"
#include "Base_ResourceParser.h"

//! Surface is 256 color palettized texture
const _int32 BITBUFFER_SCREEN_PAL_8 = 0;

//! The surface contains alpha info
const _int32 BITBUFFER_HASALPHA = SDL_SRCALPHA;

/*! \class c_Gfx_BitBuffer
	\brief Bitmap buffer encapsulating SDL_Surface

	Encapsulates SDL_Surface functionality and provides general-purpose
	bitmap functionality.
	Includes
 */
class c_Gfx_BitBuffer : public c_Base_NamedResource
	{
	private:
	  	SDL_Surface *m_SurfaceInfo;
		_bool m_Locked, m_IsDisplay;
		_int32 m_RefCount;

		void Load_SGI_Help(_uchar *p1, _uchar *p2, _uchar *p3, _uchar *p4);
		
        /*! Returns the number of bits required to store the specified number. */
        _int32 Get_BitWidth(_int32 n)
            {
            if (n >= 1) return 1;
            if (n >= 3) return 2;
            if (n >= 7) return 3;
            if (n >= 15) return 4;
            if (n >= 31) return 5;
            if (n >= 63) return 6;
            if (n >= 127) return 7;
            return 8;
             }

	public:
		c_Gfx_BitBuffer() { m_SurfaceInfo = NULL;
							  m_Locked = m_IsDisplay = false;
							  m_RefCount = 0; };
		~c_Gfx_BitBuffer() { Free(); };

		inline SDL_Surface *Get_SDLSurface(void)
            {
            #ifdef BUILD_DEBUG
                if (!m_SurfaceInfo) c_Base_Logging::Log_Msg(__LINE__, __FILE__, "NULL surface");
            #endif
            return m_SurfaceInfo;
             };
		inline _bool Set_SDLSurface(SDL_Surface *surf) { m_SurfaceInfo = surf; };

		_bool Create_RGBSurface(_int32 flags, _int32 w, _int32 h, _int32 depth, _int32 rmask, _int32 gmask, _int32 bmask, _int32 amask);
		_bool Create_WindowedScreenSurface(_int32 w, _int32 h, _int32 bpp);
		_bool Create_FullScreenSurface(_int32 w, _int32 h, _int32 bpp);
		void Free(void) { if (m_SurfaceInfo && !m_IsDisplay) SDL_FreeSurface(m_SurfaceInfo); };

		inline _bool MustLock(void) { return SDL_MUSTLOCK(m_SurfaceInfo); };
		void *Lock(void);
		void Unlock(void);

		_bool Blit(_int32 dx, _int32 dy, c_Gfx_BitBuffer *dest);
		_bool Blit(_int32 sx, _int32 sy, _int32 width, _int32 height,
				  _int32 dx, _int32 dy, c_Gfx_BitBuffer *dest);

		inline _int32 Get_Width(void) { if (!m_SurfaceInfo) return 0; return m_SurfaceInfo->w; };
		inline _int32 Get_Height(void) { if (!m_SurfaceInfo) return 0; return m_SurfaceInfo->h; };
		inline _int32 Get_Pitch(void) { if (!m_SurfaceInfo) return 0; return m_SurfaceInfo->pitch; };

		_bool Load_PCX(c_Base_ResourceParser::Chunk *chunk);  // Depreciated
		_bool Load_SGI(c_Base_ResourceParser::Chunk *chunk);  // Depreciated
		_bool LoadUsing_SDL(std::string filename);

        template <class PixelData>
        PixelData *Get_PixelPointer(void *buff, _int32 x, _int32 y);
        inline void *Get_PixelPointerRaw(void *buff, _int32 x, _int32 y);
        template <class PixelData>
        PixelData Get_Pixel(void *buff, _int32 x, _int32 y);
        inline _int32 Get_Pixel32(void *buff, _int32 x, _int32 y);
        template <class PixelData>
        inline void Set_Pixel(PixelData pix, void *buff, _int32 x, _int32 y);
        inline void Set_Pixel32(_int32 pix, void *buff, _int32 x, _int32 y);
	 };

/*! Templated helper function to get the a pointer pixel data from the specified
    (locked!) surface data.  Assumes the surface data (passed in buff) has been
    obtained from a call to Lock() and that the bits-per-pixel is the same as the
    data type passed in PixelData.  Returns a pointer to the pixel data in the
    location specified.
 */
template <class PixelData>
PixelData *c_Gfx_BitBuffer::Get_PixelPointer(void *buff, _int32 x, _int32 y)
{
// Make sure x and y are valid:
if (x < 0 || y < 0 || x > Get_Width() || y > Get_Width())
    {
#ifdef BUILD_DEBUG
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Out of bounds");
#endif
    return NULL;
     }

// Assume bpp is the sizeof(PixelData):
_int32 bpp = sizeof(PixelData); //Get_SDLSurface()->format->BytesPerPixel;

// If in debug build, ensure that it's correct:
#ifdef BUILD_DEBUG
    if (bpp != Get_SDLSurface()->format->BytesPerPixel)
        {
        c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Passed datatype and BPP of surface do not agree!");
        return 0;
         }
#endif

// Return a pointer to the pixel data:
_char *tmp = reinterpret_cast<_char *>(buff);
return (PixelData *)(tmp + (y * Get_Pitch()) + (x * bpp));
 }

/*! Function to get the a pointer pixel data from the specified
    (locked!) surface data.  Assumes the surface data (passed in buff) has been
    obtained from a call to Lock().  Returns a pointer to the pixel data in the
    location specified.
 */
void *c_Gfx_BitBuffer::Get_PixelPointerRaw(void *buff, _int32 x, _int32 y)
{
// Make sure x and y are valid:
if (x < 0 || y < 0 || x > Get_Width() || y > Get_Width())
    {
#ifdef BUILD_DEBUG
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Out of bounds");
#endif
    return NULL;
     }

// Get bpp and return a pointer:
_int32 bpp = Get_SDLSurface()->format->BytesPerPixel;
_char *tmp = reinterpret_cast<_char *>(buff);
return reinterpret_cast<void *>(tmp + (y * Get_Pitch()) + (x * bpp));
 }

/*! Templated function to get the pixel data from the specified (locked!)
    surface data.  Assumes the surface data (passed in buff) has been obtained
    from a call to Lock() and that the bits-per-pixel is the same as the data type
    passed in PixelData.  Returns the pixel data from the location specified.
 */
template <class PixelData>
PixelData c_Gfx_BitBuffer::Get_Pixel(void *buff, _int32 x, _int32 y)
{
// Make sure x and y are valid:
if (x < 0 || y < 0 || x > Get_Width() || y > Get_Width())
    {
#ifdef BUILD_DEBUG
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Out of bounds");
#endif
    return 0;
     }

// Assume bpp is the sizeof(PixelData):
_int32 bpp = sizeof(PixelData); //Get_SDLSurface()->format->BytesPerPixel;

// If in debug build, ensure that it's correct:
#ifdef BUILD_DEBUG
    if (bpp != Get_SDLSurface()->format->BytesPerPixel)
        {
        c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Passed datatype and BPP of surface do not agree!");
        return 0;
         }
#endif

// Return the pixel data:
return *Get_PixelPointer<PixelData>(buff, x, y);
 }

/*! Function to get the pixel data from the specified (locked!)
    surface data.  Assumes the surface data (passed in buff) has been obtained
    from a call to Lock().  Transforms the pixel data to 32-bit RGBA format and
    returns.
 */
_int32 c_Gfx_BitBuffer::Get_Pixel32(void *buff, _int32 x, _int32 y)
{
// Make sure x and y are valid:
if (x < 0 || y < 0 || x > Get_Width() || y > Get_Width())
    {
#ifdef BUILD_DEBUG
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Out of bounds");
#endif
    return 0;
     }

// Get bpp of surface:
_int32 bpp = Get_SDLSurface()->format->BytesPerPixel;

// Get a pointer to the specified pixel:
void *bits = Get_PixelPointerRaw(buff, x, y);

// Now transform the pixel:
if (bpp == 1)
    {
    // Grab the raw channel data:
    _uchar r, g, b, a, newr, newg, newb, newa;
    r = (*reinterpret_cast<_uchar *>(bits) & Get_SDLSurface()->format->Rmask) >> Get_SDLSurface()->format->Rshift;
    g = (*reinterpret_cast<_uchar *>(bits) & Get_SDLSurface()->format->Gmask) >> Get_SDLSurface()->format->Gshift;
    b = (*reinterpret_cast<_uchar *>(bits) & Get_SDLSurface()->format->Bmask) >> Get_SDLSurface()->format->Bshift;
    a = (*reinterpret_cast<_uchar *>(bits) & Get_SDLSurface()->format->Amask) >> Get_SDLSurface()->format->Ashift;

    // Scale channel values to 8-bit normalized:
    newr = (_uchar)((_float)r * (_float)(8 / (_float)Get_BitWidth(Get_SDLSurface()->format->Rmask >> Get_SDLSurface()->format->Rshift)));
    newg = (_uchar)((_float)g * (_float)(8 / (_float)Get_BitWidth(Get_SDLSurface()->format->Bmask >> Get_SDLSurface()->format->Gshift)));
    newb = (_uchar)((_float)b * (_float)(8 / (_float)Get_BitWidth(Get_SDLSurface()->format->Bmask >> Get_SDLSurface()->format->Bshift)));
    newa = (_uchar)((_float)a * (_float)(8 / (_float)Get_BitWidth(Get_SDLSurface()->format->Amask >> Get_SDLSurface()->format->Ashift)));

    // And return the new pixel:
    return ((newr << 24) | (newg << 16) | (newb << 8) | newa);
     }
if (bpp == 2)
    {
    // Grab the raw channel data:
    _uint16 r, g, b, a, newr, newg, newb, newa;
    r = (*reinterpret_cast<_uint16 *>(bits) & Get_SDLSurface()->format->Rmask) >> Get_SDLSurface()->format->Rshift;
    g = (*reinterpret_cast<_uint16 *>(bits) & Get_SDLSurface()->format->Gmask) >> Get_SDLSurface()->format->Gshift;
    b = (*reinterpret_cast<_uint16 *>(bits) & Get_SDLSurface()->format->Bmask) >> Get_SDLSurface()->format->Bshift;
    a = (*reinterpret_cast<_uint16 *>(bits) & Get_SDLSurface()->format->Amask) >> Get_SDLSurface()->format->Ashift;

    // Scale channel values to 8-bit normalized:
    newr = (_uint16)((_float)r * (_float)(8 / (_float)Get_BitWidth(Get_SDLSurface()->format->Rmask >> Get_SDLSurface()->format->Rshift)));
    newg = (_uint16)((_float)g * (_float)(8 / (_float)Get_BitWidth(Get_SDLSurface()->format->Bmask >> Get_SDLSurface()->format->Gshift)));
    newb = (_uint16)((_float)b * (_float)(8 / (_float)Get_BitWidth(Get_SDLSurface()->format->Bmask >> Get_SDLSurface()->format->Bshift)));
    newa = (_uint16)((_float)a * (_float)(8 / (_float)Get_BitWidth(Get_SDLSurface()->format->Amask >> Get_SDLSurface()->format->Ashift)));

    // And return the new pixel:
    return ((newr << 24) | (newg << 16) | (newb << 8) | newa);
     }
if (bpp == 3)
    {
    // Grab the raw channel data:
    _uint32 r, g, b, a, newr, newg, newb, newa;
    r = (*reinterpret_cast<_uint32 *>(bits) & Get_SDLSurface()->format->Rmask) >> Get_SDLSurface()->format->Rshift;
    g = (*reinterpret_cast<_uint32 *>(bits) & Get_SDLSurface()->format->Gmask) >> Get_SDLSurface()->format->Gshift;
    b = (*reinterpret_cast<_uint32 *>(bits) & Get_SDLSurface()->format->Bmask) >> Get_SDLSurface()->format->Bshift;
    a = (*reinterpret_cast<_uint32 *>(bits) & Get_SDLSurface()->format->Amask) >> Get_SDLSurface()->format->Ashift;

    // Scale channel values to 8-bit normalized:
    newr = (_uint32)((_float)r * (_float)(8 / (_float)Get_BitWidth(Get_SDLSurface()->format->Rmask >> Get_SDLSurface()->format->Rshift)));
    newg = (_uint32)((_float)g * (_float)(8 / (_float)Get_BitWidth(Get_SDLSurface()->format->Bmask >> Get_SDLSurface()->format->Gshift)));
    newb = (_uint32)((_float)b * (_float)(8 / (_float)Get_BitWidth(Get_SDLSurface()->format->Bmask >> Get_SDLSurface()->format->Bshift)));
    newa = (_uint32)((_float)a * (_float)(8 / (_float)Get_BitWidth(Get_SDLSurface()->format->Amask >> Get_SDLSurface()->format->Ashift)));

    // And return the new pixel:
    return ((newr << 24) | (newg << 16) | (newb << 8) | newa);
     }
if (bpp == 4)
    {
    // Grab the raw channel data:
    _uint32 r, g, b, a;
    r = (*reinterpret_cast<_uint32 *>(bits) & Get_SDLSurface()->format->Rmask) >> Get_SDLSurface()->format->Rshift;
    g = (*reinterpret_cast<_uint32 *>(bits) & Get_SDLSurface()->format->Gmask) >> Get_SDLSurface()->format->Gshift;
    b = (*reinterpret_cast<_uint32 *>(bits) & Get_SDLSurface()->format->Bmask) >> Get_SDLSurface()->format->Bshift;
    a = (*reinterpret_cast<_uint32 *>(bits) & Get_SDLSurface()->format->Amask) >> Get_SDLSurface()->format->Ashift;

    // And return the new pixel:
    return ((r << 24) | (g << 16) | (b << 8) | a);
     }

 }

/*! Templated function to set the pixel data in the specified (locked!)
    surface data.  Assumes the surface data (passed in buff) has been obtained
    from a call to Lock() and that the bits-per-pixel is the same as the data type
    passed in PixelData.
 */
template <class PixelData>
void c_Gfx_BitBuffer::Set_Pixel(PixelData pix, void *buff, _int32 x, _int32 y)
{
// Make sure x and y are valid:
if (x < 0 || y < 0 || x > Get_Width() || y > Get_Width())
    {
#ifdef BUILD_DEBUG
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Out of bounds");
#endif
    return;
     }

// Assume bpp is the sizeof(PixelData):
_int32 bpp = sizeof(PixelData); //Get_SDLSurface()->format->BytesPerPixel;

// If in debug build, ensure that it's correct:
#ifdef BUILD_DEBUG
    if (bpp != Get_SDLSurface()->format->BytesPerPixel)
        {
        c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Passed datatype and BPP of surface do not agree!");
        return 0;
         }
#endif

// Set the pixel data:
*Get_PixelPointer(buff, x, y) = pix;
 }

/*! Templated function to set the pixel data in the specified (locked!)
    surface data.  Assumes the surface data (passed in buff) has been obtained
    from a call to Lock() and that the bits-per-pixel is the same as the data type
    passed in PixelData. pix is a 32-bit RGBA pixel.
 */
void c_Gfx_BitBuffer::Set_Pixel32(_int32 pix, void *buff, _int32 x, _int32 y)
{
// Make sure x and y are valid:
if (x < 0 || y < 0 || x > Get_Width() || y > Get_Width())
    {
#ifdef BUILD_DEBUG
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Out of bounds");
#endif
    return;
     }

// Get bpp of surface:
_int32 bpp = Get_SDLSurface()->format->BytesPerPixel;

// Get a pointer to the specified pixel:
void *bits = Get_PixelPointerRaw(buff, x, y);

// Now transform the pixel:
if (bpp == 1)
    {
    // Convert the pixel to the pixel format of the buffer:
    _uchar pix = SDL_MapRGB(m_SurfaceInfo->format, (pix & 0xff000000) >> 24,
                                                          (pix & 0x00ff0000) >> 16,
                                                          (pix & 0x0000ff00) >> 8);
    _uchar newa = (_uchar)((_float)(pix & 0x000000ff) * (_float)((_float)Get_BitWidth(Get_SDLSurface()->format->Amask >> Get_SDLSurface()->format->Ashift)) / 8.0f);
    pix |= (newa << Get_SDLSurface()->format->Ashift);
    *reinterpret_cast<_uchar *>(bits) = pix;
     }
if (bpp == 2)
    {
    // Convert the pixel to the pixel format of the buffer:
    _uint16 pix = SDL_MapRGB(m_SurfaceInfo->format, (pix & 0xff000000) >> 24,
                                                           (pix & 0x00ff0000) >> 16,
                                                           (pix & 0x0000ff00) >> 8);
    _uchar newa = (_uchar)((_float)(pix & 0x000000ff) * (_float)((_float)Get_BitWidth(Get_SDLSurface()->format->Amask >> Get_SDLSurface()->format->Ashift)) / 8.0f);
    pix |= (newa << Get_SDLSurface()->format->Ashift);
    *reinterpret_cast<_uint16 *>(bits) = pix;
     }
if (bpp == 3 || bpp == 4)
    {
    // Convert the pixel to the pixel format of the buffer:
    _uint32 pix = SDL_MapRGB(m_SurfaceInfo->format, (pix & 0xff000000) >> 24,
                                                         (pix & 0x00ff0000) >> 16,
                                                         (pix & 0x0000ff00) >> 8);
    _uchar newa = (_uchar)((_float)(pix & 0x000000ff) * (_float)((_float)Get_BitWidth(Get_SDLSurface()->format->Amask >> Get_SDLSurface()->format->Ashift)) / 8.0f);
    pix |= (newa << Get_SDLSurface()->format->Ashift);
    *reinterpret_cast<_uint32 *>(bits) = pix;
     }
 }

#endif
