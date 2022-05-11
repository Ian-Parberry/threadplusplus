/// \file Thread.h
/// \brief Header for the class CThread.

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

#ifndef __Thread_h__
#define __Thread_h__

#include <limits>
#include <stdio.h>

#include "Common.h"

/// \brief The thread class.
///
/// Values and functionality that need to be shared in common by the threads.
/// The member variables of this class are declared `static` so that they
/// can be shared by classes derived from it. This is sometimes called the
/// _Borg Idiom_ in the Python community.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
class CThread: public CCommon<CTaskClass>{
  private:
    static size_t m_nNumThreads; ///< Number of threads extant.

  protected:
    size_t m_nThreadId = 0; ///< Thread identifier.
    
  public:
    CThread(); ///< Default constructor.
    
    void operator()(); ///< The code that gets run by each thread.
}; //CThread

///////////////////////////////////////////////////////////////////////////////
// Declarations of CThread variables.

template <class CTaskClass>
size_t CThread<CTaskClass>::m_nNumThreads = 0; ///< Number of threads.

///////////////////////////////////////////////////////////////////////////////
// CThread code.

/// Constructor.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
CThread<CTaskClass>::CThread():
  m_nThreadId(m_nNumThreads++){ //thread identifier
} //constructor

/// The function executed by a thread, which repeatedly pops a task
/// from the thread-safe request queue, calls its `Process()` function,
/// then places it on the result queue.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
void CThread<CTaskClass>::operator()(){
  if(CCommon<CTaskClass>::m_bVerbose)
    printf("Thread %zd starting up\n", m_nThreadId);

  CTaskClass* pTask = nullptr; //current task descriptor
  bool bActive = true; //true to stay active, false to exit thread

  while(bActive){ 
    if(CCommon<CTaskClass>::m_bForceExit) //forced exit
      bActive = false; //set inactive to exit loop

    else{ //keep processing tasks
      if(CCommon<CTaskClass>::m_qRequest.Delete(pTask)){ //grab task from request queue
        if(pTask){ //safety
          if(CCommon<CTaskClass>::m_bVerbose)
            printf("Thread %zd starting task %zd\n", m_nThreadId,
              pTask->GetTaskId());

          pTask->Process(); //process the task
          
          if(CCommon<CTaskClass>::m_bVerbose)printf("Thread %zd finished task %zd\n", 
            m_nThreadId, pTask->GetTaskId());

          pTask->SetThreadId(m_nThreadId); //set task's thread identifier
          CCommon<CTaskClass>::m_qResult.Insert(pTask); //place processed task on result queue
        } //if
      } //if

      else bActive = false; //trigger exit
    } //else
  } //while
  
  if(CCommon<CTaskClass>::m_bVerbose)
    printf("Thread %zd shutting down\n", m_nThreadId);
} //operator()()


#endif //__BaseThread_h__
