/***********************************************************************
***********Copyright (c) 2016 The Bitcoin Core developers***************
******************Copyright (c) 2010-2022 Nur1Labs**********************
>Distributed under the MIT software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#ifndef MUBDI_WALLET_TEST_FIXTURE_H
#define MUBDI_WALLET_TEST_FIXTURE_H

#include "test/test_mubdi.h"

/** Testing setup and teardown for wallet.
 */
struct WalletTestingSetup: public TestingSetup {
    WalletTestingSetup();
    ~WalletTestingSetup();
};

#endif

