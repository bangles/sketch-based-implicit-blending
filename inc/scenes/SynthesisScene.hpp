#ifndef SynthesisScene_hpp
#define SynthesisScene_hpp

#include "Pipeline.hpp"
#include "Camera.hpp"
//#include "Grid.hpp"
#include <Eigen/Dense>
#include <string>
#include <stdio.h>
#include <QOpenGLShaderProgram>
#include "input.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <QCursor>
#include <QApplication>

using namespace Eigen;

class SynthesisScene {
private:
    Camera m_camera;
    QOpenGLShaderProgram *m_program;
    Pipeline *m_pipeline;
//    Grid* grid;
public:
    SynthesisScene(QOpenGLShaderProgram *program, Pipeline *pipeline);
    ~SynthesisScene();
    //        static const glm::vec2 SCREEN_SIZE;
    void render();
    void update();
    void cleanUp();
    void setViewportAspectRatio(float aspectRatio);
};

#endif /* SynthesisScene_hpp */
