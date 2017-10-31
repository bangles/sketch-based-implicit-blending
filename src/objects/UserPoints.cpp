//  UserPoints.cpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 13/07/17.

#include "UserPoints.hpp"
using namespace Eigen;

UserPoints::UserPoints(QOpenGLShaderProgram *program) {
  this->m_program = program;
  m_vbo.create();
  m_vao.create();
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

void UserPoints::clear() {
  m_userPoints.resize(0, 0);
  m_vbo.allocate(0);
}

void UserPoints::setUserPoints(MatrixXf userPoints) {
  m_userPoints = userPoints;
  bindPoints();
}
