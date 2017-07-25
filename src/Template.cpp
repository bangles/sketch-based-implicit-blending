//
//  Template.cpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 12/07/17.
//
//

#include "Template.hpp"
#include <iostream>

using namespace ebib;

Template::Template(tdogl::Program& gProgram){
    this->gProgram = &gProgram;
    glGenBuffers(6, gVBO);
    
    MatrixXi triangles = MatrixXi(3, (NUM_SAMPLES-1) * (NUM_SAMPLES-1) * 2);
    
    int count = 0;
    for (int a = 0; a < NUM_SAMPLES - 1; a++)
    {
        for (int b = 0; b < NUM_SAMPLES - 1; b++)
        {
            //Triangle 1
            triangles.col(count++) = Vector3i(a * NUM_SAMPLES + b, (a + 1)*NUM_SAMPLES + b, (a + 1)*NUM_SAMPLES + b + 1);
            //Triangle 2
            triangles.col(count++) = Vector3i(a * NUM_SAMPLES + b, a*NUM_SAMPLES + b + 1, (a + 1)*NUM_SAMPLES + b + 1);
        }
    }
    
    MatrixXf points1(3, NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
    for (int i = 0; i < NUM_CONTROL_POINTS; i++)
        for (int j = 0; j < NUM_CONTROL_POINTS; j++)
            points1.col(j + (NUM_CONTROL_POINTS * i)) = Vector3f(i * 1.0 / (NUM_CONTROL_POINTS - 1), j * 0.5f / (NUM_CONTROL_POINTS - 1), 0.5f);
    
    
    
    MatrixXf points2(3, NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
    for (int i = 0; i < NUM_CONTROL_POINTS; i++)
        for (int j = 0; j < NUM_CONTROL_POINTS; j++)
            points2.col(j + (NUM_CONTROL_POINTS * i)) = Vector3f(i * 1.0 / (NUM_CONTROL_POINTS - 1),0.5f,j * 0.5f / (NUM_CONTROL_POINTS - 1));
    
    mPatch1 = surface.evaluateSurface(3, points1, NUM_CONTROL_POINTS, NUM_SAMPLES);
    mPatch1.triangles = triangles;
    
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, mPatch1.vertices.size() * sizeof(float), mPatch1.vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mPatch1.triangles.size() * sizeof(int), mPatch1.triangles.data(), GL_STATIC_DRAW);
    
    mPatch2 = surface.evaluateSurface(3, points2, NUM_CONTROL_POINTS,NUM_SAMPLES);
    mPatch2.triangles = triangles;
    
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[2]);
    glBufferData(GL_ARRAY_BUFFER, mPatch2.vertices.size() * sizeof(float), mPatch2.vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVBO[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mPatch2.triangles.size() * sizeof(int), mPatch2.triangles.data(), GL_STATIC_DRAW);
    
    initPoints(&points1, &points2);
}

void Template::initPoints(MatrixXf* points1, MatrixXf* points2) {
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[4]);
    glBufferData(GL_ARRAY_BUFFER, points1->size() * sizeof(GLfloat), points1->data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[5]);
    glBufferData(GL_ARRAY_BUFFER, points2->size() * sizeof(GLfloat), points2->data(), GL_STATIC_DRAW);
}

void Template::render() {
    gProgram->setUniform("myColor", glm::vec4(0.0f, 1.0f, 0.5f, 0.7f));
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVBO[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawElements(GL_TRIANGLES, mPatch1.triangles.size() * 3, GL_UNSIGNED_INT, (void*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[2]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVBO[3]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawElements(GL_TRIANGLES, mPatch2.triangles.size() * 3, GL_UNSIGNED_INT, (void*)0);
 
    renderControlPoints();
}

void Template::renderControlPoints() {    
    gProgram->setUniform("myColor", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[4]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_POINTS, 0, NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);

    gProgram->setUniform("myColor", glm::vec4(1.0f, 0.6f, 0.0f, 1.0f));
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[5]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_POINTS, 0, NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
}

