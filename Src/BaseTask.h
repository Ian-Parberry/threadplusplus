/// \file BaseTask.h
/// \brief Header for the class CBaseTask.

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

#ifndef __BaseTask_h_
#define __BaseTask_h_

#include <limits>

constexpr size_t max_size_t = std::numeric_limits<size_t>::max(); ///< Max size_t.

/// \brief Base task descriptor.
///
/// The base task descriptor describes a base task, including a task
/// identifier, a thread identifier which is to be set by the thread that
/// performs this task, and a function to perform it. You should derive your
/// task desciptor from this class. Your task descriptor should implement a
/// constructor for any task-related initialization and it should override
/// function Perform() with the code to perform your task.

class CBaseTask{
  private:
    static size_t m_nNumTasks; ///< Number of tasks extant.

  protected:
    size_t m_nTaskId = 0; ///< Task identifier.
    size_t m_nThreadId = max_size_t; ///< Identifier of thread that performed task.

  public:
    CBaseTask(); ///< Default constructor.

    virtual void Perform(); ///< Perform this task.

    const size_t GetTaskId() const; ///< Get task identifier.

    void SetThreadId(const size_t); ///< Set thread identifier.
    const size_t GetThreadId() const; ///< Get thread identifier.
}; //CBaseTask

#endif //__BaseTask_h_
