// third-party libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// standard C++ libraries
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>

// tdogl classes
#include "Program.h"
#include "Camera.h"

#define PI 3.14159265

// constants
const glm::vec2 SCREEN_SIZE(800, 600);

// globals
GLFWwindow* gWindow = NULL;
double gScrollY = 0.0;
tdogl::Program* gProgram = NULL;
tdogl::Camera gCamera;
GLuint gVAO, gVBO[8];

static const int NUM_CONTROL_POINTS = 5;
static const int NUM_SAMPLES = 100;
static const int NUM_OF_USER_POINTS = 30;

std::vector<GLuint> indices((NUM_SAMPLES-1) * (NUM_SAMPLES-1) * 6);


// loads the vertex shader and fragment shader, and links them to make the global gProgram
static void LoadShaders() {
    std::vector<tdogl::Shader> shaders;
    
    shaders.push_back(tdogl::Shader::shaderFromFile("../../resources/vs.glsl", GL_VERTEX_SHADER));
    shaders.push_back(tdogl::Shader::shaderFromFile("../../resources/fs.glsl", GL_FRAGMENT_SHADER));
    gProgram = new tdogl::Program(shaders);
}

float bSplineBasis(float U[], int o, int i, float u, int num_samples)
{
    float n = 0;
    if (o == 1)
    {
        if (u >= U[i] && u < U[i + 1])
            n = 1;
        else
            n = 0;
    }
    else
    {
        float n1 = bSplineBasis(U, o - 1, i, u, num_samples);
        float n2 = bSplineBasis(U, o - 1, i + 1, u, num_samples);
        
        float nume1 = n1 * (u - U[i]);
        float deno1 = U[i + o - 1] - U[i];
        float nume2 = n2 * (U[i + o] - u);
        float deno2 = U[i + o] - U[i + 1];
        
        if (deno1 == 0)
        {
            n = (nume2 / deno2);
        }
        else
        {
            if (deno2 == 0)
                n = (nume1 / deno1);
            else
                n = (nume1 / deno1) + (nume2 / deno2);
        }
    }
    return n;
}

std::vector<glm::vec3> bSplineSurface(int k, std::vector<glm::vec3> points, int noOfPoints, int num_samples)
{
    float x[NUM_SAMPLES] = {};
    
    float *U = new float[noOfPoints + k];
    
    for (int i = 0; i < noOfPoints + k; i++)
    {
        U[i] = 0;
        if (i < k)
        {
            U[i] = 0;
        }
        else if (i >= noOfPoints)
        {
            U[i] = noOfPoints - k + 1;
        }
        else
        {
            U[i] = i - k + 1;
        }
    }
    
    for (int i = 0; i < num_samples; i++)
    {
        x[i] = (i / (float)(num_samples)) * (noOfPoints - k + 1);
        
    }
    
    std::vector<glm::vec3> vertices(num_samples * num_samples);
    for (int a = 0; a < num_samples; a++)
    {
        for (int b = 0; b < num_samples; b++)
        {
            for (int i = 0; i < noOfPoints; i++)
            {
                glm::vec3 p2;
                for (int j = 0; j < noOfPoints; j++)
                {
                    float basis_1 = bSplineBasis(U, k, j, x[b], num_samples);
                    p2 += basis_1 * points[noOfPoints*i + j];
                }
                float basis_2 = bSplineBasis(U, k, i, x[a], num_samples);
                vertices[(b + num_samples * a)] += basis_2 * p2;
            }
        }
    }
    return vertices;
}

void initPoints(std::vector<glm::vec3> points1, std::vector<glm::vec3> points2) {
    
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[5]);
    glBufferData(GL_ARRAY_BUFFER, points1.size() * sizeof(glm::vec3), &points1[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[6]);
    glBufferData(GL_ARRAY_BUFFER, points2.size() * sizeof(glm::vec3), &points2[0], GL_STATIC_DRAW);
    
}

void initTemplate() {
    
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
            points1[j + (NUM_CONTROL_POINTS * i)] = { i * 1.0 / (NUM_CONTROL_POINTS - 1),j * 1.0 / (NUM_CONTROL_POINTS - 1), 0.0f };
    
    std::vector<glm::vec3> splineVertices1 = bSplineSurface(3, points1, NUM_CONTROL_POINTS, NUM_SAMPLES);
    
    glBindVertexArray(gVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, splineVertices1.size() * sizeof(glm::vec3), &splineVertices1[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    
    
    std::vector<glm::vec3> points2(NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
    for (int i = 0; i < NUM_CONTROL_POINTS; i++)
        for (int j = 0; j < NUM_CONTROL_POINTS; j++)
            points2[j + (NUM_CONTROL_POINTS * i)] = { i * 1.0 / (NUM_CONTROL_POINTS - 1),1.0f,j * 1.0 / (NUM_CONTROL_POINTS - 1)};
    
    
//    points2[2 + 2*NUM_CONTROL_POINTS] = { 2 * 1.0 / (NUM_CONTROL_POINTS - 1),1.5f,2 * 1.0 / (NUM_CONTROL_POINTS - 1)};
    
    std::vector<glm::vec3> splineVertices2 = bSplineSurface(3, points2, NUM_CONTROL_POINTS,NUM_SAMPLES);
    
    //    glBindVertexArray(gVAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[2]);
    glBufferData(GL_ARRAY_BUFFER, splineVertices2.size() * sizeof(glm::vec3), &splineVertices2[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVBO[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    
    initPoints(points1, points2);
}

void initGrid() {
    int numIndices = 100;
    
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
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[4]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    
}

std::vector<glm::vec3> userPoints(NUM_OF_USER_POINTS);


void initUserInputPoints() {
    float RADIUS = 0.3f;
    
    
    for (int i = 0; i <NUM_OF_USER_POINTS; i++) {
        double angle_rad = i * 360.0/NUM_OF_USER_POINTS * PI/180;
        
        userPoints[i] = {0.5 + RADIUS * glm::cos(angle_rad),
                        0.5 + RADIUS * glm::sin(angle_rad),
                        1.0f};
        
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[7]);
    glBufferData(GL_ARRAY_BUFFER, userPoints.size() * sizeof(glm::vec3), &userPoints[0], GL_STATIC_DRAW);
    
}




static void renderGrid() {
    gProgram->setUniform("myColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[4]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_LINES, 0, 100);
}

static void renderTemplate() {
    gProgram->setUniform("myColor", glm::vec4(0.0f, 1.0f, 0.5f, 0.7f));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVBO[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, (void*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[2]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVBO[3]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, (void*)0);
}

static void renderControlPoints() {
    glEnableVertexAttribArray(0);
    
    gProgram->setUniform("myColor", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[5]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_POINTS, 0, NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
    
    gProgram->setUniform("myColor", glm::vec4(1.0f, 0.6f, 0.0f, 1.0f));
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[6]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_POINTS, 0, NUM_CONTROL_POINTS * NUM_CONTROL_POINTS);
}

static void renderUserPoints() {
    glEnableVertexAttribArray(0);
    
    gProgram->setUniform("myColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    glBindBuffer(GL_ARRAY_BUFFER, gVBO[7]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_POINTS, 0, NUM_OF_USER_POINTS);
}


// draws a single frame
static void Render() {
    glClearColor(0, 0, 0, 1); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // bind the program (the shaders)
    gProgram->use();
    
    gProgram->setUniform("camera", gCamera.matrix());
    gProgram->setUniform("model", glm::mat4(1.0f));
    
    renderGrid();
    renderTemplate();
    renderControlPoints();
    renderUserPoints();
    
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    
    gProgram->stopUsing();
    
    // swap the display buffers (displays what was just drawn)
    glfwSwapBuffers(gWindow);
}


// update the scene based on the time elapsed since last update
void Update(float secondsElapsed) {
    //rotate the cube
    //    const GLfloat degreesPerSecond = 180.0f;
    //    gDegreesRotated += secondsElapsed * degreesPerSecond;
    //    while(gDegreesRotated > 360.0f) gDegreesRotated -= 360.0f;
    
    //move position of camera based on WASD keys, and XZ keys for up and down
    const float moveSpeed = 2.0; //units per second
    if (glfwGetKey(gWindow, 'S')) {
        gCamera.offsetPosition(secondsElapsed * moveSpeed * -gCamera.forward());
    }
    else if (glfwGetKey(gWindow, 'W')) {
        gCamera.offsetPosition(secondsElapsed * moveSpeed * gCamera.forward());
    }
    if (glfwGetKey(gWindow, 'A')) {
        gCamera.offsetPosition(secondsElapsed * moveSpeed * -gCamera.right());
    }
    else if (glfwGetKey(gWindow, 'D')) {
        gCamera.offsetPosition(secondsElapsed * moveSpeed * gCamera.right());
    }
    if (glfwGetKey(gWindow, 'Z')) {
        gCamera.offsetPosition(secondsElapsed * moveSpeed * -glm::vec3(0, 1, 0));
    }
    else if (glfwGetKey(gWindow, 'X')) {
        gCamera.offsetPosition(secondsElapsed * moveSpeed * glm::vec3(0, 1, 0));
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

// records how far the y axis has been scrolled
void OnScroll(GLFWwindow* window, double deltaX, double deltaY) {
    gScrollY += deltaY;
}

void OnError(int errorCode, const char* msg) {
    throw std::runtime_error(msg);
}

void cleanUp() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// the program starts here
void AppMain() {
    // initialise GLFW
    glfwSetErrorCallback(OnError);
    if (!glfwInit())
        throw std::runtime_error("glfwInit failed");
    
    // open a window with GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    gWindow = glfwCreateWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, "ebib_demo", NULL, NULL);
    if (!gWindow)
        throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");
    
    // GLFW settings
    glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(gWindow, 0, 0);
    glfwSetScrollCallback(gWindow, OnScroll);
    glfwMakeContextCurrent(gWindow);
    
    // initialise GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if (glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");
    
    // GLEW throws some errors, so discard all the errors so far
    while (glGetError() != GL_NO_ERROR) {}
    
    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    
    // make sure OpenGL version 3.2 API is available
    if (!GLEW_VERSION_3_2)
        throw std::runtime_error("OpenGL 3.2 API is not available.");
    
    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glDepthFunc(GL_LESS);
    glEnable(GL_POINT_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // load vertex and fragment shaders into opengl
    LoadShaders();

    glGenVertexArrays(1, &gVAO);
    glGenBuffers(8, gVBO);
    
    glPointSize(14.0f);
    
    initGrid();
    initTemplate();
    initUserInputPoints();
    
    // setup gCamera
    gCamera.setPosition(glm::vec3(0, 0.5, 3));
    gCamera.setViewportAspectRatio(SCREEN_SIZE.x / SCREEN_SIZE.y);
    
    // run while the window is open
    double lastTime = glfwGetTime();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    while (!glfwWindowShouldClose(gWindow)) {
        // process pending events
        glfwPollEvents();
        
        // update the scene based on the time elapsed since last update
        double thisTime = glfwGetTime();
        Update((float)(thisTime - lastTime));
        lastTime = thisTime;
        
        // draw one frame
        Render();
        
        // check for errors
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
            std::cerr << "OpenGL Error " << error << std::endl;
        
        //exit program if escape key is pressed
        if (glfwGetKey(gWindow, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(gWindow, GL_TRUE);
    }
    
    cleanUp();
    
    // clean up and exit
    glfwTerminate();
}


int main(int argc, char *argv[]) {
    try {
        AppMain();
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
