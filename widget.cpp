#include "widget.h"
#include "ui_widget.h"
#include <QUdpSocket>
#include <QHostInfo>
#include <QMessageBox>
#include <QScrollBar>
#include <QDateTime>
#include <QNetworkInterface>
#include <QProcess>

Widget::Widget(QWidget *parent,QString usrname) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    uName = usrname;
    udpSocket = new QUdpSocket(this);
    port = 2333;
    udpSocket->bind(port,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));
    sndMsg(UsrEnter);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::sndMsg(MsgType type,QString srvaddr)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    QString address = getIP();
    out<<type<<getUsr();
    switch(type)
    {
        case Msg:
            if(ui->msgTxtEdit->toPlainText() == ""){
                QMessageBox::warning(0,tr("提示"),tr("发送不能为空"),QMessageBox::Ok);
                return;
            }
            out<<address<<getMsg();
            ui->msgBrowser->verticalScrollBar()->setValue(ui->msgBrowser->verticalScrollBar()->maximum());
        break;
        case UsrEnter:
            out<<address;
        break;
        case UsrLeft:   break;
    }
    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast,port);
}

void Widget::processPendingDatagrams()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int msgType;
        in>>msgType;
        QString usrName,ipAddr,msg;
        QString time=QDateTime::currentDateTime().toString(" hh:mm:ss");
        switch(msgType)
        {
            case Msg:
                in>>usrName>>ipAddr>>msg;
                ui->msgBrowser->setTextColor(Qt::green);
                ui->msgBrowser->setCurrentFont(QFont("Times New Roman",10));
                ui->msgBrowser->append("[ "+usrName+" ]"+time);
                ui->msgBrowser->append(msg);
            break;
            case UsrEnter:
                in>>usrName>>ipAddr;
                usrEnter(usrName,ipAddr);
            break;
            case UsrLeft:
                in>>usrName;
                usrLeft(usrName,time);
            break;
        }
    }
}

void Widget::usrEnter(QString usrname,QString ipaddr)
{
    bool isEmpty = ui->usrTblWidget->findItems(usrname,Qt::MatchExactly).isEmpty();
    if(isEmpty){
        QTableWidgetItem *usr = new QTableWidgetItem(usrname);
        QTableWidgetItem *ip = new QTableWidgetItem(ipaddr);
        ui->usrTblWidget->insertRow(0);
        ui->usrTblWidget->setItem(0,0,usr);
        ui->usrTblWidget->setItem(0,1,ip);
        ui->msgBrowser->setTextColor(Qt::red);
        ui->msgBrowser->setCurrentFont(QFont("Times New Roman",8));
        ui->msgBrowser->append(tr("%1 在地址 %2 上线了").arg(usrname).arg(ipaddr));
        sndMsg(UsrEnter);
    }
}

void Widget::usrLeft(QString usrname,QString time)
{
    int rowNum = ui->usrTblWidget->findItems(usrname,Qt::MatchExactly).first()->row();
    ui->usrTblWidget->removeRow(rowNum);
    ui->msgBrowser->setTextColor(Qt::red);
    ui->msgBrowser->setCurrentFont(QFont("Times New Roman",8));
    ui->msgBrowser->append(tr("%1 在 %2 离线了").arg(usrname).arg(time));
}

QString Widget::getIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach(QHostAddress addr, list){
        if(addr.protocol() == QAbstractSocket::IPv4Protocol)
            return addr.toString();
    }
    return 0;
}

QString Widget::getUsr()
{
    return uName;
}

QString Widget::getMsg()
{
    QString msg = ui->msgTxtEdit->toHtml();
    ui->msgTxtEdit->clear();
    ui->msgTxtEdit->setFocus();
    return msg;
}

void Widget::on_sendBtn_clicked()
{
    sndMsg(Msg);
}

void Widget::on_exitBtn_clicked()
{
    close();
}

void Widget::on_clearBtn_clicked()
{
    ui->msgBrowser->clear();
}

void Widget::closeEvent(QCloseEvent *e)
{
    sndMsg(UsrLeft);
    QWidget::closeEvent(e);
}
