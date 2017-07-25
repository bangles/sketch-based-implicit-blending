//
//  Scene.cpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 11/07/17.
//
//

#include "Scene.hpp"
#include <iostream>

#define LOG(x) std::cout << x << std::endl

using namespace ebib;

const glm::vec2 Scene::SCREEN_SIZE = glm::vec2(800, 600);

Scene::Scene(GLFWwindow& gWindow){
    loadShaders();
    gCamera.setPosition(glm::vec3(0, 0.5, 3));
    gCamera.setViewportAspectRatio(SCREEN_SIZE.x / SCREEN_SIZE.y);
    this->gWindow = &gWindow;
    grid = new Grid(3 * 44, *gProgram);
    gTemplate = new Template(*gProgram);
    userPoints = new UserPoints(*gProgram);
    _processor = new Processor(gTemplate->mPatch1);
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
}

Scene::~Scene(){
}


void Scene::loadShaders() {
    std::vector<tdogl::Shader> shaders;
    shaders.push_back(tdogl::Shader::shaderFromFile("../../resources/vs.glsl", GL_VERTEX_SHADER));
    shaders.push_back(tdogl::Shader::shaderFromFile("../../resources/fs.glsl", GL_FRAGMENT_SHADER));
    gProgram = new tdogl::Program(shaders);
}

void Scene::render(){
    glClearColor(0, 0, 0, 1); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    gProgram->use();
    gProgram->setUniform("camera", gCamera.matrix());
    gProgram->setUniform("model", glm::mat4(1.0f));
    
    grid->renderGrid();
    gTemplate->render();
    userPoints->render();
    
    gProgram->stopUsing();
    glfwSwapBuffers(gWindow);

}

void Scene::process(){
//    drawTestPoints();
    _processor->process(userPoints->mUserPoints);
}

void Scene::update(float deltaTime) {
    
    //move position of camera based on WASD keys, and XZ keys for up and down
    const float moveSpeed = 2.0; //units per second
    if (glfwGetKey(gWindow, 'S')) {
        gCamera.offsetPosition(deltaTime * moveSpeed * -gCamera.forward());
    }
    else if (glfwGetKey(gWindow, 'W')) {
        gCamera.offsetPosition(deltaTime * moveSpeed * gCamera.forward());
    }
    if (glfwGetKey(gWindow, 'A')) {
        gCamera.offsetPosition(deltaTime * moveSpeed * -gCamera.right());
    }
    else if (glfwGetKey(gWindow, 'D')) {
        gCamera.offsetPosition(deltaTime * moveSpeed * gCamera.right());
    }
    if (glfwGetKey(gWindow, 'Z')) {
        gCamera.offsetPosition(deltaTime * moveSpeed * -glm::vec3(0, 1, 0));
    }
    else if (glfwGetKey(gWindow, 'X')) {
        gCamera.offsetPosition(deltaTime * moveSpeed * glm::vec3(0, 1, 0));
    }
    else if (glfwGetKey(gWindow, 'P')) {
        process();
    }
    
    //rotate camera based on mouse movement
    const float mouseSensitivity = 0.1f;
    double mouseX, mouseY;
    glfwGetCursorPos(gWindow, &mouseX, &mouseY);
    gCamera.offsetOrientation(mouseSensitivity * (float)mouseY, mouseSensitivity * (float)mouseX);
    glfwSetCursorPos(gWindow, 0, 0); //reset the mouse, so it doesn't go out of the window
    
    //increase or decrease field of view based on mouse wheel
    const float zoomSensitivity = -0.2f;
    float fieldOfView = gCamera.fieldOfView() + zoomSensitivity * (float)gScrollY;
    if (fieldOfView < 5.0f) fieldOfView = 5.0f;
    if (fieldOfView > 130.0f) fieldOfView = 130.0f;
    gCamera.setFieldOfView(fieldOfView);
    gScrollY = 0;
}

void Scene::onScroll(double deltaY) {
    gScrollY += deltaY;
}

void Scene::cleanUp() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
}

void Scene::drawTestPoints(MatrixXf points) {
    gProgram->use();
    gProgram->setUniform("camera", gCamera.matrix());
    gProgram->setUniform("model", glm::mat4(1.0f));
    
    GLuint gVBO;
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(GLfloat), points.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    gProgram->setUniform("myColor", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_POINTS, 0, NUM_OF_USER_POINTS);
    
    gProgram->stopUsing();
    glfwSwapBuffers(gWindow);
}

