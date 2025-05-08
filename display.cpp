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

int selected_point = 0;
double m_slide=100;

// lighting parameters.
bool flatShading = false;
bool bezierSurfaceLighting = true;

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
	camera.x = camera.y = camera.z = m_slide;
	gluLookAt(camera.x, camera.y, camera.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	if(bezierSurfaceLighting){
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

void onMouseButton(int button, int state, int x, int y) {
	int p = selected_point;
	if (button == GLUT_MIDDLE_BUTTON && state == 0) {
		selected_point = (p + 1) % 16;
		p = selected_point;
		printf("selected point %d\n", p);
	}
	if (button == GLUT_LEFT_BUTTON && state == 0) {
		controlPoints[p].y = controlPoints[p].y + 10;
	}
	if (button == GLUT_RIGHT_BUTTON && state == 0) {
		controlPoints[p].y = controlPoints[p].y - 10;
	}

}

double B0(double t) { return pow(1 - t, 3); }
double B1(double t) { return 3*t*pow(1-t, 2); }
double B2(double t) { return 3*pow(t, 2)*(1-t); }
double B3(double t) { return pow(t, 3); }

double horner(double u, double p0, double p1, double p2, double p3) {
	double a = -p0 + 3*p1 - 3*p2 + p3;
	double b = 3*p0 - 6*p1 + 3*p2;
	double c = -3*p0 + 3*p1;
	double d = p0;

	// horner polynomial
	return ((a * u + b) * u + c) * u + d;
}

void DrawBezierSurface(){
	// plot lines
	double axis_line_len = 20 * (sqrt(controlPoints.size()) - 1);
	glBegin(GL_LINES);
		glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(axis_line_len, 0, 0);
		glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, axis_line_len, 0);
		glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, axis_line_len);
	glEnd();

	vector<Position> surface;
	double grid = 11;
	Position new_pos(0,0,0);
	for(int kx = 0; kx < grid; kx++) {
		double u = (double) kx / (grid-1);
		for(int ky = 0; ky < grid; ky++) {
			double v = (double) ky / (grid-1);

			// horner method
			double dx[4], dy[4], dz[4];
			for(int i = 0; i < 4; i++) {
				dx[i] = horner(u, controlPoints[i*4 + 0].x,
						controlPoints[i*4 + 1].x,
						controlPoints[i*4 + 2].x,
						controlPoints[i*4 + 3].x);


				dy[i] = horner(u, controlPoints[i*4 + 0].y,
						controlPoints[i*4 + 1].y,
						controlPoints[i*4 + 2].y,
						controlPoints[i*4 + 3].y);

				dz[i] = horner(u, controlPoints[i*4 + 0].z,
						controlPoints[i*4 + 1].z,
						controlPoints[i*4 + 2].z,
						controlPoints[i*4 + 3].z);
			}

			new_pos.x = horner(v, dx[0], dx[1], dx[2], dx[3]);
			new_pos.y = horner(v, dy[0], dy[1], dy[2], dy[3]);
			new_pos.z = horner(v, dz[0], dz[1], dz[2], dz[3]);
			surface.push_back(new_pos);
		}
	}

	// draw
	glBegin(GL_QUADS);
	glColor3f(1,1,1);
	for(int i = 0; i < grid-1; i++) {
		for(int j = 0; j < grid-1; j++) {
			int index = i*grid+j;
			glVertex3f(surface[index].x, surface[index].y, surface[index].z);
			glVertex3f(surface[index+1].x, surface[index+1].y, surface[index+1].z);
			glVertex3f(surface[index+grid+1].x, surface[index+grid+1].y, surface[index+grid+1].z);
			glVertex3f(surface[index+grid].x, surface[index+grid].y, surface[index+grid].z);
		}
	}
	glEnd();

	glutPostRedisplay();
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
