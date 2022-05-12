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

/// Call `Start()` in case it isn't called elsewhere.

CTimer::CTimer(){
  Start();
} //constructor

///////////////////////////////////////////////////////////////////////////////
// String functions.

/// Converts the number to an std::string and inserts commas every three 
/// digits from the least-significant end.
/// \param n The number to be converted.
/// \return The parameter n as a comma-separated string.

const std::string CTimer::CommaSeparatedString(size_t n) const{ 
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

const std::string CTimer::to_string(float x, size_t n) const{
  if(fabs(x*pow(10, n)) < 5)return(std::string("0")); 

  //use ostringstream to limit x to n digits after the decimal point

  std::ostringstream out;
  out.precision(n);
  out << std::fixed << x;

  const std::string s = out.str(); //s contains x with precision n 

  //find last non-zero, non-decimal-point digit of s

  size_t i = s.length() - 1; //index
  while(i >= 0 && (s[i] == '0' || s[i] == '.'))
    i--;

  //truncate x to last non-zero, non-decimal-point digit

  return (i < 0)? std::string(): s.substr(0, i + 1);
} //to_string

/// Format seconds into to a string of days, hours, minutes, and seconds,
/// with a fractional number of seconds if required.
/// \param seconds Initial number of seconds.
/// \param n Number of decimal places in the seconds field of the string.
/// \return String of days, hours, minutes, and seconds.

const std::string CTimer::TimeString(float seconds, size_t n) const{
  std::string s; //the result

  const size_t days = (size_t)floor(seconds/86400);
  seconds -= days*86400;

  if(days > 0)
    s += std::to_string(days) + (days > 1? " days": " day");

  const size_t hours = (size_t)floor(seconds/3600);
  seconds -= hours*3600;

  if(hours > 0){
    if(!s.empty())s += ", ";
    s += std::to_string(hours) + " hr";
  } //if

  const size_t minutes = (size_t)floor(seconds/60);
  seconds -= minutes*60;

  if(minutes > 0){
    if(!s.empty())s += ", ";
    s += std::to_string(minutes) + " min";
  } //if
  
  if(!s.empty())s += ", ";
  s += to_string(seconds, n) + " sec";

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

