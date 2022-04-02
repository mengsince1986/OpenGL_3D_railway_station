//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//  Assignment 1
//  FILE NAME: RailwayWorld.cpp
//  See README.md for details
//  ========================================================================

#include <math.h>
#include <GL/freeglut.h>
#include "RailModels.h"

///////////////////////////////////////////////////////////////////////////////
//                              Global Variables                             //
///////////////////////////////////////////////////////////////////////////////
// Train moving angle
float THETA = 0.0;
// Train stop postion angle
int trainStopPos = 110;
// Camera defaut params
float angle = 0.0;
float eye_x = 3 * sin(angle);
float eye_y = 400;
float eye_z = 150 + 3 * cos(angle);
float look_x = eye_x + 100 * sin(angle);
float look_y = 0.0;
float look_z = eye_z - 100 * cos(angle);

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
         eye_y = 400;
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
   tracks(120, 10);

   // Create engine (locomotive)
   glPushMatrix();
   glRotatef(THETA, 0, 1, 0);
   // // Move the locomotive from its current position at the origin
   // // to position (0, 1, -120)
   glTranslatef(0, 1, -120);
   engine();
   glPopMatrix();

   // Create Locomotive headlight
   // // light1 position
   float lgt_pos_1 [] = {-10.0f, 14.0f, 0.0f, 1.0f};
   float spotdir[] = {-1, -1, 0};
   glPushMatrix();
   // translate the same as locomotive
   glRotatef(THETA, 0, 1, 0);
   glTranslatef(0, 1, -120);
   // // light1 direction
   glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotdir);
   // // light1 position
   glLightfv(GL_LIGHT1, GL_POSITION, lgt_pos_1);
   glPopMatrix();

   // Create wagons
   // and then rotate it about the y-axis by 10.5 degrees
   int wagonNum = 4;
   for (int i = 1; i < wagonNum + 1; i++) {
       glPushMatrix();
       glRotatef(-10.5 * i + THETA, 0, 1, 0);
       // // and translate it to (0, 1, -120)
       glTranslatef(0, 1, -120);
       wagon();
       glPopMatrix();
   }

   // Create a train station
   glPushMatrix();
   glTranslatef(-180, 0.1, 0);
   railwayStation();
   glPopMatrix();

   // Create oval tracks
   glPopMatrix();
   ovalMedianLine();
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
