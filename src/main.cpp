#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <GLUT/glut.h>


#include <vector>

#include "camera.hpp"
#include "shader.hpp"

//using namespace glm;
using namespace std;
GLFWwindow* window;
CCamera Camera;

// Shader sources
const GLchar* vertexSource = R"glsl(
    #version 150 core
    in vec2 position;
    in vec3 color;
    out vec3 Color;
    void main()
    {
        Color = color;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 150 core
    in vec3 Color;
    out vec4 outColor;
    void main()
    {
        outColor = vec4(Color, 1.0);
    }
)glsl";


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

void reshape(int x, int y)
{
	if (y == 0 || x == 0)
		return;  //Nothing is visible then, so return

	//Set a new projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Angle of view:40 degrees
	//Near clipping plane distance: 0.5
	//Far clipping plane distance: 20.0
	gluPerspective(40.0, (GLdouble) x / (GLdouble) y, 0.5, 20.0);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, x, y);  //Use the whole window for rendering
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
	//	glTranslatef(0.0, size, 0.0);
	//	DrawNet(size, LinesX, LinesZ);
//	glPopMatrix();

	//	glColor3f(0.0, 0.0, 1.0);
	//	glPushMatrix();
	//	glTranslatef(-halfsize, 0.0, 0.0);
	//	glRotatef(90.0, 0.0, 0.0, halfsize);
	//	DrawNet(size, LinesX, LinesZ);
	//	glTranslatef(0.0, -size, 0.0);
	//	DrawNet(size, LinesX, LinesZ);
	//	glPopMatrix();
	//
	//	glColor3f(1.0, 0.0, 0.0);
	//	glPushMatrix();
	//	glTranslatef(0.0, 0.0, -halfsize);
	//	glRotatef(90.0, halfsize, 0.0, 0.0);
	//	DrawNet(size, LinesX, LinesZ);
	//	glTranslatef(0.0, size, 0.0);
	//	DrawNet(size, LinesX, LinesZ);
	//	glPopMatrix();
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

std::vector<float> bspline(int k, float P[][3])
{
//	int noOfPoints = sizeof(P) / sizeof(*P);
	int noOfPoints = 5;

	float x[100] = { };
	int num_samples = 100;

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
		x[i] = (i / (float) num_samples) * (noOfPoints - k + 1);

	}

//	float** Q = new float*[num_samples];
	std::vector<float> vertices(num_samples * 3);

//	float *Q = new float[num_samples * 3];

//	for (int i = 0; i < num_samples * 3; ++i) {
//		Q[i] = 0;
//	}

//	for (int i = 1; i < num_samples; i++) {
//		Q[i][0] = 0;
//		Q[i][1] = 0;
//		Q[i][2] = 0;
//	}

	for (int i = 0; i < noOfPoints; i++)
	{
		for (int a = 0; a < num_samples; a++)
		{
			float basis = bSplineBasis(U, k, i, x[a], num_samples);

			vertices[3 * a] = vertices[3 * a] + basis * P[i][0];
			vertices[3 * a + 1] = vertices[3 * a + 1] + basis * P[i][1];
			vertices[3 * a + 2] = vertices[3 * a + 2] + basis * P[i][2];
		}
	}

	return vertices;
}

void renderTemplate(GLuint &vao)
{
    glBindVertexArray(vao);
    glDrawArrays(GL_LINE_STRIP, 0, 300);
}

void render(GLuint &vao)
{
    glClear(GL_COLOR_BUFFER_BIT);
//	glLoadIdentity();
//	Camera.Render();
//	glTranslatef(0.0, 0.8, 0.0);
//	glScalef(3.0, 1.0, 3.0);

//	glPushMatrix();
//	glTranslatef(0.0, 0.0, 0.0);

	renderGrid();
//	glPushMatrix();ßß//	glTranslatef(0.0, 0.0, 0.0);
	renderTemplate(vao);
//	glPopMatrix();
    
//    glfwPollEvents();
    // put the stuff we've been drawing onto the display
    glfwSwapBuffers(window);
    
//	glFlush();
//	glutSwapBuffers();

	cout << Camera.GetPosition().x << ',' << Camera.GetPosition().y << ',' << Camera.GetPosition().z << endl;
}

//void KeyDown(unsigned char key, int x, int y)
//{
//	switch (key)
//	{
//	case 27:		//ESC
//		glutDestroyWindow(window);
//		exit(0);
//		break;
//	case 'a':
//		Camera.RotateY(5.0);
//		render();
//		break;
//	case 'd':
//		Camera.RotateY(-5.0);
//		render();
//		break;
//	case 'w':
//		Camera.MoveForwards(-0.1);
//		render();
//		break;
//	case 's':
//		Camera.MoveForwards(0.1);
//		render();
//		break;
//	case 'x':
//		Camera.RotateX(5.0);
//		render();
//		break;
//	case 'y':
//		Camera.RotateX(-5.0);
//		render();
//		break;
//	case 'c':
//		Camera.StrafeRight(-0.1);
//		render();
//		break;
//	case 'v':
//		Camera.StrafeRight(0.1);
//		render();
//		break;
//	case 'f':
//		Camera.Move(F3dVector(0.0, -0.3, 0.0));
//		render();
//		break;
//	case 'r':
//		Camera.Move(F3dVector(0.0, 0.3, 0.0));
//		render();
//		break;
//	default:
//		break;
//
//	}
//}

void read_shader_src(const char *fname, std::vector<char> &buffer) {
    std::ifstream in;
    in.open(fname, std::ios::binary);
    
    if(in.is_open()) {
        // Get the number of bytes stored in this file
        in.seekg(0, std::ios::end);
        size_t length = (size_t)in.tellg();
        
        // Go to start of the file
        in.seekg(0, std::ios::beg);
        
        // Read the content of the file in a buffer
        buffer.resize(length + 1);
        in.read(&buffer[0], length);
        in.close();
        // Add a valid C - string end
        buffer[length] = '\0';
    }
    else {
        std::cerr << "Unable to open " << fname << " I'm out!" << std::endl;
        exit(-1);
    }
}


GLuint load_and_compile_shader(const char *fname, GLenum shaderType) {
    // Load a shader from an external file
    std::vector<char> buffer;
    read_shader_src(fname, buffer);
    const char *src = &buffer[0];
    
    // Compile the shader
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    // Check the result of the compilation
    GLint test;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &test);
    if(!test) {
        std::cerr << "Shader compilation failed with this message:" << std::endl;
        std::vector<char> compilation_log(512);
        glGetShaderInfoLog(shader, compilation_log.size(), NULL, &compilation_log[0]);
        std::cerr << &compilation_log[0] << std::endl;
        glfwTerminate();
        exit(-1);
    }
    return shader;
}

GLuint create_program(const char *path_vert_shader, const char *path_frag_shader) {
    // Load and compile the vertex and fragment shaders
    GLuint vertexShader = load_and_compile_shader(path_vert_shader, GL_VERTEX_SHADER);
    GLuint fragmentShader = load_and_compile_shader(path_frag_shader, GL_FRAGMENT_SHADER);
    
    // Attach the above shader to a program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    
    // Flag the shaders for deletion
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Link and use the program
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    
    return shaderProgram;
}

void initialize(GLuint &vao) {
    // Use a Vertex Array Object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    float P[5][3] =
    {
        { } };
    
    for (int i = 0; i < 5; i++)
    {
        P[i][0] = 0.5;
        P[i][1] = i * 0.2;
        P[i][2] = 0;
    }
    
    std::vector<float> splineVertices = bspline(3, P);
    
    // bSpline Points
    float* vertices = splineVertices.data();
    
    // Create a Vector Buffer Object that will store the vertices on video memory
    GLuint vbo;
    glGenBuffers(1, &vbo);
    
    // Allocate space and upload the data from CPU to GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, splineVertices.size(), vertices, GL_STATIC_DRAW);
    
    GLuint shaderProgram = create_program("../../shaders/vs.glsl", "../../shaders/fs.glsl");
    
    // Get the location of the attributes that enters in the vertex shader
    GLint position_attribute = glGetAttribLocation(shaderProgram, "position");
    
    // Specify how the data for position can be accessed
    glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // Enable the attribute
    glEnableVertexAttribArray(position_attribute);
}


int main(int argc, char **argv)
{
//	GLuint programID = LoadShaders("shaders/vs.glsl", "shaders/fs.glsl");

//	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertexShader, 1, &vertexSource, NULL);
//	glCompileShader(vertexShader);
//
//	// Create and compile the fragment shader
//	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
//	glCompileShader(fragmentShader);
//
//	// Link the vertex and fragment shader into a shader program
//	GLuint shaderProgram = glCreateProgram();
//	glAttachShader(shaderProgram, vertexShader);
//	glAttachShader(shaderProgram, fragmentShader);
//	glBindFragDataLocation(shaderProgram, 0, "outColor");
//	glLinkProgram(shaderProgram);
//	glUseProgram(shaderProgram);
//
//	// Specify the layout of the vertex data
//	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
//	glEnableVertexAttribArray(posAttrib);
//	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
//
//	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
//	glEnableVertexAttribArray(colAttrib);
//	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
    // uncomment these lines if on Apple OS X
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();
    
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    
    GLuint vao;
    
    // Initialize the data to be rendered
    initialize(vao);
    
    
//    int running = GL_TRUE;
    
//    reshape(640, 480);
//    Camera.Move(F3dVector(0.0, 0.0, 3.0));
//    Camera.MoveForwards(1.0);
    
    
    while(true) {
        // Display scene
        render(vao);
        
        // Pool for events
        glfwPollEvents();
        // Check if the window was closed
//        running = glfwGetWindowAttrib(window, GLFW_OPENED);
    }

//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
//	glutInitWindowSize(800, 480);
//	window = glutCreateWindow("ebib_demo");
//	Camera.Move(F3dVector(0.0, 0.0, 3.0));
////	Camera.MoveForwards(1.0);
//	glutDisplayFunc(render);
//	glutReshapeFunc(reshape);
//	glutKeyboardFunc(KeyDown);
//	glutMainLoop();
    
    glfwTerminate();
    return 0;
}

