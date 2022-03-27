/***********************************************************************
****************Copyright (c) 2010 Satoshi Nakamoto*********************
***********Copyright (c) 2009-2014 The Bitcoin developers***************
*************Copyright (c) 2014-2015 The Dash developers****************
*************Copyright (c) 2015-2019 The PIVX developers****************
******************Copyright (c) 2010-2022 Nur1Labs**********************
>Distributed under the MIT software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#include "libairlogic/Params.h"
#include "chainparams.h"
#include "consensus/merkle.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>
#include <limits>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock = 0;
    genesis.nVersion = nVersion;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of the genesis coinbase cannot
 * be spent as it did not originally exist in the database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "31/05/2010 - Dinar And Dirham will be number one. Become will be used global.";
    const CScript genesisOutputScript = CScript() << ParseHex("04ae82ad7aa10d3cd0ed6f0bd813dfe5f5b9ee882b833d98ff54dfb511c4eff8fa45c7f341e8f4b47a2a12e2ca31e8d9bea70e8795d345bf983e50a954353f27e2") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0xf115ea61f4b1c6dd5071634ee64f0590bf31a902e8b01e2d0f7a26d5311ee756")); //first block
	//(100, uint256("0x0000004a6d30df66c3ab07eb8708842038a49044f7582fe38765edb3168e16b3")) //first checkpoint
	//(200, uint256("0x000001de3869691cc5183fcc95c9728d6b07f7016f97a737e60ad93de3113f7a")); //updates of blockchain core
	//(3407, uint256("0x000000067eb8cbc29a9a63923b023398156becb31ae5e342fa21e7be58bc1003")); //removing zero protocol and zero library
	//(4000, uint256("0x00000f7f22aeab341b0880ed4a07890a76eee814e3ac3796baec12d934100b2f")) //v.1.5 break point. the checkpoint will continue for LC
	//(5000, uint256("0x00000339c1f602347f94ed453503c3984be93c7e2a9e74ece755a687beddd737")) //1st Halving Speed and Liqudation is more
	//(6000, uint256("0x000005a615d59b943da5c5c1b369163d6514a3f3abf730718731e57214946900")) //Burn For V.1.5. Not used Anymore
	//(19000, uint256("0x000003b9e7d29d9540400b8461ba27c97239b471b841a9cd345966584b11aa35")); //Fork For PoW end 20k block
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    0, // * UNIX timestamp of last checkpoint block
    0,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    0        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsLiese =
    boost::assign::map_list_of
    (50, uint256("0x0000003844865c0fa9ca924bc5f125fa35bdc77dd0a49b5ffce6ebf523919fc7"));
static const Checkpoints::CCheckpointData dataLiese = {
    &mapCheckpointsLiese,
    0,
    0,
    0};

static Checkpoints::MapCheckpoints mapCheckpointsArin =
    boost::assign::map_list_of
    (0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataArin = {
    &mapCheckpointsArin,
    0,
    0,
    0};

    static Checkpoints::MapCheckpoints mapCheckpointsGaZa =
    boost::assign::map_list_of
    (0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataGaZa = {
    &mapCheckpointsGaZa,
    0,
    0,
    0};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of
    (0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    0,
    0,
    0};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    0,
    0,
    0};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params() const
{
    assert(this);
    static CBigNum bnTrustedModulus(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParams = libzerocoin::ZerocoinParams(bnTrustedModulus);

    return &ZCParams;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "lilith";

        genesis = CreateGenesisBlock(1620401973, 686001, 0x1e0ffff0, 1, 10 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256("0xf115ea61f4b1c6dd5071634ee64f0590bf31a902e8b01e2d0f7a26d5311ee756"));
        assert(genesis.hashMerkleRoot == uint256("0xafa5537df767ffbd36168609fc42f44bd53152f8844fad002ed3921e18c41795"));

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.powLimit   = ~uint256(0) >> 20;   // MuBdI starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~uint256(0) >> 24;
        consensus.posLimitV2 = ~uint256(0) >> 20;
        consensus.nBudgetCycleBlocks = 43200;       // approx. 1 every 30 days
        consensus.nBudgetFeeConfirmations = 6;      // Number of confirmations for the finalization fee
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 20;       // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 51000000 * COIN;
        consensus.nPoolMaxTransactions = 3;
        consensus.nProposalEstablishmentTime = 60 * 60 * 24;    // must be at least a day old to make it into a budget
        consensus.nStakeMinAge = 12 * 60;
        consensus.nStakeMinDepth = 600;
        consensus.nTargetTimespan = 1 * 30;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 30;
        consensus.nTimeSlotLength = 15;
        consensus.strObfuscationPoolDummyAddress = "cZZUE8vsATY2iXQ3aYxS7QRYNLiZ7uLVZW";

        // spork keys
        consensus.strSporkPubKey = "04de56c0b1541fe85af32594aa51adc1bbfc36ec034626d6c70f7b108c6360db48a8ce3d3774f3378e12ec7e166c655aa136f5994dc046d9d5324a7e0658853c30";
        consensus.strSporkPubKeyOld = "04bae85ee459abc741151d25d3e98098677fbbac7acd6689a8c2f991bd8488c37725451a5d7bf08eb7715c579f08d707c5e9e4b3bcc4d55d91ed44468e6d116862";
        consensus.nTime_EnforceNewSporkKey = 1594010951;    //!> Sporks signed after  must use the new spork key
        consensus.nTime_RejectOldSporkKey = 1594010951;     //!> Fully reject old spork key after Monday, 07/06/2020 @ 4:49am (UTC)

        // majorities for block version upgrades
        consensus.nEnforceBlockUpgradeMajority = 8100; // 75%
        consensus.nRejectBlockOutdatedMajority = 10260; // 95%
        consensus.nToCheckBlockUpgradeMajority = 10800; // Approximate expected amount of blocks in 7 days (1440*7.5)

        // height-based activations
        consensus.height_last_PoW = 196960;
        consensus.height_start_PoS = 100;
        consensus.height_start_BIP65 = 1808634;             // Block v5: 82629b7a9978f5c7ea3f70a12db92633a7d2e436711500db28b97efd48b1e527
        consensus.height_start_MessSignaturesV2 = 2153200;  // height_start_TimeProtoV2
        consensus.height_start_StakeModifierNewSelection = 615800;
        consensus.height_start_StakeModifierV2 = 1967000;   // Block v6: 0ef2556e40f3b9f6e02ce611b832e0bbfe7734a8ea751c7b555310ee49b61456
        consensus.height_start_TimeProtoV2 = 2153200;       // Block v7: 14e477e597d24549cac5e59d97d32155e6ec2861c1003b42d0566f9bf39b65d5
        
        // validation by-pass
        consensus.nMuBdIBadBlockTime = 1471401614;    // Skip nBit validation of Block 259201 per PR #915
        consensus.nMuBdIBadBlockBits = 0x1c056dac;    // Skip nBit validation of Block 259201 per PR #915

        nZerocoinStartHeight = 999999999;               // to disable which height of starting Zerocoin
        nZerocoinStartTime = 253383724800;              // Disable/Active 05/31/9999
        nBlockEnforceSerialRange = ~1;                  //Enforce serial range starting this block
        nBlockRecalculateAccumulators = ~1;             //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = ~1;                     //First block that bad serials emerged
        nBlockLastGoodCheckpoint = ~1;                  //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = ~1;                  //Start enforcing the invalid UTXO's
        //vFixedSeeds.clear();
        //vSeeds.clear();

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x7c;
        pchMessageStart[1] = 0x70;
        pchMessageStart[2] = 0x0a;
        pchMessageStart[3] = 0x72;
        vAlertPubKey = ParseHex("04354a89eb0f91d5941b0433359862ae7b8f924b58607acce7913e99e5fb3a554b980817e8197f5ab0f6a8d0ce2066d049928749bd71414c32513f2b436b3fbcba");
        nDefaultPort = 46031;

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.push_back(CDNSSeedData("51.79.250.55", "51.79.250.55"));             // seed-main
        //vSeeds.push_back(CDNSSeedData("66.175.236.170", "66.175.236.170"));             // seed-second
        //vSeeds.push_back(CDNSSeedData("103.153.60.211 ", "103.153.60.211 "));         // nano-nodes


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 88);      // c
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 23);      // A
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 125);    // starting with 's'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 102);         // 1
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0xc5)(0x49)(0x70)(0xca).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0xd9)(0x06)(0x18)(0x5f).convert_to_container<std::vector<unsigned char> >();
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0xbb)(0x45)(0xb1)(0x82).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));


        /** Zerocoin */
        zerocoinModulus = "0xc95577b6dce0049b0a20c779af38079355abadde1a1d80c353f6cb697a7ae5a087bad39caa5798478551d0f9d91e6267716506f32412de1d19d17588765eb9502b85c6a18abdb05791cfd8b734e960281193705eeece210920cc922b3af3ceb178bf12c22eb565d5767fbf19545639be8953c2c38ffad41f3371e4aac750ac2d7bd614b3faabb453081d5d88fdbb803657a980bc93707e4b14233a2358c97763bf28f7c933206071477e8b371f229bc9ce7d6ef0ed7163aa5dfe13bc15f7816348b328fa2c1e69d5c88f7b94cee7829d56d1842d77d7bb8692e9fc7b7db059836500de8d57eb43c345feb58671503b932829112941367996b03871300f25efb5";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
        nZerocoinRequiredStakeDepth = 200; //The required confirmations for a zMuBdI to be stakable
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }

};
static CMainParams mainParams;

/* Liese Network Is new Light speed system by v7 network mubdi */

class CLieseParams : public CChainParams
{
public:
    CLieseParams()
    {
        networkID = CBaseChainParams::LIESE;
        strNetworkID = "liese";

        genesis = CreateGenesisBlock(1620401973, 686001, 0x1e0ffff0, 1, 10 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256("0xf115ea61f4b1c6dd5071634ee64f0590bf31a902e8b01e2d0f7a26d5311ee756"));
        assert(genesis.hashMerkleRoot == uint256("0xafa5537df767ffbd36168609fc42f44bd53152f8844fad002ed3921e18c41795"));
 
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.powLimit   = ~uint256(0) >> 20;   // MuBdI starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~uint256(0) >> 24;
        consensus.posLimitV2 = ~uint256(0) >> 20;   // 60/4 = 15 ==> use 2**4 higher limit
        consensus.nBudgetCycleBlocks = 43200;       // approx. 1 every 30 days
        consensus.nBudgetFeeConfirmations = 6;      // Number of confirmations for the finalization fee
        consensus.nCoinbaseMaturity = 10;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 20;       // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 14000000 * COIN;
        consensus.nPoolMaxTransactions = 3;
        consensus.nProposalEstablishmentTime = 60 * 60 * 24;    // must be at least a day old to make it into a budget
        consensus.nStakeMinAge = 12 * 60;
        consensus.nStakeMinDepth = 600;
        consensus.nTargetTimespan = 1 * 30;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 30;
        consensus.nTimeSlotLength = 15;
        consensus.strObfuscationPoolDummyAddress = "cZZUE8vsATY2iXQ3aYxS7QRYNLiZ7uLVZW";
       
        // spork keys
        consensus.strSporkPubKey = "04de56c0b1541fe85af32594aa51adc1bbfc36ec034626d6c70f7b108c6360db48a8ce3d3774f3378e12ec7e166c655aa136f5994dc046d9d5324a7e0658853c30";
        consensus.strSporkPubKeyOld = "04bae85ee459abc741151d25d3e98098677fbbac7acd6689a8c2f991bd8488c37725451a5d7bf08eb7715c579f08d707c5e9e4b3bcc4d55d91ed44468e6d116862";
        consensus.nTime_EnforceNewSporkKey = 1594010951;    //!> Sporks signed after  must use the new spork key
        consensus.nTime_RejectOldSporkKey = 1594010951;     //!> Fully reject old spork key after Monday, 07/06/2020 @ 4:49am (UTC)

        // majorities for block version upgrades
        consensus.nEnforceBlockUpgradeMajority = 8100; // 75%
        consensus.nRejectBlockOutdatedMajority = 10260; // 95%
        consensus.nToCheckBlockUpgradeMajority = 10800; // Approximate expected amount of blocks in 7 days (1440*7.5)

        // height-based activations
        consensus.height_last_PoW = 196960;
        consensus.height_start_PoS = 100;
        consensus.height_start_BIP65 = 1808634;             // Block v5: 82629b7a9978f5c7ea3f70a12db92633a7d2e436711500db28b97efd48b1e527
        consensus.height_start_MessSignaturesV2 = 2153200;  // height_start_TimeProtoV2
        consensus.height_start_StakeModifierNewSelection = 615800;
        consensus.height_start_StakeModifierV2 = 1967000;   // Block v6: 0ef2556e40f3b9f6e02ce611b832e0bbfe7734a8ea751c7b555310ee49b61456
        consensus.height_start_TimeProtoV2 = 2153200;       // Block v7: 14e477e597d24549cac5e59d97d32155e6ec2861c1003b42d0566f9bf39b65d5
        
        // validation by-pass
        consensus.nMuBdIBadBlockTime = 1471401614;    // Skip nBit validation of Block 259201 per PR #915
        consensus.nMuBdIBadBlockBits = 0x1c056dac;    // Skip nBit validation of Block 259201 per PR #915

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x7c;
        pchMessageStart[1] = 0x70;
        pchMessageStart[2] = 0x0a;
        pchMessageStart[3] = 0x72;
        vAlertPubKey = ParseHex("04354a89eb0f91d5941b0433359862ae7b8f924b58607acce7913e99e5fb3a554b980817e8197f5ab0f6a8d0ce2066d049928749bd71414c32513f2b436b3fbcba");
        nDefaultPort = 47031;

    	//vSeeds.push_back(CDNSSeedData("144.217.89.214", "144.217.89.214"));             // seed-main
	
        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 48);      // L
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 88);      // c
	base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 125);     // starting with 's'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 102);         // 1
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0xc5)(0x49)(0x70)(0xca).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0xd9)(0x06)(0x18)(0x5f).convert_to_container<std::vector<unsigned char> >();
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0xbb)(0x45)(0xb1)(0x82).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataLiese;
    }
};
static CLieseParams lieseParams;

/* Arin Network Is new Light speed system by v7 network mubdi */

class CArinParams : public CChainParams
{
public:
    CArinParams()
    {
        networkID = CBaseChainParams::ARIN;
        strNetworkID = "arin";
        
        genesis = CreateGenesisBlock(1620401973, 686001, 0x1e0ffff0, 1, 10 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256("0xf115ea61f4b1c6dd5071634ee64f0590bf31a902e8b01e2d0f7a26d5311ee756"));
        assert(genesis.hashMerkleRoot == uint256("0xafa5537df767ffbd36168609fc42f44bd53152f8844fad002ed3921e18c41795"));

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.powLimit   = ~uint256(0) >> 20;   // MuBdI starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~uint256(0) >> 24;
        consensus.posLimitV2 = ~uint256(0) >> 20;   // 60/4 = 15 ==> use 2**4 higher limit
        consensus.nBudgetCycleBlocks = 43200;       // approx. 1 every 30 days
        consensus.nBudgetFeeConfirmations = 6;      // Number of confirmations for the finalization fee
        consensus.nCoinbaseMaturity = 50;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 20;       // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 670000000 * COIN;
        consensus.nPoolMaxTransactions = 3;
        consensus.nProposalEstablishmentTime = 60 * 60 * 24;    // must be at least a day old to make it into a budget
        consensus.nStakeMinAge = 12 * 60;
        consensus.nStakeMinDepth = 600;
        consensus.nTargetTimespan = 1 * 30;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 30;
        consensus.nTimeSlotLength = 15;
        consensus.strObfuscationPoolDummyAddress = "cZZUE8vsATY2iXQ3aYxS7QRYNLiZ7uLVZW";

        // spork keys
        consensus.strSporkPubKey = "04de56c0b1541fe85af32594aa51adc1bbfc36ec034626d6c70f7b108c6360db48a8ce3d3774f3378e12ec7e166c655aa136f5994dc046d9d5324a7e0658853c30";
        consensus.strSporkPubKeyOld = "04bae85ee459abc741151d25d3e98098677fbbac7acd6689a8c2f991bd8488c37725451a5d7bf08eb7715c579f08d707c5e9e4b3bcc4d55d91ed44468e6d116862";
        consensus.nTime_EnforceNewSporkKey = 1594010951;    //!> Sporks signed after  must use the new spork key
        consensus.nTime_RejectOldSporkKey = 1594010951;     //!> Fully reject old spork key after Monday, 07/06/2020 @ 4:49am (UTC)

        // majorities for block version upgrades
        consensus.nEnforceBlockUpgradeMajority = 8100; // 75%
        consensus.nRejectBlockOutdatedMajority = 10260; // 95%
        consensus.nToCheckBlockUpgradeMajority = 10800; // Approximate expected amount of blocks in 7 days (1440*7.5)

        // height-based activations
        consensus.height_last_PoW = 196960;
        consensus.height_start_PoS = 1000;
        consensus.height_start_BIP65 = 1808634;             // Block v5: 82629b7a9978f5c7ea3f70a12db92633a7d2e436711500db28b97efd48b1e527
        consensus.height_start_MessSignaturesV2 = 2153200;  // height_start_TimeProtoV2
        consensus.height_start_StakeModifierNewSelection = 615800;
        consensus.height_start_StakeModifierV2 = 1967000;   // Block v6: 0ef2556e40f3b9f6e02ce611b832e0bbfe7734a8ea751c7b555310ee49b61456
        consensus.height_start_TimeProtoV2 = 2153200;       // Block v7: 14e477e597d24549cac5e59d97d32155e6ec2861c1003b42d0566f9bf39b65d5
        
        // validation by-pass
        consensus.nMuBdIBadBlockTime = 1471401614;    // Skip nBit validation of Block 259201 per PR #915
        consensus.nMuBdIBadBlockBits = 0x1c056dac;    // Skip nBit validation of Block 259201 per PR #915

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x7c;
        pchMessageStart[1] = 0x70;
        pchMessageStart[2] = 0x0a;
        pchMessageStart[3] = 0x72;
        vAlertPubKey = ParseHex("04354a89eb0f91d5941b0433359862ae7b8f924b58607acce7913e99e5fb3a554b980817e8197f5ab0f6a8d0ce2066d049928749bd71414c32513f2b436b3fbcba");
        nDefaultPort = 48031;

    	vSeeds.push_back(CDNSSeedData("144.217.89.214", "144.217.89.214"));             // seed-main
	
        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 23);      // A
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 88);      // c
	base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 125);     // starting with 's'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 102);         // 1
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0xc5)(0x49)(0x70)(0xca).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0xd9)(0x06)(0x18)(0x5f).convert_to_container<std::vector<unsigned char> >();
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0xbb)(0x45)(0xb1)(0x82).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataArin;
    }
};
static CArinParams arinParams;

/* GaZa Network Is new network for GaZa people */

class CGaZaParams : public CChainParams
{
public:
    CGaZaParams()
    {
        networkID = CBaseChainParams::GAZA;
        strNetworkID = "gaza";

        genesis = CreateGenesisBlock(1620401973, 686001, 0x1e0ffff0, 1, 10 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256("0xf115ea61f4b1c6dd5071634ee64f0590bf31a902e8b01e2d0f7a26d5311ee756"));
        assert(genesis.hashMerkleRoot == uint256("0xafa5537df767ffbd36168609fc42f44bd53152f8844fad002ed3921e18c41795"));

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.powLimit   = ~uint256(0) >> 20;   // MuBdI starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~uint256(0) >> 24;
        consensus.posLimitV2 = ~uint256(0) >> 20;   // 60/4 = 15 ==> use 2**4 higher limit
        consensus.nBudgetCycleBlocks = 43200;       // approx. 1 every 30 days
        consensus.nBudgetFeeConfirmations = 6;      // Number of confirmations for the finalization fee
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 20;       // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 35000000 * COIN;
        consensus.nPoolMaxTransactions = 3;
        consensus.nProposalEstablishmentTime = 60 * 60 * 24;    // must be at least a day old to make it into a budget
        consensus.nStakeMinAge = 12 * 60;
        consensus.nStakeMinDepth = 600;
        consensus.nTargetTimespan = 1 * 30;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 30;
        consensus.nTimeSlotLength = 15;
        consensus.strObfuscationPoolDummyAddress = "cZZUE8vsATY2iXQ3aYxS7QRYNLiZ7uLVZW";

        // spork keys
        consensus.strSporkPubKey = "04de56c0b1541fe85af32594aa51adc1bbfc36ec034626d6c70f7b108c6360db48a8ce3d3774f3378e12ec7e166c655aa136f5994dc046d9d5324a7e0658853c30";
        consensus.strSporkPubKeyOld = "04bae85ee459abc741151d25d3e98098677fbbac7acd6689a8c2f991bd8488c37725451a5d7bf08eb7715c579f08d707c5e9e4b3bcc4d55d91ed44468e6d116862";
        consensus.nTime_EnforceNewSporkKey = 1594010951;    //!> Sporks signed after  must use the new spork key
        consensus.nTime_RejectOldSporkKey = 1594010951;     //!> Fully reject old spork key after Monday, 07/06/2020 @ 4:49am (UTC)

        // majorities for block version upgrades
        consensus.nEnforceBlockUpgradeMajority = 8100; // 75%
        consensus.nRejectBlockOutdatedMajority = 10260; // 95%
        consensus.nToCheckBlockUpgradeMajority = 10800; // Approximate expected amount of blocks in 7 days (1440*7.5)

        // height-based activations
        consensus.height_last_PoW = 196960;
        consensus.height_start_PoS = 500;
        consensus.height_start_BIP65 = 1808634;             // Block v5: 82629b7a9978f5c7ea3f70a12db92633a7d2e436711500db28b97efd48b1e527
        consensus.height_start_MessSignaturesV2 = 2153200;  // height_start_TimeProtoV2
        consensus.height_start_StakeModifierNewSelection = 615800;
        consensus.height_start_StakeModifierV2 = 1967000;   // Block v6: 0ef2556e40f3b9f6e02ce611b832e0bbfe7734a8ea751c7b555310ee49b61456
        consensus.height_start_TimeProtoV2 = 2153200;       // Block v7: 14e477e597d24549cac5e59d97d32155e6ec2861c1003b42d0566f9bf39b65d5
        
        // validation by-pass
        consensus.nMuBdIBadBlockTime = 1471401614;    // Skip nBit validation of Block 259201 per PR #915
        consensus.nMuBdIBadBlockBits = 0x1c056dac;    // Skip nBit validation of Block 259201 per PR #915
   
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x7c;
        pchMessageStart[1] = 0x70;
        pchMessageStart[2] = 0x0a;
        pchMessageStart[3] = 0x72;
        vAlertPubKey = ParseHex("04354a89eb0f91d5941b0433359862ae7b8f924b58607acce7913e99e5fb3a554b980817e8197f5ab0f6a8d0ce2066d049928749bd71414c32513f2b436b3fbcba");
        nDefaultPort = 49031;

    	vSeeds.push_back(CDNSSeedData("144.217.89.214", "144.217.89.214"));             // seed-main
	
        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 38);      // G
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 88);      // c
	base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 125);     // starting with 's'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 102);         // 1
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0xc5)(0x49)(0x70)(0xca).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0xd9)(0x06)(0x18)(0x5f).convert_to_container<std::vector<unsigned char> >();
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0xbb)(0x45)(0xb1)(0x82).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataGaZa;
    }
};
static CGaZaParams gazaParams;


/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";

        genesis = CreateGenesisBlock(1620401973, 686001, 0x1e0ffff0, 1, 10 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        //assert(consensus.hashGenesisBlock == uint256("0xf115ea61f4b1c6dd5071634ee64f0590bf31a902e8b01e2d0f7a26d5311ee756"));
        //assert(genesis.hashMerkleRoot == uint256("0xafa5537df767ffbd36168609fc42f44bd53152f8844fad002ed3921e18c41795"));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~uint256(0) >> 20;   // MuBdI starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~uint256(0) >> 24;
        consensus.posLimitV2 = ~uint256(0) >> 20;
        consensus.nBudgetCycleBlocks = 144;         // approx 10 cycles per day
        consensus.nBudgetFeeConfirmations = 3;      // (only 8-blocks window for finalization on testnet)
        consensus.nCoinbaseMaturity = 15;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 4;        // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 51000000 * COIN;
        consensus.nPoolMaxTransactions = 2;
        consensus.nProposalEstablishmentTime = 60 * 5;  // at least 5 min old to make it into a budget
        consensus.nStakeMinAge = 60 * 60;
        consensus.nStakeMinDepth = 100;
        consensus.nTargetTimespan = 1 * 30;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 30;
        consensus.nTimeSlotLength = 15;
        consensus.strObfuscationPoolDummyAddress = "c57cqfGRkekRyDRNeJiLtYVEbvhXrNbmox";

        // spork keys
        consensus.strSporkPubKey = "04de56c0b1541fe85af32594aa51adc1bbfc36ec034626d6c70f7b108c6360db48a8ce3d3774f3378e12ec7e166c655aa136f5994dc046d9d5324a7e0658853c30";
        consensus.strSporkPubKeyOld = "04bae85ee459abc741151d25d3e98098677fbbac7acd6689a8c2f991bd8488c37725451a5d7bf08eb7715c579f08d707c5e9e4b3bcc4d55d91ed44468e6d116862";
        consensus.nTime_EnforceNewSporkKey = 1594010951;    //!> Sporks signed after  must use the new spork key
        consensus.nTime_RejectOldSporkKey = 1594010951;     //!> Fully reject old spork key after Monday, 07/06/2020 @ 4:49am (UTC)

        // majorities for block version upgrades
        consensus.nEnforceBlockUpgradeMajority = 4320; // 75%
        consensus.nRejectBlockOutdatedMajority = 5472; // 95%
        consensus.nToCheckBlockUpgradeMajority = 5760; // 4 days

        // height based activations
        consensus.height_last_PoW = 200;
        consensus.height_start_PoS = 100;
        consensus.height_start_BIP65 = 851019;                  // Block v5: d1ec8838ba8f644e78dd4f8e861d31e75457dfe607b31deade30e806b5f46c1c
        consensus.height_start_MessSignaturesV2 = 1347000;      // height_start_TimeProtoV2
        consensus.height_start_StakeModifierNewSelection = 51197;
        consensus.height_start_StakeModifierV2 = 1214000;       // Block v6: 1822577176173752aea33d1f60607cefe9e0b1c54ebaa77eb40201a385506199
        consensus.height_start_TimeProtoV2 = 1347000;           // Block v7: 30c173ffc09a13f288bf6e828216107037ce5b79536b1cebd750a014f4939882
                
        // validation by-pass
        consensus.nMuBdIBadBlockTime = 1489001494; // Skip nBit validation of Block 201 per PR #915
        consensus.nMuBdIBadBlockBits = 0x1e0a20bd; // Skip nBit validation of Block 201 per PR #915
 
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */

        pchMessageStart[0] = 0xc4;
        pchMessageStart[1] = 0x5e;
        pchMessageStart[2] = 0xb2;
        pchMessageStart[3] = 0x10;
        vAlertPubKey = ParseHex("046ad100472ed021f81a379e00a68bffb6bdfa19f1f7471a6e7f52adecf3f11c32ba23eda1a68924090bead98350cda2c1dc079a268c445a143d604728ec74f456");
        nDefaultPort = 46100;

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("114.124.212.245", "114.124.212.245"));         // Testnet will online

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 90);  // Testnet MuBdI addresses start with 'd'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 41);  // Testnet MuBdI script addresses start with 'H'
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 73); // starting with 'W'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 110);     // Testnet private keys start with 'm' (MuBdI defaults)
        // Testnet mubdi BIP32 pubkeys start with 'DINV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x9e)(0xbb)(0x20)(0xdd).convert_to_container<std::vector<unsigned char> >();
        // Testnet mubdi BIP32 prvkeys start with 'DHMP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x29)(0x65)(0xbc)(0x9d).convert_to_container<std::vector<unsigned char> >();
        // Testnet mubdi BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x07)(0xcf)(0xcc)(0xa4).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";

        genesis = CreateGenesisBlock(1620401973, 686001, 0x1e0ffff0, 1, 10 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        //assert(consensus.hashGenesisBlock == uint256("0xf115ea61f4b1c6dd5071634ee64f0590bf31a902e8b01e2d0f7a26d5311ee756"));
        //assert(genesis.hashMerkleRoot == uint256("0xafa5537df767ffbd36168609fc42f44bd53152f8844fad002ed3921e18c41795"));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~uint256(0) >> 20;   // MuBdI starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~uint256(0) >> 24;
        consensus.posLimitV2 = ~uint256(0) >> 20;
        consensus.nBudgetCycleBlocks = 144;         // approx 10 cycles per day
        consensus.nBudgetFeeConfirmations = 3;      // (only 8-blocks window for finalization on regtest)
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 4;        // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 51000000 * COIN;
        consensus.nPoolMaxTransactions = 2;
        consensus.nProposalEstablishmentTime = 60 * 5;  // at least 5 min old to make it into a budget
        consensus.nStakeMinAge = 0;
        consensus.nStakeMinDepth = 2;
        consensus.nTargetTimespan = 1 * 30;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 30;
        consensus.nTimeSlotLength = 15;
        consensus.strObfuscationPoolDummyAddress = "c57cqfGRkekRyDRNeJiLtYVEbvhXrNbmox";

        /* Spork Key for RegTest:
        WIF private key: 932HEevBSujW2ud7RfB1YF91AFygbBRQj3de3LyaCRqNzKKgWXi
        private key hex: bd4960dcbd9e7f2223f24e7164ecb6f1fe96fc3a416f5d3a830ba5720c84b8ca
        Address: yCvUVd72w7xpimf981m114FSFbmAmne7j9
        */
        consensus.strSporkPubKey = "04de56c0b1541fe85af32594aa51adc1bbfc36ec034626d6c70f7b108c6360db48a8ce3d3774f3378e12ec7e166c655aa136f5994dc046d9d5324a7e0658853c30";
        consensus.strSporkPubKeyOld = "";
        consensus.nTime_EnforceNewSporkKey = 0;
        consensus.nTime_RejectOldSporkKey = 0;

        // majorities for block version upgrades
        consensus.nEnforceBlockUpgradeMajority = 750;
        consensus.nRejectBlockOutdatedMajority = 950;
        consensus.nToCheckBlockUpgradeMajority = 1000;

        // height based activations
        consensus.height_last_PoW = 250;
        consensus.height_start_PoS = 100;
        consensus.height_start_BIP65 = 851019;              // Not defined for regtest. Inherit TestNet value.
        consensus.height_start_MessSignaturesV2 = 1;
        consensus.height_start_StakeModifierNewSelection = 0;
        consensus.height_start_StakeModifierV2 = 251;       // start with modifier V2 on regtest
        consensus.height_start_TimeProtoV2 = 999999999;
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */

        pchMessageStart[0] = 0x4b;
        pchMessageStart[1] = 0x9b;
        pchMessageStart[2] = 0x13;
        pchMessageStart[3] = 0x72;
        nDefaultPort = 47000;

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

static CChainParams* pCurrentParams = 0;

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::LIESE:
        return lieseParams;
    case CBaseChainParams::ARIN:
        return arinParams;
    case CBaseChainParams::GAZA:
        return gazaParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
