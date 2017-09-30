#include "Result.h"
#include <iostream>

#define LOG(x) std::cout << x << std::endl

Result::Result(MatrixXf &X, MatrixXf &Y, QOpenGLShaderProgram *program) {
  m_program = program;
  m_X = X;
  m_Y = Y;
  isSet = false;
  initializeBuffers();
}

void Result::setDistanceField(MatrixXf &df) {
  distanceField = df;
  polygonize(0.5);
  bindResult();
  isSet = true;
}

void Result::polygonize(float isoValue) { marchingSquares(m_X, m_Y, distanceField, isoValue, line_segments, triangles); }

void Result::initializeBuffers() {
  m_vbo.create();
  m_vao.create();
}

void Result::bindResult() {
  m_vbo.bind();
  m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
  m_vbo.allocate(triangles.data(), triangles.size() * 4 * sizeof(GL_DOUBLE));

  m_vao.bind();
  m_program->enableAttributeArray(0);
  m_program->setAttributeBuffer(0, GL_DOUBLE, 0, 2);
  m_vbo.release();
  m_vao.release();
}

void Result::render() {
  m_vao.bind();
  m_program->setUniformValue("color", QVector4D(0.0f, 1.0f, 0.5f, 1.0f));
  glDrawArrays(GL_TRIANGLES, 0, triangles.size());
}
