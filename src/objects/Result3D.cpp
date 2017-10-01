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
  vertices.transposeInPlace();
  faces.transposeInPlace();
}

void Result3D::initializeBuffers() {
  m_vbo.create();
  index_vbo.create();
  m_vao.create();
}

void Result3D::bindResult() {
  m_vbo.bind();
  m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
  m_vbo.allocate(vertices.data(), vertices.size() * sizeof(GL_FLOAT));

  m_vao.bind();
  m_program->enableAttributeArray(0);
  m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3);
  m_vbo.release();
  m_vao.release();

  index_vbo.bind();
  index_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
  index_vbo.allocate(faces.data(), faces.size() * sizeof(GLuint));
  index_vbo.release();
}

GLfloat faceColor[4];
void Result3D::render() {
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, faceColor);
  m_vao.bind();
  m_program->setUniformValue("color", QVector4D(0.0f, 1.0f, 0.5f, 0.7f));
  index_vbo.bind();
  glDrawElements(GL_TRIANGLES, faces.size() * 6, GL_UNSIGNED_INT, 0);
}
