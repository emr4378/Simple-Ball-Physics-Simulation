#ifndef BALL_H
#define BALL_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#	include <gl/glut.h>
#elif defined(__APPLE__) && defined(__MACH__)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include <vector>
#include "plane.h"
#include "vecmath.h"

#define NUM_SLICES 12
#define NUM_STACKS 12
#define MASS_MULT 5

//externs defined in project3.cpp
extern float ELAST, G, FRIC;

class Ball {
	public:
		Ball();
		Ball(const Ball &ball);
		Ball(const Point3 &point, double r);
		Ball(double x, double y, double z, double r);
		~Ball();

		void draw(bool solid);

		void pull(Ball &other);
		float hitTest(Ball &other);
		float hitTest(Plane &other);
		void hitReact(Plane &other, float t);

		Ball& operator=(const Ball &ball);

		double radius;
		Point3 center;
		Vector3 velocity;
		float color[3];
		double mass;
		Point3 oldCenter;

	private:
		void hitReact(Ball &other);
};
#endif
