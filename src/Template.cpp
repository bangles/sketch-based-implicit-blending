//
//  Template.cpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 12/07/17.
//
//

#include "Template.hpp"

using namespace ebib;

Template::Template(tdogl::Program& gProgram){
    this->gProgram = &gProgram;
    glGenBuffers(6, gVBO);
    
    indices = std::vector<GLuint>((NUM_SAMPLES-1) * (NUM_SAMPLES-1) * 6);
    
    int count = 0;
    for (int a = 0; a < NUM_SAMPLES - 1; a++)
    {
        for (int b = 0; b < NUM_SAMPLES - 1; b++)
        {
            indices[count++] = a * NUM_SAMPLES + b;				//0
            indices[count++] = (a + 1)*NUM_SAMPLES + b;			//100
            indices[count++] = (a + 1)*NUM_SAMPLES + b + 1;		//101
            
            indices[count++] = a*NUM_SAMPLES + b;				//0
            indices[count++] = a*NUM_SAMPLES + b + 1;			//1
            indices[count++] = (a + 1)*NUM_SAMPLES + b + 1;		//101
        }
    }
    
    std::vector<glm::vec3> points1(NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
    
    for (int i = 0; i < NUM_CONTROL_POINTS; i++)
        for (int j = 0; j < NUM_CONTROL_POINTS; j++)
            points1[j + (NUM_CONTROL_POINTS * i)] = { i * 1.0 / (NUM_CONTROL_POINTS - 1), j * 0.5f / (NUM_CONTROL_POINTS - 1), 0.5f };
    
    std::vector<glm::vec3> splineVertices1 = surface.evaluateSurface(3, points1, NUM_CONTROL_POINTS, NUM_SAMPLES);
    
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, splineVertices1.size() * sizeof(glm::vec3), &splineVertices1[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    
    
    std::vector<glm::vec3> points2(NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
    for (int i = 0; i < NUM_CONTROL_POINTS; i++)
        for (int j = 0; j < NUM_CONTROL_POINTS; j++)
            points2[j + (NUM_CONTROL_POINTS * i)] = { i * 1.0 / (NUM_CONTROL_POINTS - 1),0.5f,0.5f + j * 0.5f / (NUM_CONTROL_POINTS - 1)};
    
    std::vector<glm::vec3> splineVertices2 = surface.evaluateSurface(3, points2, NUM_CONTROL_POINTS,NUM_SAMPLES);
    
    //    glBindVertexArray(gVAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[2]);
    glBufferData(GL_ARRAY_BUFFER, splineVertices2.size() * sizeof(glm::vec3), &splineVertices2[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVBO[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    
    initPoints(points1, points2);
}

void Template::initPoints(std::vector<glm::vec3> points1, std::vector<glm::vec3> points2) {
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[4]);
    glBufferData(GL_ARRAY_BUFFER, points1.size() * sizeof(glm::vec3), &points1[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[5]);
    glBufferData(GL_ARRAY_BUFFER, points2.size() * sizeof(glm::vec3), &points2[0], GL_STATIC_DRAW);
    
}

void Template::render() {
    gProgram->setUniform("myColor", glm::vec4(0.0f, 1.0f, 0.5f, 0.7f));
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVBO[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, (void*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[2]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVBO[3]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, (void*)0);
 
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

