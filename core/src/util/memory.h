/***********************************************************************
*************Copyright (c) 2009-2010 Satoshi Nakamoto*******************
*******Copyright (c) 2009-2018 The Bitcoin Core developers**************
******************Copyright (c) 2010-2022 Nur1Labs**********************
>Distributed under the MIT software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/


#ifndef MUBDI_UTIL_MEMORY_H
#define MUBDI_UTIL_MEMORY_H

#include <memory>
#include <utility>

//! Substitute for C++14 std::make_unique.
template <typename T, typename... Args>
std::unique_ptr<T> MakeUnique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif
