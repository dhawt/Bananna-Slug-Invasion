/***************************************************************************\
  .,;:Bananna Slug Invasion:;,.

  Base_Time.h  -  Various timebase routines.
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
#ifndef __BASE_TIME__INC
#define __BASE_TIME__INC

#include "Base_Global.h"
#include "SDL/SDL.h"

/*! Millisecond-resolution timer system using SDL_GetTicks()
 */
class c_Timebase
    {
    private:

    public:

        static _uint32 Get_Time() { return SDL_GetTicks(); };
     };

/*! Timer class based on c_Timebase
 */
class c_Timer
    {
    private:
        _uint32 m_StartTime, m_EndTime, m_Elapsed;
        _bool m_isRunning;
        c_Timebase i_Timebase;

    public:
        c_Timer() { Reset(); };
        ~c_Timer() { };

        inline void Reset();
        inline void Start();
        inline _uint32 Stop();
        inline _uint32 Get_Elapsed();
     };

/*! Resets the timer to zero.
 */
inline void c_Timer::Reset()
{
m_StartTime = m_EndTime = m_Elapsed = 0;
m_isRunning = false;
 }

/*! Begins the timer if not already running.
    Does nothing if the timer is already running.
 */
inline void c_Timer::Start()
{
if (m_isRunning) return;
m_StartTime = i_Timebase.Get_Time();
m_isRunning = true;
 }

/*! Stops the timer if it is running.  If not, has no effect.
 */
inline _uint32 c_Timer::Stop()
{
if (!m_isRunning) return 0;
m_EndTime = i_Timebase.Get_Time();
m_isRunning = false;
m_Elapsed = m_EndTime - m_StartTime;
 }

/*! Returns the number of milliseconds that elapsed during the last Start()/Stop()
    cycle of the timer.
 */
inline _uint32 c_Timer::Get_Elapsed()
{
return m_Elapsed;
 }

#endif
