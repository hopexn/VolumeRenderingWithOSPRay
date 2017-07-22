
#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
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
        ambient_light_intensity = LIGHT_AMBIENT_INTENSITY_INIT;
        distant_light_intensity = LIGHT_DISTANT_INTENSITY_INIT;


        filename = "assets/volume/engine.vifo";
        loadVolume(filename);
    }

    virtual ~RenderWidget() {
        ospRelease(renderer);
    }

    void loadVolume(std::string filename) {
        this->filename = filename;
        volume.loadFromVifoFile(filename);
        setup();
        repaint();
    }

    void loadTF1D(std::string filename) {
        volume.loadTF1D(filename);
        setup();
        repaint();
    }

    void setCameraPos(Vector3f pos, Vector3f dir, Vector3f up) {
        camera.setCameraPos(pos, dir, up);
        setup();
        repaint();
    }

    void setSamplingRate(float value) {
        volume.setSamplingRate(value);
        setup();
        repaint();
    }


    void setNearClip(float nearClip) {
        camera.setNearClip(nearClip);
        setup();
        repaint();
    }

    void setSpecularRate(float specularRate, int color) {
        volume.setSpecularRate(specularRate, color);
        setup();
        repaint();
    }

    void setAmbientLightIntensity(float value) {
        this->ambient_light_intensity = value;
        setup();
        repaint();
    }

    void setDistantLightIntensity(float value) {
        this->distant_light_intensity = value;
        setup();
        repaint();
    }

    void switchGradientShading() {
        volume.switchGradientShading();
        setup();
        repaint();
    }

protected:

    void paintEvent(QPaintEvent *event) override {
        if (update_flag) {
            ospFrameBufferClear(framebuffer, OSP_FB_COLOR | OSP_FB_ACCUM);
            clearUpdateFlag();
        }
        ospRenderFrame(framebuffer, renderer, OSP_FB_COLOR | OSP_FB_ACCUM);

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
    float ambient_light_intensity, distant_light_intensity;

    void lights_setup() {
        OSPLight ambient_light = ospNewLight(renderer, "ambient");
        ospSetf(ambient_light, "intensity", ambient_light_intensity);
        ospCommit(ambient_light);

        OSPLight distant_light = ospNewLight(renderer, "distant");
        ospSetf(distant_light, "intensity", distant_light_intensity);
        ospSetVec3f(distant_light, "direction", camera.getDir());
        ospCommit(distant_light);

        std::vector<OSPLight> lights;
        lights.push_back(ambient_light);
        lights.push_back(distant_light);

        OSPData lights_data = ospNewData(lights.size(), OSP_LIGHT, lights.data());
        ospCommit(lights_data);

        ospSetObject(renderer, "lights", lights_data);
    }

    void setup() {
        ospRelease(world);
        world = ospNewModel();
        ospAddVolume(world, volume.volume);
        ospCommit(world);

        lights_setup();

        //设置Renderer
        ospSetObject(renderer, "model", world);
        ospSetObject(renderer, "camera", camera.camera);
        ospCommit(renderer);

        ospRelease(framebuffer);
        framebuffer = ospNewFrameBuffer(osp::vec2i{this->width(), this->height()}, OSP_FB_SRGBA,
                                        OSP_FB_COLOR | OSP_FB_ACCUM);
        ospCommit(framebuffer);


    }
};

#endif //RENDERWIDGET_H
