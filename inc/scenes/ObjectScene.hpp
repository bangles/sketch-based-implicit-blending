#ifndef ObjectScene_hpp
#define ObjectScene_hpp

#include <Eigen/Dense>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <Pipeline.hpp>
#include <circle.h>
#include <math.h>

using namespace Eigen;

class ObjectScene {
private:
  QOpenGLShaderProgram *m_program;
  Pipeline *m_pipeline;
  MatrixXf boundary;
  QOpenGLBuffer m_vbo[4];
  QOpenGLVertexArrayObject m_vao[4];

public:
  MatrixXf samples;

  ObjectScene(QOpenGLShaderProgram *program, Pipeline *pipeline);
  ~ObjectScene();
  void render();
  void start();
  void update();
  void cleanUp();
  void initializeBuffers();
  void updateBuffers();
  void initializeSamples();
  void initializeBoundary();
  void bindObject(int i, Circle *circle);
  MatrixXf interpolatePoints(Vector2f p1, Vector2f p2, int samples);
  MatrixXf interpolateArc(Vector2f centre, float radius, float startAngle, float endAngle, int samples);
};

#endif /* Scene_hpp */
