
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
#include "util/Vector.h"

class RenderWidget : public QWidget {
Q_OBJECT
public:
    RenderWidget() {
        setFixedHeight(RENDER_WIDGET_HEIGHT);
        setFixedWidth(RENDER_WIDGET_WIDTH);
        renderer = ospNewRenderer("scivis");
        world = ospNewModel();

        filename = "assets/volume/engine.vifo";
        loadVolume(filename);
    }

    virtual ~RenderWidget() {
        ospRelease(renderer);
    }

protected:
    void loadVolume(std::string filename) {
        volume.loadFromVifoFile(filename);

        ospRelease(world);
        world = ospNewModel();
        ospAddVolume(world, volume.volume);
        ospCommit(world);

        //设置Renderer
        ospSetObject(renderer, "model", world);
        ospSetObject(renderer, "camera", camera.camera);
        ospCommit(renderer);

        ospRelease(framebuffer);
        framebuffer = ospNewFrameBuffer(osp::vec2i{this->width(), this->height()}, OSP_FB_SRGBA,
                                        OSP_FB_COLOR | OSP_FB_ACCUM);
        ospCommit(framebuffer);
    }

    void paintEvent(QPaintEvent *event) override {
        time_t start, end;
        if(update_flag){
            ospFrameBufferClear(framebuffer, OSP_FB_COLOR | OSP_FB_ACCUM);
            clearUpdateFlag();
        }
        start = time(NULL);
        ospRenderFrame(framebuffer, renderer, OSP_FB_COLOR | OSP_FB_ACCUM);
        end = time(NULL);
        std::cout << "Rendering a frame costs: " << (end - start) << "s" << std::endl;

        QImage image(this->width(), this->height(), QImage::Format_RGBA8888);
        uchar *fdata = (uchar *) ospMapFrameBuffer(framebuffer, OSP_FB_COLOR);
        int pos, r, g, b, a;
        for (int i = 0; i < image.width(); i++) {
            for (int j = 0; j < image.height(); j++) {
                pos = (j * image.height() + i) * 4;
                r = fdata[pos + 0];
                g = fdata[pos + 1];
                b = fdata[pos + 2];
                a = fdata[pos + 3];
                image.setPixel(i, j, qRgba(r, g, b, a));
            }
        }
        QPainter painter(this);
        QRect rect(0, 0, RENDER_WIDGET_WIDTH, RENDER_WIDGET_HEIGHT);
        painter.drawImage(rect, image);
    }

    //鼠标移动事件，左键控制旋转， 右键控制移动
    void mouseMoveEvent(QMouseEvent *event) override {
        float dx = float(event->x() - last_pos.x()) / RENDER_WIDGET_WIDTH;
        float dy = float(event->y() - last_pos.y()) / RENDER_WIDGET_HEIGHT;
        if (event->buttons() & Qt::RightButton) {
            camera.translate(-dx, -dy);
            setUpdateFlag();
        } else if (event->buttons() & Qt::LeftButton) {
            camera.rotate(dx, dy);
            setUpdateFlag();
        }
        repaint();
        last_pos = event->pos();
    }

    //滚轮事件， 控制放缩
    void wheelEvent(QWheelEvent *event) override {
        if (event->delta() > 0) {
            camera.scale(CAMERA_SCALE_RATE);
        } else {
            camera.scale(1 / CAMERA_SCALE_RATE);
        }
        setUpdateFlag();
        repaint();
    }

    void mousePressEvent(QMouseEvent *event) override {
        last_pos = event->pos();
    }

    bool getUpdateFlag() {
        return update_flag;
    }

    void setUpdateFlag() {
        update_flag = true;
    }

    void clearUpdateFlag() {
        update_flag = false;
    }

private:
    MyCamera camera;
    MyVolume volume;
    OSPRenderer renderer = NULL;
    OSPModel world = NULL;
    OSPFrameBuffer framebuffer = NULL;
    std::string filename;
    QPoint last_pos;
    bool update_flag = false;
};

#endif //RENDERWIDGET_H
