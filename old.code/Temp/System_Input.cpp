/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  System_Input.cpp
  begin     : Mon Aug 26 2002
  copyright : (C) 2002 by Daniel Hawthorn
  _____________________________________________________________________
  This sourcefile is part of the Banna Slug Invasion project, which is
  free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your
  option) any later version.
\***************************************************************************/
#include "Base_Global.h"
#include "System_Input.h"

namespace n_System_Input
{

// Module locals:
SDL_mutex *m_StateMutex = NULL;
_bool       m_IsInit = false;
_uint16    *m_KeyStates = NULL;


/*! Initializes the system
    User of this system must ensure that Init() and Shutdown() are called before
    or after any other methods in the class, respectively.
 */
_bool Init(void)
{
if (m_IsInit) return STATUS_OK;
m_StateMutex = SDL_CreateMutex();
m_KeyStates = new _uint16[32];            // 512 packed bitflags for key states
if (!m_KeyStates)
    {
    c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Out of memory on Inp_Manager init! (Trouble for sure)");
    return STATUS_FAIL;
     }
m_IsInit = true;
return STATUS_OK;
 }

/*! Shuts down the system
    User of this system must ensure that Init() and Shutdown() are called before
    or after any other methods in the class, respectively.
 */
void Shutdown(void)
{
if (m_IsInit)
    {
    delete [] m_KeyStates;
    SDL_DestroyMutex(m_StateMutex);
      }
 }

/*! Sets the state of the specified key.
 */
void n_System_Input::Set_KeyState(SDLKey k, _bool state)
{
if (!m_IsInit) return;

// Lock our mutex:
#ifdef BUILD_DEBUG
    if (SDL_mutexP(m_StateMutex) == -1) c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Fatal on mutex lock");
#else
    SDL_mutexP(m_StateMutex);
#endif

_uint16 j = static_cast<_uint16>(k);
_uint16 l = j >> 4;             // Divide by 16 to get major index
_uint16 m = j & 0xfff0;         // Mod by 16 to get bit number
m_KeyStates[l] |= ((state ? 1:0) << m);     // Set the neccesary bit

// Unlock our mutex:
#ifdef BUILD_DEBUG
    if (SDL_mutexV(m_StateMutex) == -1) c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Fatal on mutex unlock");
#else
    SDL_mutexV(m_StateMutex);
#endif
 }

/*! Returns the state of the specified key.
 */
_bool n_System_Input::Get_KeyState(SDLKey k)
{
if (!m_IsInit) return false;

// Lock our mutex:
#ifdef BUILD_DEBUG
    if (SDL_mutexP(m_StateMutex) == -1) c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Fatal on mutex lock");
#else
    SDL_mutexP(m_StateMutex);
#endif

_uint16 j = static_cast<_uint16>(k);
_uint16 l = j >> 4;             // Divide by 16 to get major index
_uint16 m = j & 0xfff0;         // Mod by 16 to get bit number

_bool state = false;
if (m_KeyStates[l] & (1 << m) > 0) state = true;

// Unlock our mutex:
#ifdef BUILD_DEBUG
    if (SDL_mutexV(m_StateMutex) == -1) c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Fatal on mutex unlock");
#else
    SDL_mutexV(m_StateMutex);
#endif

return state;
 }

 } // End namespace n_System_Input
