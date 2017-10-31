//  UserPoints.hpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 13/07/17.

#ifndef UserPoints_hpp
#define UserPoints_hpp

#define PI 3.14159265

#include <Eigen/Dense>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

using namespace Eigen;

static const int NUM_OF_USER_POINTS = 10;

class UserPoints {
private:
    QOpenGLBuffer m_vbo;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLShaderProgram* m_program;

public:
    UserPoints(QOpenGLShaderProgram *program);
    ~UserPoints();
    MatrixXf m_userPoints;
    void render();
    void clear();
    void bindPoints();
    void setUserPoints(MatrixXf userPoints);
};

#endif /* UserPoints_hpp */
