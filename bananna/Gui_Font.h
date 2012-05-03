/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Gui_Font.h  -  Kerned cell font class.
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
#ifndef __GUI_FONT__INC
#define __GUI_FONT__INC

#include "Base_Global.h"
#include "Base_Logging.h"
#include "Gui_Elt.h"
#include "Gfx_BitBuffer.h"

/*! \brief GUI cell font class
	A named GUI cell font.  Contains the name of the bitmap with the actual font,
	as well as a lookup to match bitmap cells to textual characters.
    Contains routines for rendering strings of text to other bitbuffers.
 */
class c_Gui_Font : public c_Base_NamedResource
	{
	protected:
		/*! An index class for a bitmap font.  Contains a table
 			to look up bitmap cells for a specific character.
	 	 */
		class Index
			{
			protected:
				_int32 m_CharWidth,			// Width of each char in pixels
					m_CharHeight;			// Height of each char in pixels
				_char m_Indices[256],        // Indices to cell for each ascii char
                     m_Kerning[256];        // Kerned cell width for each cell
			
				_bool Copy(const Index &idx);

			public:
				Index(_int32 cwidth = 0, _int32 cheight = 0);
                Index(const Index &i) { if (&i != this) Copy(i); };
				~Index();

				_bool Init(_int32 cwidth, _int32 cheight);

				inline _bool operator=(const Index &idx) { return Copy(idx); };
				/*! Returns the cell index i in the bitmap for ascii _char c. */
				inline _int32 Get_Index(_int32 c)
                    {
                    if (c == ' ') return -1;    // -1 is whitespace
                    if (c > 0 && c < 256) return m_Indices[c];
                    return -1;      // Default to -1 for whitespace
                     };
				/*! Sets the cell index i in the bitmap for ascii char c */
				inline void Set_Index(_int32 c, _int32 i) { if (c > 0 && c < 256) m_Indices[c] = i; };
				inline _int32 Get_CharWidth(void) { return m_CharWidth; };
				inline void Set_CharWidth(_int32 w) { if (w > 0) m_CharWidth = w; };
				inline _int32 Get_CharKerning(_int32 c) { if (c > 0 && c < 256) return m_Kerning[m_Indices[c]]; return 1; };
				inline void Set_CharKerning(_int32 c, _int32 k) { if (c > 0 && c < 256) m_Kerning[m_Indices[c]] = k; };
				inline void Set_CellKerning(_int32 i, _int32 k) { if (i > 0 && i < 256) m_Kerning[i] = k; };
				inline _int32 Get_CharHeight(void) { return m_CharHeight; };
				inline void Set_CharHeight(_int32 w) { if (w > 0) m_CharHeight = w; };
			 };

	protected:
		c_Gfx_BitBuffer *m_FontBuff;
	    Index m_Index;

        /*! Returns the maximum number of cells in the bitmap. */
		inline _int32 Get_MaxCells(void)
            {
            if (!m_FontBuff) return 0;
            return (m_FontBuff->Get_Width() / m_Index.Get_CharWidth()) * (m_FontBuff->Get_Height() / m_Index.Get_CharHeight());
             };
	public:
		c_Gui_Font();
		~c_Gui_Font();

		_bool Print_Char(_int32 x, _int32 y, c_Gfx_BitBuffer &dest, _char c);
		_bool Print_Str(_int32 x, _int32 y, c_Gfx_BitBuffer &dest, _char *str);
		_bool Print(_int32 x, _int32 y, c_Gfx_BitBuffer &dest, _char *str, ...);

		_bool Load_Descriptor(c_Base_ResourceParser::Chunk *chunk);
        void Set_BitBuffer(c_Gfx_BitBuffer *buff, _int32 kernSpacing);

     };

#endif
