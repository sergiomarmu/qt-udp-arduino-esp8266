#ifndef UDPDLG_H
#define UDPDLG_H

//#include <QtGui/QDialog> // <-- Qt4
#include <QtWidgets/QDialog> // <-- Qt5
#include <QtNetwork>

namespace Ui
{
    class UDPdlg;
}

class UDPdlg : public QDialog
{
    Q_OBJECT

public:
    //UDPdlg(QWidget *parent = 0);
    explicit UDPdlg(QWidget *parent = nullptr);
    ~UDPdlg();
    bool ledBlue = false;
    bool ledRed = false;
    bool ledGreen = false;

private:
    Ui::UDPdlg *ui;
    QUdpSocket *udpSocketUdpRx;
    QUdpSocket *udpSocketSend;
    QUdpSocket *udpSocketBroadcasting;
    void vInterpretaTramaRebudaUdp(QByteArray datagrama,QHostAddress adrecaOrigen);
    void vEscriuTramesUdp(QByteArray datagrama,QHostAddress adrecaOrigen);
    void vTrametSocolUdp(QByteArray datagram,QHostAddress adreca,quint16 port);
    QTimer *timer;
    int messageNo = 1;

private slots:
    void on_botIntercanvi_clicked();
    void on_botTx_clicked();
    void slotMiroSiHiHaText(QString);
    void vDatagramaUdpRxRebut();
    void slotCanviaPortRx(QString);
    void on_checkBoxLedBlue_clicked();
    void on_checkBoxLedGreen_clicked();
    void on_checkBoxLedRed_clicked();
    void on_butSend_clicked();
    void startBroadcasting();
    void broadcastDatagram();
    void close();
};

#endif // UDPDLG_H
