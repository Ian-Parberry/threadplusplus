/// \file ThreadSafeQueue.h
/// \brief Interface for the thread-safe queue class CThreadSafeQueue.

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

#ifndef __ThreadSafeQueue_h__
#define __ThreadSafeQueue_h__

#include <Windows.h>
#include <queue>

/// \brief Thread safe queue.
///
/// A thread-safe CTaskClass structure for communicating between the worker
/// threads and the main thread. It uses critical sections for safety.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
class CThreadSafeQueue{ 
  private:
    CRITICAL_SECTION m_cCriticalSection; ///< Critical section for thread safety.
    std::queue<CTaskClass> m_stdQueue; ///< The task queue.

  public:
    CThreadSafeQueue(); ///< Constructor.
    ~CThreadSafeQueue(); ///< Destructor.

    void Insert(const CTaskClass& element); ///< Insert task at tail.
    bool Delete(CTaskClass& element); ///< Delete task from head.
    void Flush(); ///< Flush out and discard all tasks in queue.

    const size_t GetSize() const; ///< Get number of tasks.
}; //CThreadSafeQueue

/// The constructor initializes the critical section.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
CThreadSafeQueue<CTaskClass>::CThreadSafeQueue(){
  InitializeCriticalSection(&m_cCriticalSection); 
} //constructor

/// The destructor deletes the critical section.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
CThreadSafeQueue<CTaskClass>::~CThreadSafeQueue(){
  DeleteCriticalSection(&m_cCriticalSection);
} //destructor

/// Insert an element into the queue.
/// A critical section is used to ensure a thread-safe insertion.
/// \tparam CTaskClass Task descriptor.
/// \param element The element to be inserted into the queue.

template <class CTaskClass>
void CThreadSafeQueue<CTaskClass>::Insert(const CTaskClass& element){
  EnterCriticalSection(&m_cCriticalSection); 
  m_stdQueue.push(element); 
  LeaveCriticalSection(&m_cCriticalSection); 
} //Insert

/// Delete and return an element from the queue.
/// A critical section is used to ensure a thread-safe deletion.
/// \tparam CTaskClass Task descriptor.
/// \param element [OUT] The element deleted from the queue.
/// \return true if the delete was successful, ie. the queue was not empty.

template <class CTaskClass>
bool CThreadSafeQueue<CTaskClass>::Delete(CTaskClass& element){
  bool success = false; //true if there was something to delete

  EnterCriticalSection(&m_cCriticalSection); 

  if(!m_stdQueue.empty()){ //queue has something in it
    element = m_stdQueue.front(); //get element from front of queue
    m_stdQueue.pop(); //delete from front of queue
    success = true; //success
  } //if

  LeaveCriticalSection(&m_cCriticalSection);

  return success;
} //Delete

/// Flush all entries out of the queue without processing them.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
void CThreadSafeQueue<CTaskClass>::Flush(){
  EnterCriticalSection(&m_cCriticalSection); 

  while(!m_stdQueue.empty()) //queue has something in it
    m_stdQueue.pop(); //delete from front of queue

  LeaveCriticalSection(&m_cCriticalSection);
} //Flush

/// Reader function for the queue size.
/// \tparam CTaskClass Task descriptor.
/// \return The queue size.

template <class CTaskClass>
const size_t CThreadSafeQueue<CTaskClass>::GetSize() const{
  return m_stdQueue.size();
} //GetSize

#endif //__ThreadSafeQueue_h__