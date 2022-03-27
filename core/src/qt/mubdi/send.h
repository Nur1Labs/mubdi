/***********************************************************************
***************Copyright (c) 2019 The PIVX developers*******************
******************Copyright (c) 2010-2022 Nur1Labs**********************
>Distributed under the MIT software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#ifndef SEND_H
#define SEND_H

#include <QWidget>
#include <QPushButton>

#include "qt/mubdi/pwidget.h"
#include "qt/mubdi/contactsdropdown.h"
#include "qt/mubdi/sendmultirow.h"
#include "qt/mubdi/sendcustomfeedialog.h"
#include "walletmodel.h"
#include "coincontroldialog.h"
#include "qt/mubdi/tooltipmenu.h"

static const int MAX_SEND_POPUP_ENTRIES = 8;

class MuBdIGUI;
class ClientModel;
class WalletModel;
class WalletModelTransaction;

namespace Ui {
class send;
class QPushButton;
}

class SendWidget : public PWidget
{
    Q_OBJECT

public:
    explicit SendWidget(MuBdIGUI* parent);
    ~SendWidget();

    void addEntry();

    void loadClientModel() override;
    void loadWalletModel() override;

Q_SIGNALS:
    /** Signal raised when a URI was entered or dragged to the GUI */
    void receivedURI(const QString& uri);

public Q_SLOTS:
    void onChangeAddressClicked();
    void onChangeCustomFeeClicked();
    void onCoinControlClicked();
    void onOpenUriClicked();
    void onValueChanged();
    void refreshAmounts();
    void changeTheme(bool isLightTheme, QString &theme) override;

protected:
    void resizeEvent(QResizeEvent *event) override;

private Q_SLOTS:
    void onMuBdISelected(bool _isMuBdI);
    void onSendClicked();
    void onContactsClicked(SendMultiRow* entry);
    void onMenuClicked(SendMultiRow* entry);
    void onAddEntryClicked();
    void clearEntries();
    void clearAll();
    void refreshView();
    void onContactMultiClicked();
    void onDeleteClicked();
    void onResetCustomOptions(bool fRefreshAmounts);
private:
    Ui::send *ui;
    QPushButton *coinIcon;
    QPushButton *btnContacts;

    SendCustomFeeDialog* customFeeDialog = nullptr;
    bool isCustomFeeSelected = false;

    int nDisplayUnit;
    QList<SendMultiRow*> entries;
    CoinControlDialog *coinControlDialog = nullptr;

    ContactsDropdown *menuContacts = nullptr;
    TooltipMenu *menu = nullptr;
    // Current focus entry
    SendMultiRow* focusedEntry = nullptr;

    bool isMuBdI = true;
    void resizeMenu();
    QString recipientsToString(QList<SendCoinsRecipient> recipients);
    SendMultiRow* createEntry();
    bool send(QList<SendCoinsRecipient> recipients);
    bool sendZMuBdI(QList<SendCoinsRecipient> recipients);
    void updateEntryLabels(QList<SendCoinsRecipient> recipients);

};

#endif /* SEND_H */
