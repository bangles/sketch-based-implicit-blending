//
//  Template.cpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 12/07/17.
//

#include "Template.hpp"
#include <iostream>

#define LOG(x) std::cout<<x<<std::endl

using namespace ebib;

Template::Template(tdogl::Program& gProgram){
    this->gProgram = &gProgram;
    glGenBuffers(5, gVBO);
    
    mPatches[0].points = MatrixXf(3, NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
    mPatches[0].vertices = MatrixXf(3,NUM_SAMPLES * NUM_SAMPLES);;
    mPatches[0].weights = MatrixXf(NUM_CONTROL_POINTS * NUM_CONTROL_POINTS,NUM_SAMPLES * NUM_SAMPLES);
    mPatches[0].triangles = MatrixXi(3, (NUM_SAMPLES-1) * (NUM_SAMPLES-1) * 2);
    mPatches[0].grid = VectorXi::LinSpaced(NUM_CONTROL_POINTS * NUM_CONTROL_POINTS,0,NUM_CONTROL_POINTS * NUM_CONTROL_POINTS - 1);
    mPatches[0].grid.resize(NUM_CONTROL_POINTS, NUM_CONTROL_POINTS);
    
    for (int i = 0; i < NUM_CONTROL_POINTS; i++)
        for (int j = 0; j < NUM_CONTROL_POINTS; j++)
            mPatches[0].points.col(j + (NUM_CONTROL_POINTS * i)) = Vector3f(0.5f, i * 0.5f / (NUM_CONTROL_POINTS - 1), j * 1.0f / (NUM_CONTROL_POINTS - 1));
    
    int count = 0;
    for (int a = 0; a < NUM_SAMPLES - 1; a++)
    {
        for (int b = 0; b < NUM_SAMPLES - 1; b++)
        {
            //Triangle 1
            mPatches[0].triangles.col(count++) = Vector3i(a * NUM_SAMPLES + b, (a + 1)*NUM_SAMPLES + b, (a + 1)*NUM_SAMPLES + b + 1);
            //Triangle 2
            mPatches[0].triangles.col(count++) = Vector3i(a * NUM_SAMPLES + b, a*NUM_SAMPLES + b + 1, (a + 1)*NUM_SAMPLES + b + 1);
        }
    }
    
    LOG(mPatches[0].points);
    
    LOG(mPatches[0].grid);
    
    mPatches[1].points = MatrixXf(3, NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
    mPatches[1].vertices = MatrixXf(3,NUM_SAMPLES * NUM_SAMPLES);
    mPatches[1].weights = MatrixXf(NUM_CONTROL_POINTS * NUM_CONTROL_POINTS,NUM_SAMPLES * NUM_SAMPLES);
    mPatches[1].triangles = mPatches[0].triangles + (NUM_SAMPLES * NUM_SAMPLES * MatrixXi::Ones(3, (NUM_SAMPLES-1) * (NUM_SAMPLES-1) * 2));
    mPatches[1].grid = VectorXi::LinSpaced(NUM_CONTROL_POINTS * NUM_CONTROL_POINTS,NUM_CONTROL_POINTS * NUM_CONTROL_POINTS, 2 * NUM_CONTROL_POINTS * NUM_CONTROL_POINTS - 1);
    mPatches[1].grid.resize(NUM_CONTROL_POINTS, NUM_CONTROL_POINTS);


    
    for (int i = 0; i < NUM_CONTROL_POINTS; i++)
        for (int j = 0; j < NUM_CONTROL_POINTS; j++)
            mPatches[1].points.col(j + (NUM_CONTROL_POINTS * i)) = Vector3f(i * 0.5f / (NUM_CONTROL_POINTS - 1), 0.5f, j * 1.0f / (NUM_CONTROL_POINTS - 1));
    
    surface.evaluateSurface(3, mPatches[0], NUM_CONTROL_POINTS, NUM_SAMPLES);
    surface.evaluateSurface(3, mPatches[1], NUM_CONTROL_POINTS, NUM_SAMPLES);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mPatches[0].triangles.size() * sizeof(int), mPatches[0].triangles.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVBO[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mPatches[1].triangles.size() * sizeof(int), mPatches[1].triangles.data(), GL_STATIC_DRAW);
    
    processPoints();
}



void Template::render() {
    MatrixXf vertices(3, NUM_SAMPLES * NUM_SAMPLES * 2);
    
    vertices << mPatches[0].vertices,
                mPatches[1].vertices;
    
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    gProgram->setUniform("myColor", glm::vec4(0.0f, 1.0f, 0.5f, 0.7f));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVBO[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawElements(GL_TRIANGLES, mPatches[0].triangles.size() * 3, GL_UNSIGNED_INT, (void*)0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVBO[2]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawElements(GL_TRIANGLES, mPatches[1].triangles.size() * 3, GL_UNSIGNED_INT, (void*)0);
 
    renderControlPoints();
}

void Template::renderControlPoints() {
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[3]);
    glBufferData(GL_ARRAY_BUFFER, mPatches[0].points.size() * sizeof(GLfloat), mPatches[0].points.data(), GL_STATIC_DRAW);
    
    gProgram->setUniform("myColor", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_POINTS, 0, NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
    
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[4]);
    glBufferData(GL_ARRAY_BUFFER, mPatches[1].points.size() * sizeof(GLfloat), mPatches[1].points.data(), GL_STATIC_DRAW);

    gProgram->setUniform("myColor", glm::vec4(1.0f, 0.6f, 0.0f, 1.0f));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_POINTS, 0, NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
}

void Template::updatePatches() {
    surface.evaluateSurface(3, mPatches[0], NUM_CONTROL_POINTS, NUM_SAMPLES);
    surface.evaluateSurface(3, mPatches[1], NUM_CONTROL_POINTS,NUM_SAMPLES);
}

void Template::processPoints() {
    MatrixXf vertices(3, NUM_CONTROL_POINTS * NUM_CONTROL_POINTS * 2);
    
    vertices << mPatches[0].points,
    mPatches[1].points;
    
    meshInfo.line0 = mPatches[0].grid.col(0);
    meshInfo.line1 = mPatches[1].grid.col(0);
    meshInfo.spine0 = mPatches[0].grid.col(NUM_CONTROL_POINTS - 1);
    meshInfo.spine1 = mPatches[1].grid.col(NUM_CONTROL_POINTS - 1);
    meshInfo.line0o = mPatches[0].grid.col(1);
    meshInfo.line0o = mPatches[1].grid.col(1);
    
    meshInfo.slices.resize(NUM_CONTROL_POINTS * 2, NUM_CONTROL_POINTS);
    
    for(int i=0; i < NUM_CONTROL_POINTS; i++) {
        meshInfo.slices.col(i) <<   mPatches[0].grid.row(i).transpose(),
                                    mPatches[1].grid.row(i).transpose();
    }
}

