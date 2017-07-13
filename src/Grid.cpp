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
    
    std::vector<glm::vec3> vertices(numIndices);
    
    int n = (int)numIndices / 4;
    for (int i = 0; i < n; ++i)
    {
        vertices[4 * i + 0] = { -12.0f + i, 0.0f, -12.0f };
        vertices[4 * i + 1] = { -12.0f + i, 0.0f,  12.0f };
        vertices[4 * i + 2] = { -12.0f, 0.0f, -12.0f + i };
        vertices[4 * i + 3] = { 12.0f, 0.0f, -12.0f + i };
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
    glDrawArrays(GL_LINES, 0, 100);
}
