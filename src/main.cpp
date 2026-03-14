#include "app/MainWindow.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("VirtualDjSetPro");
    QApplication::setOrganizationName("FIWAtec");

    QSurfaceFormat::setDefaultFormat(QSurfaceFormat::defaultFormat());

    MainWindow window;
    window.show();

    return app.exec();
}
