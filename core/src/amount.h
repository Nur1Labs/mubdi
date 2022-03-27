/***********************************************************************
**************Copyright (c) 2009-2010 Satoshi Nakamoto******************
***********Copyright (c) 2009-2014 The Bitcoin developers***************
*************Copyright (c) 2017-2019 The PIVX developers****************
******************Copyright (c) 2010-2022 Nur1Labs**********************
>Distributed under the MIT software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#ifndef MUBDI_AMOUNT_H
#define MUBDI_AMOUNT_H

#include "serialize.h"

#include <stdlib.h>
#include <string>

/** Amount in MuBdI (Can be negative) */
typedef int64_t CAmount;

static const CAmount COIN = 100000000;
static const CAmount CENT = 1000000;

/**
 * Fee rate in MuBdI per kilobyte: CAmount / kB
 */
class CFeeRate
{
private:
    CAmount nSatoshisPerK; // unit is satoshis-per-1,000-bytes
public:
    CFeeRate() : nSatoshisPerK(0) {}
    explicit CFeeRate(const CAmount& _nSatoshisPerK) : nSatoshisPerK(_nSatoshisPerK) {}
    CFeeRate(const CAmount& nFeePaid, size_t nSize);
    CFeeRate(const CFeeRate& other) { nSatoshisPerK = other.nSatoshisPerK; }

    CAmount GetFee(size_t size) const;                  // unit returned is satoshis
    CAmount GetFeePerK() const { return GetFee(1000); } // satoshis-per-1000-bytes

    friend bool operator<(const CFeeRate& a, const CFeeRate& b) { return a.nSatoshisPerK < b.nSatoshisPerK; }
    friend bool operator>(const CFeeRate& a, const CFeeRate& b) { return a.nSatoshisPerK > b.nSatoshisPerK; }
    friend bool operator==(const CFeeRate& a, const CFeeRate& b) { return a.nSatoshisPerK == b.nSatoshisPerK; }
    friend bool operator<=(const CFeeRate& a, const CFeeRate& b) { return a.nSatoshisPerK <= b.nSatoshisPerK; }
    friend bool operator>=(const CFeeRate& a, const CFeeRate& b) { return a.nSatoshisPerK >= b.nSatoshisPerK; }
    std::string ToString() const;

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(nSatoshisPerK);
    }
};

#endif /*  MUBDI_AMOUNT_H */