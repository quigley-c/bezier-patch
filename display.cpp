#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<string.h>
#include<math.h>
#include<GL/glut.h>
#include<vector>
#include<assert.h>
#include"display.h"

using namespace std;

vector<Position> controlPoints;
Position camera;

int WIDTH_WINDOWS;
int HEIGHT_WINDOWS;

double m_slide=100;

// lighting parameters.
bool flatShading = false;
bool bezierSurfaceMapping = false;
bool bezierSurfaceLighting = false;

// data for the lighting
// for x-y-z axis
GLfloat redSurface[]   = {1.0, 0.0, 0.0, 1.0};
GLfloat greenSurface[]   = {0.0, 1.0, 0.0, 1.0};
GLfloat blueSurface[]   = {0.0, 0.0, 1.0, 1.0};
GLfloat darkSurface[]   = {1.0, 0.0, 0.0, 1.0};

//for lighting
GLfloat lightAmbient[] =  {0.1, 0.1, 0.1, 1.0};
GLfloat lightDiffuse[] =  {0.7, 0.7, 0.7, 1.0};
GLfloat lightSpecular[] = {0.4, 0.4, 0.4, 1.0};
GLfloat lightPosition[] = {100, 100.0, 100.0, 0.0};
GLfloat lightDirection[] ={0.0, 0.0, -1.0};
GLfloat shininess       = 50;

// for the materials
GLfloat matAmbient [] = {0.0, 1.0, 0.0, 1.0};
GLfloat matDiffuse [] = {0.0, 1.0, 0.0, 1.0};
GLfloat matSpecular[] = {1.0, 1.0, 1.0, 1.0};

void setup() {
    glClearColor(0, 0, 0, 1.0); // *should* display black background

	int grid_w = 4;
	int grid_h = 4;
	for(int i = 0; i < grid_h; i++) {
		for(int j = 0; j < grid_w; j++) {
			// x, y, z right-handed
            controlPoints.push_back(Position(i*20, 0, j*20));
		}
	}
}

void display(){
	// glClear(GL_COLOR_BUFFER_BIT); // clear window
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT );
	glEnable(GL_DEPTH_TEST);

	glLoadIdentity();

	//set gluLookAt and gluPerspective
	projection(WIDTH_WINDOWS, HEIGHT_WINDOWS, 1); // set projection.
	// gluLookAt(100, 100, m_slide, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	camera.x = camera.y = camera.z = m_slide;
	gluLookAt(camera.x, camera.y, camera.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	if(bezierSurfaceMapping || bezierSurfaceLighting){
		// lighting

		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		//flat shading or smooth shading
		if(flatShading) glShadeModel(GL_FLAT);
		else            glShadeModel(GL_SMOOTH);

	}

	DrawBezierSurface();
    glutSwapBuffers();
}

void DrawBezierSurface(){
	// draw your own Bezier Surface here.
	// plot lines
	double axis_line_len = 20 * (sqrt(controlPoints.size()) - 1);
	glBegin(GL_LINES);
		glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(axis_line_len, 0, 0);
		glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, axis_line_len, 0);
		glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, axis_line_len);
	glEnd();

	glBegin(GL_POINTS);
	glColor3f(1, 1, 1);
	for(int i = 0; i < controlPoints.size(); i++) {
		glVertex3f(controlPoints[i].x, controlPoints[i].y, controlPoints[i].z);
	}
	glEnd();
}

void reshape( int w, int h ){
   glViewport( 0, 0, (GLsizei)w, (GLsizei)h ); // set to size of window
   glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho( 0, w, h, 0, -1, 1 );
    WIDTH_WINDOWS = w;  // records width globally
    HEIGHT_WINDOWS = h; // records height globally

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void projection(int width, int height, int perspectiveORortho){
  float ratio = (float)width/height;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (perspectiveORortho)
      gluPerspective(60, ratio, 1, 1000);
  else
      glOrtho(-ratio, ratio, -ratio, ratio, 1, 1000);
  glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
}
