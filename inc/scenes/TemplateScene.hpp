#ifndef TemplateScene_hpp
#define TemplateScene_hpp


#include "Grid.hpp"
#include "Pipeline.hpp"
#include "Camera.hpp"
#include <Eigen/Dense>
#include <string>
#include <stdio.h>
#include <QOpenGLShaderProgram>
#include "input.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <QCursor>
#include <QApplication>

using namespace Eigen;

class TemplateScene {
private:
    Camera m_camera;
    QOpenGLShaderProgram *m_program;
    Pipeline *m_pipeline;
    Grid* grid = NULL;
public:
    TemplateScene(QOpenGLShaderProgram *program, Pipeline *pipeline);
    ~TemplateScene();
    //        static const glm::vec2 SCREEN_SIZE;
    void render();
    void update();
    void cleanUp();
    void setViewportAspectRatio(float aspectRatio);
};

#endif /* TemplateScene_hpp */
