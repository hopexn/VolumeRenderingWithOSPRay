
#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QWidget>
#include "Camera.h"
#include "Volume.h"

class RenderWidget : public QWidget {
Q_OBJECT
public:
protected:
    void paintEvent(QPaintEvent *event) {

    }

private:
    MyCamera camera;
    MyVolume volume;
};


#endif //RENDERWIDGET_H
