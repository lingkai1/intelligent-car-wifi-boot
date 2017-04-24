#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>
#include "basetsd.h"

namespace Ui {
class Widget;
}

struct position
{
    union
    {
        float x;
        char xChar[4];
    };
    union
    {
        float y;
        char yChar[4];
    };
    union
    {
        float orit;
        char oritChar[4];
    };
};

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
      bool IsIPaddress(QString ip, char *ipR);
protected:
    void paintEvent(QPaintEvent *);
private:
    Ui::Widget *ui;
    position pose;
    QUdpSocket *receiver;
    QUdpSocket *sender;
    UINT32 destIp;
    int delta;
    QTimer dataTimer;
    char ProcessBarTag[8];
    char ProcessBarCount[8];
    char startPaintCarPosition;
    bool stopOder;
    union sp
    {
        char c_speed[4];
        float f_speed;
    }speed;
       char dataSend[10];
private slots:
    void processPendingDatagram();

    void on_start_clicked();
    void on_stop_clicked();

    void on_increase_clicked();
    void on_decrease_clicked();
    void timeoutslot();
    void on_languige_toggled(bool checked);
    void on_radarProgressBar_destroyed();

};


#endif // WIDGET_H
