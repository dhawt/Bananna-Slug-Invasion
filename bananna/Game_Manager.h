/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Game_Manager.h  -  Main game manager class.  Handles all game logic, i.e.,
                     game state, levels, animation, etc.
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
#ifndef __GAME_MANAGER__INC
#define __GAME_MANAGER__INC

#include "SDL/SDL.h"
#include "Base_Global.h"
#include "Base_Logging.h"
#include "System_Input.h"

// Some game state definitions:
#define GAMESTATE_VOID          0x00
#define GAMESTATE_INIT          0x01
#define GAMESTATE_TITLES        0x02
#define GAMESTATE_MENU          0x04
#define GAMESTATE_GAME          0x08
#define GAMESTATE_HIGHSCORES    0x10
#define GAMESTATE_CREDITS       0x20
#define GAMESTATE_SHUTDOWN      0x40


/*! \brief Game Manager class
    Game manager class.  Interfaces with c_Inp_ReplayManager for all user input.
 */
class c_Game_Manager
    {
    public:
        // Structures with vars for each game state:
        struct s_GameState_Init
            {
            _int32 m_FrameCount;

             };

        struct s_GameState_Titles
            {
            _int32 m_FrameCount;

             };

        struct s_GameState_Menu
            {
            _int32 m_FrameCount;

             };

        struct s_GameState_Game
            {
            _int32 m_FrameCount;

             };

        struct s_GameState_HighScores
            {
            _int32 m_FrameCount;

             };

        struct s_GameState_Credits
            {
            _int32 m_FrameCount;

             };

        struct s_GameState_Shutdown
            {
            _int32 m_FrameCount;

             };

    private:
        _bool m_IsInit;
        _bool m_Quit;
        _int32 m_GameState, m_PrevGameState;
        s_GameState_Init        m_GameState_Init;
        s_GameState_Titles      m_GameState_Titles;
        s_GameState_Menu        m_GameState_Menu;
        s_GameState_Game        m_GameState_Game;
        s_GameState_HighScores  m_GameState_HighScores;
        s_GameState_Credits     m_GameState_Credits;
        s_GameState_Shutdown    m_GameState_Shutdown;

    public:
        c_Game_Manager() : m_IsInit(false), m_Quit(false)
            {
             }
        ~c_Game_Manager() { };

        _bool Init(void);

        void Post_Quit(void);
        _bool Query_Quit(void) { return m_Quit; };  //!< Returns true if app should quit
        _int32 Run(void);

        _int32 Get_GameState(void);
        _int32 Get_GameStateData(void *&data);

     };


#endif
