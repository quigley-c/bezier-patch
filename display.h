#pragma once

struct Position{
    Position() : x(0), y(0),z(0), u(0), v(0) {}
    Position(float m, float n){
      x=m; y=n; z = 0; u = 0; v = 0;
    }
    Position(float m, float n, float t){
      x=m; y=n; z = t; u = 0; v = 0;
    }
    Position(float m, float n,float t, float i, float j){
        x = m; y =n; z = t; u = i; v = j;
    }
    float x;
    float y;
    float z;
    float u;
    float v;
};

void setup();
void display();
void reshape(int, int);
void projection(int, int, int);
//void DrawCubicSpline();
void DrawBezierSurface();
