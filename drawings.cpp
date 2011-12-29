/**
 * Drawings
 *
 * Contains all methods related to drawing (viewports, scenes, effects, etc).
 *
 * The project4.cpp file was getting a bit big, so I split it up and put
 * everything related to drawing in here. Thats why if you check the header
 * file you'll see basically all the variables from project3.cpp have been
 * shared/made visible here (extern-ilized).
 *
 * @author Eduardo Rodrigues - emr4378
 */

#include "drawings.h"

/**
 * DrawString function
 *
 * Stupid OpenGL/GLUT doesn't have a method to print a string to the screen,
 * only individual characters. This function solves that, at least for 2D
 * text which is all I needed for the HUD.
 *
 * @param str - string, the string to be printed onto the screen 
 * @param x - int, the x position where text should be printed
 * @param y - int, the y position where text should be printed
 * @param top - bool, indicates wheteher (x, y) is topleft or bottom left
 */
void drawString(string str, int x, int y, bool top) {
	if (top) {
		glRasterPos2i(x, y-18);
	} else {
		glRasterPos2i(x, y);
	}
	for(unsigned int i = 0; i < str.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
	}
}


/**
 * Lightning function
 *
 * Draws a "lightning bolt" type thing between 2 points.
 *
 * Bahahaha oh dear lord what this generates looks like shit without any
 * type of lighting or glowing. Hopefully will be better in Project4, but for
 * now whatevs.
 *
 * @param p1 - Point3, start point of bolt
 * @param p2 - Point3, end point of bolt
 */
void lightning(Point3 p1, Point3 p2) {
	Vector3 vectTo = p2-p1;
	Point3 curP = p1;

	int div = 12;
	int rMax = (vectTo.length()/div) + rand() % 10;
	if (rMax == 0) {
		rMax = 1;
	}
	vectTo = vectTo / div;

	glBegin(GL_LINE_STRIP);
		glVertex3f(p1.x, p1.y, p1.z);

		for (int i = 1; i < div; i++) {
			curP = p1 + (vectTo * i);
			float rX = rand() % rMax  - rMax/2;
			float rY = rand() % rMax  - rMax/2;
			float rZ = rand() % rMax  - rMax/2;
			Vector3 random(	rX, rY, rZ);
	
			curP += random;	
			glVertex3f(curP.x, curP.y, curP.z);

		}
		glVertex3f(p2.x, p2.y, p2.z);
	glEnd();
	
}

void clearLight(GLenum light) {
	float blank[] = {0.0, 0.0, 0.0};
	glLightfv(light, GL_AMBIENT, blank);
	glLightfv(light, GL_DIFFUSE, blank);
	glLightfv(light, GL_SPECULAR, blank);
}

void clearMaterial() {
	float blank[] = {0.0, 0.0, 0.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blank);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blank);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blank);
}

void drawSpotlights(int wid) {
	GLenum lArr[4] = { GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7 };
	for (int i = 0; i < 4; i++) {
		glPushMatrix();
			float color[4] = {	rand() % 10/10.0f, 
								rand() % 10/10.0f, 
								rand() % 10/10.0f,
							 	1};

			glRotatef(i*90, 0, 1, 0);
			float d = cubeWid - wid/2;


			Point3 pos(d, d, d);
			Point3 tP = Point3::rotate(bigBall->center, Vector3(0, 1, 0), -i*90);
			Vector3 dest = tP - pos;
			Vector3 orig(0, 0, -1);
			dest.normalize();
			orig.normalize();

			float tempAngle = orig*dest;
			tempAngle = acos(tempAngle)*180/PI;
			Vector3 axis = orig^dest;

			glTranslatef(d, d, d);
			glRotatef(tempAngle, axis.x, axis.y, axis.z);
			glScalef(1, 1, 2);
		
			if (light) {
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);
			}
			glColor3f(	color[0], 
						color[1], 
						color[2]);
		
			if(solid) {
				glutSolidCube(wid/2);
			} else {
				glutWireCube(wid/2);
			}

			glLightfv(lArr[i], GL_DIFFUSE, color);
			glLightfv(lArr[i], GL_AMBIENT, color);
			
			float specArr[] = {.1, .1, .1, 1};
			glLightfv(lArr[i], GL_SPECULAR, specArr);

			float posArr[] = { 0, 0, 0, 1};
			glLightfv(lArr[i], GL_POSITION, posArr);

			float cut = 5.0f;
			glLightfv(lArr[i], GL_SPOT_CUTOFF, &cut);

			float dirArr[] = {0, 0, -1};
			glLightfv(lArr[i], GL_SPOT_DIRECTION, dirArr);
			glEnable(lArr[i]);
			clearMaterial();
		glPopMatrix();
	}
}

/**
 * DrawFloor function
 * 
 * Draws the floor of the scene, which is this random color flashing tile
 * thing. I loves it.
 *
 * @param wid - int, width of each individual tile
 */
void drawFloor(int wid) {
	glPushMatrix();
		glTranslatef(0, -cubeWid, 0);
		float xArr[4] = {0, wid, wid, 0};
		float zArr[4] = {0, 0, wid, wid};
		float pos[] = {-cubeWid, 0, -cubeWid};
		glLightfv(GL_LIGHT1, GL_POSITION, pos);
		for (int x = -cubeWid; x < cubeWid; x += wid) {
			for (int z = -cubeWid; z < cubeWid; z += wid) {
				glPushMatrix();
					float col[] = {rand() % 10/10.0f, 
									rand() % 10/10.0f, 
									rand() % 10/10.0f};
					
					if (light) {
						glNormal3f(0.0, 1.0, 0.0);
						glMaterialfv(GL_FRONT_AND_BACK, 
										 GL_DIFFUSE, 
										 col);
					}
					
					glTranslatef(x, 0, z);
					if (solid) {
						glBegin(GL_QUADS);
					} else {
						glBegin(GL_LINE_LOOP);
					}
					for (int t = 0; t < 4; t++) {
						glColor3f(	col[0], 
									col[1], 
									col[2]);
						glVertex3f(xArr[t], 0, zArr[t]);
					}
					glEnd();
				glPopMatrix();
			}
		}
	glPopMatrix();
}

/**
 * DrawWalls function
 *
 * Draws the walls (all 4) of the scene. Each wall is a different type of
 * object just to mix things up a bit.
 *
 * @param wid - int, width of each individual tile
 */
void drawWalls(int wid) {
	for (int i = 0; i < 360; i += 90) {
		glPushMatrix();
			glRotatef(i, 0, 1, 0);
			glTranslatef(0, 0, cubeWid+wid/2);
			for (int x = -cubeWid; x < cubeWid; x += wid) {
				for (int y = -cubeWid; y < cubeWid; y += wid) {
					glPushMatrix();
						float color[3] = {abs(x)/(float)cubeWid, 
	 									  abs(y)/(float)cubeWid, 
										  cCount/(float)cubeWid};

						if (light) {
							glMaterialfv(GL_FRONT_AND_BACK, 
										 GL_DIFFUSE, 
										 color);
						}


						glColor3f(	color[0], 
									color[1], 
									color[2]);
	
						glTranslatef(x+wid/2, y+wid/2, 0);
						if (solid) {
							switch (i) {
								case 0:
									glutSolidCube(wid);
									break;
								case 90:
									glRotatef(180, 0, 1, 0);
									glutSolidCone(wid/2, wid/2, 
												 10, 10);
									break;
								case 180:
									glutSolidTorus(wid/5, wid/3, 
												  6, 6);
									break;
								case 270:
									glScalef(wid/3, wid/3, wid/3);
									glutSolidDodecahedron();
									break;
							}
						} else {
							switch (i) {
								case 0:
									glutWireCube(wid);
									break;
								case 90:
									glRotatef(180, 0, 1, 0);
									glutWireCone(wid/2, wid/2, 
												 10, 10);
									break;
								case 180:
									glutWireTorus(wid/5, wid/3, 
												  6, 6);
									break;
								case 270:
									glScalef(wid/3, wid/3, wid/3);
									glutWireDodecahedron();
									break;
							}
						}
					glPopMatrix();
				}
			}
		glPopMatrix();
	}
}

/**
 * DrawHUD function
 *
 * This function draws the Heads up display. I know. Craziness. I should 
 * really start naming my function better. Sorry about that.
 *
 * Draws the awesome instructions/keybindings, user modifiable vars and this
 * sexy as hell arrow that points to big ball at all times (I lost it alot).
 * To draw text I had to make my own function; it's somewhere up above.
 */
void drawHUD() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, width, 0.0, height, -50, 50);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
			glDisable(GL_DEPTH_TEST);

			//START: Instruction drawing
			glColor3f(1, 1, 1);
			for (int i = 0; i < 15; i++) {
				drawString(strArr[i], 0, height - i*15, true);
			}
			//END

			//START: Modifiable vars drawing
 			stringstream bleh;
			bleh << "Particles: ";
			bleh << balls.size();
			drawString(bleh.str(), width*4/5, height*4/5, true);

			for (int i = 0; i < COUNT; i++) {
				if (userSel == i) {
					glColor3f(0, 1, 1);
				} else {
					glColor3f(1, 1, 1);
				}
				bleh.str("");
				switch (i) {
					case ELASTICITY:
						bleh << "Elasticity: ";
						bleh << ELAST;
						break;
					case GRAVITY:
						bleh << "Gravity: ";
						bleh << G;
						break;
					case AIRFRICTION:
						bleh << "Air Friction: ";
						bleh << AIRFRIC;
						break;
					case FRICTION:
						bleh << "Friction: ";
						bleh << FRIC;
						break;	
					case BALLSPEED:
						bleh << "Shot speed: ";
						bleh << shotSpeed;
				}
				drawString(bleh.str(), width*4/5, height*4/5-(i+1)*15, true);
			}
			//END

			//START: Crosshair drawing
			glColor3f(1, 1, 1);
			glEnable(GL_COLOR_LOGIC_OP);
			glLogicOp(GL_XOR);

			glLineWidth(CROSSHAIR_WIDTH/2);
			glBegin(GL_LINES);  
				glVertex2f(width/2 - CROSSHAIR_WIDTH, height/2);    
				glVertex2f(width/2 + CROSSHAIR_WIDTH, height/2);  
				glVertex2f(width/2, height/2 - CROSSHAIR_WIDTH);    
				glVertex2f(width/2, height/2 + CROSSHAIR_WIDTH);
			glEnd();  
			glLineWidth(1);
			glDisable(GL_COLOR_LOGIC_OP);
			//END

			//START: draw arrow pointing to bigBall
			Vector3 vectTo = cam - bigBall->center;
			vectTo = Vector3::rotate(vectTo, upV, -yaw);
			vectTo = Vector3::rotate(vectTo, rightV, -pitch);
			vectTo.normalize();
			vectTo *= 90;

			glTranslatef(width/2, height-50, 0);
			glRotatef(vectTo.x, 0, 1, 0);
			glRotatef(vectTo.y, 1, 0, 0);
			glutWireCone(30, 50, 6, 5);
			//END
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

/**
 * DrawNurb function
 *
 * Draws the sexiest freaking Nurb you ever did see. Look at it. Seriously,
 * just look at it. Watch how it follows the big ball. Did you notice it's
 * getting lit correctly by both the spotlights AND the bigball light?
 * That shit took hours, son.
 */
void drawNurb() {
	float xArr[4] = {-cubeWid, -cubeWid/3, cubeWid/3, cubeWid};
	float zArr[4] = {-cubeWid, -cubeWid/3, cubeWid/3, cubeWid};

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if ((x == 1 || x == 2) && (y == 1 || y == 2)) {
				ctlpoints[x][y][0] = bigBall->center.x;
				ctlpoints[x][y][1] = bigBall->center.y;
				ctlpoints[x][y][2] = bigBall->center.z;
			} else {
				ctlpoints[x][y][0] = xArr[x];
				ctlpoints[x][y][1] = cubeWid;
				ctlpoints[x][y][2] = zArr[y];
			}
		}
	}

	if (theNurb == NULL) {
		theNurb = gluNewNurbsRenderer();
		gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 10.0);
		gluNurbsProperty(theNurb, GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
		gluNurbsProperty(theNurb, GLU_U_STEP, 20);
		gluNurbsProperty(theNurb, GLU_V_STEP, 20);
	}
	float color[] = { 0.0f, 1.0f, 0.0f, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	glColor3f(	color[0], 
				color[1], 
				color[2]);
	if(solid) {
		gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
	} else {
		gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON);
	}



  	GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
	glPushMatrix();
		glNormal3f(0.0, -1.0, 0.0);
		gluBeginSurface(theNurb);
			gluNurbsSurface(theNurb, 
					8, knots,
					8, knots,
					4 * 3,
					3,
					&ctlpoints[0][0][0], 
					4, 4,
					GL_MAP2_VERTEX_3);
		gluEndSurface(theNurb);
   glPopMatrix();
}

/**
 * DrawScene function
 *
 * Loops thru and tells all Ball objects to draw, determines if lightning
 * effects should be drawn and calls functions to draw floor and walls.
 *
 * @param nurb - boolean, indicates whether or not to draw nurb
 */
void drawScene(bool nurb) {
	if (fog) {
		glEnable (GL_FOG);
		glFogi (GL_FOG_MODE, GL_EXP);
		float fogColor[] = {0, 0, 1, 1};
		glFogfv (GL_FOG_COLOR, fogColor);
		float density = .001;
		glFogf (GL_FOG_DENSITY, density);
	}

	if (light) {
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);

		clearLight(GL_LIGHT0);
		glEnable(GL_LIGHT0);

		if (boundsEnabled) {
			drawSpotlights(cubeWid/10);
		}
		clearMaterial();
	}

	
	for (unsigned int i = 0; i < balls.size(); i++) {
		if (light) {
			glMaterialfv(GL_FRONT, GL_DIFFUSE, balls[i]->color);
			if (balls[i] == bigBall) {
				float eCol[4] = {balls[i]->color[0],
								 balls[i]->color[1],
								 balls[i]->color[2],
								 .5f};
				glMaterialfv(GL_FRONT_AND_BACK, 
							 GL_EMISSION, 
							 eCol);
				float pos[] = { balls[i]->center.x,
								balls[i]->center.y,
								balls[i]->center.z, 
								1};
				glLightfv(GL_LIGHT0, GL_POSITION, pos);
				glLightfv(GL_LIGHT0, GL_DIFFUSE, bigBall->color);
			}
		}
		balls[i]->draw(solid);
		
		for (unsigned int j = i+1; lightningEnabled && j < balls.size(); j++) {
			if ((*balls[i]).center.distanceToSquared((*balls[j]).center) 
				< 400) {
				lightning((*balls[i]).center, (*balls[j]).center);
			}
		}
		clearMaterial();
	}

	if (boundsEnabled) {
		drawFloor(cubeWid/10);
		drawWalls(cubeWid/10);
		if (nurb) {
			drawNurb();
		}
	}

	for (int i = 0; i < 8; i++) {	
		glDisable(GL_LIGHT0 + i);
	}
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);

}

/**
 * DrawMainView function
 *
 * Sets up the main (fullscreen) viewport and calls drawScene for it. Performs
 * some sexy vector math as well to handle camera rotation and whatnot.
 */
void drawMainView() {
	//reset/clear everything
	glViewport(0, 0, width, height); 
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity(); 
	gluPerspective(60, (GLfloat)width / height, 1.0, 1000.0); 

	glMatrixMode(GL_MODELVIEW); 	
	glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 1, 1);
	glLoadIdentity();

	//set up initial camera vectors
	upV = Vector3(0, 1, 0);
	rightV = Vector3(1, 0, 0);
	forwardV = rightV;

	//rotate right and forward around up for yaw correction (left/right)
	rightV = Vector3::rotate(rightV, upV, yaw);
	forwardV = Vector3::rotate(forwardV, upV, yaw-90);

	//rotate forward around right for pitch correction (up/down)
	forwardV = Vector3::rotate(forwardV, rightV, pitch);

	//take cross product of forward and right to get up vector
	upV = forwardV^rightV;

	//normalize
	upV.normalize();
	rightV.normalize();
	forwardV.normalize();

	//update camera using up, right and forward vectors
	gluLookAt(cam.x, cam.y, cam.z,
				forwardV.x+cam.x, forwardV.y+cam.y, forwardV.z+cam.z,
				upV.x, upV.y, upV.z);

	drawScene(true);
}

/**
 * DrawSmallView function
 *
 * Scissors out and makes small viewport, then calls drawScene. Uses different
 * camera angle though (straight above the bigBall).
 */
void drawSmallView() {
	glScissor(width*4/5, height*4/5, width, height); 
	glEnable(GL_SCISSOR_TEST);

	glViewport(width*4/5, height*4/5, width/5, height/5); 
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity(); 
	gluPerspective(60, (GLfloat)width / height, 1.0, cubeWid*3); 

	glMatrixMode(GL_MODELVIEW); 	
	glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 1, 1);
	glLoadIdentity();

	Point3 temp = bigBall->center;
	gluLookAt(temp.x, temp.y + cubeWid, temp.z,
			temp.x, temp.y, temp.z,
			0, 0, 1);
	drawScene(false);
	
	glDisable(GL_SCISSOR_TEST);
}
