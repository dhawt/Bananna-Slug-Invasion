/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Game_Manager.cpp
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
#include "Game_Manager.h"
#include "System_Resource.h"
#include "System_API.h"
#include "Base_Time.h"

/*! \brief Initializes the game manager system
    Pass pointers to other subsystems to which the manager
    must have access.
 */
_bool c_Game_Manager::Init(void)
{
// Setup each state and state data:
m_GameState = GAMESTATE_INIT;
m_PrevGameState = GAMESTATE_VOID;
m_GameState_Init.m_FrameCount = 0;
m_GameState_Titles.m_FrameCount = 0;
m_GameState_Menu.m_FrameCount = 0;
m_GameState_Game.m_FrameCount = 0;
m_GameState_HighScores.m_FrameCount = 0;
m_GameState_Credits.m_FrameCount = 0;
m_GameState_Shutdown.m_FrameCount = 0;

m_IsInit = true;
return STATUS_OK;
 }

/*! \brief Sets the state to the "really quit?" dialog.
 */
void c_Game_Manager::Post_Quit(void)
{
//!!! TODO: implement

m_Quit = true;
 }

/*! \brief Main function for manager thread.
 */
_int32 c_Game_Manager::Run(void)
{
if (!m_IsInit)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Can't run uninitialized manager!");
    return 0;
     }

// Logic for each game state:
if (m_GameState == GAMESTATE_INIT)
    {
    /**********************************************************
    ***********************************************************

      Initialization - GAMESTATE_INIT

    ***********************************************************
    **********************************************************/

    // Do all initializations here on first pass:
    if (m_GameState_Init.m_FrameCount == 0)
        {

         }

    // Increment frame counter:
    m_GameState_Init.m_FrameCount ++;
    if (m_GameState_Init.m_FrameCount > 1)  // Give it two frames so the display manager
        {                                 // gets a chance to see it
        m_GameState_Init.m_FrameCount = 0;
        m_GameState = GAMESTATE_TITLES;
         }
     }

if (m_GameState == GAMESTATE_TITLES)
    {
    /**********************************************************
    ***********************************************************

      Various introductions - GAMESTATE_TITLES

    ***********************************************************
    **********************************************************/

    // Handle input:
    if (n_System_Input::Dequeue_Press(NULL)) m_GameState = GAMESTATE_MENU;
	 }

if (m_GameState == GAMESTATE_MENU)
    {
    /**********************************************************
    ***********************************************************

      Menu screen - GAMESTATE_MENU

    ***********************************************************
    **********************************************************/

    // Do all initializations here on first pass:
    if (m_GameState_Init.m_FrameCount == 0)
        {

         }

    // Handle input:
    if (n_System_Input::Get_KeyState(SDLK_ESCAPE)) Post_Quit();

    // Increment frame counter:
    m_GameState_Menu.m_FrameCount ++;
    //if (m_GameState_Init.m_FrameCount > 1)
    //    {
    //    m_GameState_Init.m_FrameCount = 0;
    //    m_GameState = GAMESTATE_TITLES;
    //     }
     }

return 1;
 }

/*! \brief Returns the current game state.
 */
_int32 c_Game_Manager::Get_GameState(void)
{
return m_GameState;
 }

/*! Sets parameter data to point to the state data for the current state.
    Returns the game state, so calling function must cast the pointer to the
    appropriate type to properly access the data.
 */
_int32 c_Game_Manager::Get_GameStateData(void *&data)
{
if (m_GameState == GAMESTATE_INIT) data = &m_GameState_Init;
else if (m_GameState == GAMESTATE_TITLES) data = &m_GameState_Titles;
else if (m_GameState == GAMESTATE_MENU) data = &m_GameState_Menu;
else if (m_GameState == GAMESTATE_GAME) data = &m_GameState_Game;
else if (m_GameState == GAMESTATE_HIGHSCORES) data = &m_GameState_HighScores;
else if (m_GameState == GAMESTATE_CREDITS) data = &m_GameState_Credits;
else if (m_GameState == GAMESTATE_SHUTDOWN) data = &m_GameState_Shutdown;
return m_GameState;
 }
