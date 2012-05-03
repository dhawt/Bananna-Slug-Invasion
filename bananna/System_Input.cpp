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
/*\file */
#include "System_Input.h"
#include "Base_Global.h"
#include "Base_Logging.h"
#include "SDL/SDL.h"

namespace n_System_Input
{

// Module locals:
_bool             m_IsInit = false;
_uint16          *m_KeyStates = NULL;
SDL_keysym        m_Queue[8];
_int32            m_NumQueued = 0;


/*! Initializes the system
    User of this system must ensure that Init() and Shutdown() are called before
    or after any other methods in the class, respectively.
 */
_bool Init(void)
{
if (m_IsInit) return STATUS_OK;
m_KeyStates = new _uint16[32];            // 512 packed bitflags for key states, i.e., 64 bytes
memset(m_KeyStates, 0, 64);   // Clear the array
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
      }
 }

/*! Sets the state of the specified key.
 */
void n_System_Input::Handle_Key(SDL_KeyboardEvent *key)
{
if (!m_IsInit || !key) return;

// Find the byte and bit address for this keysym in our packed array:
_uint16 j = static_cast<_uint16>(key->keysym.sym);
_uint16 l = j >> 4;             // Divide by 16 to get major index
_uint16 m = j & 0x000f - 1;         // Mod by 16 to get bit number

// Update the state depending on the event:
if (key->type == SDL_KEYDOWN)
    {
    m_KeyStates[l] |= (1 << m);

c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Event from key: %s mapping to: %d,%d state: %x", SDL_GetKeyName(key->keysym.sym), l, m, m_KeyStates[l]);

    if (m_NumQueued < 8)            // If we have space in the queue, add this event
        {
        m_NumQueued ++;
        m_Queue[m_NumQueued - 1] = key->keysym;
         }
     }
else
{
    m_KeyStates[l] &= ~(1 << m);
c_Base_Logging::Log_Msg(__LINE__, __FILE__, "Event from key: %s mapping to: %d,%d state: %x", SDL_GetKeyName(key->keysym.sym), l, m, m_KeyStates[l]);
 }

 }

/*! Returns true if the specified key is currently depressed.
 */
_bool n_System_Input::Get_KeyState(SDLKey k)
{
if (!m_IsInit) return false;

// Find the byte and bit address for this keysym in our packed array:
_uint16 j = static_cast<_uint16>(k);
_uint16 l = j >> 4;             // Divide by 16 to get major index
_uint16 m = j & 0x000f - 1;         // Mod by 16 to get bit number

// Get the state of aformentioned key:
_bool state = false;
if (m_KeyStates[l] & (1 << m)) state = true;

// And return the state of the key:
return state;
 }

/*! Retrieves the next keypress from the queue.  Returns false if there are no
    presses in the queue, otherwise true, and returns the press information in k.
    Passing NULL for parameter k simply dequeues without returning information.
 */
_bool n_System_Input::Dequeue_Press(SDL_keysym *k)
{
if (!m_IsInit) return false;
_bool found = false;

if (m_NumQueued > 0)
    {
    m_NumQueued --;
    if (k) *k = m_Queue[m_NumQueued];
    found = true;
     }

return found;
 }

/*! Clears all queued keypresses.
 */
void n_System_Input::Clear_Queue(void)
{
if (!m_IsInit) return;

m_NumQueued = 0;

 }

 } // End namespace n_System_Input
