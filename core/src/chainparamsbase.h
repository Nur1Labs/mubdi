/***********************************************************************
*************Copyright (c) 2014 The Bitcoin developers******************
************Copyright (c) 2017-2019 The PIVX developers*****************
******************Copyright (c) 2010-2022 Nur1Labs**********************
>Distributed under the MIT software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#ifndef MUBDI_CHAINPARAMSBASE_H
#define MUBDI_CHAINPARAMSBASE_H

#include <string>
#include <vector>

/**
 * CBaseChainParams defines the base parameters (shared between mubdi-cli and mubdid)
 * of a given instance of the MuBdI system.
 */
class CBaseChainParams
{
public:
    enum Network {
        MAIN,
        LIESE,
        ARIN,
        GAZA,
        TESTNET,
        REGTEST,

        MAX_NETWORK_TYPES
    };

    const std::string& DataDir() const { return strDataDir; }
    int RPCPort() const { return nRPCPort; }

protected:
    CBaseChainParams() {}

    int nRPCPort;
    std::string strDataDir;
    Network networkID;
};

/**
 * Return the currently selected parameters. This won't change after app startup
 * outside of the unit tests.
 */
const CBaseChainParams& BaseParams();

/** Sets the params returned by Params() to those for the given network. */
void SelectBaseParams(CBaseChainParams::Network network);

/**
 * Looks for -regtest or -testnet and returns the appropriate Network ID.
 * Returns MAX_NETWORK_TYPES if an invalid combination is given.
 */
CBaseChainParams::Network NetworkIdFromCommandLine();

/**
 * Calls NetworkIdFromCommandLine() and then calls SelectParams as appropriate.
 * Returns false if an invalid combination is given.
 */
bool SelectBaseParamsFromCommandLine();

/**
 * Return true if SelectBaseParamsFromCommandLine() has been called to select
 * a network.
 */
bool AreBaseParamsConfigured();

#endif /* MUBDI_CHAINPARAMSBASE_H */
