#ifndef ObjectScene_hpp
#define ObjectScene_hpp

#include "input.h"
#include <Eigen/Dense>
#include <Pipeline.hpp>
#include <QApplication>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <circle.h>
#include <math.h>

using namespace Eigen;

static const int STATE_MOVE = 0;
static const int STATE_SKETCH = 1;
static const int STATE_MOVE_ACTIVATED = 2;

class ObjectScene {

private:
  QOpenGLShaderProgram *m_program;
  Pipeline *m_pipeline;
  MatrixXf boundary;
  QOpenGLBuffer m_vbo[4];
  QOpenGLVertexArrayObject m_vao[4];
  Circle *activatedCircle;
  //  bool scribbling;

public:
  //  MatrixXf samples;
  vector<Vector2f> samples;
  int state;

  ObjectScene(QOpenGLShaderProgram *program, Pipeline *pipeline);
  ~ObjectScene();
  void render();
  void start();
  void update();
  void cleanUp();
  void initializeBuffers();
  void updateBuffers();
  void initializeBoundary();
  void bindObject(int i, Circle *circle);
  void setState(int state);
  void addUserPoint(Vector2f point);
  void handleMouseDrag(QPoint point, float width, float height);
  void handleMouseClick(QPoint point, float width, float height);
  void handleMouseRelease();
  MatrixXf interpolatePoints(Vector2f p1, Vector2f p2, int samples);
  MatrixXf interpolateArc(Vector2f centre, float radius, float startAngle, float endAngle, int samples);
};

#endif /* Scene_hpp */
