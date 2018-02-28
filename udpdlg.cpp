#include "udpdlg.h"
#include "ui_udpdlg.h"

UDPdlg::UDPdlg(QWidget *parent)
    : QDialog(parent), ui(new Ui::UDPdlg)
{
    bool ok;
    ui->setupUi(this);
    ui->lineEditTx->setFocus();
    ui->botTx->setEnabled(false);
    udpSocketUdpRx = new QUdpSocket(this);
    udpSocketSend = new QUdpSocket(this);
    udpSocketUdpRx->bind(ui->lineEditPortRx->text().toInt(&ok,10));
    connect(udpSocketUdpRx, SIGNAL(readyRead()),this, SLOT(vDatagramaUdpRxRebut()));


    udpSocketBroadcasting = new QUdpSocket(this);
    timer = new QTimer(this);
    connect(ui->btStartBroadcasting, SIGNAL(clicked(bool)), this, SLOT(startBroadcasting()));
    connect(ui->btEndBroadcasting, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(timer, SIGNAL(timeout()), this, SLOT(broadcastDatagram()));

}

UDPdlg::~UDPdlg()
{
    delete ui;
}

void UDPdlg::slotMiroSiHiHaText(QString qsz){
    if(qsz.length())
        ui->botTx->setEnabled(true);
    else
        ui->botTx->setEnabled(false);
}

void UDPdlg::vDatagramaUdpRxRebut(){
    while (udpSocketUdpRx->hasPendingDatagrams()) {
         QByteArray datagram;
         QHostAddress adrecaOrigen;
         quint16 portOrigen;
         datagram.resize(udpSocketUdpRx->pendingDatagramSize());
         udpSocketUdpRx->readDatagram(datagram.data(),datagram.size(),
                                 &adrecaOrigen,&portOrigen);
         vInterpretaTramaRebudaUdp(datagram,adrecaOrigen);
     }
}

void UDPdlg::vInterpretaTramaRebudaUdp(QByteArray datagrama,QHostAddress adrecaOrigen){
    ui->plainTextEdit->appendHtml(trUtf8("%1").arg(datagrama.data()));
    ui->plainTextEdit->appendHtml(adrecaOrigen.toString());

    ui->plainTextEdit->ensureCursorVisible();
}

void UDPdlg::on_botTx_clicked(){
    QString qsz; bool ok;

    qsz = ui->lineEditTx->text();
    quint16 nPortTx= ui->lineEditPortTx->text().toInt(&ok,10);
    //vTrametSocolUdp(qsz.toAscii(), // <-- Qt4
    vTrametSocolUdp(qsz.toLatin1(),  // <-- Qt5
                    QHostAddress(ui->lineEditIP->text()),
                    nPortTx);
    //vEscriuTramesUdp(qsz.toAscii(),// <-- Qt4
    vEscriuTramesUdp(qsz.toLatin1(), // <-- Qt5
                      QHostAddress(ui->lineEditIP->text()));
}

void UDPdlg::vTrametSocolUdp(QByteArray datagram,QHostAddress adreca,quint16 port){
        QUdpSocket *udpSocketTX = new QUdpSocket(this);
        udpSocketTX->writeDatagram(datagram.data(),datagram.size(),adreca,port);
        delete udpSocketTX;
}

void UDPdlg::vEscriuTramesUdp(QByteArray datagrama,QHostAddress adrecaOrigen){
   QString qsz;

    qsz = trUtf8("UDPc Tx%1<font color= red> CLNT %2 : <font color= orange>%3").
                    arg(QDateTime::currentDateTime().toLocalTime().toString("yyyyMMddhhmmsszzz")).
                    arg(adrecaOrigen.toString()).
                    arg(datagrama.data());
    ui->plainTextEdit->appendHtml(qsz);
    ui->plainTextEdit->ensureCursorVisible();

}

void UDPdlg::slotCanviaPortRx(QString qsz){
    bool ok;
    disconnect(udpSocketUdpRx);
    udpSocketUdpRx->abort();
    delete udpSocketUdpRx;

    udpSocketUdpRx = new QUdpSocket(this);
    udpSocketUdpRx->bind(qsz.toInt(&ok,10));
    connect(udpSocketUdpRx, SIGNAL(readyRead()),this, SLOT(vDatagramaUdpRxRebut()));
}

void UDPdlg::on_botIntercanvi_clicked(){
    QString qszAux;

    qszAux = ui->lineEditPortRx->text();
    ui->lineEditPortRx->setText(ui->lineEditPortTx->text());
    ui->lineEditPortTx->setText(qszAux);
}

void UDPdlg::on_checkBoxLedBlue_clicked()
{
    QByteArray datagram;
    QString qsz;
    int nPort = ui->lnPortSend->text().toInt();
    if(!ledBlue){
        qsz = "13h";
        ledBlue=true;
    }else{
        qsz = "13l";
        ledBlue = false;
    }
    datagram = qsz.toUtf8();

    udpSocketSend->writeDatagram(datagram.data(), QHostAddress(ui->lineEditIP->text()), nPort);
}

void UDPdlg::on_checkBoxLedGreen_clicked()
{
    QByteArray datagram;
    QString qsz;
    int nPort = ui->lnPortSend->text().toInt();
    if(!ledGreen){
        qsz = "12h";
        ledGreen=true;
    }else{
        qsz = "12l";
        ledGreen = false;
    }
    datagram = qsz.toUtf8();

    udpSocketSend->writeDatagram(datagram.data(), QHostAddress(ui->lineEditIP->text()), nPort);
}

void UDPdlg::on_checkBoxLedRed_clicked()
{
    QByteArray datagram;
    QString qsz;
    int nPort = ui->lnPortSend->text().toInt();
    if(!ledRed){
        qsz = "15h";
        ledRed=true;
    }else{
        qsz = "15l";
        ledRed = false;
    }
    datagram = qsz.toUtf8();

    udpSocketSend->writeDatagram(datagram.data(),QHostAddress(ui->lineEditIP->text()), nPort);
}

void UDPdlg::on_butSend_clicked()
{
    udpSocketSend = new QUdpSocket(this);
    QByteArray datagram;

    int nPort = ui->lnPortSend->text().toInt();
    QString qsz = ui->lnSendMsg->text();
    datagram = qsz.toUtf8();

    udpSocketSend->writeDatagram(datagram.data(), QHostAddress(ui->lineEditIP->text()), nPort);
}

void UDPdlg::startBroadcasting()
{
    ui->btStartBroadcasting->setEnabled(false);
    timer->start(1000);
}

void UDPdlg::broadcastDatagram()
{
    udpSocketBroadcasting = new QUdpSocket(this);
    ui->lbStatusBroadcasting->setText(tr("Now broadcasting datagram %1").arg(messageNo));
    QByteArray datagram = "Broadcast message " + QByteArray::number(messageNo);
    udpSocketBroadcasting->writeDatagram(datagram, QHostAddress::Broadcast, 45454);
    ++messageNo;
}

void UDPdlg::close(){
    disconnect(udpSocketBroadcasting);
    udpSocketBroadcasting->abort();
    delete udpSocketBroadcasting;
    ui->btStartBroadcasting->setEnabled(true);
    timer->stop();
}
