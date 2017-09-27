#ifndef Grid_hpp
#define Grid_hpp

#include<vector>
#include <Eigen/Dense>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

using namespace Eigen;

class Grid {
private:
    std::vector<QVector3D> vertices;
    QOpenGLBuffer m_vbo;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLShaderProgram* m_program;

public:
    Grid(int numIndices, QOpenGLShaderProgram *program);
    ~Grid();
    void render();
};


#endif /* Grid_hpp */
