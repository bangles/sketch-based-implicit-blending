#ifndef GLWidget_H
#define GLWidget_H

#include <QWidget>
#include <QOpenGLWidget>
#include <glu.h>
#include <GL.h>
#include <QMatrix4x4>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include "TemplateScene.hpp"
#include "ObjectScene.hpp"
#include "SynthesisScene.hpp"
#include<QApplication>
#include "input.h"

static const int STATE_TEMPLATE = 0;
static const int STATE_OBJECTS = 1;
static const int STATE_SYNTHESIS = 2;


class GLWidget : public QOpenGLWidget,
        protected QOpenGLFunctions
{
    Q_OBJECT
public:
    Pipeline *m_pipeline;
    ObjectScene *objectScene;
    TemplateScene *templateScene;
    SynthesisScene *synthesisScene;
    GLWidget(QWidget *parent = 0);
    ~GLWidget();
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void teardownGL();
    void startTemplateScene();
    void startObjectScene();
    void startSynthesisScene();

protected slots:
    void update();

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    int state = 0;
    QOpenGLShaderProgram m_program;
    void printContextInformation();
};

#endif // GLWidget_H
