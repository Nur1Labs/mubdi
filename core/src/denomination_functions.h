/***********************************************************************
*************Copyright (c) 2017-2018 The PIVX developers****************
******************Copyright (c) 2010-2022 Nur1Labs**********************
>Distributed under the MIT software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#include "reverse_iterate.h"
#include "util.h"
#include "libairlogic/Denominations.h"
#include "zmub/zerocoin.h"
#include <list>
#include <map>
std::vector<CZerocoinMint> SelectMintsFromList(const CAmount nValueTarget, CAmount& nSelectedValue,
                                               int nMaxNumberOfSpends,
                                               int& nCoinsReturned,
                                               const std::list<CZerocoinMint>& listMints,
                                               const std::map<libzerocoin::CoinDenomination, CAmount> mapDenomsHeld,
                                               int& nNeededSpends
                                               );

int calculateChange(
    int nMaxNumberOfSpends,
    const CAmount nValueTarget,
    const std::map<libzerocoin::CoinDenomination, CAmount>& mapOfDenomsHeld,
    std::map<libzerocoin::CoinDenomination, CAmount>& mapOfDenomsUsed);

void listSpends(const std::vector<CZerocoinMint>& vSelectedMints);
