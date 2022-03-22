/// \file Thread.cpp
/// \brief Code for the class CThread.

// MIT License
//
// Copyright (c) 2022 Ian Parberry
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <stdio.h>

#include "Thread.h"

size_t CThread::m_nNumThreads = 0; ///< Number of threads.

/// Constructor.

CThread::CThread():
  m_nThreadId(m_nNumThreads++){ //thread identifier
} //constructor

/// The function executed by a thread, which repeatedly pops a task
/// from the thread-safe request queue, calls its `Process()` function,
/// then places it on the result queue.

void CThread::operator()(){
  if(m_bVerbose)printf("Thread %zd starting up\n", m_nThreadId);

  CTask* pTask = nullptr; //current task descriptor
  bool bActive = true; //true to stay active, false to exit thread

  while(bActive){ 
    if(m_bForceExit) //forced exit
      bActive = false; //set inactive to exit loop

    else{ //keep processing tasks
      if(m_qRequest.Delete(pTask)){ //grab a task from the request queue
        if(pTask){ //safety
          if(m_bVerbose)printf("Thread %zd starting task %zd\n", 
            m_nThreadId, pTask->GetTaskId());

          pTask->Process(); //process the task
          
          if(m_bVerbose)printf("Thread %zd finished task %zd\n", 
            m_nThreadId, pTask->GetTaskId());

          pTask->SetThreadId(m_nThreadId); //set task's thread identifier
          m_qResult.Insert(pTask); //place processed task on result queue
        } //if
      } //if

      else switch(m_eThreadIdleMode){ //no tasks, so idle
        case eThreadIdleMode::Sleep: Sleep(10); break; //avoid busy-wait       
        case eThreadIdleMode::Exit:  bActive = false; break; //trigger exit
      } //switch
    } //else
  } //while
  
  if(m_bVerbose)printf("Thread %zd shutting down\n", m_nThreadId);
} //operator()()
