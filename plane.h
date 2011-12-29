#ifndef PLANE_H
#define PLANE_H

#include "vecmath.h"

class Plane {

	public:
		static Plane rotate(const Plane a, Vector3 u, double angle);

		Plane();
		Plane(const Plane& other);
		Plane(const Vector3& p, const Vector3& norm);
		Plane(const Point3& a, const Point3& b, const Point3& c);

		double distanceTo(const Point3& p);
		double distanceTo(const Vector3& v);

		Vector3 normal;
		Vector3 aPoint;
		bool twoSided;
		float d;

	private:
};
#endif
