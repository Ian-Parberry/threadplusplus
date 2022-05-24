/// \file Common.h
/// \brief Header for the class CCommon.

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

#ifndef __Common_h_
#define __Common_h_

#include "ThreadSafeQueue.h"

/// \brief Common.
///
/// Variables to be shared between the threads and the thread manager,
/// including the request queue, the result queue, and a Boolean value
/// to be set if and when you want all threads to terminate without
/// completing any more tasks.
/// \tparam CTaskClass Task descriptor.

template <class CTaskClass>
class CCommon{
  protected:
    static CThreadSafeQueue<CTaskClass*> m_qRequest; ///< Request queue.
    static CThreadSafeQueue<CTaskClass*> m_qResult; ///< Result queue.

    static bool m_bForceExit; ///< Force exit flag.
}; //CCommon

///////////////////////////////////////////////////////////////////////////////
// Declarations of CCommon variables.

template <class CTaskClass>
CThreadSafeQueue<CTaskClass*> CCommon<CTaskClass>::m_qRequest; ///< Request queue.

template <class CTaskClass>
CThreadSafeQueue<CTaskClass*> CCommon<CTaskClass>::m_qResult; ///< Result queue.

template <class CTaskClass>
bool CCommon<CTaskClass>::m_bForceExit = false; ///< Force exit flag.

#endif //__Common_h_
