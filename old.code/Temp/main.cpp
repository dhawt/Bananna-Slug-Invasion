/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  main.cpp
  begin     : Mon Aug 19 22:24:12 UTC 2002
  copyright : (C) 2002 by Daniel Hawthorn
  _____________________________________________________________________
  This sourcefile is part of the Banna Slug Invasion project, which is
  free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your
  option) any later version.
\***************************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "System_API.h"

int main(int argc, char *argv[])
{
// Init logging:
if (c_Base_Logging::Begin("run.log") != STATUS_OK)
    {
    printf("Init error: Can't begin logging!\n");
    return 0;
     }

// Init, run, shutdown, and jet:
n_System_API::Init();
n_System_API::Start();
n_System_API::Shutdown();
c_Base_Logging::End();

return 0;
 }
