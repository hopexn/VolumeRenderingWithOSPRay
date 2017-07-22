#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>
#include "RenderWidget.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow() {
        setFixedWidth(1024);
        setFixedHeight(768);
        centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        centralLayout = new QHBoxLayout(centralWidget);

        render = new RenderWidget;
        createActions();
        createMenus();
        rightLayout = new QGridLayout;
        rightLayout->setAlignment(Qt::AlignTop);
        samplingRateLabel = new QLabel(tr("Sampling rate:"));
        samplingRateLabel->setFixedWidth(100);
        samplingRateLabel->setFixedHeight(20);
        samplingRateSpinBox = new QDoubleSpinBox;
        samplingRateSlider = new QSlider(Qt::Horizontal);
        samplingRateSpinBox->setRange(0, RENDER_MAX_SAMPLING_RATE);
        samplingRateSlider->setRange(0, (int) (RENDER_MAX_SAMPLING_RATE * 100));
        samplingRateSpinBox->setValue(VOLUME_SAMPLINGRATE_INIT);
        samplingRateSlider->setValue((int) (VOLUME_SAMPLINGRATE_INIT * 100));
        connect(samplingRateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(samplingRateChanged(double)));
        connect(samplingRateSlider, SIGNAL(valueChanged(int)), this, SLOT(samplingRateChanged(int)));

        clippingLabel = new QLabel("Near clipping:");
        clippingSpinBox = new QDoubleSpinBox;
        clippingSlider = new QSlider(Qt::Horizontal);
        clippingSpinBox->setRange(0, RENDER_MAX_NEAR_CLIPPING);
        clippingSlider->setRange(0, (int) (RENDER_MAX_NEAR_CLIPPING * 100));
        clippingSpinBox->setValue(RENDER_CLIPPING_INIT);
        clippingSlider->setValue((int) (RENDER_CLIPPING_INIT * 100));
        connect(clippingSpinBox, SIGNAL(valueChanged(double)), this, SLOT(nearClipChanged(double)));
        connect(clippingSlider, SIGNAL(valueChanged(int)), this, SLOT(nearClipChanged(int)));

        specularLabel = new QLabel("Specular rate:");
        //RED
        specularSpinBoxRed = new QDoubleSpinBox;
        specularSliderRed = new QSlider(Qt::Horizontal);
        specularSpinBoxRed->setRange(0, RENDER_MAX_SPECULAR_RATE);
        specularSliderRed->setRange(0, (int) (RENDER_MAX_SPECULAR_RATE * 100));
        specularSpinBoxRed->setValue(RENDER_SPECULAR_RATE_INIT);
        specularSliderRed->setValue((int) (RENDER_SPECULAR_RATE_INIT * 100));
        connect(specularSpinBoxRed, SIGNAL(valueChanged(double)),
                this, SLOT(specularRateRedChanged(double)));
        connect(specularSliderRed, SIGNAL(valueChanged(int)),
                this, SLOT(specularRateRedChanged(int)));
        //GREEN
        specularSpinBoxGreen = new QDoubleSpinBox;
        specularSliderGreen = new QSlider(Qt::Horizontal);
        specularSpinBoxGreen->setRange(0, RENDER_MAX_SPECULAR_RATE);
        specularSliderGreen->setRange(0, (int) (RENDER_MAX_SPECULAR_RATE * 100));
        specularSpinBoxGreen->setValue(RENDER_SPECULAR_RATE_INIT);
        specularSliderGreen->setValue((int) (RENDER_SPECULAR_RATE_INIT * 100));
        connect(specularSpinBoxGreen, SIGNAL(valueChanged(double)),
                this, SLOT(specularRateGreenChanged(double)));
        connect(specularSliderGreen, SIGNAL(valueChanged(int)),
                this, SLOT(specularRateGreenChanged(int)));
        //BLUE
        specularSpinBoxBlue = new QDoubleSpinBox;
        specularSliderBlue = new QSlider(Qt::Horizontal);
        specularSpinBoxBlue->setRange(0, RENDER_MAX_SPECULAR_RATE);
        specularSliderBlue->setRange(0, (int) (RENDER_MAX_SPECULAR_RATE * 100));
        specularSpinBoxBlue->setValue(RENDER_SPECULAR_RATE_INIT);
        specularSliderBlue->setValue((int) (RENDER_SPECULAR_RATE_INIT * 100));
        connect(specularSpinBoxBlue, SIGNAL(valueChanged(double)),
                this, SLOT(specularRateBlueChanged(double)));
        connect(specularSliderBlue, SIGNAL(valueChanged(int)),
                this, SLOT(specularRateBlueChanged(int)));

        rightLayout->addWidget(samplingRateLabel, 0, 0, 1, 2);
        rightLayout->addWidget(samplingRateSpinBox, 1, 0, 1, 1);
        rightLayout->addWidget(samplingRateSlider, 1, 1, 1, 1);

        rightLayout->addWidget(clippingLabel, 3, 0, 1, 2);
        rightLayout->addWidget(clippingSpinBox, 4, 0, 1, 1);
        rightLayout->addWidget(clippingSlider, 4, 1, 1, 1);

        rightLayout->addWidget(specularLabel, 6, 0, 1, 2);
        rightLayout->addWidget(specularSpinBoxRed, 7, 0, 1, 1);
        rightLayout->addWidget(specularSliderRed, 7, 1, 1, 1);
        rightLayout->addWidget(specularSpinBoxGreen, 8, 0, 1, 1);
        rightLayout->addWidget(specularSliderGreen, 8, 1, 1, 1);
        rightLayout->addWidget(specularSpinBoxBlue, 9, 0, 1, 1);
        rightLayout->addWidget(specularSliderBlue, 9, 1, 1, 1);

        centralLayout->addWidget(render);
        centralLayout->addLayout(rightLayout, 10);
    }

    virtual ~MainWindow() {
        delete render;
        delete fileMenu;
        delete shaderMenu;
        delete viewMenu;
        delete helpMenu;
        delete openAct;
        delete exitAct;
        delete TF1DAct;
        delete TF1DShadingAct;
        delete posXView;
        delete negXView;
        delete posYView;
        delete negYView;
        delete posZView;
        delete negZView;
        delete about;

//        delete centralWidget;  //这两个已经被释放， 无需再次释放
//        delete centralLayout;
        delete rightLayout;

        delete samplingRateLabel;
        delete samplingRateSpinBox;
        delete samplingRateSlider;

        delete clippingSlider;
        delete clippingSpinBox;
        delete clippingLabel;
    }

    void createActions() {
        //File Menu
        openAct = new QAction(tr("&Open"), this);
        openAct->setShortcut(QKeySequence::Open);
        openAct->setStatusTip(tr("Open a volume file"));
        connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

        exitAct = new QAction(tr("Exit"), this);
        exitAct->setShortcut(QKeySequence::Quit);
        exitAct->setStatusTip(tr("Exit"));
        connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

        //Shader Menu
        TF1DAct = new QAction(tr("&TF1D Shader"), this);
        connect(TF1DAct, SIGNAL(triggered()), this, SLOT(tf1d()));

        TF1DShadingAct = new QAction(tr("&TF1D Shading Shader"), this);
        connect(TF1DShadingAct, SIGNAL(triggered()), this, SLOT(tf1d_shading()));


        //View Menu
        posXView = new QAction(tr("Positive X View"), this);
        connect(posXView, SIGNAL(triggered()), this, SLOT(pos_x_view()));

        posYView = new QAction(tr("Positive Y View"), this);
        connect(posYView, SIGNAL(triggered()), this, SLOT(pos_y_view()));

        posZView = new QAction(tr("Positive Z View"), this);
        connect(posZView, SIGNAL(triggered()), this, SLOT(pos_z_view()));

        negXView = new QAction(tr("Negative X View"), this);
        connect(negXView, SIGNAL(triggered()), this, SLOT(neg_x_view()));

        negYView = new QAction(tr("Negative Y View"), this);
        connect(negYView, SIGNAL(triggered()), this, SLOT(neg_y_view()));

        negZView = new QAction(tr("Negative Z View"), this);
        connect(negZView, SIGNAL(triggered()), this, SLOT(neg_z_view()));

        about = new QAction(tr("About us"));
        connect(about, SIGNAL(triggered()), this, SLOT(about_us()));
    }

    void createMenus() {
        fileMenu = menuBar()->addMenu(tr("&File"));
        fileMenu->addAction(openAct);
        fileMenu->addAction(exitAct);

        shaderMenu = menuBar()->addMenu(tr("&Shader"));
        shaderMenu->addAction(TF1DAct);
        shaderMenu->addAction(TF1DShadingAct);

        viewMenu = menuBar()->addMenu(tr("&View"));
        viewMenu->addAction(posXView);
        viewMenu->addAction(posYView);
        viewMenu->addAction(posZView);
        viewMenu->addAction(negXView);
        viewMenu->addAction(negYView);
        viewMenu->addAction(negZView);

        helpMenu = menuBar()->addMenu(tr("Help"));
        helpMenu->addAction(about);
    }

private:
    RenderWidget *render;

    QWidget *centralWidget;
    QHBoxLayout *centralLayout;
    QGridLayout *rightLayout;

    //设置采样率
    QLabel *samplingRateLabel;
    QDoubleSpinBox *samplingRateSpinBox;
    QSlider *samplingRateSlider;

    //设置截断距离
    QLabel *clippingLabel;
    QDoubleSpinBox *clippingSpinBox;
    QSlider *clippingSlider;


    //设置镜面反射率
    QLabel *specularLabel;
    QDoubleSpinBox *specularSpinBoxRed;
    QSlider *specularSliderRed;

    QDoubleSpinBox *specularSpinBoxGreen;
    QSlider *specularSliderGreen;

    QDoubleSpinBox *specularSpinBoxBlue;
    QSlider *specularSliderBlue;


    QString currVolumeFile;
    QString currTF1DFile;
    //菜单项
    QMenu *fileMenu, *shaderMenu, *viewMenu, *helpMenu;
    //菜单子项
    QAction *openAct, *exitAct;
    QAction *TF1DAct, *TF1DShadingAct;
    QAction *posXView, *negXView, *posYView, *negYView, *posZView, *negZView;
    QAction *about;
private slots:

    void open() {
        currVolumeFile = QFileDialog::getOpenFileName(this, tr("Open a Volume Data"), currVolumeFile,
                                                      tr("Volume Data (*.vifo)"));
        render->loadVolume(currVolumeFile.toStdString());
    };

    void tf1d() {
        currTF1DFile = QFileDialog::getOpenFileName(this, tr("Open a TF1D Data"), currVolumeFile,
                                                    tr("TF1D Data (*.TF1D)"));
        std::cout << "flag" << std::endl;
        render->loadTF1D(currTF1DFile.toStdString());
    }

    void tf1d_shading() {

    }

    void pos_x_view() {
        render->setCameraPos(Vector3f(1.0f, 0, 0), Vector3f(-1.0f, 0, 0), Vector3f(0, 1.0f, 0));
    }

    void pos_y_view() {
        render->setCameraPos(Vector3f(0, 1.0f, 0), Vector3f(0, -1.0f, 0), Vector3f(1.0f, 0, 0));
    }

    void pos_z_view() {
        render->setCameraPos(Vector3f(0, 0, 1.0f), Vector3f(0, 0, -1.0f), Vector3f(0, 1.0f, 0));
    }

    void neg_x_view() {
        render->setCameraPos(Vector3f(-1.0f, 0, 0), Vector3f(1.0f, 0, 0), Vector3f(0, 1.0f, 0));
    }

    void neg_y_view() {
        render->setCameraPos(Vector3f(0, -1.0f, 0), Vector3f(0, 1.0f, 0), Vector3f(1.0f, 0, 0));
    }

    void neg_z_view() {
        render->setCameraPos(Vector3f(0, 0, -1.0f), Vector3f(0, 0, 1.0f), Vector3f(0, 1.0f, 0));
    }

    void about_us() {
        render->setCameraPos(Vector3f(1.0f, 0, 0), Vector3f(-1.0f, 0, 0), Vector3f(0, 1.0f, 0));
    }

    void samplingRateChanged(int value) {
        samplingRateSpinBox->setValue((double) value / 100);
        render->setSamplingRate((float) value / 100);
    }

    void samplingRateChanged(double value) {
        samplingRateSlider->setValue((int) (value * 100));
        render->setSamplingRate((float) value);
    }

    void nearClipChanged(int value) {
        clippingSpinBox->setValue((double) value / 100);
        render->setNearClip((float) value / 100);
    }

    void nearClipChanged(double value) {
        clippingSlider->setValue((int) (value * 100));
        render->setNearClip((float) value);
    }

    void specularRateRedChanged(int value) {
        specularSpinBoxRed->setValue((double) value / 100);
        render->setSpecularRate((float) value / 100, SPECULAR_COLOR_RED);
    }

    void specularRateRedChanged(double value) {
        specularSliderRed->setValue((int) (value * 100));
        render->setSpecularRate((float) value, SPECULAR_COLOR_RED);
    }

    void specularRateGreenChanged(int value) {
        specularSpinBoxGreen->setValue((double) value / 100);
        render->setSpecularRate((float) value / 100, SPECULAR_COLOR_GREEN);
    }

    void specularRateGreenChanged(double value) {
        specularSliderGreen->setValue((int) (value * 100));
        render->setSpecularRate((float) value, SPECULAR_COLOR_GREEN);
    }

    void specularRateBlueChanged(int value) {
        specularSpinBoxBlue->setValue((double) value / 100);
        render->setSpecularRate((float) value / 100, SPECULAR_COLOR_BLUE);
    }

    void specularRateBlueChanged(double value) {
        specularSliderBlue->setValue((int) (value * 100));
        render->setSpecularRate((float) value, SPECULAR_COLOR_BLUE);
    }
};

#endif // MAINWINDOW_H
