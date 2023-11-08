/ Copyright (c) 2019-2020 The PIVX developers
// Copyright (c) 2023 The Bitcoin Adult Core Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "qt/pivx/chatwidget.h"
#include "qt/pivx/forms/ui_chatwidget.h"
#include "qt/pivx/sendconfirmdialog.h"
#include "qt/pivx/txrow.h"
#include "qt/pivx/qtutils.h"
#include "guiutil.h"
#include "walletmodel.h"
#include "clientmodel.h"
#include "optionsmodel.h"
#include "utiltime.h"
#include <vector>
#include <QPainter>	
#include <QPainter>
#include <QModelIndex>
#include <QList>
#include <QGraphicsLayout>

#define DECORATION_SIZE 65
#define NUM_ITEMS 3
#define SHOW_EMPTY_CHART_VIEW_THRESHOLD 4000
#define REQUEST_LOAD_TASK 1
#define CHART_LOAD_MIN_TIME_INTERVAL 15

ChatWidget::ChatWidget(PIVXGUI* parent) :
    PWidget(parent),
    ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
    
    this->setStyleSheet(parent->styleSheet());
    this->setContentsMargins(0,0,0,0);

    // Containers
    setCssProperty({this}, "container");
    ui->left->setContentsMargins(0,0,0,0);

    setCssSubtitleScreen(ui->lblChatMessageDestination);
    setCssSubtitleScreen(ui->lblChatMessageToSend);
    setCssSubtitleScreen(ui->lblChatMessageReceived);
    setCssBtnSecondary(ui->btnSendMessage);
    setCssProperty({ui->txtChatDestinationAddress}, "chat-primary-message");
    setCssProperty(ui->txtChatMessageReceived, "chat-text-message-received");
    setCssProperty(ui->txtChatMessageToSend, "chat-text-message-to-send");
    // Title
    setCssTitleScreen(ui->lblChatTitle);
    ui->lblChatTitle->setText("Anonymous encrypted  Chat by BTAD Wallet");
    ui->lblChatMessageDestination->setText("Destination Address:");
    ui->lblChatMessageReceived->setText("Received Messages:");
    ui->lblChatMessageToSend->setText("Message to send:");
    // Subtitle 
    //setCssSubtitleScreen(ui->labelSubtitle);
    connect(parent, &PIVXGUI::newChatMessage, this, &ChatWidget::incommingChatMessage);
    connect(ui->btnSendMessage, &QPushButton::clicked, this, &ChatWidget::onChatSendButtonClicked);
    connect(ui->txtChatDestinationAddress, &QLineEdit::textChanged, [this](){chatDestinationAddressChanged(ui->txtChatDestinationAddress->text());});
}


void ChatWidget::loadWalletModel()
{  
    // update the display unit, to not use the default ("PASSION")
}

int ChatWidget::incommingChatMessage(QString strAddress, QString strMessage) {

    QString from ="<b>"+strAddress+" wrote: </b><br>" + strMessage +"<br>";
    QString history = ui->txtChatMessageReceived->toHtml();
    ui->txtChatMessageReceived->setHtml(from+history);
    inform("New Message received");
    return 1;
}

void ChatWidget::onChatSendButtonClicked() {
    
    QString message = ui->txtChatMessageToSend->toPlainText();
    QString destination = ui->txtChatDestinationAddress->text().trimmed();
    bool staking_only = false;
    bool is_shielded = false;
    const bool valid = walletModel->validateAddress(destination);
    if(!valid) {
        inform("No valid address.");
        return;
    }
    if(message.length() > 1000) {
        inform("Message size greater than allowed (1000)");
        return;
    }
    if(message.length() < 1) {
        inform("Message empty nothing to send!");
        return;
    }
    if (g_connman->GetNodeCount(CConnman::CONNECTIONS_ALL) == 0) {
        inform("No active connections to Network, can't send message!");
        return;
    }
    CChat::getInstance()->AddMessageToSend(destination.toStdString(),message.toStdString());
    ui->txtChatMessageToSend->setText("");
    inform("Message sent to "+ destination);
    
}

bool ChatWidget::chatDestinationAddressChanged(const QString& str)
{
    
    bool shielded = false;
    if (!str.isEmpty()) {
        QString trimmedStr = str.trimmed();
        const bool valid = walletModel->validateAddress(trimmedStr);
        if (!valid) {
            // check URI
            SendCoinsRecipient rcp;
            if (GUIUtil::parseBitcoinURI(trimmedStr, &rcp)) {
                ui->txtChatDestinationAddress->setText(rcp.address);
                QString label = walletModel->getAddressTableModel()->labelForAddress(rcp.address);
                if (!label.isNull() && !label.isEmpty()){
                    ui->txtChatDestinationAddress->setText(label);
                } else if (!rcp.message.isEmpty())
                    ui->txtChatDestinationAddress->setText(rcp.message);

            } else {
                setCssProperty(ui->txtChatDestinationAddress, "chat-address-destination-error");
            }
        } else {
            setCssProperty(ui->txtChatDestinationAddress, "chat-address-destination");
        }
        updateStyle(ui->txtChatDestinationAddress);
        return valid;
    }
    setCssProperty(ui->txtChatDestinationAddress, "chat-address-destination");
    updateStyle(ui->txtChatDestinationAddress);
    return false;
}

ChatWidget::~ChatWidget()
{
    delete ui;
}

