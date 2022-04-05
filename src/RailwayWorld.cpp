//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//  Assignment 1
//  FILE NAME: RailwayWorld.cpp
//  Author: MENG ZHANG
//  Student ID: 71682325
//  ========================================================================

#include <math.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/freeglut.h>
#include "RailModels.h"

#include <iostream>
#include <fstream>
#include <climits>
#include <math.h>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
//                              Global Variables                             //
///////////////////////////////////////////////////////////////////////////////
// Train moving index
int THETA = 0;
int lightTimer = 0;
int airPlaneTheta = 0;
// Train current postion
float icurrX;
float icurrZ;
// Train stop postion angle
// int trainStopPos = 110;
int trainStopX = 40;
int trainStopZ = 40;
// Camera defaut params
string cameraMode = "default";
float angle = 0.0;
float eye_x = 3 * sin(angle);
float eye_y = 300;
float eye_z = 150 + 3 * cos(angle);
float look_x = eye_x + 100 * sin(angle);
float look_y = 0.0;
float look_z = eye_z - 100 * cos(angle);
// Track Coordinates
float *posX, *posZ;
int nvert;				// total number of vertices
float *posAngleCache;   // a array of  the moving posAngles for each vertices

///////////////////////////////////////////////////////////////////////////////
//                          Track Coordinates Loader                         //
///////////////////////////////////////////////////////////////////////////////

void loadOvalFile(const char* fname)
{
	ifstream fp_in;
	fp_in.open(fname, ios::in);
	if(!fp_in.is_open())
	{
		cout << "Error opening mesh file" << endl;
		exit(1);
	}

	fp_in >> nvert;

    posX = new float[nvert];
    posZ = new float[nvert];
    posAngleCache = new float[nvert];

    //read vertex list
	for(int i=0; i < nvert; i++) {
        fp_in >> posX[i] >> posZ[i];
        posAngleCache[i] = -1;
    }

	fp_in.close();
	cout << " File successfully read." << endl;
}

///////////////////////////////////////////////////////////////////////////////
//                          Timer Callback Function
///////////////////////////////////////////////////////////////////////////////
void myTimer(int value)
{
    static int trainTimer = 0;
    icurrX = posX[THETA];
    icurrZ = posZ[THETA];
    if (icurrX == trainStopX && icurrZ == trainStopZ) {
        trainTimer++;
        if (trainTimer >= 60) {
            THETA++;
            trainTimer = 0;
        }
        glutPostRedisplay();
        glutTimerFunc(50, myTimer, 0);
    } else {
        THETA++;
        glutPostRedisplay();
        glutTimerFunc(50, myTimer, 0);
    }

    if (lightTimer > 300) {
        lightTimer = 0;
    }
    lightTimer++;

    airPlaneTheta++;
}

///////////////////////////////////////////////////////////////////////////////
//                            Keybord and  Special Key Events                            //
///////////////////////////////////////////////////////////////////////////////
void keyboard(unsigned char key, int x, int y) {
     if (key == 'c')
    {
        if (cameraMode == "default") {
            cameraMode = "station";
        } else if (cameraMode == "station") {
            cameraMode = "driver";
        } else if (cameraMode == "driver") {
            cameraMode = "default";
        }
    }
}


void special(int key, int x, int y)
{
	if(key == GLUT_KEY_LEFT) angle -= 0.05;  //Change direction
	else if(key == GLUT_KEY_RIGHT) angle += 0.05;
	else if(key == GLUT_KEY_DOWN)
	{  //Move backward
		eye_x -= 3*sin(angle);
        eye_z += 3*cos(angle);
	}
	else if(key == GLUT_KEY_UP)
	{ //Move forward
		eye_x += 3*sin(angle);
		eye_z -= 3*cos(angle);
	}
    else if(key == GLUT_KEY_PAGE_DOWN)
    { // Decrease camera height
        if (eye_y > 10) {
            eye_y -= 10;
        }
    }
    else if(key == GLUT_KEY_PAGE_UP)
    { // Increase camera height
            eye_y += 10;
    }
    else if(key == GLUT_KEY_HOME)
    { // Camera back to defaut view
         angle = 0.0;
         eye_x = 3*sin(angle);
         eye_y = 300;
         eye_z = 150 + 3*cos(angle);
         look_x = eye_x + 100*sin(angle);
         look_y = 0.0;
         look_z = eye_z - 100*cos(angle);
    }
	look_x = eye_x + 100*sin(angle);
	look_z = eye_z - 100*cos(angle);
	glutPostRedisplay();
}

///////////////////////////////////////////////////////////////////////////////
//                               Initialization                              //
///////////////////////////////////////////////////////////////////////////////
void initialize(void)
{
    // load track coordinates
    loadOvalFile("Oval.txt");
    // set lighting color
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};

    // Texturing
    loadWagonTexture();
    loadRoofTexture();
    loadShadeTexture();
    loadConcreteTexture();
    loadHouseFrontTexture();
    // Lighting Config
	glEnable(GL_LIGHTING);
    // // Create a lighting source GL_LIGHT0
    glEnable(GL_LIGHT0);
    // glDisable(GL_LIGHT0);
    // // Create a lighting source GL_LIGHT1
    glEnable(GL_LIGHT1);

    // // Define GL_LIGHT0  ambient, diffuse, specular properties
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

    // // Define GL_LIGHT1  ambient, diffuse, specular properties
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.01);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glClearColor (0.0, 0.0, 0.0, 0.0);  //Background colour

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60., 1.0, 10.0, 1000.0);   //Perspective projection

    // Enable material surface colour values
    // // Use surface colour defined with glColor3f() as ambient and diffuse
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    // // set common specular colour & shininess term
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);
}

///////////////////////////////////////////////////////////////////////////////
//                                  Display                                  //
///////////////////////////////////////////////////////////////////////////////
void display(void)
{
    // // Calculate the moving parameters for the engine
   // // if THETA reaches the last vertix, wrap to the first one
   if (THETA >= nvert) {
       THETA = 0;
   }
   float radToDegree = 180 / 3.14159265;
   icurrX = posX[THETA];
   icurrZ = posZ[THETA];
   float dirX, dirZ;
   float icurrXPlusK;
   float icurrZPlusK;
   float icurrXMinusK;
   float icurrZMinusK;
   float posAngle;
   if (posAngleCache[THETA] == -1) {
       // if no posAngle at this vertix has been cached
      
       if (THETA == nvert - 1) {
           icurrXPlusK = posX[0];
       } else {
           icurrXPlusK = posX[THETA + 1];
       }
      
       if (THETA == nvert - 1) {
           icurrZPlusK = posZ[0];
       } else {
           icurrZPlusK = posZ[THETA + 1];
       }
      
       if (THETA == 0) {
           icurrXMinusK = posX[nvert-1];
       } else {
           icurrXMinusK = posX[THETA - 1];
       }
      
       if (THETA == 0) {
           icurrZMinusK = posZ[nvert-1];
       } else {
           icurrZMinusK = posZ[THETA - 1];
       }

       // get x and y of  moving direction vector
       dirX = icurrXPlusK - icurrXMinusK;
       dirZ = icurrZPlusK - icurrZMinusK;
       // convert moving direction vector to a unit vector
       glm::vec3 dirV(dirX, 1, dirZ);
       glm::vec3 dirUv = glm::normalize(dirV);
       // get the posAngle from the x and z of moving direction vector
       float uX = dirUv[0];
       float uZ = dirUv[2];
       posAngle = atan2(uZ, -uX) * radToDegree;
       posAngleCache[THETA] = posAngle;
   } else
   {   // if posAngle is already cached
       posAngle = posAngleCache[THETA];
   }
    
   // light0 position variable (directly above the origin)
   //   float lgt_pos[] = {0.0f, 50.0f, 0.0f, 1.0f};
   float lgt_pos[] = {0.0f, 100.0f, 50.0f, 1.0f};

   glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // Camera view config
   if (cameraMode == "driver")
   {
       //       glTranslatef(icurrX, 1, icurrZ);
       //       glRotatef(posAngle, 0, 1, 0);
       float driverViewX, driverViewZ;
       if (THETA + 15 > nvert - 1){
           driverViewX = posX[THETA + 15 - nvert];
           driverViewZ = posZ[THETA + 15 - nvert];
       } else {
           driverViewX = posX[THETA + 15];
           driverViewZ = posZ[THETA + 15];
       }
       gluLookAt(icurrX, 25, icurrZ,  driverViewX, 20, driverViewZ,   0.0, 1.0, 0.0);
   } else if (cameraMode == "station")
   {
       gluLookAt(150, 30, 130,  80, 30, 50,   0.0, 1.0, 0.0);
   } else if (cameraMode == "default") {
       gluLookAt(eye_x, eye_y, eye_z,  look_x, look_y, look_z,   0.0, 1.0, 0.0);
   }

   // Light0 positoin
   glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);

   // Create floor
   floor();

   // Create track median line
   glPushMatrix();
   //   createTrackMedianLine(nvert, posX, posZ);
   tracks(7, nvert, posX, posZ);
   sleepers(nvert, posX, posZ);
   glPopMatrix();

   // Create tunnel
   glPushMatrix();
   tunnel(20, 105, nvert, posX, posZ);
   glPopMatrix();

   // Create engine (locomotive) moving around tracks
   // //  Create and move the engine
   glPushMatrix();
   glTranslatef(icurrX, 1, icurrZ);
   glRotatef(posAngle, 0, 1, 0);
   engine();
   glPopMatrix();

   // Create Locomotive headlight
   float lgt_pos_1 [] = {-10.0f, 14.0f, 0.0f, 1.0f};
   float spotdir[] = {-1, -1, 0};
   glPushMatrix();
   // //  translate the same as locomotive
   glTranslatef(icurrX, 1, icurrZ);
   glRotatef(posAngle, 0, 1, 0);
   // // light1 direction
   glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotdir);
   // // light1 position
   glLightfv(GL_LIGHT1, GL_POSITION, lgt_pos_1);
   glPopMatrix();

   // Create wagons
   float wagonPosAngle;
   int wagonTHETA = THETA - 22;
   int wagonNum = 3;
   for (int i = 1; i < wagonNum + 1; i++) {
       if (wagonTHETA < 0) {
           wagonTHETA = nvert + wagonTHETA;
       }
       float wagonCurrX = posX[wagonTHETA];
       float wagonCurrZ = posZ[wagonTHETA];
       if (posAngleCache[wagonTHETA] == -1) {
           float wagonCurrXPlusK;
           if (wagonTHETA == nvert - 1) {
               wagonCurrXPlusK = posX[0];
           } else {
               wagonCurrXPlusK = posX[wagonTHETA + 1];
           }
           float wagonCurrZPlusK;
           if (wagonTHETA == nvert - 1) {
               wagonCurrZPlusK = posZ[0];
           } else {
               wagonCurrZPlusK = posZ[wagonTHETA + 1];
           }
           float wagonCurrXMinusK;
           if (wagonTHETA == 0) {
               wagonCurrXMinusK = posX[nvert-1];
           } else {
               wagonCurrXMinusK = posX[wagonTHETA - 1];
           }
           float wagonCurrZMinusK;
           if (wagonTHETA == 0) {
               wagonCurrZMinusK = posZ[nvert-1];
           } else {
               wagonCurrZMinusK = posZ[wagonTHETA - 1];
           }
           float wagonDirX = wagonCurrXPlusK - wagonCurrXMinusK;
           float wagonDirZ = wagonCurrZPlusK - wagonCurrZMinusK;
           glm::vec3 wagonDirV(wagonDirX, 1, wagonDirZ);
           glm::vec3 wagonDirUv = glm::normalize(wagonDirV);
           float wagonUX = wagonDirUv[0];
           float wagonUZ = wagonDirUv[2];
           wagonPosAngle = atan2(wagonUZ, -wagonUX) * radToDegree;
           posAngleCache[wagonTHETA] = wagonPosAngle;
       } else {
           wagonPosAngle = posAngleCache[wagonTHETA];
       }
       glPushMatrix();
       glTranslatef(wagonCurrX, 1, wagonCurrZ);
       glRotatef(wagonPosAngle, 0, 1, 0);
       //       glScalef(0.7, 0.7, 0.7);
       wagon();
       glPopMatrix();
       wagonTHETA = wagonTHETA - 22;
   }


   // Create a train station
   glPushMatrix();
   glTranslatef(30, 1, 105);
   glRotatef(90, 0, 1, 0);
   railwayStation();
   glPopMatrix();

   // Create signal Lights
   // // signal light-1
   glPushMatrix();
   glTranslatef(-10, 0, -18);
   glRotatef(90, 0, 1, 0);
   signalLights(lightTimer);
   glPopMatrix();

   // // signal light-2
   glPushMatrix();
   glTranslatef(-10, 0, -60);
   glRotatef(90, 0, 1, 0);
   signalLights(lightTimer);
   glPopMatrix();

   // // signal light-3
   glPushMatrix();
   glTranslatef(-80, 0, 10);
   signalLights(lightTimer);
   glPopMatrix();

   // // signal light-4
   glPushMatrix();
   glTranslatef(-117, 0, 12);
   signalLights(lightTimer);
   glPopMatrix();

   // Create an airplane
   glPushMatrix();
   glRotatef(-airPlaneTheta, -0.2, 1, 0);
   glTranslatef(0, 70, 80);
   airPlane(lightTimer);
   glPopMatrix();
   
   glutSwapBuffers();   //Useful for animation
}

///////////////////////////////////////////////////////////////////////////////
//                                    Main                                   //
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH);
   glutInitWindowSize (600, 600);
   glutInitWindowPosition (50, 50);
   glutCreateWindow ("mzh103 COSC363 Assignment");
   initialize();

   glutDisplayFunc(display);
   // Call keybord
   glutKeyboardFunc(keyboard);
   // Call special keybord
   glutSpecialFunc(special);
   // Call the timer
   glutTimerFunc(50, myTimer, 0);
   glutMainLoop();
   return 0;
}
