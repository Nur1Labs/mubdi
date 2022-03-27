/***********************************************************************
***************Copyright (c) 2019 The PIVX developers*******************
******************Copyright (c) 2010-2022 Nur1Labs**********************
>Distributed under the MIT software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#ifndef SETTINGSWALLETOPTIONSWIDGET_H
#define SETTINGSWALLETOPTIONSWIDGET_H

#include <QWidget>
#include <QDataWidgetMapper>
#include "qt/mubdi/pwidget.h"
namespace Ui {
class SettingsWalletOptionsWidget;
}

class SettingsWalletOptionsWidget : public PWidget
{
    Q_OBJECT

public:
    explicit SettingsWalletOptionsWidget(MuBdIGUI* _window, QWidget *parent = nullptr);
    ~SettingsWalletOptionsWidget();

    void setMapper(QDataWidgetMapper *mapper);

public Q_SLOTS:
    void onResetClicked();

private:
    Ui::SettingsWalletOptionsWidget *ui;
};

#endif /* SETTINGSWALLETOPTIONSWIDGET_H */
