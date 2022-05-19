/// \file Timer.cpp
/// \brief Code for the class CTimer.

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

#include <sstream>

#include "Timer.h"

#if defined(_MSC_VER) //Windows Visual Studio 
  #include <windows.h>
#else //g++, *nix
  #include <time.h>
#endif

///////////////////////////////////////////////////////////////////////////////
// Helper functions.

/// Convert a number to an `std::string` and insert commas every three 
/// digits from the least-significant end.
/// \param n The number to be converted.
/// \return The number as a comma-separated string.

const std::string CommaSeparatedString(const size_t n){ 
  std::string s = std::to_string(n); //string for comma-separated number
  const int nStart = (int)s.length() - 3; //position of first comma

  for(int i=nStart; i>0; i-=3) //every third character
    s.insert(i, ","); //insert a comma

  return s;
} //CommaSeparatedString

/// Convert float to a fixed-precision string without trailing zeros.
/// Unfortunately, `std::to_string` does not have a way to limit precision,
/// so we'll have to get creative with `std::ostringstream` instead.
/// \param x Floating point number
/// \param n Maximum number of digits after the decimal.
/// \return Fixed-precision string with no trailing zeros.

const std::string to_string(const float x, const size_t n){
  std::ostringstream out;
  out.precision(n);
  out << std::fixed << x;

  std::string s = out.str(); //s contains x with precision n 

  //find last non-zero, non-decimal-point digit of s

  int i = (int)s.length() - 1; //index

  while(i >= 0 && (s[i] == '0' || s[i] == '.'))
    i--;

  //truncate x to last non-zero, non-decimal-point digit

  return s.substr(0, i + 1);
} //to_string

/// \brief Append comma separator.
///
/// Append a comma and a space to a string provided it is non-empty.
/// \param s [in, out] String to append to.

void AppendCommaSeparator(std::string& s){
  if(!s.empty())s += ", ";
} //AppendCommaSeparator

///////////////////////////////////////////////////////////////////////////////
// CTimer functions.

/// Call `Start()` in case it isn't called elsewhere.

CTimer::CTimer(){
  Start();
} //constructor

///////////////////////////////////////////////////////////////////////////////
// String functions.

/// Format seconds into to a string of days, hours, minutes, and fSeconds,
/// with a fractional number of seconds if required.
/// \param fSeconds Initial number of seconds.
/// \param n Number of decimal places in the seconds field of the string.
/// \return String of days, hours, minutes, and seconds.

const std::string CTimer::TimeString(float fSeconds, size_t n) const{
  std::string s; //for the time string, initially empty
  
  constexpr size_t SECONDS_PER_MINUTE = 60;
  constexpr size_t SECONDS_PER_HOUR = 60*SECONDS_PER_MINUTE;
  constexpr size_t SECONDS_PER_DAY = 24*SECONDS_PER_HOUR;

  //number of days

  const size_t nDays = (size_t)floor(fSeconds/SECONDS_PER_DAY);
  fSeconds -= nDays*SECONDS_PER_DAY;

  if(nDays > 0)
    s += std::to_string(nDays) + (nDays > 1? " days": " day");

  //number of hours

  const size_t nHours = (size_t)floor(fSeconds/SECONDS_PER_HOUR);
  fSeconds -= nHours*SECONDS_PER_HOUR;

  if(nHours > 0){
    AppendCommaSeparator(s); 
    s += std::to_string(nHours) + " hr";
  } //if

  //number of minutes

  const size_t nMinutes = (size_t)floor(fSeconds/SECONDS_PER_MINUTE);
  fSeconds -= nMinutes*SECONDS_PER_MINUTE;

  if(nMinutes > 0){
    AppendCommaSeparator(s);
    s += std::to_string(nMinutes) + " min";
  } //if
  
  AppendCommaSeparator(s);

  //number of seconds

  if(nMinutes >= 1){
    const size_t nSeconds = size_t(std::ceil(fSeconds));
    if(nSeconds > 0)
      s += std::to_string(nSeconds) + " sec";
  } //if

  else if(fSeconds >= 1.0f)
    s += to_string(fSeconds, n) + " sec";
  
  //number of milliseconds when less than 1 second

  else if(fSeconds >= 0.001f)
    s += std::to_string(size_t(std::ceilf(fSeconds*1000.0f))) + " ms";

  else s += "< 1 ms"; //less than a millisecond, so whatever

  //s now contains a properly formatted time string

  return s;
} //TimeString

//////////////////////////////////////////////////////////////////////////////
// Time functions.

/// Start timing by saving the current elapsed and CPU times.

void CTimer::Start(){
  m_tpElapsedTimeStart = sysclock::now(); //time_point
  m_fCPUTimeStart = CPUTime()/1000.0f;
} //Start

/// Get time and date string from a systime_point.
/// \param p An instance of systime_point.
/// \return The corresponding time and date string.

const std::string CTimer::GetDateAndTime(const systime_point p) const{
  const time_t t = sysclock::to_time_t(p);
  const std::string s = ctime(&t);
  return s.substr(0, s.length() - 1);
} //GetDateAndTime

/// Get the current date and time as a string. Uses `GetDateAndTime()` to do
/// the heavy lifting.
/// \return Current date and time string.

const std::string CTimer::GetCurrentDateAndTime() const{
  return GetDateAndTime(sysclock::now());
} //GetCurrentDateAndTime

/// Get the amount of time elapsed since the timer was started.
/// \return Elapsed time.

const std::string CTimer::GetElapsedTime() const{
  const std::chrono::duration<float> d = sysclock::now() - m_tpElapsedTimeStart;
  return TimeString(d.count(), 2);
} //GetElapsedTime

/// Get CPU time for this process in seconds. It's annoying that `std::chron`
/// has no cross-platform support for this.
/// \return User CPU time in milliseconds.

const uint64_t CTimer::CPUTime() const{
#if defined(_MSC_VER) //Windows Visual Studio 
  uint64_t llCNS = 0; //for CPU time in centinanoseconds

  HANDLE hProcess = //handle to this process
    OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, GetCurrentProcessId()); 

  if(hProcess != nullptr){ //handle is valid
    FILETIME ftCreation, ftExit, ftKernel; //unused
    FILETIME ftUser; //this one we want 

    if(GetProcessTimes(hProcess, &ftCreation, &ftExit, &ftKernel, &ftUser))
      llCNS = *((uint64_t*)&ftUser);  //get time values
    
    CloseHandle(hProcess); //close the process handle
  } //if

  return uint64_t(llCNS)/10000; 
#else //g++, *nix
  return uint64_t(1000.0*(double)(clock())/CLOCKS_PER_SEC);
#endif
} //CPUTime

/// Get the amount of CPU time used since the timer was started.
/// \return CPU time in seconds.

const std::string CTimer::GetCPUTime() const{
  return TimeString(CPUTime()/1000.0f - m_fCPUTimeStart, 2);
} //GetCPUTime

