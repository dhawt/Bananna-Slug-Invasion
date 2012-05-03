/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Base_Global.h  -  Contains general program-wide global #defines,
                    as well as the neccessary header inclusions to
                    set up the memory manager.
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
#ifndef __BASE_GLOBAL__INC
#define __BASE_GLOBAL__INC

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "mmgr.h"
#include "Base_Status.h"


// Global defines for engine:
#define BUILD_DEBUG
//#define BUILD_RELEASE

// Data types:
#define     _bool   bool
#define     _char   char
#define     _uchar  unsigned char
#define     _byte   _char
#define     _ubyte  _uchar
#define     _int8   _byte
#define     _uint8  _ubyte
#define     _int16  short
#define     _uint16 unsigned short
#define     _int32  int
#define     _uint32 unsigned int
#define     _int64  int64
#define     _uint64 unsigned int64
#define     _float  float
#define     _double double

#endif
