/// \file Common.h
/// \brief Header for the class CCommon.

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

#ifndef __Common_h_
#define __Common_h_

#include "ThreadSafeQueue.h"
#include "Task.h"

/// \brief Thread idle mode.
///
/// Indicates the activity to be performed when a thread is idle.

enum class eThreadIdleMode{
  Sleep, Exit
}; //eThreadIdleMode

/// \brief Common.
///
/// Shared variables.

class CCommon{
  protected:
    static CThreadSafeQueue<CTask*> m_qRequest; ///< Request queue.
    static CThreadSafeQueue<CTask*> m_qResult; ///< Result queue.

    static eThreadIdleMode m_eThreadIdleMode; ///< Thread idle mode.
    static bool m_bForceExit; ///< Force exit flag.
    static bool m_bVerbose; ///< Verbosity flag.
}; //CCommon

#endif //__Common_h_
