/**
 * Plane class
 *
 * Contains all information needed to represent an infinite plane, as well
 * as functions to determine a Point's distance from it. Also contains method
 * to rotate Plane around axis.
 */

#include "plane.h"

using namespace std;

/**
 * Rotate function - static
 *
 * Rotates the given plane around the given axis by the given angle
 *
 * @param a - Plane, the plane to be rotated
 * @param u - Vector3, axis to rotate around
 * @param angle - double, angle to rotate by (in degrees)
 * @return Plane - the rotated plane
 */
static Plane rotate(const Plane a, Vector3 u, double angle) {
	Plane result(a);
	Vector3::rotate(result.normal, u, angle);
	Vector3::rotate(result.aPoint, u, angle);
	return result;
}

/**
 * Default constructor
 */
Plane::Plane() : normal(Vector3(0, 1, 0)), aPoint(Vector3(0, 0, 0)) {
	d = normal * aPoint;
}

/**
 * Copy constructor
 */
Plane::Plane(const Plane& other) : normal(other.normal),
							       aPoint(other.aPoint) {
	d = normal * aPoint;
}

/**
 * Point/surface normal constructor
 */
Plane::Plane(const Vector3& p, const Vector3& norm) : aPoint(p),
													 normal(norm) {
	d = normal * aPoint;
}

/**
 * 3-point constructor
 *
 * Any plane can be defined by 3 points. This function takes those 3 points
 * and performs vector math on them to get the desired values to store
 */
Plane::Plane(const Point3& a, const Point3& b, const Point3& c) {
	aPoint = Vector3(a.x, a.y, a.z);
	
	Vector3 s1 = b - a;
	Vector3 s2 = c - a;

	normal = s1^s2;
	normal.normalize();

	d = normal * aPoint;
}

/**
 * DistanceTo
 *
 * Determines how far a Point is from the plane.
 *
 * @param p - Point3&, reference to point
 * @return double - distance to point. If negative, is opposite side of normal
 */
double Plane::distanceTo(const Point3& p) {
	return distanceTo(Vector3(p.x, p.y, p.z));
}

/**
 * DistanceTo
 *
 * Determines how far a Vector is from the plane.
 *
 * @param p - Point3&, reference to point
 * @return double - distance to point. If negative, is opposite side of normal
 */
double Plane::distanceTo(const Vector3& v) {
	d = normal * aPoint;
	double ret;
	normal.normalize();

	ret = ((normal * v) + d) / normal.length();
	return ret;
}
