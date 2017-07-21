#include <iostream>
#include <QApplication>
#include <QImage>
#include <QPixmap>
#include <QLabel>

#include "ospray/ospray.h"
#include "Volume.h"
#include "Camera.h"
#include "RenderWidget.h"

using namespace std;
using namespace osp;

int main(int argc, const char **argv) {
    ospInit(&argc, argv);
    QApplication app(argc, (char **) argv);

    RenderWidget render;

    render.show();

    return app.exec();
}