//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//  Assignment 1
//  FILE NAME: RailwayWorld.cpp
//  See README.md for details
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
// Train moving angle
int THETA = 0;
// Train stop postion angle
int trainStopPos = 110;
// Camera defaut params
float angle = 0.0;
float eye_x = 3 * sin(angle);
float eye_y = 200;
float eye_z = 150 + 3 * cos(angle);
float look_x = eye_x + 100 * sin(angle);
float look_y = 0.0;
float look_z = eye_z - 100 * cos(angle);
// Track Coordinates
float *posX, *posZ;
int nvert;				// total number of vertices
float *posAngleCache;   // to store the moving posAngles for each vertices

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

	fp_in >> nvert;			    // read number of vertices, polygons, edges (not used)

    posX = new float[nvert];                        //create arrays
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

/*
void load8CurveCords()
{
    // Eight Curve parametric equations:
    // x = a * sin(t)
    // y = a * sin(t) * cos(t)
    float trackRadius = 100;
    float toRad = 3.14159265/180.0;  //Conversion from degrees to radians
    float angle, x, z;
    nvert = 360;
    posX = new float[nvert];                        //create arrays
    posZ = new float[nvert];
    posAngleCache = new float[nvert];
    for (int i = 0; i < nvert; i++) {
        angle = i * toRad;
        posX[i]  = trackRadius * sin(angle);
        posZ[i] = trackRadius * sin(angle) * cos(angle);
        posAngleCache[i] = -1;
    }
}
*/

///////////////////////////////////////////////////////////////////////////////
//                          Timer Callback Function
///////////////////////////////////////////////////////////////////////////////
void myTimer(int value)
{
    static int trainTimer = 0;
    int trainPos = THETA;
    if (trainPos == trainStopPos || (trainPos-trainStopPos)%360 == 0) {
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
}

///////////////////////////////////////////////////////////////////////////////
//                             Special Key Events                            //
///////////////////////////////////////////////////////////////////////////////
// To enable the use of left and right arrow keys to rotate the scene

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
         eye_y = 200;
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
    //    load8CurveCords();
    // set lighting color
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};

    // Lighting Config
	glEnable(GL_LIGHTING);
    // // Create a lighting source GL_LIGHT0
	glEnable(GL_LIGHT0);
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
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
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
   // light0 position variable (directly above the origin)
   float lgt_pos[] = {0.0f, 50.0f, 0.0f, 1.0f};

   glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // Camera view config
   // // Move camra backwards by changing z from 180 to 250
   // gluLookAt (-80, 50, 250, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   gluLookAt(eye_x, eye_y, eye_z,  look_x, look_y, look_z,   0.0, 1.0, 0.0);
   glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   // light0 position

   // Create floor
   floor();
   // Create tracks
   // // mean radius 120 units, width 10 units
   // tracks(120, 10);

   // // use oval tracks
   // glPushMatrix();
   //   ovalTracks(nvert, posX, posZ);
   //   glPopMatrix();
   // // use eightCurveTracks

   // // use 8 cruve tracks;
   glPushMatrix();
   createTrackMedianLine(nvert, posX, posZ);
   glPopMatrix();
   
   // Create engine (locomotive) moving around circle
   // glPushMatrix();
   //   glRotatef(THETA, 0, 1, 0);
   // // Move the locomotive from its current position at the origin
   // // to position (0, 1, -120)
   //   glTranslatef(0, 1, -120);
   //   engine();
   //   glPopMatrix();


   // Create engine (locomotive) moving around loaded tracks
   if (THETA >= nvert) {
       THETA = 0;
   }
   float radToDegree = 180 / 3.14159265;
   float icurrX = posX[THETA];
   float icurrZ = posZ[THETA];
   float posAngle;
   if (posAngleCache[THETA] == -1) {
       float icurrXPlusK;
       if (THETA == nvert - 1) {
           icurrXPlusK = posX[0];
       } else {
           icurrXPlusK = posX[THETA + 1];
       }
       float icurrZPlusK;
       if (THETA == nvert - 1) {
           icurrZPlusK = posZ[0];
       } else {
           icurrZPlusK = posZ[THETA + 1];
       }
       float icurrXMinusK;
       if (THETA == 0) {
           icurrXMinusK = posX[nvert-1];
       } else {
           icurrXMinusK = posX[THETA - 1];
       }
       float icurrZMinusK;
       if (THETA == 0) {
           icurrZMinusK = posZ[nvert-1];
       } else {
           icurrZMinusK = posZ[THETA - 1];
       }

       float dirX = icurrXPlusK - icurrXMinusK;
       float dirZ = icurrZPlusK - icurrZMinusK;
       glm::vec3 dirV(dirX, 1, dirZ);
       glm::vec3 dirUv = glm::normalize(dirV);
       float uX = dirUv[0];
       float uZ = dirUv[2];
       posAngle = atan2(uZ, -uX) * radToDegree;
       posAngleCache[THETA] = posAngle;
   } else
   {
       posAngle = posAngleCache[THETA];
   }
   glPushMatrix();
   // // Move the locomotive from its current position at the origin
   // // to position (0, 1, -120)
   glTranslatef(icurrX, 1, icurrZ);
   glRotatef(posAngle, 0, 1, 0);
   // glScalef(0.5, 0.5, 0.5);
   engine();
   glPopMatrix();

   /*
   // Create engine moving around 8 curve
   float a = 265.0;
   float toRad = 3.14159265/180.0;  //Conversion from degrees to radians
   float radToDegree = 180 / 3.14159265;
   float currentT = THETA * toRad;
   float currentX = a * sin(currentT);
   float currentZ = a * sin(currentT) * cos(currentT);
   float nextT = (THETA + 1) * toRad;
   float nextX = a * sin(nextT);
   float nextZ = a * sin(nextT) * cos(nextT);
   float dirX = (nextX - currentX);
   float dirZ = (nextZ - currentZ);
   float dirV = sqrt(dirX * dirX + 1 * 1 + dirZ * dirZ);
   float dirUX = dirX / dirV;
   float dirUZ = dirZ / dirV;
   float rotateAngle = atan2(dirUZ, -dirUX) * radToDegree;
   glPushMatrix();
   glTranslatef(a * sin(THETA*toRad), 1.0, a*sin(THETA*toRad)*cos(THETA*toRad));
   glRotatef(rotateAngle, 0, 1, 0);
   glRotatef(rotateAngle, 0, 1, 0);
   engine();
   glPopMatrix();
   */

   // Create Locomotive headlight
   // // light1 position
   float lgt_pos_1 [] = {-10.0f, 14.0f, 0.0f, 1.0f};
   float spotdir[] = {-1, -1, 0};
   glPushMatrix();
   // translate the same as locomotive
   //glRotatef(THETA, 0, 1, 0);
   // glTranslatef(0, 1, -120);
   glTranslatef(icurrX, 1, icurrZ);
   glRotatef(posAngle, 0, 1, 0);
   // // light1 direction
   glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotdir);
   // // light1 position
   glLightfv(GL_LIGHT1, GL_POSITION, lgt_pos_1);
   glPopMatrix();

   // Create wagons
   // and then rotate it about the y-axis by 10.5 degrees for circle tracks
   /*
   int wagonNum = 4;
   for (int i = 1; i < wagonNum + 1; i++) {
       glPushMatrix();
       glRotatef(-10.5 * i + THETA, 0, 1, 0);
       // // and translate it to (0, 1, -120)
       glTranslatef(0, 1, -120);
       wagon();
       glPopMatrix();
   }
   */
   float wagonCurrX, wagonCurrZ, wagonPosAngle, wagonCurrXPlusK, wagonCurrZPlusK, wagonCurrXMinusK, wagonCurrZMinusK, wagonDirX, wagonDirZ;
   int wagonTHETA = THETA - 22;
   int wagonNum = 3;
   for (int i = 1; i < wagonNum + 1; i++) {
       if (wagonTHETA < 0) {
           wagonTHETA = nvert + wagonTHETA;
       }
       wagonCurrX = posX[wagonTHETA];
       wagonCurrZ = posZ[wagonTHETA];
       if (posAngleCache[wagonTHETA] == -1) {
           if (wagonTHETA == nvert - 1) {
               wagonCurrXPlusK = posX[0];
           } else {
               wagonCurrXPlusK = posX[wagonTHETA + 1];
           }
           if (wagonTHETA == nvert - 1) {
               wagonCurrZPlusK = posZ[0];
           } else {
               wagonCurrZPlusK = posZ[wagonTHETA + 1];
           }
           if (wagonTHETA == 0) {
               wagonCurrXMinusK = posX[nvert-1];
           } else {
               wagonCurrXMinusK = posX[wagonTHETA - 1];
           }
           if (wagonTHETA == 0) {
               wagonCurrZMinusK = posZ[nvert-1];
           } else {
               wagonCurrZMinusK = posZ[wagonTHETA - 1];
           }

           wagonDirX = wagonCurrXPlusK - wagonCurrXMinusK;
           wagonDirZ = wagonCurrZPlusK - wagonCurrZMinusK;
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
   glTranslatef(0, 1, -90);
   glRotatef(-90, 0, 1, 0);
   glScalef(0.5, 0.5, 0.5);
   railwayStation();
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
   // Call special keybord
   glutSpecialFunc(special);
   // Call the timer
   glutTimerFunc(50, myTimer, 0);
   glutMainLoop();
   return 0;
}
