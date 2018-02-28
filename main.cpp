//#include <QtGui/QApplication> // <-- Qt4
#include <QtWidgets/QApplication> // <-- Qt5
#include "udpdlg.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UDPdlg w;
    w.show();
    return a.exec();
}
