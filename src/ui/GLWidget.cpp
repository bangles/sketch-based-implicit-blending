#include "GLWidget.hpp"
#include <QDebug>
#include <QKeyEvent>
#include <QString>

GLWidget::~GLWidget() {
  makeCurrent();
  teardownGL();
}

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) { setFocusPolicy(Qt::FocusPolicy::StrongFocus); }

/*******************************************************************************
 * OpenGL Events
 ******************************************************************************/

void GLWidget::initializeGL() {
  // Initialize OnGLFunctions();
  initializeOpenGLFunctions();
  connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
  printContextInformation();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//  glShadeModel(GL_SMOOTH);
//  glPointSize(3.0f);
  glPointSize(7.0f);

  m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/vs.glsl");
  m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/fs.glsl");
  m_program.link();

  m_pipeline = new Pipeline(&m_program);
  templateScene = new TemplateScene(&m_program, m_pipeline);
  synthesisScene = new SynthesisScene(&m_program, m_pipeline);
  objectScene = new ObjectScene(&m_program, m_pipeline);
}

void GLWidget::resizeGL(int width, int height) {
  templateScene->setViewportAspectRatio((float)width / height);
  synthesisScene->setViewportAspectRatio((float)width / height);
}

void GLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_program.bind();
  m_program.setUniformValue("model", QMatrix4x4());

  switch (state) {
  case STATE_TEMPLATE:
    templateScene->render();
    break;
  case STATE_OBJECTS:
    objectScene->render();
    break;
  case STATE_SYNTHESIS:
    synthesisScene->render();
    break;
  }

  GLenum err;
  if ((err = glGetError()) != GL_NO_ERROR) {
    qDebug() << "OpenGL error: " << err;
  }

  m_program.release();
}

void GLWidget::update() {
  // Update input
  Input::update();

  switch (state) {
  case STATE_TEMPLATE:
    templateScene->update();
    break;
  case STATE_OBJECTS:
    //        objectScene->update();
    break;
  case STATE_SYNTHESIS:
    synthesisScene->update();
    break;
  }
  // Schedule a redraw
  QOpenGLWidget::update();
}

void GLWidget::teardownGL() {
  // Currently we have no data to teardown
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
  if (event->isAutoRepeat()) {
    event->ignore();
  } else {
    Input::registerKeyPress(event->key());
  }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event) {
  if (event->isAutoRepeat()) {
    event->ignore();
  } else {
    Input::registerKeyRelease(event->key());
  }
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    switch (state) {
    case STATE_OBJECTS:
      objectScene->handleMouseClick(event->pos(), this->width(), this->height());
      break;
    }
  }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() & Qt::LeftButton) {
    switch (state) {
    case STATE_OBJECTS:
      objectScene->handleMouseDrag(event->pos(), this->width(), this->height());
      break;
    }
  }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
  switch (state) {
  case STATE_OBJECTS:
    objectScene->handleMouseRelease();
    break;
  }
}

void GLWidget::printContextInformation() {
  QString glType;
  QString glVersion;
  QString glProfile;

  // Get Version Information
  glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
  glVersion = reinterpret_cast<const char *>(glGetString(GL_VERSION));

// Get Profile Information

#define CASE(c)                                                                                                                                                                                        \
  case QSurfaceFormat::c:                                                                                                                                                                              \
    glProfile = #c;                                                                                                                                                                                    \
    break
  switch (format().profile()) {
    CASE(NoProfile);
    CASE(CoreProfile);
    CASE(CompatibilityProfile);
  }
#undef CASE

  // qPrintable() will print our QString w/o quotes around it.
  qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}

void GLWidget::startTemplateScene() {
  m_program.removeAllShaders();
  m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/vs.glsl");
  m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/fs.glsl");
  m_program.link();
  state = STATE_TEMPLATE;
}

void GLWidget::startObjectScene() {
  m_program.removeAllShaders();
  m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/vs.glsl");
  m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/fs.glsl");
  m_program.link();
  state = STATE_OBJECTS;
}

void GLWidget::startSynthesisScene() {
  m_program.removeAllShaders();
  m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/vs_synthesis.glsl");
  m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/fs_synthesis.glsl");
  m_program.link();
  state = STATE_SYNTHESIS;
}
