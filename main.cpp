#include <iostream>
#include <QApplication>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include "ospray/ospray.h"
#include "Volume.h"
#include "Camera.h"

using namespace std;
using namespace osp;

int main(int argc, const char **argv) {
    ospInit(&argc, argv);

    MyVolume volume;
    volume.loadFromVifoFile("assets/volume/engine.vifo");

    OSPModel world = ospNewModel();
    ospAddVolume(world, volume.getVolume());
    ospCommit(world);

    MyCamera camera;

    //设置Renderer
    OSPRenderer renderer = ospNewRenderer("scivis");
    ospSetObject(renderer, "model", world);
    ospSetObject(renderer, "camera", camera.getCamera());


    OSPLight light = ospNewLight(renderer, "ambient");
    ospCommit(light);
    OSPData lights = ospNewData(1, OSP_LIGHT, &light, 0);
    ospCommit(lights);
    ospSetData(renderer, "lights", lights);
    ospCommit(renderer);

    vec2i image_size= {512, 512};

    OSPFrameBuffer framebuffer = ospNewFrameBuffer(image_size, OSP_FB_SRGBA, OSP_FB_COLOR | OSP_FB_ACCUM);
    ospCommit(framebuffer);

    ospFrameBufferClear(framebuffer, OSP_FB_COLOR | OSP_FB_ACCUM);
    ospRenderFrame(framebuffer, renderer, OSP_FB_COLOR | OSP_FB_ACCUM);

    uint32_t *fdata = (uint32_t *) ospMapFrameBuffer(framebuffer, OSP_FB_COLOR);

    QApplication app(argc, (char **) argv);
    QLabel label;

    QImage image(image_size.x, image_size.y, QImage::Format_RGB32);
    uchar *ufdatq = (uchar *) fdata;
    for (int i = 0; i < image_size.x; i++) {
        for (int j = 0; j < image_size.y; j++) {
            int pos, r, g, b;
            pos = (j * image_size.x + i) * 4;
            r = ufdatq[pos + 0];
            g = ufdatq[pos + 1];
            b = ufdatq[pos + 2];
            image.setPixel(i, j, qRgb(r, g, b));
        }
    }
    label.setPixmap(QPixmap::fromImage(image));
    label.show();

    return app.exec();
}