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

#include <queue>
#include <mutex>

///////////////////////////////////////////////////////////////////////////////
// CThreadSafeQueue definition.

/// \brief Thread safe queue.
///
/// A thread-safe queue of task descriptors for communicating between the
/// threads and the thread manager. It uses an `std::mutex` for safety.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
class CThreadSafeQueue{ 
  private:
    std::mutex m_stdMutex; ///< Mutex for thread safety.
    std::queue<CTaskClass> m_stdQueue; ///< The task descriptor queue.

  public:
    CThreadSafeQueue(); ///< Constructor.
    ~CThreadSafeQueue(); ///< Destructor.

    void Insert(const CTaskClass& element); ///< Insert task at tail.
    bool Delete(CTaskClass& element); ///< Delete task from head.
    void Flush(); ///< Flush out and discard all tasks in queue.
}; //CThreadSafeQueue

///////////////////////////////////////////////////////////////////////////////
// CThreadSafeQueue code.

/// Default constructor.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
CThreadSafeQueue<CTaskClass>::CThreadSafeQueue(){
} //constructor

/// Default destructor.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
CThreadSafeQueue<CTaskClass>::~CThreadSafeQueue(){
} //destructor

/// Insert a task descriptor into the queue. A mutex is used to ensure
/// thread safety.
/// \tparam CTaskClass Task descriptor.
/// \param element The element to be inserted into the queue.

template <class CTaskClass>
void CThreadSafeQueue<CTaskClass>::Insert(const CTaskClass& element){
  m_stdMutex.lock(); 
  m_stdQueue.push(element); 
  m_stdMutex.unlock();
} //Insert

/// Delete and return a task descriptor from the queue. A mutex is used to
/// ensure thread safety.
/// \tparam CTaskClass Task descriptor.
/// \param element [OUT] The element deleted from the queue.
/// \return true if the delete was successful, ie. the queue was not empty.

template <class CTaskClass>
bool CThreadSafeQueue<CTaskClass>::Delete(CTaskClass& element){
  bool success = false; //true if there was something to delete
  
  m_stdMutex.lock();  

  if(!m_stdQueue.empty()){ //queue has something in it
    element = m_stdQueue.front(); //get element from front of queue
    m_stdQueue.pop(); //delete from front of queue
    success = true; //success
  } //if
  
  m_stdMutex.unlock();

  return success;
} //Delete

/// Flush all task descriptors out of the queue without processing them.
/// A mutex is used to ensure thread safety.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
void CThreadSafeQueue<CTaskClass>::Flush(){
  m_stdMutex.lock();

  while(!m_stdQueue.empty()) //queue has something in it
    m_stdQueue.pop(); //delete from front of queue
  
  m_stdMutex.unlock();
} //Flush

#endif //__ThreadSafeQueue_h__