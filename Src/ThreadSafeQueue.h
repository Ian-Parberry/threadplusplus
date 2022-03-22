/// \file ThreadSafeQueue.h
/// \brief Interface for the event queue class CThreadSafeQueue.

#ifndef __ThreadSafeQueue_h__
#define __ThreadSafeQueue_h__

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

#include <Windows.h>
#include <queue>

/// \brief Thread safe queue.
///
/// A thread-safe data structure for communicating between the
/// worker threads and the main thread. It uses critical
/// sections for safety.

template <class data> class CThreadSafeQueue{ 
  private:
    CRITICAL_SECTION m_cCriticalSection; ///< Critical section for thread safety.
    std::queue<data> m_stdQueue; ///< The queue.

  public:
    CThreadSafeQueue(); ///< Constructor.
    ~CThreadSafeQueue(); ///< Destructor.

    void Insert(const data& element); ///< Insert event at tail.
    bool Delete(data& element); ///< Delete event from head.
    void Flush(); ///< Flush out and discard all items in queue.
    size_t GetSize(); ///< Get queue size.
}; //CThreadSafeQueue

/// The constructor initializes the critical section.
/// \tparam data Type definition for queue members.

template <class data> CThreadSafeQueue<data>::CThreadSafeQueue(){
  InitializeCriticalSection(&m_cCriticalSection); 
} //constructor

/// The destructor deletes the critical section.
/// \tparam data Type definition for queue members.

template <class data> CThreadSafeQueue<data>::~CThreadSafeQueue(){
  DeleteCriticalSection(&m_cCriticalSection);
} //destructor

/// Insert an element into the queue.
/// A critical section is used to ensure a thread-safe insertion.
/// \tparam data Type definition for queue members.
/// \param element The element to be inserted into the queue.

template <class data> void CThreadSafeQueue<data>::Insert(const data& element){
  EnterCriticalSection(&m_cCriticalSection); 
  m_stdQueue.push(element); 
  LeaveCriticalSection(&m_cCriticalSection); 
} //Insert

/// Delete and return an element from the queue.
/// A critical section is used to ensure a thread-safe deletion.
/// \tparam data Type definition for queue members.
/// \param element [OUT] The element deleted from the queue.
/// \return true if the delete was successful, ie. the queue was not empty.

template <class data> bool CThreadSafeQueue<data>::Delete(data& element){
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
/// \tparam data Type definition for queue members.

template <class data> void CThreadSafeQueue<data>::Flush(){
  EnterCriticalSection(&m_cCriticalSection); 

  while(!m_stdQueue.empty()) //queue has something in it
    m_stdQueue.pop(); //delete from front of queue

  LeaveCriticalSection(&m_cCriticalSection);
} //Flush

/// Reader function for the queue size.
/// \tparam data Type definition for queue members.
/// \return The queue size.

template <class data> size_t CThreadSafeQueue<data>::GetSize(){
  return m_stdQueue.size();
} //GetSize

#endif //__ThreadSafeQueue_h__