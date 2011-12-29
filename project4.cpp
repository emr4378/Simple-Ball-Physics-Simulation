/**
 * Project4 - Decently awesome ball/particle physics thing
 *
 * Contains main method and all OpenGL callbacks used.
 *
 * This project also makes use of vecmath.h provided in a previous project,
 * because it's the shit.
 *
 * Randomize all the things!
 *
 * @author Eduardo Rodrigues - emr4378
 */

#include <cmath>
#include <cstdlib>
#include <vector>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#	include <gl/glut.h>
#elif defined(__APPLE__) && defined(__MACH__)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include "drawings.h"
#include "vecmath.h"
#include "ball.h"
#include "plane.h"

//define keyboard input constants
#define KEY_EXIT 'q'
#define KEY_FORWARD 'w'
#define KEY_BACKWARD 's'
#define KEY_LEFT 'a'
#define KEY_RIGHT 'd'
#define KEY_UP 32 //Spacebar
#define KEY_DOWN 'z'
#define KEY_TAB 9 //TAB
#define KEY_RESET 'r'
#define KEY_WIREFRAME 49 //1
#define KEY_SHADED 50 //2
#define KEY_FOG 'f'
#define KEY_LIGHT 'l'
#define KEY_DEC '='
#define KEY_INC '-'
#define KEY_T_BOUND 'b'
#define KEY_T_LIGHTNING 'v'

//define constants for user interactivity
#define MOVE_SPEED 7
#define MOVE_MULT 3
#define USER_VAL_DIFF .01f
#define GRAV_DIFF .00001f
#define MAX_DIST_SQ 1000000
#define PI 3.141592654

int width = 800;
int height = 600;

float AIRFRIC = 1.0f; //air resistance
float FRIC = 1.0f; //friction of wall collisions
float ELAST = 0.5f; //elasticity of collisions
float G = 0.01f; //gravity
int shotSpeed = 10;

int timerWait = 35;
int cubeWid = 200;
float moveSpeed;
int oldx, oldy, cCount; 
double yaw, pitch;
Point3 cam;
Vector3 gravity(0.0, 0.0, 0.0);
Vector3 forwardV, upV, rightV;
bool keys[256];
bool solid, subcCount, attract, boundsEnabled, lightningEnabled, shooting;
int userSel = ELASTICITY;
vector<Ball*> balls;
Ball* bigBall;

bool fog, light;
Point3 hitPoint(0, 0, cubeWid-.1);
GLfloat ctlpoints[4][4][3];
GLUnurbsObj *theNurb;

string strArr[] = { "Q - quit",
					"Mouse - aim",
					"Left click - shoot",
					"Right click - push big ball",
					"WASD - movement",
					"Space/Z - up/down",
					"Shift - fast cam",
					"R - reset scene",
					"1 - render wire objects",
					"2 - render flat objects",
					"L - lights toggle",
					"F - fog toggle",
					"B - level toggle",
					"V - lightning toggle (SLOW!)",
					"+/- - inc/dec current variable",
					"Tab - change selected variable" };

/**
 * init function
 *
 * Initializes scene; resets variables and repositions user camera.
 * Used for resetting scene (KEY_RESET, should be 'r').
 *
 * Initial scene consists of one giant ball/sphere and a ton of smaller ones
 */
void init() {
	fog = true;
	light = true;
	cCount = yaw = pitch = 0;
	boundsEnabled = true;
	lightningEnabled = false;
	cam = Point3(0.0, 0.0, -cubeWid);


	vector<Ball*>::iterator iter;
	for (iter = balls.begin(); iter != balls.end(); iter++) {
		delete *iter;
	}
	balls.clear();
	int amt = 12;
	for (int x = -amt/2; x < amt/2; x++) {
		for (int z = -amt/2; z < amt/2; z++) {
			int i = (x*20) + z;
			float rad = rand() % 3 + 2;
			Ball* ball = new Ball(	0, ((z - 10) * 5) + 2, 0, rad);

			(*ball).velocity.x = rand() % 4 - 2;
			(*ball).velocity.y = rand() % 4 - 2;
			(*ball).velocity.z = rand() % 4 - 2;

			(*ball).color[0] = (float)rand()/(float)RAND_MAX;
			(*ball).color[1] = (float)rand()/(float)RAND_MAX;
			(*ball).color[2] = (float)rand()/(float)RAND_MAX;

			balls.push_back(ball);
		}
	}

	bigBall = new Ball(0, 0, 0, 20);
	(*bigBall).mass = 10000;

	(*bigBall).color[0] = (float)rand()/(float)RAND_MAX;
	(*bigBall).color[1] = (float)rand()/(float)RAND_MAX;
	(*bigBall).color[2] = (float)rand()/(float)RAND_MAX;

	balls.insert(balls.begin(), bigBall);
}

/**
 * timerTime callback function
 *
 * Oh lordy theres a lot going on in here. 
 * First we make sure all user modifiable values are within valid ranges.
 * Then we handle all user input (mouse and key presses).
 * After that we update all the balls (position, hittesting, etc).
 * Finally, we request a redisplay (most likely something changed)
 *
 * @param data - int, random value required to be passed (should be 0)
 */
void timerTime(int data) {
	//START: Range checking
	if (shotSpeed < 0) shotSpeed = 0;
	if (AIRFRIC > 1.0f) AIRFRIC = 1.0f;
	if (AIRFRIC < 0.0f) AIRFRIC = 0.0f;
	if (ELAST > 1.0f) ELAST = 1.0f;
	if (ELAST < 0.0f) ELAST = 0.0f;
	if (FRIC > 1.0f) FRIC = 1.0f;
	if (FRIC < 0.0f) FRIC = 0.0f;
	if (G > 1.0f) G = 1.0f;
	if (G < 0.0f) G = 0.0f;
	//END

	//START: User input handling
	if (shooting) {
		Ball *temp = new Ball(cam.x, cam.y, cam.z, rand() % 5 + 2);
		temp->center -= rightV * 10;
		(*temp).color[0] = (float)rand()/(float)RAND_MAX;
		(*temp).color[1] = (float)rand()/(float)RAND_MAX;
		(*temp).color[2] = (float)rand()/(float)RAND_MAX;
		temp->velocity = Vector3::rotate(forwardV, upV, 1) * shotSpeed;
		balls.push_back(temp);
	}

	if(keys[KEY_FORWARD]) {
		cam.x += sin(yaw * PI/180) * moveSpeed;
		cam.z += cos(yaw * PI/180) * moveSpeed;
	} else if (keys[KEY_BACKWARD]) {
		cam.x -= sin(yaw * PI/180) * moveSpeed;
		cam.z -= cos(yaw * PI/180) * moveSpeed;
	}

	if (keys[KEY_LEFT]) {
		cam.x += cos(yaw * PI/180) * moveSpeed;
		cam.z -= sin(yaw * PI/180) * moveSpeed;
	} else if (keys[KEY_RIGHT]) {
		cam.x -= cos(yaw * PI/180) * moveSpeed;
		cam.z += sin(yaw * PI/180) * moveSpeed;
	}

	if (keys[KEY_UP]) {
		cam.y += moveSpeed;	
	} else if (keys[KEY_DOWN]) {
		cam.y -= moveSpeed;
	}

	if (keys[KEY_EXIT]) {
		exit(0);
	}

	if (keys[KEY_RESET]) {
		init();
	}

	if (keys[KEY_WIREFRAME] && solid) {
		solid = false;
	} else if (keys[KEY_SHADED] && !solid) {
		solid = true;
	}

	if (keys[KEY_INC]) {
		switch (userSel) {
			case ELASTICITY:
				ELAST -= USER_VAL_DIFF;
				break;
			case GRAVITY:
				G -= USER_VAL_DIFF/100;
				break;
			case AIRFRICTION:
				AIRFRIC -= USER_VAL_DIFF/10;
				break;
			case FRICTION:
				FRIC -= USER_VAL_DIFF/10;
				break;
			case BALLSPEED:
				shotSpeed -= 1;
				break;
		}
	} else if (keys[KEY_DEC]) {
		switch (userSel) {
			case ELASTICITY:
				ELAST += USER_VAL_DIFF;
				break;
			case GRAVITY:
				G += USER_VAL_DIFF/100;
				break;
			case AIRFRICTION:
				AIRFRIC += USER_VAL_DIFF/10;
				break;
			case FRICTION:
				FRIC += USER_VAL_DIFF/10;
				break;
			case BALLSPEED:
				shotSpeed += 1;
				break;
		}
	}

	if (subcCount) {
		cCount --;
	 } else {
		cCount++;
	}

	if (cCount <= 0) {
		subcCount = false;
		cCount = 0;
	} else if (cCount >= cubeWid) {
		subcCount = true;
		cCount = cubeWid;
	}

	if (attract) {
		Vector3 vectTo = (*bigBall).center - cam;
		vectTo.normalize();

		(*bigBall).velocity = -vectTo * moveSpeed;
	}
	//END
	
	//START: Ball updating and hit testing
	for (int i = 0; i < balls.size(); i++) {
		Ball* temp = balls[i];		
		if (bigBall->center.distanceToSquared(temp->center) >= MAX_DIST_SQ) {
			//remove ball if way far away
			balls.erase(balls.begin() + i);
		} else {
			//update position/velocity
			temp->center += temp->velocity;
			temp->velocity += gravity;
			temp->velocity *= AIRFRIC;

			//interact with all other balls
			for (int j = i+1; j < balls.size(); j++) {
				temp->pull(*balls[j]);
				temp->hitTest(*balls[j]);
			}

			//hittest the boundaries
			if (boundsEnabled) {
				if (temp->center.x < -cubeWid + temp->radius) {
					temp->center.x = -cubeWid + temp->radius;
					temp->velocity.x *= -ELAST;
					temp->velocity *= FRIC;
					if (temp == bigBall) {
						hitPoint = temp->center;
						hitPoint.z = cubeWid-.1;
					}
				} else if (temp->center.x > cubeWid - temp->radius) {
					temp->center.x = cubeWid - temp->radius;
					temp->velocity.x *= -ELAST;
					temp->velocity *= FRIC;
				}
				if (temp->center.y < -cubeWid + temp->radius) {
					temp->center.y = -cubeWid + temp->radius;
					temp->velocity.y *= -ELAST;
					temp->velocity *= FRIC;
				} else if (temp->center.y > cubeWid - temp->radius) {
					temp->center.y = cubeWid - temp->radius;
					temp->velocity.y *= -ELAST;
					temp->velocity *= FRIC;
				}
				if (temp->center.z < -cubeWid + temp->radius) {
					temp->center.z = -cubeWid + temp->radius;
					temp->velocity.z *= -ELAST;
					temp->velocity *= FRIC;
				} else if (temp->center.z > cubeWid - temp->radius) {
					temp->center.z = cubeWid - temp->radius;
					temp->velocity.z *= -ELAST;
					temp->velocity *= FRIC;
				}
			}

			temp->oldCenter = temp->center;
		}
	}
	//END

	glutPostRedisplay();
	glutTimerFunc(timerWait, timerTime, 0);
}

/**
 * displayFunc callback
 * 
 * Calls all the methods that handle drawing to the screen
 */
void displayFunc() {
	drawMainView();
	drawHUD();
	drawSmallView();

    glFlush();
	glutSwapBuffers();	
}

/**
 * keyboardFunc callback
 *
 * Handles the keyboard key presses. Updates boolean value in an array
 * for checking in the timerFunc. Useful for keys that will be continously
 * held down, like movement (otherwise shit had a delay/was choppy and Sonic
 * says: "That's nooooo good.")
 *
 * Does handle toggles though, since who cares if they're continously
 * pressed.
 *
 * @param key - char, the key being pressed
 * @param x - int, the x position of the mouse
 * @param y - int, the y position of the mouse
 */
void keyboardFunc(unsigned char key, int x, int y) {
	if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
		moveSpeed = MOVE_MULT * MOVE_SPEED;
		if (key >= 65 && key <= 90) {
			key += 32;
		}
	} else {
		moveSpeed = MOVE_SPEED;
	}
	switch (key) {
		case KEY_T_BOUND:
			boundsEnabled = !boundsEnabled;
			break;
		case KEY_T_LIGHTNING:
			lightningEnabled = !lightningEnabled;
			break;
		case KEY_TAB:
			userSel = (userSel + 1) % COUNT;
			break;
		case KEY_LIGHT:
			light = !light;
			break;
		case KEY_FOG:
			fog = !fog;
			break;
	}
	keys[key] = true;
}

/**
 * keyboardUpFunc callback
 *
 * Handles the keyboard key releases. Updates boolean value in an array
 * for checking in the timerFunc. Useful for keys that will be continously
 * held down, like movement (otherwise shit had a delay/was choppy and Sonic
 * says: "That's nooooo good.")
 *
 * @param key - char, the key being pressed
 * @param x - int, the x position of the mouse
 * @param y - int, the y position of the mouse
 */
void keyboardUpFunc(unsigned char key, int x, int y) {
	if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
		moveSpeed = 3 * MOVE_SPEED;
		if (key >= 65 && key <= 90) {
			key += 32;
		}
	} else {
		moveSpeed = MOVE_SPEED;
	}
	keys[key] = false;
}

/**
 * reshapeFunc callback
 *
 * Called when the screen is reshaped/resized (WOAH, crazy).
 * Stores the new width and height, repositions the mouse to the center of
 * the screen.
 *
 * @param w - int, new width
 * @param h - int, new height
 */
void reshapeFunc(int w, int h) {
	width = w;
	height = h;
	oldx = w/2;
	oldy = h/2;
	glutWarpPointer(oldx, oldy);
}  

/**
 * mouseFunc callback
 * 
 * Called when a mouse button changes state
 * Mine only deals with left and right clicking, because meh.
 *
 * @param button - int, value indicating button changed
 * @param state - int, value indicating state of button
 * @param x - int, x position of mouse
 * @param y - int, y position of mouse
 */
void mouseFunc(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
		shooting = (state == GLUT_DOWN);
    }

	if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			Vector3 vectTo = (*bigBall).center - cam;
			vectTo.normalize();
			
			if(glutGetModifiers() != GLUT_ACTIVE_SHIFT) {
				attract = true;
			} else {
				(*bigBall).velocity = vectTo*15;
			}
		} else {
			attract = false;
		}
	}
}

/**
 * mouseMoved function
 *
 * Updates the yaw and pitch for the camera, based on current x, y of
 * mouse and previous x, y of mouse. Then calls for redisplay, since camera
 * has rotated some way.
 *
 * Prevents camera from spinning vertically (can only look straight up/down).
 *
 * @param x - int, current x position of mouse
 * @param y - int, current y position of mouse
 */
void mouseMoved(int x, int y) {
	float div = 10;
	float diffx = (x-oldx)/div;
	float diffy = (y-oldy)/div;

	double r = 20;
	yaw -= diffx;
	pitch += diffy;

	if (pitch >= 90) pitch = 90;
	if (pitch <= -90) pitch = -90;

	if (diffx != 0  || diffy != 0) {
		glutPostRedisplay();
		glutWarpPointer(oldx, oldy);
	}
}

/**
 * motionFunc callback
 * 
 * Called when mouse moves and left/primary button is down.
 * Calls the mouseMoved to update the camera.
 * This comment was completely necessary.
 *
 * @param x - int, x position of mouse
 * @param y - int, y position of mouse
 */
void motionFunc(int x, int y) {
	mouseMoved(x, y);
}

/**
 * passiveMotionFunc callback
 * 
 * Called when mouse moves and left/primary button is up.
 * Calls the mouseMoved to update the camera.
 * This comment was even more necessary.
 *
 * @param x - int, x position of mouse
 * @param y - int, y position of mouse
 */
void passiveMotionFunc(int x, int y) {
	mouseMoved(x, y);
}

/**
 * idleFunc callback
 *
 * This function does nothing. It gets called whenever no other callbacks
 * are being called. Strout said to avoid using this, but the writeup said
 * we needed to. 
 *
 * This way everybody wins!
 */
void idleFunc() {
}


int main (int argc, char *argv[]) {
	srand(0);

	//set up initial window display
    glutInit(&argc, argv);  
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);    
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("Eduardo Rodrigues - Project3");  

    // register callback functions
    glutDisplayFunc(displayFunc);
    glutMouseFunc(mouseFunc);
    glutMotionFunc(motionFunc);
    glutPassiveMotionFunc(passiveMotionFunc);
    glutKeyboardFunc(keyboardFunc);
	glutKeyboardUpFunc(keyboardUpFunc);
    glutReshapeFunc(reshapeFunc);
	glutIdleFunc(idleFunc);
	glutTimerFunc(timerWait, timerTime, 0);

	//remove cursor and enable fullscreen
	glutSetCursor(GLUT_CURSOR_NONE);
	glutFullScreen();

    //initialize the stuffs and run the loop
    init();
    glutMainLoop();

    return 0;
}

