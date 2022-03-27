/***********************************************************************
***************Copyright (c) 2019 The PIVX developers*******************
******************Copyright (c) 2010-2022 Nur1Labs**********************
>Distributed under the MIT software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#ifndef SETTINGSBACKUPWALLET_H
#define SETTINGSBACKUPWALLET_H

#include <QWidget>
#include "qt/mubdi/pwidget.h"

namespace Ui {
class SettingsBackupWallet;
}

class SettingsBackupWallet : public PWidget
{
    Q_OBJECT

public:
    explicit SettingsBackupWallet(MuBdIGUI* _window, QWidget *parent = nullptr);
    ~SettingsBackupWallet();

private Q_SLOTS:
    void backupWallet();
    void selectFileOutput();
    void changePassphrase();

private:
    Ui::SettingsBackupWallet *ui;
    QString filename;
};

#endif /* SETTINGSBACKUPWALLET_H */
