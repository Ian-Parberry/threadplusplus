/// \file ThreadManager.cpp
/// \brief Code for the class CThreadManager.

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

#include <functional>

#include "ThreadManager.h"

/// Default constructor.

CThreadManager::CThreadManager(){
  CreateTasks();
  Spawn();
  Wait();
  Report();
} //constructor

/// The destructor. Delete any remaining tasks in the request and result
/// queues. Both queues should be empty at this point, but this is for safety.

CThreadManager::~CThreadManager(){
  CTask* pTask = nullptr; //task pointer
  
  ///delete any remaining tasks in the request queue

  while(m_qRequest.Delete(pTask)) 
    delete pTask; 
  
  ///delete any remaining tasks in the result queue

  while(m_qResult.Delete(pTask)) 
    delete pTask; 
} //destructor

/// Create a bunch of tasks.

void CThreadManager::CreateTasks(){
  for(size_t i=0; i<100; i++)
    m_qRequest.Insert(new CTask);
} //CreateTasks 

/// Spawn one less than the maximum number of concurrent threads provided by
/// the hardware (leaving one for the main thread).
/// \param m Thread idle mode.

void CThreadManager::Spawn(){ 
  for(size_t i=0; i<std::thread::hardware_concurrency() - 1; i++)
    m_vThreads.push_back(std::thread((CThread())));
} //Spawn 

/// Force all threads to terminate and wait until they do.

void CThreadManager::ForceExit(){ 
  m_bForceExit = true;
  Wait();
} //ForceExit

/// Wait for all threads to terminate (that is, execute a join) then return.

void CThreadManager::Wait(){
  for_each(m_vThreads.begin(), m_vThreads.end(), std::mem_fn(&std::thread::join));
} //Wait

/// Report a result.
/// \param pTask Pointer to a task.

void CThreadManager::Report(CTask* pTask){
  if(pTask)printf("Task %zd was processed by thread %zd\n",
    pTask->GetTaskId(), pTask->GetThreadId());
} //Report

/// Report all completed tasks from the result queue and delete the tasks.

void CThreadManager::Report(){ 
  CTask* pTask = nullptr; //task pointer

  while(m_qResult.Delete(pTask)){ //for each task in the result queue
    Report(pTask); //report it
    delete pTask; //delete the task
  } //while
} //Report