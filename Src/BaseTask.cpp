/// \file BaseTask.cpp
/// \brief Code for the class CBaseTask.

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

#include "BaseTask.h"

size_t CBaseTask::m_nNumTasks = 0; ///< Number of tasks

/// The default constructor.

CBaseTask::CBaseTask(){
  m_nTaskId = m_nNumTasks++; //set task identifier to next task
} //constructor

/// Perform the task. This function is a stub that is to be overridden in
/// derived classes.

void CBaseTask::Perform(){ 
  //this is a stub
} //Perform

/// Reader function for the task identifier.
/// \return The task identifier.

const size_t CBaseTask::GetTaskId() const{
  return m_nTaskId;
} //GetTaskId

/// Set the thread identifier. This is to be called by the processing thread.
/// \param id Thread identifier for of the processing thread.

void CBaseTask::SetThreadId(const size_t id){
  m_nThreadId = id;
} //SetThreadId

/// Reader function for the thread identifier. If this is not correctly set by
/// the processing thread, then this will be a very large number.
/// \return The thread identifier.

const size_t CBaseTask::GetThreadId() const{
  return m_nThreadId;
} //GetThreadId
