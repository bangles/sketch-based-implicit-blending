#ifndef Result3D_H
#define Result3D_H

#include <Eigen/Dense>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <eigen3/unsupported/Eigen/CXX11/Tensor>
#include <igl/copyleft/marching_cubes.h>
#include <igl/per_vertex_normals.h>

using namespace Eigen;

class Result3D {

private:
  QOpenGLBuffer vertex_vbo;
  QOpenGLBuffer normal_vbo;
  QOpenGLBuffer index_vbo;
  QOpenGLVertexArrayObject m_vao;
  QOpenGLShaderProgram *m_program;
  int res;

public:
  bool isSet;

  MatrixXf vertices;
  MatrixXf normals;
  MatrixXi faces;

  Result3D(int in_res, QOpenGLShaderProgram *program);
  void setDistanceField(Tensor<float, 3> &df);
  void polygonize(Tensor<float, 3> &df, float isoValue);
  void initializeBuffers();
  void bindResult();
  void render();
};

#endif // Result3D_H
