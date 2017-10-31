//  Template.cpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 12/07/17.

#include "Template.hpp"
#include <iostream>

#define LOG(x) std::cout<<x<<std::endl

Template::Template(QOpenGLShaderProgram *program) : index_vbo(QOpenGLBuffer::IndexBuffer){
    this->m_program = program;
    
    mPatches[0].points = MatrixXf(3, NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
    mPatches[0].vertices = MatrixXf(3,NUM_SAMPLES * NUM_SAMPLES);
    mPatches[0].weights = MatrixXf(NUM_SAMPLES * NUM_SAMPLES,NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
    mPatches[0].triangles = MatrixXi(3, (NUM_SAMPLES-1) * (NUM_SAMPLES-1) * 2);
    mPatches[0].grid = VectorXi::LinSpaced(NUM_CONTROL_POINTS * NUM_CONTROL_POINTS,0,NUM_CONTROL_POINTS * NUM_CONTROL_POINTS - 1);
    mPatches[0].grid.resize(NUM_CONTROL_POINTS, NUM_CONTROL_POINTS);

    int count = 0;
    for (int a = 0; a < NUM_SAMPLES - 1; a++)
    {
        for (int b = 0; b < NUM_SAMPLES - 1; b++)
        {
            //Triangle 1
            mPatches[0].triangles.col(count++) = Vector3i(a * NUM_SAMPLES + b, a * NUM_SAMPLES + (b + 1), (a + 1) * NUM_SAMPLES + (b + 1));
        }
    }
    
    for (int a = 0; a < NUM_SAMPLES - 1; a++)
    {
        for (int b = 0; b < NUM_SAMPLES - 1; b++)
        {
            //Triangle 2
            mPatches[0].triangles.col(count++) = Vector3i(a * NUM_SAMPLES + b, (a + 1) * NUM_SAMPLES + (b + 1), (a + 1) * NUM_SAMPLES + b);
        }
    }
    
    mPatches[1].points = MatrixXf(3, NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
    mPatches[1].vertices = MatrixXf(3,NUM_SAMPLES * NUM_SAMPLES);
    mPatches[1].weights = MatrixXf(NUM_SAMPLES * NUM_SAMPLES,NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
    mPatches[1].triangles = mPatches[0].triangles + (NUM_SAMPLES * NUM_SAMPLES * MatrixXi::Ones(3, (NUM_SAMPLES-1) * (NUM_SAMPLES-1) * 2));
    mPatches[1].grid = VectorXi::LinSpaced(NUM_CONTROL_POINTS * NUM_CONTROL_POINTS,NUM_CONTROL_POINTS * NUM_CONTROL_POINTS, 2 * NUM_CONTROL_POINTS * NUM_CONTROL_POINTS - 1);
    mPatches[1].grid.resize(NUM_CONTROL_POINTS, NUM_CONTROL_POINTS);

    surface1 = new BSplineSurface(mPatches[0], BSPLINE_ORDER, NUM_CONTROL_POINTS);
    surface2 = new BSplineSurface(mPatches[1], BSPLINE_ORDER, NUM_CONTROL_POINTS);
    
    initializeBuffers();
    setup();
    processPoints();
}

void Template::setup(){
    for (int i = 0; i < NUM_CONTROL_POINTS; i++)
        for (int j = 0; j < NUM_CONTROL_POINTS; j++)
            mPatches[0].points.col(j + (NUM_CONTROL_POINTS * i)) = Vector3f(0.5f, j * 0.5f / (NUM_CONTROL_POINTS - 1), i * 1.0f / (NUM_CONTROL_POINTS - 1));

    for (int i = 0; i < NUM_CONTROL_POINTS; i++)
        for (int j = 0; j < NUM_CONTROL_POINTS; j++)
            mPatches[1].points.col(j + (NUM_CONTROL_POINTS * i)) = Vector3f(j * 0.5f / (NUM_CONTROL_POINTS - 1), 0.5f, i * 1.0f / (NUM_CONTROL_POINTS - 1));

    surface1->evaluateSurface();
    surface2->evaluateSurface();
    updateBuffers();
}

void Template::render() {
    m_vao[0].bind();
    m_program->setUniformValue("color", QVector4D(0.0f, 1.0f, 0.5f, 0.7f));

    index_vbo.bind();
    glDrawElements(GL_TRIANGLES, mPatches[1].triangles.size() * 6, GL_UNSIGNED_INT, 0);

    m_vao[1].bind();
    m_program->setUniformValue("color", QVector4D(1.0f, 0.0f, 1.0f, 1.0f));
    glDrawArrays(GL_POINTS, 0, NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);

    m_vao[2].bind();
    m_program->setUniformValue("color", QVector4D(1.0f, 0.6f, 0.0f, 1.0f));
    glDrawArrays(GL_POINTS, 0, NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
}


void Template::updatePatches() {
    surface1->evaluateSurface();
    surface2->evaluateSurface();
    updateBuffers();
}

void Template::processPoints() {
    MatrixXf vertices(3, NUM_CONTROL_POINTS * NUM_CONTROL_POINTS * 2);
    
    vertices << mPatches[0].points,
                mPatches[1].points;
    
    meshInfo.line0 = mPatches[0].grid.row(0);
    meshInfo.line1 = mPatches[1].grid.row(0);
    meshInfo.spine0 = mPatches[0].grid.row(NUM_CONTROL_POINTS - 1);
    meshInfo.spine1 = mPatches[1].grid.row(NUM_CONTROL_POINTS - 1);
    meshInfo.line0o = mPatches[0].grid.row(1);
    meshInfo.line1o = mPatches[1].grid.row(1);
    
    meshInfo.slices.resize(NUM_CONTROL_POINTS , NUM_CONTROL_POINTS * 2);
    
    for(int i=0; i < NUM_CONTROL_POINTS; i++) {
        meshInfo.slices.row(i) <<   mPatches[1].grid.col(i).transpose(), mPatches[0].grid.col(i).transpose();
    }
}

void Template::evaluate(float u, float v, bool isPatch1, Vector3f &point) {
    if(isPatch1)
        surface1->evaluate(u, v, point);
    else
        surface2->evaluate(u, v, point);
}

void Template::initializeBuffers(){
    m_vbo[0].create(); m_vbo[1].create(); m_vbo[2].create();
    m_vao[0].create(); m_vao[1].create(); m_vao[2].create();
    index_vbo.create();
}

void Template::updateBuffers() {
    MatrixXf vertices(3, NUM_SAMPLES * NUM_SAMPLES * 2);
    vertices << mPatches[0].vertices, mPatches[1].vertices;

    MatrixXi triangles(3, (NUM_SAMPLES-1) * (NUM_SAMPLES-1) * 4);
    triangles << mPatches[0].triangles, mPatches[1].triangles;

//  Patches
    m_vbo[0].bind();
    m_vbo[0].setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vbo[0].allocate(vertices.data(), vertices.size() * sizeof(float));
    m_vao[0].bind();
    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    m_vbo[0].release();
    m_vao[0].release();

    index_vbo.bind();
    index_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    index_vbo.allocate(triangles.data(), triangles.size() * sizeof(GLuint));
    index_vbo.release();

//  Points 1
    m_vbo[1].bind();
    m_vbo[1].setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vbo[1].allocate(mPatches[0].points.data(), mPatches[0].points.size() * sizeof(GLfloat));
    m_vao[1].bind();
    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    m_vao[1].release();
    m_vbo[1].release();

//  Points 1
    m_vbo[2].bind();
    m_vbo[2].setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vbo[2].allocate(mPatches[1].points.data(), mPatches[1].points.size() * sizeof(GLfloat));
    m_vao[2].bind();
    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    m_vao[2].release();
    m_vbo[2].release();
}


