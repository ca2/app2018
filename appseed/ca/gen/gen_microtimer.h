//////////////////////////////////////////////////////////////////////////////
// Timer.h
// =======
// High Resolution Timer.
// This timer is able to measure the elapsed time with 1 micro-second accuracy
// in both Windows, Linux and Unix system
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2003-01-13
// UPDATED: 2006-01-13
//
// Copyright (c) 2003 Song Ho Ahn
//////////////////////////////////////////////////////////////////////////////
#pragma once


#ifndef WINDOWS
#include <sys/time.h> // Unix based system specific
#endif


namespace gen
{


   class microtimer
   {
   public:


       double startTimeInMicroSec;                 // starting time in micro-second
       double endTimeInMicroSec;                   // ending time in micro-second
       int    stopped;                             // stop flag

   #ifdef WINDOWS
       LARGE_INTEGER frequency;                    // ticks per second
       LARGE_INTEGER startCount;                   //
       LARGE_INTEGER endCount;                     //
   #else
       timeval startCount;                         //
       timeval endCount;                           //
   #endif


       microtimer();                                    // default constructor
       ~microtimer();                                   // default destructor

       void   start();                             // start timer
       void   stop();                              // stop the timer
       double getElapsedTime();                    // get elapsed time in second
       double getElapsedTimeInSec();               // get elapsed time in second (same as getElapsedTime)
       double getElapsedTimeInMilliSec();          // get elapsed time in milli-second
       double getElapsedTimeInMicroSec();          // get elapsed time in micro-second

       double getTime();                           // get time in second
       double getTimeInSec();                      // get time in second (same as getElapsedTime)
       double getTimeInMilliSec();                 // get time in milli-second
       double getTimeInMicroSec();                 // get time in micro-second

   };



} // namespace gen




