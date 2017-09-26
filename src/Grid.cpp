#include "Grid.hpp"

Grid::Grid(int numIndices, QOpenGLShaderProgram *program){
    this->m_program = program;
    vertices = std::vector<QVector3D>(numIndices);
    
    const int VERTEX_NUMBER = 12;
    
    int n = (int)numIndices / VERTEX_NUMBER;
    for (int i = 0; i < n; ++i)
    {
        //x-z plane
        vertices[VERTEX_NUMBER * i + 0] = QVector3D(i * 0.1f, 0.0f, 0.0f );
        vertices[VERTEX_NUMBER * i + 1] = QVector3D( i * 0.1f, 0.0f,  1.0f );
        
        vertices[VERTEX_NUMBER * i + 2] = QVector3D( 0.0f, 0.0f, i * 0.1f );
        vertices[VERTEX_NUMBER * i + 3] = QVector3D( 1.0f, 0.0f, i * 0.1f );
        
        //x-y plane
        vertices[VERTEX_NUMBER * i + 4] = QVector3D( i * 0.1f, 0.0f, 0.0f );
        vertices[VERTEX_NUMBER * i + 5] = QVector3D( i * 0.1f, 1.0f,  0.0f );
        
        vertices[VERTEX_NUMBER * i + 6] = QVector3D( 0.0f, i * 0.1f, 0.0f );
        vertices[VERTEX_NUMBER * i + 7] = QVector3D( 1.0f, i * 0.1f, 0.0f );
        
        //y-z plane
        vertices[VERTEX_NUMBER * i + 8] = QVector3D( 0.0f, i * 0.1f, 0.0f );
        vertices[VERTEX_NUMBER * i + 9] = QVector3D( 0.0f, i * 0.1f, 1.0f );
        
        vertices[VERTEX_NUMBER * i + 10] = QVector3D( 0.0f, 0.0f, i * 0.1f );
        vertices[VERTEX_NUMBER * i + 11] = QVector3D( 0.0f, 1.0f, i * 0.1f );
    }
    
    m_program->bind();
    m_vbo.create();
    m_vbo.bind();
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vbo.allocate(&vertices[0], vertices.size() * sizeof(QVector3D));

    m_vao.create();
    m_vao.bind();

    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3);

    m_program->release();
    m_vao.release();
    m_vbo.release();
    
}

Grid::~Grid(){
    m_vbo.destroy();
}

void Grid::render(){
    m_vao.bind();
    
    m_program->setUniformValue("color", QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
    glDrawArrays(GL_LINES, 6, 2);

    m_program->setUniformValue("color", QVector4D(0.0f, 0.0f, 1.0f, 1.0f));
    glDrawArrays(GL_LINES, 8, 2);

    m_program->setUniformValue("color", QVector4D(0.0f, 1.0f, 0.0f, 1.0f));
    glDrawArrays(GL_LINES, 10, 2);
    
    m_program->setUniformValue("color", QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
    glDrawArrays(GL_LINES, 12, vertices.size());

    m_vao.release();
    
}
