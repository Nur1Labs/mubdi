/***********************************************************************
**************Copyright (c) 2009-2010 Satoshi Nakamoto******************
********Copyright (c) 2009-2016 The Bitcoin Core developers*************
******************Copyright (c) 2010-2022 Nur1Labs**********************
>Distributed under the MIT software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#ifndef MUBDI_CONSENSUS_PARAMS_H
#define MUBDI_CONSENSUS_PARAMS_H

#include "amount.h"
#include "libairlogic/Params.h"
#include "uint256.h"
#include <map>
#include <string>

namespace Consensus {

/**
 * Parameters that influence chain consensus.
 */
struct Params {
    uint256 hashGenesisBlock;
    bool fPowAllowMinDifficultyBlocks;
    uint256 powLimit;
    uint256 posLimitV1;
    uint256 posLimitV2;
    int nBudgetCycleBlocks;
    int nBudgetFeeConfirmations;
    int nCoinbaseMaturity;
    int nFutureTimeDriftPoW;
    int nFutureTimeDriftPoS;
    int nMasternodeCountDrift;
    CAmount nMaxMoneyOut;
    int nPoolMaxTransactions;
    int64_t nProposalEstablishmentTime;
    int nStakeMinAge;
    int nStakeMinDepth;
    int64_t nTargetTimespan;
    int64_t nTargetTimespanV2;
    int64_t nTargetSpacing;
    int nTimeSlotLength;
    std::string strObfuscationPoolDummyAddress;

    // spork keys
    std::string strSporkPubKey;
    std::string strSporkPubKeyOld;
    int64_t nTime_EnforceNewSporkKey;
    int64_t nTime_RejectOldSporkKey;

    // majorities for block version upgrades
    int nEnforceBlockUpgradeMajority;
    int nRejectBlockOutdatedMajority;
    int nToCheckBlockUpgradeMajority;

    // height-based activations
    int height_last_PoW;
    int height_start_PoS;
    int height_start_BIP65;                         // Blocks v5 start
    int height_start_InvalidUTXOsCheck;
    int height_start_MessSignaturesV2;
    int height_start_StakeModifierNewSelection;
    int height_start_StakeModifierV2;               // Blocks v6 start
    int height_start_TimeProtoV2;                   // Blocks v7 start
    
    // validation by-pass
    int64_t nMuBdIBadBlockTime;
    unsigned int nMuBdIBadBlockBits;


    int64_t TargetTimespan(const bool fV2 = true) const { return fV2 ? nTargetTimespanV2 : nTargetTimespan; }
    uint256 ProofOfStakeLimit(const bool fV2) const { return fV2 ? posLimitV2 : posLimitV1; }
    bool MoneyRange(const CAmount& nValue) const { return (nValue >= 0 && nValue <= nMaxMoneyOut); }
    bool IsMessSigV2(const int nHeight) const { return nHeight >= height_start_MessSignaturesV2; }
    bool IsTimeProtocolV2(const int nHeight) const { return nHeight >= height_start_TimeProtoV2; }
    bool IsStakeModifierV2(const int nHeight) const { return nHeight >= height_start_StakeModifierV2; }

    int FutureBlockTimeDrift(const int nHeight) const
    {
        // PoS (TimeV2): 14 seconds
        if (IsTimeProtocolV2(nHeight)) return nTimeSlotLength - 1;
        // PoS (TimeV1): 3 minutes - PoW: 2 hours
        return (nHeight > height_last_PoW ? nFutureTimeDriftPoS : nFutureTimeDriftPoW);
    }

    bool IsValidBlockTimeStamp(const int64_t nTime, const int nHeight) const
    {
        // Before time protocol V2, blocks can have arbitrary timestamps
        if (!IsTimeProtocolV2(nHeight)) return true;
        // Time protocol v2 requires time in slots
        return (nTime % nTimeSlotLength) == 0;
    }

    bool HasStakeMinAgeOrDepth(const int contextHeight, const uint32_t contextTime,
            const int utxoFromBlockHeight, const uint32_t utxoFromBlockTime) const
    {
        // before stake modifier V2, we require the utxo to be nStakeMinAge old
        if (!IsStakeModifierV2(contextHeight))
            return (utxoFromBlockTime + nStakeMinAge <= contextTime);
        // with stake modifier V2+, we require the utxo to be nStakeMinDepth deep in the chain
        return (contextHeight - utxoFromBlockHeight >= nStakeMinDepth);
    }
};
} // namespace Consensus

#endif /* MUBDI_CONSENSUS_PARAMS_H */
