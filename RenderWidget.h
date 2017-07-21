
#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <string>
#include "ospray/ospray.h"
#include "Camera.h"
#include "Volume.h"
#include "OpenGL.h"

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
        ospRenderFrame(framebuffer, renderer, OSP_FB_COLOR);
        end = time(NULL);
        std::cout << "Rendering a frame costs: " << (end - start) << "s" << std::endl;

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

    void mouseMoveEvent(QMouseEvent *event) override {
        float dx = float(event->x() - last_pos.x()) / WIDTH;
        float dy = float(event->y() - last_pos.y()) / HEIGHT;

        if (event->buttons() & Qt::LeftButton) {
            rotation.x += 3.14 * dx;
            rotation.y += 3.14 * dy;
            camera.rotate(rotation);
            repaint();
        } else if (event->button() & Qt::RightButton) {
            rotation.x += 3.14 * dx;
            rotation.z += 3.14 * dy;
        }
        last_pos = event->pos();
    }
    void mousePressEvent(QMouseEvent *event) {
        last_pos = event->pos();
    }
private:
    MyCamera camera;
    MyVolume volume;
    OSPRenderer renderer = NULL;
    OSPModel world = NULL;
    std::string filename;
    QPoint last_pos;
    osp::vec3f rotation;
};


#endif //RENDERWIDGET_H
