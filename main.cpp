#include <iostream>
#include <QApplication>
#include "ospray/ospray.h"
#include "RenderWidget.h"

int main(int argc, const char **argv) {
    ospInit(&argc, argv);
    QApplication app(argc, (char **) argv);

    RenderWidget render;

    render.show();
    return app.exec();
}