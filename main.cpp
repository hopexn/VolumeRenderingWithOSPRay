#include <iostream>
#include <QApplication>
#include "ospray/ospray.h"
#include "RenderWidget.h"
#include "MainWindow.h"

int main(int argc, const char **argv) {
    ospInit(&argc, argv);
    QApplication app(argc, (char **) argv);

    MainWindow mainWindow;

    mainWindow.show();
    return app.exec();
}