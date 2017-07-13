//
//  UserPoints.cpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 13/07/17.
//
//

#include "UserPoints.hpp"

using namespace ebib;

UserPoints::UserPoints(tdogl::Program& gProgram){
    this->gProgram = &gProgram;
    glGenBuffers(1, &gVBO);
    
    std::vector<glm::vec3> userPoints(NUM_OF_USER_POINTS);
    float RADIUS = 0.3f;
    
    for (int i = 0; i < NUM_OF_USER_POINTS; i++) {
        double angle_rad = i * 360.0/NUM_OF_USER_POINTS * PI/180;
        
        userPoints[i] = {0.5 + RADIUS * glm::cos(angle_rad),
            0.5 + RADIUS * glm::sin(angle_rad),
            1.0f};
        
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, userPoints.size() * sizeof(glm::vec3), &userPoints[0], GL_STATIC_DRAW);

    
}

void UserPoints::render(){
    glEnableVertexAttribArray(0);
    gProgram->setUniform("myColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_POINTS, 0, NUM_OF_USER_POINTS);

}
