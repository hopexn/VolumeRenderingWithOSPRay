#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QVBoxLayout>
#include "RenderWidget.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow() {
        setFixedWidth(1024);
        setFixedHeight(768);
        render = new RenderWidget;
        createActions();
        createMenus();
        layout()->addWidget(render);
    }

    virtual ~MainWindow() {
        delete render;
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
        defaultView = new QAction(tr("Default View"), this);
        connect(defaultView, SIGNAL(triggered()), this, SLOT(default_view()));

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
        viewMenu->addAction(defaultView);
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
    QString currVolumeFile;
    QString currTF1DFile;
    //菜单项
    QMenu *fileMenu, *shaderMenu, *viewMenu, *helpMenu;
    //菜单子项
    QAction *openAct, *exitAct;
    QAction *TF1DAct, *TF1DShadingAct;
    QAction *defaultView, *posXView, *negXView, *posYView, *negYView, *posZView, *negZView;
    QAction *about;
private slots:

    void open() {
        currVolumeFile = QFileDialog::getOpenFileName(this, tr("Open a Volume Data"), currVolumeFile,
                                                      tr("Volume Data (*.vifo)"));
        render->openVolumeFile(currVolumeFile.toStdString());
    };

    void tf1d() {
        currTF1DFile = QFileDialog::getOpenFileName(this, tr("Open a TF1D Data"), currVolumeFile,
                                                    tr("TF1D Data (*.TF1D)"));
        render->loadTF1D(currVolumeFile.toStdString());
    }

    void tf1d_shading() {

    }

    void default_view() {
        pos_z_view();
    }

    void pos_x_view() {
        render->setCameraPos(Vector3f(1.0f, 0, 0), Vector3f(-1.0f, 0, 0), Vector3f(0, 1.0f, 0));
        render->setup();
        render->repaint();
    }

    void pos_y_view() {
        render->setCameraPos(Vector3f(0, 1.0f, 0), Vector3f(0, -1.0f, 0), Vector3f(1.0f, 0, 0));
        render->setup();
        render->repaint();
    }

    void pos_z_view() {
        render->setCameraPos(Vector3f(0, 0, 1.0f), Vector3f(0, 0, -1.0f), Vector3f(0, 1.0f, 0));
        render->setup();
        render->repaint();
    }

    void neg_x_view() {
        render->setCameraPos(Vector3f(-1.0f, 0, 0), Vector3f(1.0f, 0, 0), Vector3f(0, 1.0f, 0));
        render->setup();
        render->repaint();
    }

    void neg_y_view() {
        render->setCameraPos(Vector3f(0, -1.0f, 0), Vector3f(0, 1.0f, 0), Vector3f(1.0f, 0, 0));
        render->setup();
        render->repaint();
    }

    void neg_z_view() {
        render->setCameraPos(Vector3f(0, 0, -1.0f), Vector3f(0, 0, 1.0f), Vector3f(0, 1.0f, 0));
        render->setup();
        render->repaint();
    }

    void about_us() {
        render->setCameraPos(Vector3f(1.0f, 0, 0), Vector3f(-1.0f, 0, 0), Vector3f(0, 1.0f, 0));
        render->setup();
        render->repaint();
    }
};


#endif // MAINWINDOW_H
