/// \file BaseThreadManager.h
/// \brief Header and code for the class CBaseThreadManager.

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

#ifndef __BaseThreadManager_h__
#define __BaseThreadManager_h__

#include <thread>
#include <functional>

#include "ThreadSafeQueue.h"
#include "Thread.h"
#include "BaseTask.h"

///////////////////////////////////////////////////////////////////////////////
// CBaseThreadManager definition.

/// \brief The base thread manager.
///
/// The thread manager takes care of the health and feeding of the threads.
/// You should derive your thread manager from this class with the template
/// CTaskClass being your task descriptor class derived from CBaseTask.
/// Your thread manager should implement a constructor for any task-related
/// initialization and it should override function ProcessTask() with the
/// processing required for your task.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
class CBaseThreadManager: public CCommon<CTaskClass>{
  protected:
    std::vector<std::thread> m_vThread; ///< Thread list.
    size_t m_nNumThreads = 0; ///< Number of threads in use.
    
    virtual void ProcessTask(CTaskClass*); ///< Process the result of a task.

  public:
    CBaseThreadManager(); ///< Constructor.
    virtual ~CBaseThreadManager(); ///< Destructor.

    void Insert(CTaskClass*); ///< Insert a task.

    void Spawn(); ///< Spawn threads.
    void Wait(); ///< Wait for threads to finish all tasks.
    void ForceExit(); ///< Force all threads to terminate.
    void Process(); ///< Process results of all tasks.

    const size_t GetNumThreads() const; ///< Get number of threads.
}; //CBaseThreadManager

///////////////////////////////////////////////////////////////////////////////
// CBaseThreadManager code.

/// Default constructor.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
CBaseThreadManager<CTaskClass>::CBaseThreadManager(){
  m_nNumThreads = std::thread::hardware_concurrency() - 1;
} //constructor

/// The destructor deletes any remaining tasks in the request and result
/// queues. Both queues should be empty at this point, but this is for safety.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
CBaseThreadManager<CTaskClass>::~CBaseThreadManager(){
  CTaskClass* pTask = nullptr; //task pointer
  
  //delete any remaining tasks in the request queue

  while(CCommon<CTaskClass>::m_qRequest.Delete(pTask)) 
    delete pTask; 
  
  //delete any remaining tasks in the result queue

  while(CCommon<CTaskClass>::m_qResult.Delete(pTask)) 
    delete pTask; 
} //destructor

/// Insert a task descriptor into the request queue.
/// \tparam CTaskClass Task descriptor.
/// \param p Pointer to a task.

template <class CTaskClass>
void CBaseThreadManager<CTaskClass>::Insert(CTaskClass* p){
  CCommon<CTaskClass>::m_qRequest.Insert(p);
} //Insert

/// Spawn one less than the maximum number of concurrent threads provided by
/// the hardware (leaving one for the main thread).
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
void CBaseThreadManager<CTaskClass>::Spawn(){ 
  for(size_t i=0; i<m_nNumThreads; i++)
    m_vThread.push_back(std::thread((CThread<CTaskClass>(i))));
} //Spawn 

/// Force all threads to terminate and wait until they do.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
void CBaseThreadManager<CTaskClass>::ForceExit(){ 
  CCommon<CTaskClass>::m_bForceExit = true;
  Wait();
} //ForceExit

/// Wait for all threads to terminate (that is, execute a join) then return.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
void CBaseThreadManager<CTaskClass>::Wait(){
  for_each(m_vThread.begin(), m_vThread.end(), std::mem_fn(&std::thread::join));
} //Wait

/// Process the results of a task. This function is a stub which you should
/// override in your derived thread manager class.
/// \tparam CTaskClass Task descriptor.
/// \param pTask Pointer to a task descriptor.

template <class CTaskClass>
void CBaseThreadManager<CTaskClass>::ProcessTask(CTaskClass* pTask){
  //stub
} //ProcessTask

/// Process and delete all completed task descriptors from the result queue.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
void CBaseThreadManager<CTaskClass>::Process(){ 
  CTaskClass* pTask = nullptr; //task pointer

  while(CCommon<CTaskClass>::m_qResult.Delete(pTask)){ //for each task descriptor
    ProcessTask(pTask); //process it
    delete pTask; //delete the task descriptor
  } //while
} //Process

/// Reader function for the number of threads used by this application.
/// Assumes that `m_nNumThreads` contains this value.
/// \return Number of threads used.

template <class CTaskClass>
const size_t CBaseThreadManager<CTaskClass>::GetNumThreads() const{
  return m_nNumThreads;
} //GetNumThreads

#endif //__BaseThreadManager_h__
