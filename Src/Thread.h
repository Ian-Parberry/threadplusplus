/// \file Thread.h
/// \brief Header for the class CThread.

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

#ifndef __Thread_h__
#define __Thread_h__

#include <limits>

#include "Common.h"

/// \brief The thread class.
///
/// Values and functionality that need to be shared in common by the threads.
/// The member variables of this class are declared `static` so that they
/// can be shared by classes derived from it. This is sometimes called the
/// _Borg Idiom_ in the Python community.

class CThread: public CCommon{
  private:
    static size_t m_nNumThreads; ///< Number of threads extant.

  protected:
    size_t m_nThreadId = 0; ///< Thread identifier.
    
  public:
    CThread(); ///< Default constructor.

    void operator()(); ///< The code that gets run by each thread.
}; //CThread

#endif //__Thread_h__
