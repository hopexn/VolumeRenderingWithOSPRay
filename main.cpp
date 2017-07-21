#include <iostream>
#include <QApplication>
#include <QImage>
#include <QPixmap>
#include <QLabel>

#include "ospray/ospray.h"
#include "Volume.h"
#include "Camera.h"
#include <ctime>

using namespace std;
using namespace osp;

int main(int argc, const char **argv) {
    ospInit(&argc, argv);

    time_t start, end;

    MyVolume volume;
    volume.loadFromVifoFile(string("assets/volume/engine.vifo"));

    OSPModel world = ospNewModel();
    ospAddVolume(world, volume.getVolume());
    ospCommit(world);

    MyCamera camera;
    vec3f pos = {0, 0, 1.7};
    camera.setPos(pos);
    camera.setup();

    //设置Renderer
    OSPRenderer renderer = ospNewRenderer("scivis");
    if (renderer == NULL) {
        cout << "Undefined renderer!" << endl;
        return 1;
    }
    ospSetObject(renderer, "model", world);
    ospSetObject(renderer, "camera", camera.getCamera());

    ospCommit(renderer);

    vec2i image_size = {512, 512};

    OSPFrameBuffer framebuffer = ospNewFrameBuffer(image_size, OSP_FB_SRGBA, OSP_FB_COLOR);
    ospCommit(framebuffer);

    ospFrameBufferClear(framebuffer, OSP_FB_COLOR);

    start = time(NULL);
    cout << "Rendering start" << endl;
    ospRenderFrame(framebuffer, renderer, OSP_FB_COLOR);
    end = time(NULL);
    cout << "Rendering end" << endl;
    cout << "Time cost: " << (end - start) << endl;
    uint32_t *fdata = (uint32_t *) ospMapFrameBuffer(framebuffer, OSP_FB_COLOR);

    QApplication app(argc, (char **) argv);
    QImage image(image_size.x, image_size.y, QImage::Format_RGBA8888);
    QLabel label;
    {
        int pos, r, g, b;
        uchar *ufdatq = (uchar *) fdata;
        for (int i = 0; i < image_size.x; i++) {
            for (int j = 0; j < image_size.y; j++) {
                pos = (j * image_size.x + i) * 4;
                r = ufdatq[pos + 0];
                g = ufdatq[pos + 1];
                b = ufdatq[pos + 2];
                image.setPixel(i, j, qRgb(r, g, b));
            }
        }
    }
    label.setPixmap(QPixmap::fromImage(image));
    label.show();

    return app.exec();
}