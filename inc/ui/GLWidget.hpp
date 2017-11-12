#ifndef GLWidget_H
#define GLWidget_H

#include "ObjectScene.hpp"
#include "SynthesisScene.hpp"
#include "TemplateScene.hpp"
#include "input.hpp"
#include "Utils.hpp"
#include <GL.h>
#include <QApplication>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QWidget>
#include <glu.h>

static const int STATE_TEMPLATE = 0;
static const int STATE_OBJECTS = 1;
static const int STATE_SYNTHESIS = 2;

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
public:
  int state = 0;
  Pipeline *m_pipeline;
  ObjectScene *objectScene;
  TemplateScene *templateScene;
  SynthesisScene *synthesisScene;
  QOpenGLShaderProgram m_program;
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
  void mouseMoveEvent(QMouseEvent *event);

private:
  void printContextInformation();
};

#endif // GLWidget_H
