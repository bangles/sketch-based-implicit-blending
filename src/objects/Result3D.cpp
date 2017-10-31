#include "Result3D.h"
#include "VolumeGenerator.h"
#include <iostream>

#define LOG(x) std::cout << x << std::endl

Result3D::Result3D(int in_res, QOpenGLShaderProgram *program) : index_vbo(QOpenGLBuffer::IndexBuffer) {
  m_program = program;
  isSet = false;
  res = in_res;
  initializeBuffers();
}

void Result3D::setDistanceField(Tensor<float, 3> &df) {
  polygonize(df, 0.5);
  bindResult();
  isSet = true;
}

void Result3D::polygonize(Tensor<float, 3> &df, float isoValue) {
  MatrixXf GV(res * res * res, 3);
  VectorXf resMap = VectorXf::LinSpaced(res, MIN[0], MAX[0]);
  VectorXf S(df.size());

  for (int z = 0; z < res; z++) {
    for (int y = 0; y < res; y++) {
      for (int x = 0; x < res; x++) {
        GV.row(x + res * (y + res * z)) = RowVector3f(resMap(x), resMap(y), resMap(z));
        S[x + res * (y + res * z)] = df(x, y, z) - isoValue;
      }
    }
  }

  igl::copyleft::marching_cubes(S, GV, res, res, res, vertices, faces);
  igl::per_vertex_normals(vertices, faces, normals);

  normals.transposeInPlace();
  vertices.transposeInPlace();
  faces.transposeInPlace();
}

void Result3D::initializeBuffers() {
  vertex_vbo.create();
  index_vbo.create();
  normal_vbo.create();
  m_vao.create();
}

void Result3D::resetBuffers() {
  m_vao.release();
  vertex_vbo.allocate(0);
  index_vbo.allocate(0);
  normal_vbo.allocate(0);
}

void Result3D::bindResult() {
  m_vao.bind();

  vertex_vbo.bind();
  vertex_vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
  vertex_vbo.allocate(vertices.data(), vertices.size() * sizeof(GL_FLOAT));
  m_program->enableAttributeArray(0);
  m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3);
  vertex_vbo.release();

  normal_vbo.bind();
  normal_vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
  normal_vbo.allocate(normals.data(), normals.size() * sizeof(GL_FLOAT));
  m_program->enableAttributeArray(1);
  m_program->setAttributeBuffer(1, GL_FLOAT, 0, 3);
  normal_vbo.release();

  m_vao.release();

  index_vbo.bind();
  index_vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
  index_vbo.allocate(faces.data(), faces.size() * sizeof(GLuint));
  index_vbo.release();
}

void Result3D::render() {
  //    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, faceColor);
  m_vao.bind();
  m_program->setUniformValue("color", QVector3D(0.3f, 0.5f, 0.9f));
  index_vbo.bind();
  glDrawElements(GL_TRIANGLES, faces.size() * 6, GL_UNSIGNED_INT, 0);
}
