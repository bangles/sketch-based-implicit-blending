#include "TemplateScene.hpp"
#include <iostream>

#define LOG(x) std::cout << x << std::endl

TemplateScene::TemplateScene(QOpenGLShaderProgram *program, Pipeline *pipeline) {
  this->m_program = program;
  this->m_pipeline = pipeline;
  grid = new Grid(3 * 44, m_program);
  m_camera.setPosition(glm::vec3(0, 0.5, 3));
}

TemplateScene::~TemplateScene() {}

void TemplateScene::render() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  m_program->setUniformValue("camera", QMatrix4x4(glm::value_ptr(m_camera.matrix())).transposed());
  grid->render();
  m_pipeline->m_template->render();
  m_pipeline->userPoints->render();
  //    drawTestPoints(gTemplate->mPatch1.points);
}

// void TemplateScene::process(){
//    m_processor->process(userPoints->mUserPoints);
//    m_template->updatePatches();
//    m_processor->updateSearcher();
//}

// void TemplateScene::start(){
//    m_pipeline->start();
//}

void TemplateScene::update() {
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
//  if (Input::keyPressed(Qt::Key_P)) {
//    process();
//  }
//  if (Input::keyPressed(Qt::Key_I)) {
//    start();
//  }
}

void TemplateScene::cleanUp() {}

void TemplateScene::setViewportAspectRatio(float aspectRatio) { m_camera.setViewportAspectRatio(aspectRatio); }
