
#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QWidget>
#include <QPainter>
#include <string>
#include "ospray/ospray.h"
#include "Camera.h"
#include "Volume.h"
#include "GL/glew.h"

#define WIDTH 512
#define HEIGHT 512

class RenderWidget : public QWidget {
Q_OBJECT
public:
    RenderWidget() {
        renderer = ospNewRenderer("scivis");
        world = ospNewModel();

        filename = "assets/volume/engine.vifo";
        volume.loadFromVifoFile(filename);
    }

    virtual ~RenderWidget() {
        ospRelease(renderer);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        time_t start, end;
        QPainter painter(this);
        QRect rect(0, 0, WIDTH, HEIGHT);


        ospRelease(world);
        world = ospNewModel();
        ospAddVolume(world, volume.volume);
        ospCommit(world);

        //设置Renderer
        ospSetObject(renderer, "model", world);
        ospSetObject(renderer, "camera", camera.getCamera());
        ospCommit(renderer);

        osp::vec2i image_size = {WIDTH, HEIGHT};
        OSPFrameBuffer framebuffer = ospNewFrameBuffer(image_size, OSP_FB_SRGBA, OSP_FB_COLOR);
        ospCommit(framebuffer);

        ospFrameBufferClear(framebuffer, OSP_FB_COLOR);

        start = time(NULL);
        std::cout << "Rendering start" << std::endl;
        ospRenderFrame(framebuffer, renderer, OSP_FB_COLOR);
        end = time(NULL);
        std::cout << "Rendering end" << std::endl;
        std::cout << "Time cost: " << (end - start) << "s" << std::endl;

        QImage image(image_size.x, image_size.y, QImage::Format_RGBA8888);
        uchar *fdata = (uchar *) ospMapFrameBuffer(framebuffer, OSP_FB_COLOR);
        int pos, r, g, b, a;
        for (int i = 0; i < image_size.x; i++) {
            for (int j = 0; j < image_size.y; j++) {
                pos = (j * image_size.x + i) * 4;
                r = fdata[pos + 0];
                g = fdata[pos + 1];
                b = fdata[pos + 2];
                a = fdata[pos + 3];
                image.setPixel(i, j, qRgba(r, g, b, a));
            }
        }
        painter.drawImage(rect, image);
    }

private:
    MyCamera camera;
    MyVolume volume;
    OSPRenderer renderer = NULL;
    OSPModel world = NULL;
    std::string filename;
};


#endif //RENDERWIDGET_H
