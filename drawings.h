#ifndef DRAWINGS_H
#define DRAWINGS_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#	include <gl/glut.h>
#elif defined(__APPLE__) && defined(__MACH__)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include <vector>
#include <sstream>
#include <algorithm>

#include "vecmath.h"
#include "ball.h"
#include "plane.h"

#define CROSSHAIR_WIDTH 10

//externs defined in project3.cpp
extern int cCount, cubeWid, width, height;
extern bool solid, boundsEnabled, lightningEnabled, fog, light;
extern vector<Ball*> balls;
extern double yaw, pitch;
extern Point3 cam;
extern Vector3 forwardV, upV, rightV;
extern Ball* bigBall;
extern string strArr[];
extern int userSel, shotSpeed;
extern float ELAST, G, AIRFRIC, FRIC;
extern Point3 hitPoint;
extern GLfloat ctlpoints[4][4][3];
extern GLUnurbsObj *theNurb;

enum UserSel { ELASTICITY, 
				GRAVITY, 
				AIRFRICTION, 
				FRICTION, 
				BALLSPEED, 
				COUNT = 5 };

void drawString(string str, int x, int y);

int quickHull();
void lightning(Point3 p1, Point3 p2);
void drawFloor(int wid);
void drawWalls(int wid);
void drawHUD();
void drawNurb();

void drawScene(bool nurb);
void drawMainView();
void drawSmallView();

#endif
