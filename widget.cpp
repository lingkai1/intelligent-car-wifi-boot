#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <string>
#include <sstream>
#include <QPainter>
#define destIpstr "192.168.191.2"
using namespace std;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    receiver = new QUdpSocket(this);
    sender = new QUdpSocket(this);
    Qt::WindowFlags flags = 0;
    flags |= Qt::WindowMinimizeButtonHint;
    setWindowFlags(Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint);
    receiver->bind(45454,QUdpSocket::ShareAddress);
    connect(receiver,SIGNAL(readyRead()),this,SLOT(processPendingDatagram()));
    stopOder = 0;
    startPaintCarPosition = 0;
//    初始化pose位置

    pose.x=155.681;
    pose.y=61.5476;
    pose.orit = 1.45;
    ui->stop->setStyleSheet("QPushButton{border-image: url(:/normal/normal/stop.png);}"
                                  "QPushButton:hover{border-image: url(:/normal/normal/stop.png);}"
                                  "QPushButton:pressed{border-image: url(:/abnornmal/abnormal/stopAb.png)}");
    ui->start->setStyleSheet("QPushButton{border-image: url(:/normal/normal/start.png);}"
                                  "QPushButton:hover{border-image: url(:/normal/normal/start.png);}"
                                  "QPushButton:pressed{border-image: url(:/abnornmal/abnormal/startAb.png)}");

    ui->increase->setStyleSheet("QPushButton{border-image: url(:/speedButton/speedButton/up.png);}"
                                  "QPushButton:hover{border-image: url(:/speedButton/speedButton/up.png);}"
                                  "QPushButton:pressed{border-image: url(:/speedButton/speedButton/up.png)}");
    ui->decrease->setStyleSheet("QPushButton{border-image: url(:/speedButton/speedButton/down.png);}"
                                  "QPushButton:hover{border-image: url(:/speedButton/speedButton/down.png);}"
                                  "QPushButton:pressed{border-image: url(:/speedButton/speedButton/down.png)}");


    ui->start->setEnabled(false);
//            ui->lineNormal->hide();
//            ui->lightNormal->hide();
//            ui->bioNormal->hide();
//            ui->eightNormal->hide();
//            ui->sixNormal->hide();
//            ui->radarNormal->hide();
//            ui->gpsNormal->hide();
//            ui->controlNormal->hide();
    setFixedSize(this->width(), this->height());
    ui->sendMsgLineEdit->setText(destIpstr);
    ui->speedSet->setText("40");

    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(timeoutslot()));

//   ui->languige->setStyleSheet("QCheckBox::indicator {width: 20px; height: 20px;}");
    dataTimer.setInterval(20);// 50 times per second
    dataTimer.start();
    for(int i =0;i<8;i++)
    {
        ProcessBarTag[i] = 0;
        ProcessBarCount[i] = 0;
    }
    update();
    repaint();
//    position tt;
//    paintCarPosition(tt);
    delta = 0;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::processPendingDatagram() //处理等待的数据报
{
   // char checkState[10] = {0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};
    char checkState[9] = {0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};
//    datagram = 10+4+4+4=18
    char tmpCheck[9];
    while(receiver->hasPendingDatagrams())  //拥有等待的数据报
    {
        QByteArray datagram; //拥于存放接收的数据报

        //让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
        datagram.resize(receiver->pendingDatagramSize());

        //接收数据报，将其存放到datagram中AA
        receiver->readDatagram(datagram.data(),datagram.size());

        //将数据报内容显示出来

        if(datagram.data()[0] == 0x00)
        {
            if(datagram.data()[1] == 1)
            {
//                ui->lineNormal->show();
//                ui->lineAbnormal->hide();
//                new
                ProcessBarTag[0] = 1;
            }
            else if (datagram.data()[1] == 0)
            {
                ui->lineAbnormal->show();
                ui->lineNormal->hide();
//                new
                ProcessBarTag[0] = 0;
                ProcessBarCount[0] = 0;
            }

            if(datagram.data()[2] == 1)
            {
//                ui->lightNormal->show();
//                ui->lightAbnormal->hide();
//                new
                ProcessBarTag[1] = 1;
            }
            else if (datagram.data()[2] == 0)
            {
                ui->lightAbnormal->show();
                ui->lightNormal->hide();

                //                new
                                ProcessBarTag[1] = 0;
                                ProcessBarCount[1] = 0;
            }

            if(datagram.data()[3] == 1)
            {
//                ui->bioNormal->show();
//                ui->bioAbnormal->hide();
//                new
                ProcessBarTag[2] = 1;
            }
            else if (datagram.data()[3] == 0)
            {
                ui->bioAbnormal->show();
                ui->bioNormal->hide();
                //                new
                                ProcessBarTag[2] = 0;
                                ProcessBarCount[2] = 0;
            }

            if(datagram.data()[4] == 1)
            {
//                ui->eightNormal->show();
//                ui->eightAbnormal->hide();
//                new
                ProcessBarTag[3] = 1;
            }
            else if (datagram.data()[4] == 0)
            {
                ui->eightAbnormal->show();
                ui->eightNormal->hide();
                //                new
                                ProcessBarTag[3] = 0;
                                ProcessBarCount[3] = 0;
            }

            if(datagram.data()[5] == 1)
            {
//                ui->sixNormal->show();
//                ui->sixAbnormal->hide();
                //                new
                                ProcessBarTag[4] = 1;

            }
            else if (datagram.data()[5] == 0)
            {
                ui->sixAbnormal->show();
                ui->sixNormal->hide();
                //                new
                                ProcessBarTag[4] = 0;
                                ProcessBarCount[4] = 0;
            }

            if(datagram.data()[6] == 1)
            {
//                ui->radarNormal->show();
//                ui->radarAbnormal->hide();

                //                new
                                ProcessBarTag[5] = 1;
            }
            else if (datagram.data()[6] == 0)
            {
                ui->radarAbnormal->show();
                ui->radarNormal->hide();
                //                new
                                ProcessBarTag[5] = 0;
                                ProcessBarCount[5] = 0;
            }

            if(datagram.data()[7] == 1)
            {
//                ui->gpsNormal->show();
//                ui->gpsAbnormal->hide();

                //                new
                                ProcessBarTag[6] = 1;
            }
            else if (datagram.data()[7] == 0)
            {
                ui->gpsAbnormal->show();
                ui->gpsNormal->hide();
                //                new
                                ProcessBarTag[6] = 0;
                                ProcessBarCount[6] = 0;
            }

            if(datagram.data()[8] == 1)
            {
//                ui->controlNormal->show();
//                ui->controlAbnormal->hide();

                //                new
                                ProcessBarTag[7] = 1;
            }
            else if (datagram.data()[8] == 0)
            {
                ui->controlAbnormal->show();
                ui->controlNormal->hide();
                //                new
                                ProcessBarTag[7] = 0;
                                ProcessBarCount[7] = 0;
            }


            memcpy(tmpCheck,datagram.data(),9);   //放弃最后一个速度设置成功字节
            ui->start->setEnabled(true);
            startPaintCarPosition = 1;

            //            读出xyz以及 角度坐标
            memcpy(pose.xChar,&datagram.data()[10],4);
            memcpy(pose.yChar,&datagram.data()[14],4);
            memcpy(pose.oritChar,&datagram.data()[18],4);
//                更新位置
            update();


           // ui->label->setText(datagram);
            if(strcmp(tmpCheck,checkState)==0)  //所有灯都已打开 开启速度设置按钮，并开始绘制车的距离。
            {
               // ui->start->setEnabled(true);
               //startPaintCarPosition = 1;

                ////            读出xyz以及 角度坐标
                //memcpy(pose.xChar,&datagram.data()[10],4);
                //memcpy(pose.yChar,&datagram.data()[14],4);
                //memcpy(pose.oritChar,&datagram.data()[18],4);
//               // 更新位置
                //update();


               //// ui->label->setText(datagram);
            }
            else
            {
                ui->start->setEnabled(false);
                startPaintCarPosition = 0;
               // ui->label->setText(datagram);
            }

            if(datagram.data()[9] == 0x01)               //返回已经启动命令
            {
                if(stopOder == 0) //设置的不是停止命令
                {
                  ui->start_green->show();
                  ui->start_red->hide();
                }
                else            // 设置的是停止命令
                {
                    ui->start_green->hide();
                    ui->start_red->show();
                }
            }
        }

    }
}

void Widget::on_start_clicked()
{
   string str = ui->speedSet->text().toStdString();

   static char ip[4];//myCom->write(ui->sendMsgLineEdit->text().toLatin1());
   static char ip1[4];//myCom->write(ui->sendMsgLineEdit->text().toLatin1());
   if(IsIPaddress(ui->sendMsgLineEdit->text(),ip))
       {
              ip1[3]=ip[0];
              ip1[2]=ip[1];
              ip1[1]=ip[2];
              ip1[0]=ip[3];
              memcpy(&destIp,ip1,4);
                 }
//    int found = str.find_first_of('.');
//    string front = str.substr(0,found);
//    string end = str.substr(found+1 ,str.size()-found-1);
//    if(found == string::npos)
//    {
//        QMessageBox::warning(this, tr("warning"),
//                                                    tr("请输入有效的浮点数"),
//                                                    QMessageBox::Yes);return 0;
//    }
//    if()
    stringstream sstr(str);
    if(!(sstr >> speed.f_speed))
    {
        QMessageBox::warning(this, tr("warning"),
                                                           tr("请输入有效的浮点数"),
                                                          QMessageBox::Yes);return;

    }
    else if (speed.f_speed<0 || speed.f_speed>45)
    {
        QMessageBox::warning(this, tr("warning"),
                                             tr("请输入有效的浮点数（最高速度45km/h）"),
                                              QMessageBox::Yes);return;
    }
    else if(speed.f_speed == 0)
    {
        stopOder = 1;            //设置停止命令

        dataSend[0] = 0x01;
        dataSend[1] = speed.c_speed[0];
        dataSend[2] = speed.c_speed[1];
        dataSend[3] = speed.c_speed[2];
        dataSend[4] = speed.c_speed[3];
        sender->writeDatagram(dataSend,10,
               QHostAddress(destIp),45454);

    }
    else
    {
        stopOder = 0;         //设置运行命令

        dataSend[0] = 0x01;
        dataSend[1] = speed.c_speed[0];
        dataSend[2] = speed.c_speed[1];
        dataSend[3] = speed.c_speed[2];
        dataSend[4] = speed.c_speed[3];
        sender->writeDatagram(dataSend,10,
               QHostAddress(destIp),45454);
    }



    //以ASCII码形式将行编辑框中的数据写入串口


//    return;




}


void Widget::on_stop_clicked()
{
    stopOder = 0;         //设置运行命令
    speed.f_speed = 0;
    dataSend[0] = 0x01;
    dataSend[1] = speed.c_speed[0];
    dataSend[2] = speed.c_speed[1];
    dataSend[3] = speed.c_speed[2];
    dataSend[4] = speed.c_speed[3];

    sender->writeDatagram(dataSend,10,
           QHostAddress(destIp),45454);
    delta = delta + 10;
    update();


}

bool Widget::IsIPaddress(QString ip, char *ipR)
{
    string str = ip.toStdString();
    uint found =  str.find_first_of('.');
    int before = 0;
    int temp;
    string s = str.substr(before,found - before);
    before = found;
    if(found == string::npos) {QMessageBox::warning(this, tr("warning"),
                                            tr("不是一个有效的IPv4地址，请输入如192.168.1.1格式"),
                                            QMessageBox::Yes);return 0;}
     temp = atoi(s.c_str());
     if (temp < 255 && temp > 0)
         ipR[0] =  (char)temp;
     else
     {
       QMessageBox::warning(this, tr("warning"),
                    tr("不是一个有效的IPv4地址，请输入如192.168.1.1格式"),
                    QMessageBox::Yes);
       return 0;
     }
    for(int i = 1; i <= 2; i++)
    {
      found =  str.find_first_of('.',found+1);
      if(found == string::npos) {QMessageBox::warning(this, tr("warning"),
                                                     tr("不是一个有效的IPv4地址，请输入如192.168.1.1格式"),
                                                     QMessageBox::Yes);return 0;}
      s = str.substr(before+1,found - before-1);
      before = found;
      temp = atoi(s.c_str());
      if (temp < 255 && temp > 0)
          ipR[i] =  (char)temp;
      else
      {
       QMessageBox::warning(this, tr("warning"),
                            tr("不是一个有效的IPv4地址，请输入如192.168.1.1格式"),
                             QMessageBox::Yes);
            return 0;
        }
    }
    found =  str.length();
    if(found == string::npos) {QMessageBox::warning(this, tr("warning"),
                                            tr("不是一个有效的IPv4地址，请输入如192.168.1.1格式"),
                                            QMessageBox::Yes);return 0;}
    s = str.substr(before+1 ,found - before-1);
    before = found;
    temp = atoi(s.c_str());
    if (temp < 255 && temp > 0)
        ipR[3] =  (char)temp;
    else
    {
      QMessageBox::warning(this, tr("warning"),
                   tr("不是一个有效的IPv4地址，请输入如192.168.1.1格式"),
                   QMessageBox::Yes);
        return 0;
    }
    return 1;
}






void Widget::on_increase_clicked()
{
    stringstream sstr(ui->speedSet->text().toStdString());
    float speed;
    sstr >> speed;
    speed = speed + 1;
    stringstream ssstr;
    ssstr << speed;
    ui->speedSet->setText( ssstr.str().c_str());
}



void Widget::on_decrease_clicked()
{
    stringstream sstr(ui->speedSet->text().toStdString());
    float speed;
    sstr >> speed;
    if(speed > 1)
    speed = speed - 1;
    stringstream ssstr;
    ssstr << speed;
    ui->speedSet->setText( ssstr.str().c_str());
}
// fresh the proceebar here
void Widget::timeoutslot()
{
    if(ProcessBarTag[0] == 1 && ProcessBarCount[0] < 30){ProcessBarCount[0] += 6;}
    if(ProcessBarTag[0] == 1 && ProcessBarCount[0] >= 30&&ProcessBarCount[0] < 80){ProcessBarCount[0] += 5;}
    if(ProcessBarTag[0] == 1 && ProcessBarCount[0] >= 80&&ProcessBarCount[0] < 100){ProcessBarCount[0] += 10;}


    if(ProcessBarTag[1] == 1 && ProcessBarCount[1] < 30){ProcessBarCount[1] += 6;}
    if(ProcessBarTag[1] == 1 && ProcessBarCount[1] >= 30&&ProcessBarCount[1] < 80){ProcessBarCount[1] += 5;}
    if(ProcessBarTag[1] == 1 && ProcessBarCount[1] >= 80&&ProcessBarCount[1] < 100){ProcessBarCount[1] += 5;}

    if(ProcessBarTag[2] == 1 && ProcessBarCount[2] < 100){ProcessBarCount[2] += 5;}

    if(ProcessBarTag[3] == 1 && ProcessBarCount[3] < 100){ProcessBarCount[3] += 5;}

    if(ProcessBarTag[4] == 1 && ProcessBarCount[4] < 30){ProcessBarCount[4] += 3;}
    if(ProcessBarTag[4] == 1 && ProcessBarCount[4] >= 30&&ProcessBarCount[4] < 80){ProcessBarCount[4] += 5;}
    if(ProcessBarTag[4] == 1 && ProcessBarCount[4] >= 80&&ProcessBarCount[4] < 100){ProcessBarCount[4] += 5;}

    if(ProcessBarTag[5] == 1 && ProcessBarCount[5] < 30){ProcessBarCount[5] += 3;}
    if(ProcessBarTag[5] == 1 && ProcessBarCount[5] >= 30&&ProcessBarCount[5] < 80){ProcessBarCount[5] += 5;}
    if(ProcessBarTag[5] == 1 && ProcessBarCount[5] >= 80&&ProcessBarCount[5] < 100){ProcessBarCount[5] += 5;}

    if(ProcessBarTag[6] == 1 && ProcessBarCount[6] < 30){ProcessBarCount[6] += 2;}
    if(ProcessBarTag[6] == 1 && ProcessBarCount[6] >= 30&&ProcessBarCount[6] < 80){ProcessBarCount[6] += 5;}
    if(ProcessBarTag[6] == 1 && ProcessBarCount[6] >= 80&&ProcessBarCount[6] < 100){ProcessBarCount[6] += 4;}

    if(ProcessBarTag[7] == 1 && ProcessBarCount[7] < 30){ProcessBarCount[7] += 2;}
    if(ProcessBarTag[7] == 1 && ProcessBarCount[7] >= 30&&ProcessBarCount[7] < 80){ProcessBarCount[7] += 4;}
    if(ProcessBarTag[7] == 1 && ProcessBarCount[7] >= 80&&ProcessBarCount[7] < 100){ProcessBarCount[7] += 3;}


    if(ProcessBarCount[0]==100)
     {
        ui->radarProgressBar->setVisible(0);
        ui->radarNormal->show();
        ui->radarAbnormal->hide();
    }
    if(ProcessBarCount[1]==100)
     {
        ui->gpsProgressBar->setVisible(0);
        ui->gpsNormal->show();
        ui->gpsAbnormal->hide();
      }
    if(ProcessBarCount[2]==100)
     {
        ui->lightProgressBar->setVisible(0);
        ui->lightNormal->show();
        ui->lightAbnormal->hide();
      }
    if(ProcessBarCount[3]==100)
      {
        ui->controlProgressBar->setVisible(0);
        ui->controlNormal->show();
        ui->controlAbnormal->hide();
      }
    if(ProcessBarCount[4]==100)
    {
        ui->lineProgressBar->setVisible(0);
        ui->lineNormal->show();
        ui->lineAbnormal->hide();
    }
    if(ProcessBarCount[5]==100)
     {
        ui->sixProgressBar->setVisible(0);
        ui->sixNormal->show();
        ui->sixAbnormal->hide();
      }
    if(ProcessBarCount[6]==100)
    {
        ui->eightProgressBar->setVisible(0);
        ui->eightNormal->show();
        ui->eightAbnormal->hide();
    }
    if(ProcessBarCount[7]==100)
    {
        ui->bioProgressBar->setVisible(0);
        ui->bioNormal->show();
        ui->bioAbnormal->hide();
    }



    ui->radarProgressBar->setValue(ProcessBarCount[0]);
    ui->gpsProgressBar->setValue(ProcessBarCount[1]);
    ui->lightProgressBar->setValue(ProcessBarCount[2]);
    ui->controlProgressBar->setValue(ProcessBarCount[3]);
    ui->lineProgressBar->setValue(ProcessBarCount[4]);
    ui->sixProgressBar->setValue(ProcessBarCount[5]);
    ui->eightProgressBar->setValue(ProcessBarCount[6]);
    ui->bioProgressBar->setValue(ProcessBarCount[7]);

}



void Widget::on_languige_toggled(bool checked)
{

    if(ui->languige->checkState()== Qt::Checked )
    {
        ui->radar->setText("雷达");ui->stereo->setText("双目摄像头");
        ui->light->setText("红绿灯摄像头");ui->gps->setText("GPS信号");
        ui->control->setText("控制");ui->six->setText("64线激光");ui->eight->setText("8线激光");ui->line->setText("车道线摄像头");
//        把所有需要加的中文都加进来


    }
    else//        把所有需要加的英文都加进来
    {
        ui->radar->setText("RADAR");ui->stereo->setText("STEREO");
        ui->gps->setText("GPS SINGNAL");ui->light->setText("TRAFFIC LIGHT");
        ui->control->setText("CONTROLLER");ui->six->setText("64-LADAR");ui->eight->setText("8-LADAR");ui->line->setText("LANE LINE");
        }
}

void Widget::on_radarProgressBar_destroyed()
{

}

void Widget::paintEvent(QPaintEvent *)
{

//    Widget::raise();
        QPainter painter(this);
        QPixmap pix;

        pix.load(":/decorete/decorate/map3.jpg");
//        pix.load("../release/up.png");
        painter.drawPixmap(1700, 200, 801*1.2, 731*1.2, pix);

        pix.load(":/decorete/decorate/car.bmp");

//        旋转pix
        double angle=-(pose.orit-0.588-1.57);
        double cosangle=cos(angle);
        double sinangle=sin(angle);
        QMatrix leftmatrix(cosangle,sinangle,-sinangle,cosangle,0,0);
        //leftmatrix.rotate(-(pose.orit-0.4933-1.57) * 57.29578);
        //leftmatrix.rotate(-(-0.4933-1.57) * 57.29578);

       painter.translate(1700, 200);




        painter.drawPixmap(pose.x*0.763843348428742 + pose.y*0.481877354764404+173.193994314378 -15,
                           pose.x*0.48235959931155 - pose.y*(0.766549350633963)+106.846885697904-10,
                           pix.transformed(leftmatrix,Qt::FastTransformation));//Qt::KeepAspectRatioByExpanding));//


        //    painter.drawPixmap(100, 100, 129, 66, pix);




}
