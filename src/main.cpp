#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <GLUT/glut.h>

#include <vector>

#include "Camera.hpp"
#include "shader.hpp"

//using namespace glm;
using namespace std;
GLFWwindow* window;
Camera *cam;

GLint windowWidth = 800;              // Width of our window
GLint windowHeight = 600;              // Heightof our window

GLint midWindowX = windowWidth / 2; // Middle of the window horizontally
GLint midWindowY = windowHeight / 2; // Middle of the window vertically

GLfloat fieldOfView = 45.0f;            // Define our field of view (i.e. how quickly foreshortening occurs)
GLfloat near = 2.0f;             // The near (Z Axis) point of our viewing frustum (default 2.0f)
GLfloat far = 1500.0f;          // The far  (Z Axis) point of our viewing frustum (default 1500.0f)


static const int NUM_CONTROL_POINTS = 5;
static const int NUM_SAMPLES = 100;

void DrawNet(GLfloat size, GLint LinesX, GLint LinesZ)
{
	glBegin(GL_LINES);
	for (int xc = 0; xc < LinesX; xc++)
	{
		glVertex3f(-size / 2.0 + xc / (GLfloat) (LinesX - 1) * size, 0.0, size / 2.0);
		glVertex3f(-size / 2.0 + xc / (GLfloat) (LinesX - 1) * size, 0.0, size / -2.0);
	}
	for (int zc = 0; zc < LinesX; zc++)
	{
		glVertex3f(size / 2.0, 0.0, -size / 2.0 + zc / (GLfloat) (LinesZ - 1) * size);
		glVertex3f(size / -2.0, 0.0, -size / 2.0 + zc / (GLfloat) (LinesZ - 1) * size);
	}
	glEnd();
}


void renderGrid()
{
	GLfloat size = 2.0;
	GLint LinesX = 30;
	GLint LinesZ = 30;

	GLfloat halfsize = size / 2.0;
	glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);

    
	DrawNet(size, LinesX, LinesZ);
	glPopMatrix();

}

void handleMouseMove(GLFWwindow *window, double mouseX, double mouseY)
{
    cam->handleMouseMove(mouseX, mouseY);
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

std::vector<GLfloat> bSplineSurface(int k, float P[NUM_CONTROL_POINTS][NUM_CONTROL_POINTS][3], int noOfPoints, int num_samples)
{
    //	int noOfPoints = sizeof(P) / sizeof(*P);
    // int num_samples = 100;
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
        x[i] = (i / (float)num_samples) * (noOfPoints - k + 1);
        
    }
    
    std::vector<GLfloat> vertices(num_samples * num_samples * 3);
    
    for (int a = 0; a < num_samples; a++)
    {
        for (int b = 0; b < num_samples; b++)
        {
            for (int i = 0; i < noOfPoints; i++)
            {
                float p2[3] = {};
                for (int j = 0; j < noOfPoints; j++)
                {
                    float basis_1 = bSplineBasis(U, k, j, x[b], num_samples);
                    p2[0] += basis_1 * P[j][i][0];
                    p2[1] += basis_1 * P[j][i][1];
                    p2[2] += basis_1 * P[j][i][2];
                }
                float basis_2 = bSplineBasis(U, k, i, x[a], num_samples);
                vertices[3 * (b + num_samples * a) + 0] += basis_2 * p2[0];
                vertices[3 * (b + num_samples * a) + 1] += basis_2 * p2[1];
                vertices[3 * (b + num_samples * a) + 2] += basis_2 * p2[2];
            }
        }
    }
    
    return vertices;
}

void initGL(){
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
    // uncomment these lines if on Apple OS X
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(1024, 768, "ebib_demo", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
}

GLuint VertexArrayID;
GLuint vertexbuffer;
GLuint elementBuffer;
GLuint programID;
std::vector<GLuint> indices((NUM_SAMPLES) * (NUM_SAMPLES) * 6);

void render()
{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Use our shader
    glUseProgram(programID);
    
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
                          0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                          3,                  // size
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );
    
    // Draw the triangle !
    //glDrawArrays(GL_TRIANGLES, 0, splineVertices.size()/3); // 3 indices starting at 0 -> 1 triangle
    glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_SHORT, (void*)0);
    
    glDisableVertexAttribArray(0);
    
    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
    
}

void initTemplate(){
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    float P[NUM_CONTROL_POINTS][NUM_CONTROL_POINTS][3] = {{{}}};
    
    for (int i = 0; i < NUM_CONTROL_POINTS; i++)
    {
        for (int j = 0; j < NUM_CONTROL_POINTS; j++)
        {
            P[i][j][0] = i * 1.0f / NUM_CONTROL_POINTS;
            P[i][j][1] = j * 1.0f / NUM_CONTROL_POINTS;
            P[i][j][2] = 0.0f;
        }
    }
    
    std::vector<GLfloat> splineVertices = bSplineSurface(3, P, NUM_CONTROL_POINTS, NUM_SAMPLES);
    
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, splineVertices.size() * sizeof(GLfloat), &splineVertices[0], GL_STATIC_DRAW);
    
    int count = 0;
    for (int a = 0; a < NUM_SAMPLES; a++)
    {
        for (int b = 0; b < NUM_SAMPLES; b++)
        {
            indices[count++] = a*NUM_SAMPLES + b;				//0
            indices[count++] = (a + 1)*NUM_SAMPLES + b;			//100
            indices[count++] = (a + 1)*NUM_SAMPLES + b + 1;		//101
            
            indices[count++] = a*NUM_SAMPLES + b;				//0
            indices[count++] = a*NUM_SAMPLES + b + 1;			//1
            indices[count++] = (a + 1)*NUM_SAMPLES + b + 1;		//101
        }
    }
    
    
    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    
}

void cleanUp() {
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &elementBuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);
}

int main(int argc, char **argv)
{
    initGL();
    initTemplate();
    
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("../../shaders/vs.glsl", "../../shaders/fs.glsl");
    
    do {
       
        render();
        
        } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);
    
    cleanUp();
    glfwTerminate();
    return 0;
}

