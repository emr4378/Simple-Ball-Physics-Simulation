/**
 * Ball class
 *
 * This shit is sick. Stores all information relevent to a ball and 
 * contains method for drawing the ball.
 * 
 * Also has methods for hittests balls can perform against various objects.
 *
 * @author Eduardo Rodrigues - emr4378
 */

#include "ball.h"
using namespace std;

/**
 * Default constructor
 *
 * Initializes center to origin (0, 0, 0) and radius to 1
 */
Ball::Ball() : center(0, 0, 0), oldCenter(center), 
				radius(1), mass(MASS_MULT) {}

/**
 * Copy constructor
 *
 * @param ball - Ball&, reference to Ball object to be copied
 */
Ball::Ball(const Ball &ball) : center(ball.center), 
								radius(ball.radius), 
								mass(ball.mass),
								oldCenter(ball.center) {}

/**
 * Point/radius constructor
 *
 * @param point - Point3&, reference to Point3 object to be center of ball
 * @param r - double, radius of ball
 */
Ball::Ball(const Point3 &point, double r) : center(point), 
											radius(r), 
											mass(r*MASS_MULT),
											oldCenter(point) {}

/**
 * Point-component/radius constructor
 *
 * @param x - double, x position of center point
 * @param y - double, y position of center point
 * @param z - double, z position of center point
 */
Ball::Ball(double x, double y, double z, double r) : center(x, y, z), 
													 radius(r),
													 mass(r*MASS_MULT),
													 oldCenter(x, y, z) {}
/**
 * Deconstructor
 */
Ball::~Ball() {
}

/**
 * Draw function
 *
 * Handles drawing the ball. Pushes/Pops matrices so transformation matrix
 * will be same state before and after call.
 *
 * @param solid - bool, indicates if wireframe or flat/solid
 */
void Ball::draw(bool solid) {
	glColor3f(color[0], color[1], color[2]);
	glPushMatrix();
		glTranslatef(center.x, center.y, center.z);
		if (solid) {
			glutSolidSphere(radius, NUM_SLICES, NUM_STACKS);
		} else {
			glutWireSphere(radius, NUM_SLICES, NUM_STACKS);
		}
	glPopMatrix();
}

/**
 * Pull function
 * 
 * Handles the gravity interaction between this and another ball.
 *
 * @param other - Ball&, reference to ball to interact with
 */
void Ball::pull(Ball &other) {
	if (G != 0) {
		Vector3 vectTo = center-other.center;
		float grav = G * mass * other.mass / (vectTo.lengthSquared());

		vectTo.normalize();
		vectTo *= grav;

		velocity -= vectTo/mass;
		other.velocity += vectTo/other.mass;
	}
}

/**
 * HitReact function
 *
 * Handles reaction between this and another ball after collision.
 *
 * @param other - Ball&, reference to ball to interact with
 */ 
void Ball::hitReact(Ball &other) {
	Vector3 vectTo = center - other.center;
	float d = vectTo.length();
	Vector3 mtd =  vectTo * (((radius + other.radius)-d)/d); 

	float im1 = 1 / mass; 
	float im2 = 1 / other.mass;

	center += (mtd*(im1 / (im1 + im2)));
	other.center -= (mtd * (im2 / (im1 + im2)));

	Vector3 relVel = velocity-other.velocity;

	double impulse = vectTo*(relVel * -(1.0f + ELAST));
	impulse /= vectTo*(vectTo * (1/mass + 1/other.mass));

	Vector3 rA = vectTo*(impulse/mass);
	Vector3 rB = vectTo*(-impulse/other.mass);

	velocity += rA;
	other.velocity += rB;
}

/**
 * HitTest function
 *
 * Determines if this and the given ball are colliding or would be colliding
 * between current timestep (t = 0) and next timestep (t = 1). Basically
 * sees if the velocity vectors intersect and, if so, when.
 *
 * @param other - Ball&, reference to ball to interact with
 * @return float - time, in [0, 1], when collision occurs, or -1 if none
 */
float Ball::hitTest(Ball &other) {
	double rab = radius + other.radius;

	if (center.distanceToSquared(other.center) < pow(rab, 2)) {
		hitReact(other);
		return 0;
	} else {
		double t1 = -1;
		double t2 = -1;

		Vector3 AB = other.center-center;
		Vector3 vab = other.velocity-velocity;

		double a = vab * vab;
		double b = 2 * (vab * AB);
		double c = (AB * AB) - (rab*rab);
		double disc = b*b - 4 * a * c;

		if (disc >= 0 && a != 0) {
			//overlapping in future
			t1 = (-b + sqrt(b*b - 4 * a * c)) / (2*a);
			t2 = (-b - sqrt(b*b - 4 * a * c)) / (2*a);
			if (t1 > t2) {
				swap(t1, t2);
			}
		}

		if (t1 >= 0 && t1 <= 1) {
			//only care if within next 1 "timestep"
			center += velocity *t1;
			other.center += other.velocity * t1;
			hitReact(other);
			return t1;
		} else {
			return -1;
		}
	}
}

/**
 * HitReact function
 *
 * Handles reaction between this and an infinite plane
 *
 * @param other - Plane&, reference to plane to interact with
 * @param t - float, timestep at which collision occurs
 */ 
void Ball::hitReact(Plane &other, float t) {
	Vector3 pStart(center.x, center.y, center.z);
	Vector3 norm = other.normal;

	center += (velocity * t);
	velocity -= (1.0f + ELAST) * (velocity * norm) * norm;

	velocity *= FRIC;
}

/**
 * HitTest function
 *
 * Determines if this and the given plane are colliding or would be colliding
 * between current timestep (t = 0) and next timestep (t = 1). 
 *
 * Does this by determining what side the ball is currently on, and what side
 * it will be on at next timestep. If different sides, collision must have
 * occured.
 *
 * @param other - Ball&, reference to ball to interact with
 * @return float - time, in [0, 1], when collision occurs, or -1 if none
 */
float Ball::hitTest(Plane &other) {
	Vector3 tempVel = velocity;
	tempVel.normalize();

	Vector3 pStart(center.x, center.y, center.z);
	Vector3 pEnd = pStart + velocity;

	int start = 0;
	int end = 0;
	float dist = other.distanceTo(pStart);
	if (dist < -radius) {
		start = -1;
	} else if (dist > -radius) {
		start = 1;
	}

	dist = other.distanceTo(pEnd);
	if (dist < -radius) {
		end = -1;
	} else if (dist > -radius) {
		end = 1;
	}

	Vector3 norm = other.normal;
	if (start < end) {
		float t = -(other.distanceTo(pStart) + radius) / 
					(norm * velocity);
		return t;
	} else {
		return -1;
	}
}

/**
 * Assignment operator
 *
 * @param ball - const Ball&, reference to Ball object to be assigned
 * @return Ball& - reference to this Ball object
 */
Ball& Ball::operator=(const Ball &ball){
	if (this != &ball) {
		center = ball.center;
		radius = ball.radius;
		mass = ball.mass;
		velocity = ball.velocity;
		for (int i = 0; i < 3; i++) {
			color[i] = ball.color[i];
		}
		oldCenter = ball.oldCenter;
	}
	return *this;
}

