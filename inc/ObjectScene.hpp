#ifndef ObjectScene_hpp
#define ObjectScene_hpp

#include <Eigen/Dense>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <circle.h>
#include <math.h>

using namespace Eigen;

class ObjectScene {
private:
  bool isBlended;

  QOpenGLShaderProgram *m_program;
  Circle *circle1, *circle2;
  MatrixXf boundary;
  MatrixXf samples;
  //    QOpenGLBuffer index_vbo[2];
  QOpenGLBuffer m_vbo[4];
  QOpenGLVertexArrayObject m_vao[4];

public:
  ObjectScene(QOpenGLShaderProgram *program);
  ~ObjectScene();
  void render();
  void blend();
  void start();
  void update();
  void cleanUp();
  void initializeBuffers();
  void updateBuffers();
  void initializeSamples();
  void convertSamples();
  void initializeBoundary();
  void bindObject(int i, Circle *circle);
  MatrixXf interpolatePoints(Vector2f p1, Vector2f p2, int samples);
  MatrixXf interpolateArc(Vector2f centre, float radius, float startAngle, float endAngle, int samples);
};

#endif /* Scene_hpp */
