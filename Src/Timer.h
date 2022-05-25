/// \file Timer.h
/// \brief Header for the class CTimer.

// MIT License
//
// Copyright (c) 2022 Ian Parberry
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef __Timer_h__
#define __Timer_h__

#include <ctime>
#include <chrono>
#include <string>
#include <cinttypes>

typedef std::chrono::system_clock sysclock; ///< Shorthand for system clock.

typedef std::chrono::time_point
  <std::chrono::system_clock> systime_point; ///< Shorthand for sysclock time point.

const std::string CommaSeparatedString(const size_t); ///< Comma separated string.

/// \brief Timer for elapsed time and CPU time.
///
/// The timer is charged with everything that is time-related, in particular,
/// elapsed time, CPU time, and clock time. The member functions return time
/// intervals measured from the time `Start()` is called to the current time.
/// The CPU time reported is the total summed over all threads. Uses
/// `std::chrono` to do the heavy lifting. 

class CTimer{
  private:
    systime_point m_tpElapsedTimeStart; ///< Clock time.
    float m_fCPUTimeStart; ///< CPU time in seconds.

    const uint64_t CPUTime() const; ///< Compute CPU time.
    const std::string TimeString(float seconds, size_t n) const; ///< Time as a string.

    const std::string GetDateAndTime(const systime_point) const; ///< Get date and time.

  public:
    CTimer(); ///< Constructor.
    
    const std::string GetTimeAndDate() const; ///< Get current time and date.

    void Start(); ///< Start timing.
    
    const std::string GetElapsedTime() const; ///< Get elapsed time in seconds.
    const std::string GetCPUTime() const; ///< Get CPU time in seconds.
}; //CTimer

#endif //__Timer_h__
