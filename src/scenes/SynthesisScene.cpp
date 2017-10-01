#include "SynthesisScene.hpp"
#include <iostream>

#define LOG(x) std::cout << x << std::endl

SynthesisScene::SynthesisScene(QOpenGLShaderProgram *program, Pipeline *pipeline) {
  this->m_program = program;
  this->m_pipeline = pipeline;
//  grid = new Grid(3 * 44, m_program);
  m_camera.setPosition(glm::vec3(0, 0, 2));
}

SynthesisScene::~SynthesisScene() {}

void SynthesisScene::render() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  m_program->setUniformValue("camera", QMatrix4x4(glm::value_ptr(m_camera.matrix())).transposed());

  if (m_pipeline->result3D->isSet) {
//    grid->render();
    m_pipeline->result3D->render();
  }
}

void SynthesisScene::update() {
  // Camera Transformation
  if (Input::keyPressed(Qt::Key_Shift)) {
    m_camera.offsetOrientation(mouseSensitivity * Input::mouseDelta().y(), mouseSensitivity * (float)Input::mouseDelta().x());
    QApplication::setOverrideCursor(QCursor(Qt::BlankCursor));
  }
  if (Input::keyReleased(Qt::Key_Shift)) {
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
  }
  if (Input::keyPressed(Qt::Key_W)) {
    m_camera.offsetPosition(moveSpeed * m_camera.forward());
  }
  if (Input::keyPressed(Qt::Key_S)) {
    m_camera.offsetPosition(moveSpeed * -m_camera.forward());
  }
  if (Input::keyPressed(Qt::Key_A)) {
    m_camera.offsetPosition(moveSpeed * -m_camera.right());
  }
  if (Input::keyPressed(Qt::Key_D)) {
    m_camera.offsetPosition(moveSpeed * m_camera.right());
  }
  if (Input::keyPressed(Qt::Key_Z)) {
    m_camera.offsetPosition(moveSpeed * -glm::vec3(0, 1, 0));
  }
  if (Input::keyPressed(Qt::Key_X)) {
    m_camera.offsetPosition(moveSpeed * glm::vec3(0, 1, 0));
  }
}

void SynthesisScene::cleanUp() {}

void SynthesisScene::setViewportAspectRatio(float aspectRatio) { m_camera.setViewportAspectRatio(aspectRatio); }
