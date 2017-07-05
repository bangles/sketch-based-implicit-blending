#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <GL/glut.h>

#include "camera.hpp"

using namespace glm;
using namespace std;
int window;
CCamera Camera;

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
	glPopMatrix();

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

	glFlush();
	glutSwapBuffers();
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

float* bspline(int k, float P[5][3]) {
	int noOfPoints = sizeof(P)/sizeof(*P);

	float x[100] = {};
	int num_samples = 100;

	float *U = new float[noOfPoints + k];

	for (int i = 0; i < noOfPoints + k - 1; i++)
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

	for(int i=0;i<num_samples;i++) {
		x[i] = (i/(float)num_samples) * (noOfPoints - k + 1);

	}

	float** Q = new float*[num_samples];
	
	for (int i = 0; i < num_samples; ++i) {
		Q[i] = new float[3];
	}

	for (int i = 1; i < num_samples; i++) {
		Q[i][0] = 0;
		Q[i][1] = 0;
		Q[i][2] = 0;
	}

	for(int i = 0; i<noOfPoints;i++) {
		for(int a = 1; a<num_samples;a++) {
			float basis = bSplineBasis(U, k, i, x[a], num_samples);

			Q[a][0] = Q[a][0] + basis * P[i][0];
			Q[a][1] = Q[a][1] + basis * P[i][1];
			Q[a][2] = Q[a][2] + basis * P[i][2];
		}
	}

	return &Q[0][0];
}


void renderTemplate()
{
	float P[5][3] = { {} };

	for (int i = 0; i < 5; i++) {
		P[i][0] = 10;
		P[i][1] = i * 2;
		P[i][2] = 0;
	}

	float * bspline1 = bspline(3, P);
}

void render(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	Camera.Render();
//	glTranslatef(0.0, 0.8, 0.0);
//	glScalef(3.0, 1.0, 3.0);

	renderGrid();
	renderTemplate();

	cout << Camera.GetPosition().x << ',' << Camera.GetPosition().y << ',' << Camera.GetPosition().z << endl;
}

void KeyDown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:		//ESC
		glutDestroyWindow(window);
		exit(0);
		break;
	case 'a':
		Camera.RotateY(5.0);
		render();
		break;
	case 'd':
		Camera.RotateY(-5.0);
		render();
		break;
	case 'w':
		Camera.MoveForwards(-0.1);
		render();
		break;
	case 's':
		Camera.MoveForwards(0.1);
		render();
		break;
	case 'x':
		Camera.RotateX(5.0);
		render();
		break;
	case 'y':
		Camera.RotateX(-5.0);
		render();
		break;
	case 'c':
		Camera.StrafeRight(-0.1);
		render();
		break;
	case 'v':
		Camera.StrafeRight(0.1);
		render();
		break;
	case 'f':
		Camera.Move(F3dVector(0.0, -0.3, 0.0));
		render();
		break;
	case 'r':
		Camera.Move(F3dVector(0.0, 0.3, 0.0));
		render();
		break;
	default:
		break;

	}
}

int main(int argc, char **argv)
{
//	GLuint programID = LoadShaders( "shaders/vs.glsl", "shaders/fs.glsl" );
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 480);
	window = glutCreateWindow("ebib_demo");
	Camera.Move(F3dVector(0.0, 0.0, 3.0));
//	Camera.MoveForwards(1.0);
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(KeyDown);
	glutMainLoop();
	return 0;
}

