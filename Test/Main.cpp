/// \file Main.cpp
/// \brief Code for the function main().

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

#include "ThreadManager.h"
#include "Task.h"
#include "Timer.h"

/// \brief Main.
///
/// Main.
/// \return 0 (What could possibly go wrong?)

int main(){
  CThreadManager* pThreadManager = new CThreadManager; //thread manager
  CTimer* pTimer = new CTimer; //timer for elapsed and CPU time

  for(size_t i=0; i<100; i++) //create 100 tasks
    pThreadManager->Insert(new CTask);

  pTimer->Start(); //start timing CPU and elapsed time
  printf("Starting at %s.\n", pTimer->GetCurrentDateAndTime().c_str());

  pThreadManager->Spawn(); //spawn threads
  pThreadManager->Wait(); //wait for threads to finish
  
  printf("Finishing at %s.\n", pTimer->GetCurrentDateAndTime().c_str());
  printf("Elapsed time %s.\n", pTimer->GetElapsedTime().c_str());
  printf("CPU time %s.\n", pTimer->GetCPUTime().c_str());

  printf("Processing results computed by threads.\n");
  pThreadManager->Process(); //process results

  //clean up and exit
  
  delete pTimer;
  delete pThreadManager;

  return 0;
} //main
