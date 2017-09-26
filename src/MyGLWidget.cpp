#include "MyGLWidget.h"
#include <QDebug>
#include <QString>
#include <QKeyEvent>

MyGLWidget::~MyGLWidget()
{
    makeCurrent();
    teardownGL();
}

MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
}

/*******************************************************************************
 * OpenGL Events
 ******************************************************************************/

void MyGLWidget::initializeGL()
{
    // Initialize OnGLFunctions();
    initializeOpenGLFunctions();
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
    printContextInformation();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(8.0f);

    m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/vs.glsl");
    m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/fs.glsl");
    m_program.link();

    templateScene = new TemplateScene(&m_program);
    objectScene = new ObjectScene(&m_program);
}

void MyGLWidget::resizeGL(int width, int height)
{
    templateScene->setViewportAspectRatio(width/height);
}

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program.bind();
    m_program.setUniformValue("model", QMatrix4x4());

    switch(state) {
    case STATE_TEMPLATE:
        templateScene->render();
        break;
    case STATE_OBJECTS:
        objectScene->render();
        break;
    }

    GLenum err;
    if ((err = glGetError()) != GL_NO_ERROR) {
        qDebug() << "OpenGL error: " << err;
    }

    m_program.release();
}

void MyGLWidget::update()
{
    // Update input
    Input::update();

    switch(state) {
    case STATE_TEMPLATE:
        templateScene->update();
        break;
    case STATE_OBJECTS:
//        objectScene->update();
        break;
    }
    // Schedule a redraw
    QOpenGLWidget::update();
}

void MyGLWidget::teardownGL()
{
    // Currently we have no data to teardown
}

void MyGLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
        event->ignore();
    }
    else
    {
        Input::registerKeyPress(event->key());
    }
}

void MyGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
        event->ignore();
    }
    else
    {
        Input::registerKeyRelease(event->key());
    }
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
  Input::registerMousePress(event->button());
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
  Input::registerMouseRelease(event->button());
}


void MyGLWidget::printContextInformation()
{
    QString glType;
    QString glVersion;
    QString glProfile;

    // Get Version Information
    glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

    // Get Profile Information

#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
    switch (format().profile())
    {
    CASE(NoProfile);
    CASE(CoreProfile);
    CASE(CompatibilityProfile);
    }
#undef CASE

    // qPrintable() will print our QString w/o quotes around it.
    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}

void MyGLWidget::startTemplateScene(){
    state = STATE_TEMPLATE;
}

void MyGLWidget::startObjectScene(){
    state = STATE_OBJECTS;
}
