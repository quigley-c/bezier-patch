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

int main(int argc, char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(1000,500);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Spline and Surface Demo");
    setup();

    // initializing callbacks
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    //glutMouseFunc(mouse);  // define your own mouse event.
    //glutMotionFunc(motion);  // define your own motion event, e.g., rotate OBJ model.

    //Creates Menu on Right Click
    // CreateMenu();

    glutMainLoop();
    return 0;

}
