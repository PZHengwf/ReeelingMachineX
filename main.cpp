#include "widget.h"
#include <QApplication>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    Widget w;
    w.setGeometry(0,30,800,480);
    w.showMaximized();
   // w.show();
    
    return a.exec();
}
