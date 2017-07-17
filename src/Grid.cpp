//
//  Grid.cpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 11/07/17.
//
//

#include "Grid.hpp"

using namespace ebib;

Grid::Grid(int numIndices, tdogl::Program& gProgram){
    this->gProgram = &gProgram;
    glGenBuffers(1, &gVBO);
    
    vertices = std::vector<glm::vec3>(numIndices);
    
    const int VERTEX_NUMBER = 12;
    
    int n = (int)numIndices / VERTEX_NUMBER;
    for (int i = 0; i < n; ++i)
    {
        //x-z plane
        vertices[VERTEX_NUMBER * i + 0] = { i * 0.1f, 0.0f, 0.0f };
        vertices[VERTEX_NUMBER * i + 1] = { i * 0.1f, 0.0f,  1.0f };
        
        vertices[VERTEX_NUMBER * i + 2] = { 0.0f, 0.0f, i * 0.1f };
        vertices[VERTEX_NUMBER * i + 3] = { 1.0f, 0.0f, i * 0.1f };
        
        //x-y plane
        vertices[VERTEX_NUMBER * i + 4] = { i * 0.1f, 0.0f, 0.0f };
        vertices[VERTEX_NUMBER * i + 5] = { i * 0.1f, 1.0f,  0.0f };
        
        vertices[VERTEX_NUMBER * i + 6] = { 0.0f, i * 0.1f, 0.0f };
        vertices[VERTEX_NUMBER * i + 7] = { 1.0f, i * 0.1f, 0.0f };
        
        //y-z plane
        vertices[VERTEX_NUMBER * i + 8] = { 0.0f, i * 0.1f, 0.0f };
        vertices[VERTEX_NUMBER * i + 9] = { 0.0f, i * 0.1f, 1.0f };
        
        vertices[VERTEX_NUMBER * i + 10] = { 0.0f, 0.0f, i * 0.1f };
        vertices[VERTEX_NUMBER * i + 11] = { 0.0f, 1.0f, i * 0.1f };
    }
    
    //glBindVertexArray(gVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    
}

void Grid::renderGrid(){
    gProgram->setUniform("myColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_LINES, 0, vertices.size());
}
