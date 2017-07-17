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
    volume.setSamplingRate(100);
    volume.loadFromVifoFile(string("assets/volume/engine.vifo"));

    OSPModel world = ospNewModel();
    ospAddVolume(world, volume.getVolume());
    ospCommit(world);

    MyCamera camera;
    vec3f pos = {0, 0, 2};
//    vec3f dir = {0, -1, 0};
//    vec3f up = {0, 0, 1};
    camera.setPos(pos);
//    camera.setUp(up);
//    camera.setDir(dir);
    camera.setup();

    //设置Renderer
    OSPRenderer renderer = ospNewRenderer("scivis");
    if (renderer == NULL) {
        cout << "Undefined renderer!" << endl;
        return 1;
    }
    ospSetObject(renderer, "model", world);
    ospSetObject(renderer, "camera", camera.getCamera());
    ospSetf(renderer, "aoTransparentEnabled", 1.0f);
    //! lighting
    OSPLight ambient_light = ospNewLight(renderer, "AmbientLight");
    ospSet1f(ambient_light, "intensity", 1.0f);
    ospCommit(ambient_light);
    OSPLight directional_light = ospNewLight(renderer, "DirectionalLight");
    ospSet1f(directional_light, "intensity", 2.f);
    ospSetVec3f(directional_light, "position", osp::vec3f{0, 0, 2});
    ospCommit(directional_light);
    std::vector<OSPLight> light_list{ambient_light, directional_light};
    OSPData lights =
            ospNewData(light_list.size(), OSP_OBJECT, light_list.data());
    ospCommit(lights);
    ospSetData(renderer, "lights", lights);
    ospCommit(renderer);

    vec2i image_size = {512, 512};

    OSPFrameBuffer framebuffer = ospNewFrameBuffer(image_size, OSP_FB_SRGBA,
                                                   OSP_FB_COLOR | OSP_FB_ACCUM | OSP_FB_VARIANCE);
    ospCommit(framebuffer);

    ospFrameBufferClear(framebuffer, OSP_FB_COLOR | OSP_FB_ACCUM | OSP_FB_VARIANCE);
    ospRenderFrame(framebuffer, renderer, OSP_FB_COLOR | OSP_FB_ACCUM | OSP_FB_VARIANCE);

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