/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Disp_Manager.cpp
  begin     : Wed Aug 21 2002
  copyright : (C) 2002 by Daniel Hawthorn
  _____________________________________________________________________
  This sourcefile is part of the Banna Slug Invasion project, which is
  free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your
  option) any later version.
\***************************************************************************/
#include "Base_Global.h"
#include "Disp_Manager.h"
#include "System_Resource.h"
#include "System_API.h"

/*! \brief Initializes the display manager system
    Pass pointers to other subsystems to which the event manager
    must have access.
 */
_bool c_Disp_Manager::Init(c_Game_Manager *gm)
{
m_IsInit = true;
if (!gm)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "NULL manager passed!");
    m_IsInit = false;
     }
i_GameManager = gm;
return m_IsInit ? STATUS_OK:STATUS_FAIL;
 }

/*! \brief Main function for manager thread.
    Does one frame update.
 */
_int32 c_Disp_Manager::Run(void)
{
if (!m_IsInit)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Can't run uninitialized manager!");
    return 0;
     }

// Static vars:
static c_Gui_Font *font;

// Display each game state:
if (i_GameManager->Get_GameState() == GAMESTATE_INIT)
    {
    /**********************************************************
    ***********************************************************

      Initialization - GAMESTATE_INIT

    ***********************************************************
    **********************************************************/

    c_Game_Manager::s_GameState_Init *statedata;
    i_GameManager->Get_GameStateData(static_cast<void *>(statedata));

    // Load and setup some fonts:
    if (!n_System_Resource::Load_Resource_BitBuffer("Font_VerdanaBig.png")) { c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Fail"); return 0; };
    if (!n_System_Resource::Load_Resource_Font("Font_VerdanaBig.rdf")) { c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Fail"); return 0; };
    n_System_Resource::Get_Resource_Font("Font_VerdanaBig.rdf")->Set_BitBuffer(n_System_Resource::Get_Resource_BitBuffer("Font_VerdanaBig.png"), 2);

    // Load some graphics:
    if (!n_System_Resource::Load_Resource_BitBuffer("Slug_1.png")) { c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Fail"); return 0; };
     }

if (i_GameManager->Get_GameState() == GAMESTATE_TITLES)
    {
    /**********************************************************
    ***********************************************************

      Various introductions - GAMESTATE_TITLES

    ***********************************************************
    **********************************************************/
    c_Gfx_BitBuffer *disp, *slug;

    c_Game_Manager::s_GameState_Titles *statedata;
    i_GameManager->Get_GameStateData(static_cast<void *>(statedata));

	disp = n_System_API::Get_Display_BitBuffer();
    font = n_System_Resource::Get_Resource_Font("Font_VerdanaBig.rdf");
    font->Print(10, 10, *disp, "Bananna Slug Invasion v0.1 (pre-release)");
    font->Print(10, 30, *disp, "Copyright 2002 Daniel Hawthorn");
    font->Print(10, 50, *disp, "See documentation for license information");
    font->Print(10, 460, *disp, "Press any key to continue...");

    slug = n_System_Resource::Get_Resource_BitBuffer("Slug_1.png");
    if (!slug) return 0;
    slug->Blit(400, 10, disp);
	 }

if (i_GameManager->Get_GameState() == GAMESTATE_MENU)
    {
    /**********************************************************
    ***********************************************************

      Menu screen - GAMESTATE_MENU

    ***********************************************************
    **********************************************************/
    c_Gfx_BitBuffer *disp;

    c_Game_Manager::s_GameState_Menu *statedata;
    i_GameManager->Get_GameStateData(static_cast<void *>(statedata));

	disp = n_System_API::Get_Display_BitBuffer();
    font = n_System_Resource::Get_Resource_Font("Font_VerdanaBig.rdf");
    font->Print(10, 10, *disp, "Menu screen - frame: %d", statedata->m_FrameCount);
     }

// Update the display for this frame:
n_System_API::Update_Display();

return 1;
 }

