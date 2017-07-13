//
//  Scene.hpp
//  ebib_demo
//
//  Created by Ishmeet Singh Kohli on 11/07/17.
//
//

#ifndef Scene_hpp
#define Scene_hpp


#include "Grid.hpp"
#include "Camera.h"
#include "Template.hpp"
#include "UserPoints.hpp"

#include <string>
#include <stdio.h>
#include <GLFW/glfw3.h>


namespace ebib{
    
    class Scene {
    private:
        GLuint gVAO;
        double gScrollY = 0.0;
        GLFWwindow* gWindow = NULL;
        tdogl::Program* gProgram = NULL;
        tdogl::Camera gCamera;
        Template* gTemplate = NULL;
        Grid* grid = NULL;
        UserPoints* userPoints = NULL;
        void loadShaders();
    public:
        Scene(GLFWwindow& gWindow);
        ~Scene();
        static const glm::vec2 SCREEN_SIZE;
        void render();
        void update(float deltaTime);
        void onScroll(double deltaY);
        void cleanUp();
    };
}

#endif /* Scene_hpp */
