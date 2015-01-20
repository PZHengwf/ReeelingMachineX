#include "widget.h"
#include "stdio.h"
/**********************************************************/
LED::LED(const QColor &color, QWidget *parent)
        : QWidget(parent)
{
    m_color = color;
    m_on = false;
}

void LED::paintEvent(QPaintEvent *)
{
    if(m_on)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(m_color);
        painter.drawEllipse(width()/3.5, width()/3.5,width()/2.5, width()/2.5);
    }
    else
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QColor(Qt::white));
        painter.drawEllipse(width()/3.5, width()/3.5,width()/2.5, width()/2.5);
    }
}

bool LED::isOn() const
{
    return m_on;
}

void LED::setOn(bool on)
{
    m_on = on;
    update();
}

LED::~LED()
{

}


/***********************************************************************/
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("RMachine");
    setMinimumSize(640,480);
    setupUI();
    selectSerialPortToUse();
    setDataFile();

}
Widget::~Widget()
{
    
}


void Widget::setupUI()
{
    QGridLayout *layout = new QGridLayout(this);

    setStyleSheet("QLCDNumber{background-color: rgb(170, 170, 0);font: 12pt '华文中宋';} QGroupBox,QCheckBox,QLabel {font: 12pt '华文中宋';} ");
    angleLabel = new QLabel("手柄角度");
    angleLCD = new QLCDNumber;
    angleLCD->setDigitCount(6);
    angleLCD->setSegmentStyle(QLCDNumber::Flat);
    ahcdinitLabel = new QLabel("255");
    ahcdinitLabel->setAlignment(Qt::AlignCenter);
    ahcdinitLabel->setFrameShape(QFrame::StyledPanel);
    ahcdrtLabel = new QLabel("255");
    ahcdrtLabel->setAlignment(Qt::AlignCenter);
    ahcdrtLabel->setFrameShape(QFrame::StyledPanel);
    angleTipLabel = new QLabel("下降角度为正");
    layout->addWidget(angleLabel,0,0,1,1);
    layout->addWidget(angleLCD,0,1,1,1);
    layout->addWidget(ahcdinitLabel,0,2,1,1);
    layout->addWidget(ahcdrtLabel,0,3,1,1);
    layout->addWidget(angleTipLabel,0,4,1,1);

    setVelLabel = new QLabel("设定速度");
    setVelLCD = new QLCDNumber;
    setVelLCD->setDigitCount(6);
    setVelLCD->setSegmentStyle(QLCDNumber::Flat);
    layout->addWidget(setVelLabel,1,0,1,1);
    layout->addWidget(setVelLCD,1,1,1,1);

    pwmLabel = new QLabel("PWM电压(V)");
    pwmLCD = new QLCDNumber;
    pwmLCD->setDigitCount(6);
    pwmLCD->setSegmentStyle(QLCDNumber::Flat);
    layout->addWidget(pwmLabel,1,2,1,1);
    layout->addWidget(pwmLCD,1,3,1,1);

    rtVelLabel = new QLabel("瞬时速度");
    rtVelLCD = new QLCDNumber;
    rtVelLCD->setDigitCount(6);
    rtVelLCD->setSegmentStyle(QLCDNumber::Flat);

    layout->addWidget(rtVelLabel,2,0,1,1);
    layout->addWidget(rtVelLCD,2,1,1,1);
    rtLenLabel = new QLabel("瞬时缆长");
    rtLenLCD = new QLCDNumber;
    rtLenLCD->setDigitCount(6);
    rtLenLCD->setSegmentStyle(QLCDNumber::Flat);
    layout->addWidget(rtLenLabel,2,2,1,1);
    layout->addWidget(rtLenLCD,2,3,1,1);

    pressureLabel = new QLabel("压力");
    pressureLCD = new QLCDNumber;
    pressureLCD->setDigitCount(6);
    pressureLCD->setSegmentStyle(QLCDNumber::Flat);
    layout->addWidget(pressureLabel,3,0,1,1);
    layout->addWidget(pressureLCD,3,1,1,1);

    reelingCtrLabel = new QLabel("收放次数");
    reelingCtrLCD = new QLCDNumber;
    reelingCtrLCD->setDigitCount(6);
    reelingCtrLCD->setSegmentStyle(QLCDNumber::Flat);
    layout->addWidget(reelingCtrLabel,3,2,1,1);
    layout->addWidget(reelingCtrLCD,3,3,1,1);

    autoSetLenLabel = new QLabel("自动设缆");
    autoSetLenLCD = new QLCDNumber;
    autoSetLenLCD->setDigitCount(6);
    autoSetLenLCD->setSegmentStyle(QLCDNumber::Flat);


    dataFileStatusCheckBox = new QCheckBox("数据保存");
    dataFileStatusCheckBox->setEnabled(false);
    dataFileLabel = new QLabel;
    dataFileLabel->setFrameShape(QFrame::StyledPanel);


    QHBoxLayout *hLayout1 = new QHBoxLayout;
    hLayout1->addWidget(autoSetLenLabel);
    hLayout1->addWidget(autoSetLenLCD);
    hLayout1->addWidget(dataFileStatusCheckBox);
    hLayout1->addWidget(dataFileLabel);
    layout->addLayout(hLayout1,4,0,1,4);

    QLabel *gulunVelocityLabel = new QLabel("鼓轮速度");
    gulunVelocityLCD = new QLCDNumber();
    gulunVelocityLCD->setDigitCount(6);
    gulunVelocityLCD->setSegmentStyle(QLCDNumber::Flat);
    layout->addWidget(gulunVelocityLabel,5,0,1,1);
    layout->addWidget(gulunVelocityLCD,5,1,1,1);



    QGroupBox *statusGroup = new QGroupBox(QString("status"));
    statusGroup->setFlat(true);
    statusGroup->setStyleSheet("background-color: rgb(170, 170, 127)");
    upLED = new LED(QColor(Qt::red));
    upLED->setOn(true);
    QLabel *upLEDLabel = new QLabel("上限");
    approachLED = new LED(QColor(Qt::green));
    approachLED->setOn(false);
    QLabel *approachLEDLabel = new QLabel("接近");
    bottomLED = new LED(QColor(Qt::red));
    bottomLED->setOn(false);
    QLabel *bottomLEDLabel = new QLabel("下限");

    QGridLayout *statusGroupLayout = new QGridLayout(statusGroup);
    statusGroupLayout->addWidget(upLEDLabel,0,0,1,1);
    statusGroupLayout->addWidget(upLED,0,1,1,1);
    statusGroupLayout->addWidget(approachLEDLabel,2,0,1,1);
    statusGroupLayout->addWidget(approachLED,2,1,1,1);
    statusGroupLayout->addWidget(bottomLEDLabel,1,0,1,1);
    statusGroupLayout->addWidget(bottomLED,1,1,1,1);

    layout->addWidget(statusGroup,1,4,3,1);



    QGroupBox *serialportGroup = new QGroupBox();
    serialportGroup->setFlat(true);
    serialportGroup->setStyleSheet("background-color: rgb(170,170,127)");
    QLabel *serialPortStatusLabel = new QLabel("串口");
    serialPortStatusLED = new LED(QColor(Qt::red));
    QHBoxLayout *hLayout2 = new QHBoxLayout(serialportGroup);
    hLayout2->addWidget(serialPortStatusLabel);
    hLayout2->addWidget(serialPortStatusLED);
    layout->addWidget(serialportGroup,5,4,1,1);

}






void Widget::selectSerialPortToUse()
{
    multiOpenSerialportTimer = new QTimer(this);
    multiOpenSerialportTimer->setInterval(1000);
    connect(multiOpenSerialportTimer,SIGNAL(timeout()),this,SLOT(onMultiOpenSerialPortTimerStart()));
    QList<QextPortInfo>  ports = QextSerialEnumerator::getPorts(); //获取计算机上所有串口
    QString portNameToUse = ports.at(0).portName;   //选择第一个计算机串口，获取串口名称
    serialPortOfPC = new QextSerialPort(portNameToUse);
    if(serialPortOfPC->open(QIODevice::ReadWrite))  //如果打开串口成功
    {
        serialPortStatusLED->setOn(true);
        connect(serialPortOfPC,SIGNAL(readyRead()),this,SLOT(onSerialPortReadyRead()));
        serialPortOfPC->setBaudRate(BAUD9600);
        serialPortOfPC->setFlowControl(FLOW_OFF);
        serialPortOfPC->setDataBits(DATA_8);
        serialPortOfPC->setParity(PAR_NONE);
        serialPortOfPC->setStopBits(STOP_1);
        qDebug() << true;
    }
    else
    {
        serialPortStatusLED->setOn(false);
        multiOpenSerialportTimer->start();
        qDebug() << false;
    }
}

void Widget::onSerialPortReadyRead()
{
    static const char refA[] = {0xFF,0xAA};
    QByteArray bytes;

    qint16 bufferLen = serialPortOfPC->bytesAvailable ();
    if ( bufferLen < 20)
    {
        return;
    }
    bytes.resize(bufferLen);
    serialPortOfPC->read(bytes.data(), bytes.size());

    QByteArray refAA = QByteArray::fromRawData(refA,sizeof(refA));
    if(!bytes.startsWith(refAA))
    {
        return;
    }
    display (bytes);
}

void Widget::onMultiOpenSerialPortTimerStart()
{
    if(serialPortOfPC->open(QIODevice::ReadWrite))  //如果打开串口成功
    {
        multiOpenSerialportTimer->stop();  //关闭定时器
        serialPortStatusLED->setOn(true);
        connect(serialPortOfPC,SIGNAL(readyRead()),this,SLOT(onSerialPortReadyRead()));
        serialPortOfPC->setBaudRate(BAUD9600);
        serialPortOfPC->setFlowControl(FLOW_OFF);
        serialPortOfPC->setDataBits(DATA_8);
        serialPortOfPC->setParity(PAR_NONE);
        serialPortOfPC->setStopBits(STOP_1);
        qDebug() << true;
    }
    else
    {
        serialPortStatusLED->setOn(false);
        qDebug() << false;
    }
}


void Widget::setDataFile()
{
    if(!QDir("C:/RMachine.Parameter").exists())  //如果文件存在，则跳过，否则创建文件夹
    {
        QDir("C:/").mkdir("RMachine.Parameter");   //创建参数存储文件夹
    }
    //文件命名序号存在于一个.ini文件中，文件命名序号依次增加
    QSettings *iniFileSetting =new QSettings("C:/RMachine.Parameter/RMFileNameIndex.ini" ,QSettings::IniFormat);
    quint64 index0 = iniFileSetting->value("FileNameIndex/index","Default").toULongLong();
    if(index0 == 1025)
    {
        index0 = 0;
    }
    iniFileSetting->setValue("FileNameIndex/index",index0+1);

    QString name = QString("RM_Trial_") + QString::number(index0) + QString(".dat");
    QString fileName = QString("C:/RMachine.Parameter/") + name;
    dataFile.setFileName (fileName);
    if(!dataFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))  //截取以前的内容
    {
        exit(1216) ;
    }
    dataFileStatusCheckBox->setChecked(true);
    dataFileLabel->setText(name);

    fout.setDevice (&dataFile);
}


void Widget::display(QByteArray &bytes)
{
    quint8 RXArray[100]={0} , ch = 0;
    char buffers[50] = {0};

    for(quint8 i = 2 ; i < bytes.size (); i++ )//去掉开头两个字节的开始码
    {
        ch = bytes[i]&0xFF;
        RXArray[i] = ch;
    }

    ahcdinit = RXArray[2];                   //ahcdinit
    sprintf(buffers,"%d",ahcdinit*16);
    ahcdinitLabel->setText (QString(buffers));

    ch = RXArray[3];                //ahcd,并计算对应角度
    ahcd = ch;
    ch = RXArray[4];
    ahcd |= (ch << 8);
    sprintf(buffers,"%d",ahcd);
    ahcdrtLabel->setText(QString(buffers));
    //angleOfStick = 25.0/65.0*ahcd - ahcdinit*25.0/65.0;
    angleOfStick = 360.0/4096.0*(ahcd - ahcdinit);
    sprintf(buffers,"%.1f",angleOfStick);
    angleLCD->display(QString(buffers));

    ch =RXArray[5];        //PCA0CPH0
    PCA0CPH0 = ch;
    dutyPWM = (256 - PCA0CPH0)/256.0*100.0;
    qreal volt = 6.96*dutyPWM-3.47;      //
    sprintf(buffers,"%.2f",volt);
    pwmLCD->display(QString(buffers));

    qint16 tmp;
    ch = RXArray[6];       //setVel
    tmp = ch;
    ch = RXArray[7];
    tmp |= (ch << 8);
    setVel = tmp/100.0;
    sprintf(buffers,"%.2f",setVel);
    setVelLCD->display(QString(buffers));

    ch = RXArray[8];       //rtVel
    tmp = ch;
    ch = RXArray[9];
    tmp |= (ch << 8);
    rtVel = tmp/100.0;
    sprintf(buffers,"%.2f",rtVel);
    rtVelLCD->display(QString(buffers));

    ch = RXArray[10];       //rtLen
    tmp = ch;
    ch = RXArray[11];
    tmp |= (ch << 8);
    rtLength = tmp/10.0;
    sprintf(buffers,"%.1f",rtLength);
    rtLenLCD->display(QString(buffers));

    ch = RXArray[12];       //autoLen
    tmp = ch;
    ch = RXArray[13];
    tmp |= (ch << 8);
    autoLength = tmp/10;
    sprintf(buffers,"%.0f",autoLength);
    autoSetLenLCD->display(QString(buffers));

    status = RXArray[14];
    if((status&0x40)==0x40)   //显示下限
    {
        bottomLED->setOn(true);
    }
    else
    {
        bottomLED->setOn(false);
    }
    if((status&0x20)==0x20)   //接近指示灯
    {
        approachLED->setOn(true);
    }
    else
    {
        approachLED->setOn(false);
    }
    if((status&0x10)==0x10)   //上限
    {
        upLED->setOn(true);
    }
    else
    {
        upLED->setOn(false);
    }


    ch = RXArray[15];       //pressure
    tmp = ch;
    ch = RXArray[16];
    tmp |= (ch << 8);
    pressure = tmp/100.0;
    sprintf(buffers,"%.2f",pressure);
    pressureLCD->display(QString(buffers));


    ch = RXArray[17];       //reelingCtr
    tmp = ch;
    ch = RXArray[18];
    tmp |= (ch << 8);
    reelingCtr = tmp;
    sprintf(buffers,"%d",reelingCtr);
    reelingCtrLCD->display(QString(buffers));

    ch = RXArray[19];       //鼓轮速度,gulunVelocity
    tmp = ch;
    ch = RXArray[20];
    tmp |= (ch << 8);
    gulunVelocity = tmp/100.0;
    sprintf(buffers,"%.2f",(double)gulunVelocity);
    gulunVelocityLCD->display(QString(buffers));



    fout << qSetFieldWidth(10) << ahcdinit << ahcd << angleOfStick << PCA0CPH0 << dutyPWM << volt << setVel
                               << rtVel  << rtLength << autoLength << status << pressure << reelingCtr << endl;
}



