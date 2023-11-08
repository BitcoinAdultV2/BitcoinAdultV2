// Copyright (c) 2019-2020 The PIVX developers
// Copyright (c) 2023 The Bitcoin Adult Core Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include "qt/pivx/pwidget.h"
#include "qt/pivx/furabstractlistitemdelegate.h"
#include "qt/pivx/furlistrow.h"
#include "transactiontablemodel.h"
#include "qt/pivx/txviewholder.h"
#include "transactionfilterproxy.h"

#include <atomic>
#include <cstdlib>
#include <QWidget>
#include <QLineEdit>
#include <QMap>

#if defined(HAVE_CONFIG_H)
#include "config/pivx-config.h" /* for USE_QTCHARTS */
#endif

class PIVXGUI;
class WalletModel;

namespace Ui {
class ChatWidget;
}


class ChatWidget : public PWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(PIVXGUI* _window);
    ~ChatWidget();

    int incommingChatMessage(QString strAddress, QString strMessage);
    void loadWalletModel() override;

public Q_SLOTS:

Q_SIGNALS:

private Q_SLOTS:
    bool chatDestinationAddressChanged(const QString& str);
    void onChatSendButtonClicked();

private:
    Ui::ChatWidget *ui;
};

#endif // CHATWIDGET_H
