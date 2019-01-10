#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QUdpSocket;

namespace Ui {
class Widget;
}

enum MsgType{Msg, UsrEnter, UsrLeft};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent,QString usrname);
    ~Widget();
protected:
    void usrEnter(QString usrname,QString ipaddr);
    void usrLeft(QString usrname,QString time);
    void sndMsg(MsgType type,QString srvaddr="");
    void closeEvent(QCloseEvent *event);
    QString getIP();
    QString getUsr();
    QString getMsg();
private:
    Ui::Widget *ui;
    QUdpSocket *udpSocket;
    qint16 port;
    QString uName;
private slots:
    void processPendingDatagrams();
    void on_sendBtn_clicked();
    void on_exitBtn_clicked();
    void on_clearBtn_clicked();
};

#endif // WIDGET_H
