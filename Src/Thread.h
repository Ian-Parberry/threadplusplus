/// \file Thread.h
/// \brief Header and code for the class CThread.

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

#include "Common.h"

///////////////////////////////////////////////////////////////////////////////
// CThread definition.

/// \brief The thread class.
///
/// Values and functionality for the threads.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
class CThread: public CCommon<CTaskClass>{
  protected:
    size_t m_nThreadId = 0; ///< Thread identifier.
    
  public:
    CThread(size_t); ///< Constructor.
    
    void operator()(); ///< The code that gets run by each thread.
}; //CThread

///////////////////////////////////////////////////////////////////////////////
// CThread code.

/// Constructor.
/// \param n Thread identifier.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
CThread<CTaskClass>::CThread(size_t n):
  m_nThreadId(n){ //thread identifier
} //constructor

/// The function executed by a thread, which repeatedly pops a task from the
/// thread-safe request queue, calls its Perform() function, then places it
/// on the result queue. It exits when the request queue is empty or when an
/// exit is forced by CCommon<CTaskClass>::m_bForceExit being set to true.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
void CThread<CTaskClass>::operator()(){
  bool bActive = true; //true to stay active, false to exit thread

  while(bActive){ //perform task loop
    CTaskClass* pTask = nullptr; //current task descriptor
   
    if(CCommon<CTaskClass>::m_bForceExit) //forced exit
      bActive = false; //trigger exit from loop

    else if(CCommon<CTaskClass>::m_qRequest.Delete(pTask) && pTask){ //next task
      pTask->SetThreadId(m_nThreadId); //set task's thread identifier
      pTask->Perform(); //perform the task
      CCommon<CTaskClass>::m_qResult.Insert(pTask); //performed task result
    } //if

    else bActive = false; //request queue empty, so trigger exit from loop
  } //while
} //operator()()

#endif //__BaseThread_h__
