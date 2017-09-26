//  Template.hpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 12/07/17.

#ifndef Template_hpp
#define Template_hpp

#include "BSplineSurface.hpp"
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

static const int NUM_CONTROL_POINTS = 5;
static const int BSPLINE_ORDER = 3;

struct MeshInfo{
    VectorXi line0;
    VectorXi line1;

    VectorXi spine0;
    VectorXi spine1;

    VectorXi line0o;
    VectorXi line1o;

    MatrixXi slices;
};

class Template {
private:
//    GLuint gVBO[5];
    QOpenGLBuffer index_vbo;
    QOpenGLBuffer m_vbo[3];
    QOpenGLVertexArrayObject m_vao[3];
    QOpenGLShaderProgram* m_program;
    BSplineSurface *surface1, *surface2;

public:
    Template(QOpenGLShaderProgram *program);
    ~Template();

    Patch mPatches[2];
    MeshInfo meshInfo;
    void evaluate(float u, float v, bool isPatch1, Vector3f &point);
    void render();
    void updatePatches();
    void processPoints();
    void initializeBuffers();
    void updateBuffers();
};


#endif /* Template_hpp */
