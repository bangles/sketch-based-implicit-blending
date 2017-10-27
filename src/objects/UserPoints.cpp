//  UserPoints.cpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 13/07/17.

#include "UserPoints.hpp"

using namespace Eigen;

UserPoints::UserPoints(QOpenGLShaderProgram *program) {
  this->m_program = program;
//  m_userPoints = MatrixXf(3, NUM_OF_USER_POINTS);

//  float RADIUS = 0.5f;

//  for (int i = 0; i < NUM_OF_USER_POINTS; i++) {
//    double angle_rad = 0.3 + 0.4 * PI * i / NUM_OF_USER_POINTS;
//    m_userPoints.col(i) = Vector3f(RADIUS * std::sin(angle_rad), RADIUS * std::cos(angle_rad), 0.5f);
//  }
  m_vbo.create();
  m_vao.create();
//  bindPoints();
}

void UserPoints::bindPoints() {
  m_program->bind();
  m_vbo.bind();
  m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
  m_vbo.allocate(m_userPoints.data(), m_userPoints.size() * sizeof(GLfloat));
  m_vao.bind();
  m_program->enableAttributeArray(0);
  m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3);
  m_program->release();
  m_vao.release();
  m_vbo.release();
}

void UserPoints::render() {
  m_vao.bind();
  m_program->setUniformValue("color", QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
  glDrawArrays(GL_POINTS, 0, m_userPoints.cols());
  m_vao.release();
}

void UserPoints::setUserPoints(MatrixXf userPoints) {
  m_userPoints = userPoints;
  bindPoints();
}
