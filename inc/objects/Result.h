#ifndef Result_H
#define Result_H

#include <Eigen/Dense>
#include <MarchingSquares.h>
#include <igl/copyleft/marching_cubes.h>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

using namespace Eigen;

class Result {

private:
  MatrixXf m_X;
  MatrixXf m_Y;
  QOpenGLBuffer m_vbo;
  QOpenGLVertexArrayObject m_vao;
  QOpenGLShaderProgram* m_program;

public:
  MatrixXf distanceField;
  MatrixXf gradient[2];
  bool isSet;

  std::vector<Vector2d> line_segments, triangles;

  Result(MatrixXf &X, MatrixXf &Y, QOpenGLShaderProgram *program);
  void setDistanceField(MatrixXf &df);
  void polygonize(float isoValue);
  void initializeBuffers();
  void bindResult();
  void render();
};

#endif // Result_H