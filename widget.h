#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLCDNumber>
#include <QRadioButton>
#include <QGridLayout>
#include <QCheckBox>
#include "qextserialport/qextserialport.h"
#include "qextserialport/qextserialenumerator.h"
#include  <QGroupBox>
#include <QPushButton>
#include <QTimer>
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QPainter>


class LED : public QWidget
{
    Q_OBJECT
public:
    LED(const QColor &color, QWidget *parent = 0);
    ~LED();
    bool isOn() const;
    void setOn(bool on);
protected:
    virtual void paintEvent(QPaintEvent *);   
private:
    QColor m_color;
    bool m_on;
};



class Widget : public QWidget
{
    Q_OBJECT
    
public:
    Widget(QWidget *parent = 0);
    ~Widget();
private:
    QLabel *pwmLabel;
    QLCDNumber *pwmLCD;
    QLabel *ahcdinitLabel,*ahcdrtLabel;
    QLabel *angleLabel,*angleTipLabel;
    QLCDNumber *angleLCD;

    QLabel *setVelLabel;
    QLCDNumber *setVelLCD;

    QLabel *rtVelLabel;
    QLCDNumber *rtVelLCD;

    QLabel *rtLenLabel;
    QLCDNumber *rtLenLCD;

    QLabel *autoSetLenLabel;
    QLCDNumber *autoSetLenLCD;

    QLabel *pressureLabel;
    QLCDNumber *pressureLCD;

    QLabel *reelingCtrLabel;
    QLCDNumber *reelingCtrLCD;

    QLCDNumber *gulunVelocityLCD;



    LED *upLED,*bottomLED,*approachLED;



    QextSerialPort *serialPortOfPC;
    LED *serialPortStatusLED;
    QTimer *multiOpenSerialportTimer;
    QFile dataFile;
    QLabel *dataFileLabel;
    QCheckBox *dataFileStatusCheckBox;

    QTextStream fout;
    quint8 ahcdinit,PCA0CPH0,status;
    qint16 ahcd;
    qint16 reelingCtr;
    qreal  setVel,rtVel,rtLength,autoLength,dutyPWM,pressure,angleOfStick;
    qreal  gulunVelocity;

    void setupUI();
    void selectSerialPortToUse();
    void setDataFile();
    void display(QByteArray &);

public slots:

    void onSerialPortReadyRead();
    void onMultiOpenSerialPortTimerStart();

};

#endif // WIDGET_H
