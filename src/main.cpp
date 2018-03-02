#include <QDebug>

#include "pdfshrink.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    PdfShrink app(argc, argv);

    MainWindow mainWin;
    mainWin.show();

    return app.exec();
}

