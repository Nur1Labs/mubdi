/***********************************************************************
***************Copyright (c) 2019 The PIVX developers*******************
******************Copyright (c) 2010-2022 Nur1Labs**********************
>Distributed under the MIT software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#ifndef COINCONTROLMUBDIWIDGET_H
#define COINCONTROLMUBDIWIDGET_H

#include <QDialog>

namespace Ui {
class CoinControlMuBdIWidget;
}

class CoinControlMuBdIWidget : public QDialog
{
    Q_OBJECT

public:
    explicit CoinControlMuBdIWidget(QWidget *parent = nullptr);
    ~CoinControlMuBdIWidget();

private:
    Ui::CoinControlMuBdIWidget *ui;
};

#endif /* COINCONTROLMUBDIWIDGET_H */
