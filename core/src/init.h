/***********************************************************************
**************Copyright (c) 2009-2010 Satoshi Nakamoto******************
***********Copyright (c) 2009-2014 The Bitcoin developers***************
************Copyright (c) 2017-2019 The PIVX developers*****************
******************Copyright (c) 2010-2022 Nur1Labs**********************
>Distributed under the MIT software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#ifndef MUBDI_INIT_H
#define MUBDI_INIT_H

#include <string>

class CScheduler;
class CWallet;

namespace boost
{
class thread_group;
} // namespace boost

extern CWallet* pwalletMain;

void StartShutdown();
bool ShutdownRequested();
/** Interrupt threads */
void Interrupt();
void Shutdown();
void PrepareShutdown();
bool AppInit2();

/** Initialize MuBdI core: Basic context setup.
 *  @note This can be done before daemonization. Do not call Shutdown() if this function fails.
 *  @pre Parameters should be parsed and config file should be read.
 */
bool AppInitBasicSetup();

/** The help message mode determines what help message to show */
enum HelpMessageMode {
    HMM_MUBDID,
    HMM_MUBDI_QT
};

/** Help for options shared between UI and daemon (for -help) */
std::string HelpMessage(HelpMessageMode mode);
/** Returns licensing information (for -version) */
std::string LicenseInfo();

#endif /* MUBDI_INIT_H */
