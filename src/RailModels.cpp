//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//  Assignment 1
//  FILE NAME: RailModels.cpp
//  Author: MENG ZHANG
//  Student ID: 71682325
//  ========================================================================

#include <cmath>
#include <GL/freeglut.h>
#include "RailModels.h"

#include <iostream>
#include <fstream>
#include <climits>
#include <math.h>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
//                                   FLOOR                                   //
///////////////////////////////////////////////////////////////////////////////

void floor()
{
	float white[4] = {1., 1., 1., 1.};
	float black[4] = {0};
	glColor4f(0.4, 0.4, 0.4, 1.0);  //The floor is gray in colour
	glNormal3f(0.0, 1.0, 0.0);

    // lab02.3 Mkae floors diffuse surfaces.
    // Supress specular reflections from the floor
    // by temporaily setting the specular colour to
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);

	// The floor is made up of several tiny squares on a 400 x 400 grid.
    // Each square has a unit size.
    // // set floor size to be 1000 x 1000
    int floorSize = 500;
	glBegin(GL_QUADS);
	for(int i = -floorSize; i < floorSize; i++)
	{
		for(int j = -floorSize;  j < floorSize; j++)
		{
			glVertex3f(i, 0, j);
			glVertex3f(i, 0, j+1);
			glVertex3f(i+1, 0, j+1);
			glVertex3f(i+1, 0, j);
		}
	}

    // Single quad floor
    /*
    glVertex3f(-1000, 0, -1000);
    glVertex3f(-1000, 0, 1000);
    glVertex3f(1000, 0, 1000);
    glVertex3f(1000, 0, -1000);
    */
	glEnd();

    // lab02.3 Reset the specular property to white after the glEnd()
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);

}

///////////////////////////////////////////////////////////////////////////////
//                               Railway Tracks                              //
///////////////////////////////////////////////////////////////////////////////

void tracks(float medRadius, float width)
{
	float inRad  = medRadius - width * 0.5;
	float outRad = medRadius + width * 0.5;
	float angle1,angle2, ca1,sa1, ca2,sa2;
	float x1,z1, x2,z2, x3,z3, x4,z4;  //four points of a quad
    float toRad = 3.14159265/180.0;  //Conversion from degrees to radians

	glColor4f(0.0, 0.0, 0.3, 1.0);
    glBegin(GL_QUADS);
	float radius = inRad;
	for (int i = 0; i < 2; i++)   //Two parallel tracks (radius = inRad, outRad)
	{
		for (int i = 0; i < 360; i += 5)    //5 deg intervals
		{
			angle1 = i * toRad;       //Computation of angles, cos, sin etc
			angle2 = (i + 5) * toRad;
			ca1 = cos(angle1); ca2 = cos(angle2);
			sa1 = sin(angle1); sa2 = sin(angle2);
			x1 = (radius - 0.5)*sa1; z1 = (radius - 0.5)*ca1;
			x2 = (radius + 0.5)*sa1; z2 = (radius + 0.5)*ca1;
			x3 = (radius + 0.5)*sa2; z3 = (radius + 0.5)*ca2;
			x4 = (radius - 0.5)*sa2; z4 = (radius - 0.5)*ca2;

			glNormal3f(0., 1., 0.);       //Quad 1 facing up
			glVertex3f(x1, 1.0, z1);
			glVertex3f(x2, 1.0, z2);
			glVertex3f(x3, 1.0, z3);
			glVertex3f(x4, 1.0, z4);

			glNormal3f(-sa1, 0.0, -ca1);   //Quad 2 facing inward
			glVertex3f(x1, 0.0, z1);
			glVertex3f(x1, 1.0, z1);
			glNormal3f(-sa2, 0.0, -ca2);
			glVertex3f(x4, 1.0, z4);
			glVertex3f(x4, 0.0, z4);

			glNormal3f(sa1, 0.0, ca1);   //Quad 3 facing outward
			glVertex3f(x2, 1.0, z2);
			glVertex3f(x2, 0.0, z2);
			glNormal3f(sa2, 0.0, ca2);
			glVertex3f(x3, 0.0, z3);
			glVertex3f(x3, 1.0, z3);
		}
		radius = outRad;
	}
	glEnd();
}

void createTrackMedianLine(const int nvert, const float* x, const float* z)
{
    glColor4f(0.0, 1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
    glNormal3f(0, 1, 0);
    for (int i = 0; i < nvert; i++)
    {
        glVertex3f(x[i], 1.0, z[i]);
    }
    glEnd();
}


//--------------- MODEL BASE --------------------------------------
// This is a common base for the locomotive and wagons
// The base is of rectangular shape (20 length x 10 width x 2 height)
// and has wheels and connectors attached.
//-----------------------------------------------------------------
void base()
{
    glColor4f(0.2, 0.2, 0.2, 1.0);   //Base color
    glPushMatrix();
      glTranslatef(0.0, 4.0, 0.0);
      glScalef(20.0, 2.0, 10.0);     //Size 20x10 units, thickness 2 units.
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();					//Connector between wagons
      glTranslatef(11.0, 4.0, 0.0);
      glutSolidCube(2.0);
    glPopMatrix();

    //4 Wheels (radius = 2 units)
	//x, z positions of wheels:
	float wx[4] = {  -8,   8,   -8,    8 };
	float wz[4] = { 5.1, 5.1, -5.1, -5.1 };
    glColor4f(0.5, 0., 0., 1.0);    //Wheel color
	GLUquadric *q = gluNewQuadric();   //Disc

	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		glTranslatef(wx[i], 2.0, wz[i]);
		gluDisk(q, 0.0, 2.0, 20, 2);
		glPopMatrix();
	}
}

//--------------- LOCOMOTIVE --------------------------------
// This simple model of a locomotive consists of the base,
// cabin and boiler
//-----------------------------------------------------------
void engine()
{
    base();

    //Cab
    glColor4f(0.8, 0.8, 0.0, 1.0);
    glPushMatrix();
      glTranslatef(7.0, 8.5, 0.0);
      glScalef(6.0, 7.0, 10.0);
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(6.0, 14.0, 0.0);
      glScalef(4.0, 4.0, 8.0);
      glutSolidCube(1.0);
    glPopMatrix();

    //Boiler
	GLUquadric *q = gluNewQuadric();   //Cylinder
    glPushMatrix();
      glColor4f(0.5, 0., 0., 1.0);
      glTranslatef(4.0, 10.0, 0.0);
      glRotatef(-90.0, 0., 1., 0.);
      gluCylinder(q, 5.0, 5.0, 14.0, 20, 5);
      glTranslatef(0.0, 0.0, 14.0);
      gluDisk(q, 0.0, 5.0, 20, 3);
      glColor4f(1.0, 1.0, 0.0, 1.0);
      glTranslatef(0.0, 4.0, 0.2);
      gluDisk(q, 0.0, 1.0, 20,2);  //headlight!
    glPopMatrix();

}

///////////////////////////////////////////////////////////////////////////////
//                                     WAGON                                    //
///////////////////////////////////////////////////////////////////////////////

void wagon()
{
   
    base();

    float wagonLength = 9;

	glColor4f(1.0, 1.0, 1.0, 1.0);

    // glEnable(GL_TEXTURE_2D);
    // glBindTexture(GL_TEXTURE_2D, txId);
    
	//3 large polygons (front, back, top)
	glBegin(GL_QUADS);
    glNormal3f(0.0, 0.0, 1.0);   //Facing +z (Front side)
    //    glTexCoord2f(0., 0.);
      glVertex3f(-wagonLength, 5.0, 6.0);
    //    glTexCoord2f(1., 0.);
      glVertex3f(wagonLength, 5.0, 6.0);
    //    glTexCoord2f(1., 87./256.);
      glVertex3f(wagonLength, 17.0, 6.0);
    //    glTexCoord2f(0., 87./256.);
      glVertex3f(-wagonLength, 17.0, 6.0);

      glNormal3f(0.0, 0.0, -1.0);   //Facing -z (Back side)
    //    glTexCoord2f(0., 0.);
      glVertex3f(wagonLength, 5.0, -6.0);
    //    glTexCoord2f(1., 0.);
      glVertex3f(-wagonLength, 5.0,-6.0);
    //    glTexCoord2f(1., 87./256.);
      glVertex3f(-wagonLength, 17.0,-6.0);
    //    glTexCoord2f(0., 87./256.);
      glVertex3f(wagonLength, 17.0, -6.0);

      glNormal3f(0.0, 1.0, 0.0);   //Facing +y (Top side)
    // glTexCoord2f(0., 115./256.);
      glVertex3f(-wagonLength  , 17.0, 6.0);
    // glTexCoord2f(1., 115./256.);
      glVertex3f(wagonLength  , 17.0,  6.0);
    // glTexCoord2f(1., 212./256.);
      glVertex3f(wagonLength, 17.0, -6.0);
    // glTexCoord2f(0., 212./256.);
      glVertex3f(-wagonLength, 17.0, -6.0);
	glEnd();

	// 2 small side polygons (left, right)
    // glDisable(GL_TEXTURE_2D);
    
	glColor4f(0.5, 0.5, 0.0, 1.0);
	glBegin(GL_QUADS);
      glNormal3f(-1.0, 0.0, 0.0);   //Facing -x (Left side)
	  glVertex3f(-wagonLength, 5.0, -6.0);
	  glVertex3f(-wagonLength, 5.0, 6.0);
	  glVertex3f(-wagonLength, 17.0, 6.0);
	  glVertex3f(-wagonLength, 17.0, -6.0);

	  glNormal3f(1.0, 0.0, 0.0);   //Facing +x (Right side)
	  glVertex3f(wagonLength, 5.0, 6.0);
	  glVertex3f(wagonLength, 5.0, -6.0);
	  glVertex3f(wagonLength, 17.0, -6.0);
	  glVertex3f(wagonLength, 17.0, 6.0);
	glEnd();

}

///////////////////////////////////////////////////////////////////////////////
//                              Railway Station                               //
///////////////////////////////////////////////////////////////////////////////
void railwayStation()
{
    // Quads base
    glPushMatrix();
    glTranslatef(0, 0, 0);
    // glRotatef(90, 0, 1, 0);
    //    blockBase(40, 80, 5);
    blockBase(30, 70, 5);
    glPopMatrix();

    // Quads building-1
    glPushMatrix();
    glTranslatef(-15.0, 4, 40.0);
    glRotatef(90, 0, 1, 0);
    block1(30, 10, 35);
    glPopMatrix();

    // // building-1 roof
    glPushMatrix();
    glTranslatef(-15.0, 39, 40.0);
    glRotatef(90, 0, 1, 0);
    roof(30, 10, 20);
    glPopMatrix();
    
    // // building-1 shade cover
    glPushMatrix();
    glTranslatef(10.0, 36.8, 40.0);
    glRotatef(-90, 0, 1, 0);
    slope2(30, 20, 2);
    glPopMatrix();
    
    // // building-1 shade cover sticks
    glPushMatrix();
    glTranslatef(10, 30, 68);
    glRotatef(25, 0, 0, 1);
    glRotatef(180, 0, 1, 0);
    block1(16, 0.3, 0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(10, 30, 15);
    glRotatef(25, 0, 0, 1);
    glRotatef(180, 0, 1, 0);
    block1(16, 0.3, 0.3);
    glPopMatrix();

    // Quads building-2
    glPushMatrix();
    glTranslatef(-15.0, 4, 0.0);
    glRotatef(90, 0, 1, 0);
    block2(12, 10, 20);
    glPopMatrix();

    // base slope
    glPushMatrix();
    glTranslatef(0.0, 0.0, -105.0);
    slope1(30, 35, 5);
    glPopMatrix();
}

void blockBase(float length, float width, float height)
{
    float buildingbaseHeight = 0.01;

	glColor4f(0.8, 0.8, 0.8, 1.0);

    // glEnable(GL_TEXTURE_2D);
    // glBindTexture(GL_TEXTURE_2D, txId);
    
	// 3 large polygons (front, back, top)
	glBegin(GL_QUADS);
      glNormal3f(0.0, 0.0, 1.0);   //Facing +z (Front side)
    //    glTexCoord2f(0., 0.);
      glVertex3f(-length, buildingbaseHeight, width);
    //    glTexCoord2f(1., 0.);
      glVertex3f(length, buildingbaseHeight, width);
    //    glTexCoord2f(1., 87./256.);
      glVertex3f(length, height, width);
    //    glTexCoord2f(0., 87./256.);
      glVertex3f(-length, height, width);

      glNormal3f(0.0, 0.0, -1.0);   //Facing -z (Back side)
    //    glTexCoord2f(0., 0.);
      glVertex3f(length, buildingbaseHeight, -width);
    //    glTexCoord2f(1., 0.);
      glVertex3f(-length, buildingbaseHeight,-width);
    //    glTexCoord2f(1., 87./256.);
      glVertex3f(-length, height,-width);
    //    glTexCoord2f(0., 87./256.);
      glVertex3f(length, height, -width);

      glNormal3f(0.0, 1.0, 0.0);   //Facing +y (Top side)
    // glTexCoord2f(0., 115./256.);
      glVertex3f(-length, height, width);
    // glTexCoord2f(1., 115./256.);
      glVertex3f(length, height,  width);
    // glTexCoord2f(1., 212./256.);
      glVertex3f(length, height, -width);
    // glTexCoord2f(0., 212./256.);
      glVertex3f(-length, height, -width);
	glEnd();

	// 2 small side polygons (left, right)
    // glDisable(GL_TEXTURE_2D);
	glColor4f(0.5, 0.5, 0.5, 1.0);
	glBegin(GL_QUADS);
      glNormal3f(-1.0, 0.0, 0.0);   //Facing -x (Left side)
	  glVertex3f(-length, buildingbaseHeight, -width);
	  glVertex3f(-length, buildingbaseHeight, width);
	  glVertex3f(-length, height, width);
	  glVertex3f(-length, height, -width);

      glNormal3f(1.0, 0.0, 0.0);   //Facing +x (Right side)
	  glVertex3f(length, buildingbaseHeight, width);
	  glVertex3f(length, buildingbaseHeight, -width);
	  glVertex3f(length, height, -width);
	  glVertex3f(length, height, width);
	glEnd();
}

void block1(float length, float width, float height)
{
    float buildingbaseHeight = 0.01;

	glColor4f(0.2, 0.3, 0.3, 1.0);

    // glEnable(GL_TEXTURE_2D);
    // glBindTexture(GL_TEXTURE_2D, txId);
    
	// 3 large polygons (front, back, top)
	glBegin(GL_QUADS);
      glNormal3f(0.0, 0.0, 1.0);   //Facing +z (Front side)
    //    glTexCoord2f(0., 0.);
      glVertex3f(-length, buildingbaseHeight, width);
    //    glTexCoord2f(1., 0.);
      glVertex3f(length, buildingbaseHeight, width);
    //    glTexCoord2f(1., 87./256.);
      glVertex3f(length, height, width);
    //    glTexCoord2f(0., 87./256.);
      glVertex3f(-length, height, width);

      glNormal3f(0.0, 0.0, -1.0);   //Facing -z (Back side)
    //    glTexCoord2f(0., 0.);
      glVertex3f(length, buildingbaseHeight, -width);
    //    glTexCoord2f(1., 0.);
      glVertex3f(-length, buildingbaseHeight,-width);
    //    glTexCoord2f(1., 87./256.);
      glVertex3f(-length, height,-width);
    //    glTexCoord2f(0., 87./256.);
      glVertex3f(length, height, -width);

      glNormal3f(0.0, 1.0, 0.0);   //Facing +y (Top side)
    // glTexCoord2f(0., 115./256.);
      glVertex3f(-length, height, width);
    // glTexCoord2f(1., 115./256.);
      glVertex3f(length, height,  width);
    // glTexCoord2f(1., 212./256.);
      glVertex3f(length, height, -width);
    // glTexCoord2f(0., 212./256.);
      glVertex3f(-length, height, -width);
	glEnd();

	// 2 small side polygons (left, right)
    // glDisable(GL_TEXTURE_2D);
	glColor4f(0.5, 0.5, 0.5, 1.0);
	glBegin(GL_QUADS);
      glNormal3f(-1.0, 0.0, 0.0);   //Facing -x (Left side)
	  glVertex3f(-length, buildingbaseHeight, -width);
	  glVertex3f(-length, buildingbaseHeight, width);
	  glVertex3f(-length, height, width);
	  glVertex3f(-length, height, -width);

      glNormal3f(1.0, 0.0, 0.0);   //Facing +x (Right side)
	  glVertex3f(length, buildingbaseHeight, width);
	  glVertex3f(length, buildingbaseHeight, -width);
	  glVertex3f(length, height, -width);
	  glVertex3f(length, height, width);
	glEnd();
}

void block2(float length, float width, float height)
{
    float buildingbaseHeight = 0.01;

	glColor4f(0.2, 0.3, 0.3, 1.0);

    // glEnable(GL_TEXTURE_2D);
    // glBindTexture(GL_TEXTURE_2D, txId);
    
	// 3 large polygons (front, back, top)
	glBegin(GL_QUADS);
      glNormal3f(0.0, 0.0, 1.0);   //Facing +z (Front side)
    //    glTexCoord2f(0., 0.);
      glVertex3f(-length, buildingbaseHeight, width);
    //    glTexCoord2f(1., 0.);
      glVertex3f(length, buildingbaseHeight, width);
    //    glTexCoord2f(1., 87./256.);
      glVertex3f(length, height, width);
    //    glTexCoord2f(0., 87./256.);
      glVertex3f(-length, height, width);

      glNormal3f(0.0, 0.0, -1.0);   //Facing -z (Back side)
    //    glTexCoord2f(0., 0.);
      glVertex3f(length, buildingbaseHeight, -width);
    //    glTexCoord2f(1., 0.);
      glVertex3f(-length, buildingbaseHeight,-width);
    //    glTexCoord2f(1., 87./256.);
      glVertex3f(-length, height,-width);
    //    glTexCoord2f(0., 87./256.);
      glVertex3f(length, height, -width);

      glNormal3f(0.0, 1.0, 0.0);   //Facing +y (Top side)
    // glTexCoord2f(0., 115./256.);
      glVertex3f(-length, height, width);
    // glTexCoord2f(1., 115./256.);
      glVertex3f(length, height,  width);
    // glTexCoord2f(1., 212./256.);
      glVertex3f(length, height, -width);
    // glTexCoord2f(0., 212./256.);
      glVertex3f(-length, height, -width);
	glEnd();

	// 2 small side polygons (left, right)
    // glDisable(GL_TEXTURE_2D);
	glColor4f(0.5, 0.5, 0.5, 1.0);
	glBegin(GL_QUADS);
      glNormal3f(-1.0, 0.0, 0.0);   //Facing -x (Left side)
	  glVertex3f(-length, buildingbaseHeight, -width);
	  glVertex3f(-length, buildingbaseHeight, width);
	  glVertex3f(-length, height, width);
	  glVertex3f(-length, height, -width);

      glNormal3f(1.0, 0.0, 0.0);   //Facing +x (Right side)
	  glVertex3f(length, buildingbaseHeight, width);
	  glVertex3f(length, buildingbaseHeight, -width);
	  glVertex3f(length, height, -width);
	  glVertex3f(length, height, width);
	glEnd();
}

void slope1(float length, float width, float height)
{

    float buildingbaseHeight = 0.0;

	glColor4f(0.8, 0.8, 0.8, 1.0);

    // glEnable(GL_TEXTURE_2D);
    // glBindTexture(GL_TEXTURE_2D, txId);
    
	// 3 large polygons (front, back, top)
	glBegin(GL_QUADS);
      glNormal3f(0.0, 0.0, 1.0);   //Facing +z (Front side)
    //    glTexCoord2f(0., 0.);
      glVertex3f(-length, buildingbaseHeight, width);
    //    glTexCoord2f(1., 0.);
      glVertex3f(length, buildingbaseHeight, width);
    //    glTexCoord2f(1., 87./256.);
      glVertex3f(length, height, width);
    //    glTexCoord2f(0., 87./256.);
      glVertex3f(-length, height, width);

      glNormal3f(0.0, 0.0, -1.0);   //Facing -z (Back side)
    //    glTexCoord2f(0., 0.);
      glVertex3f(length, buildingbaseHeight, -width);
    //    glTexCoord2f(1., 0.);
      glVertex3f(-length, buildingbaseHeight,-width);
    //    glTexCoord2f(1., 87./256.);
      glVertex3f(-length, height, width);
    //    glTexCoord2f(0., 87./256.);
      glVertex3f(length, height, width);

      glNormal3f(0.0, 1.0, 0.0);   //Facing +y (bottom side)
    // glTexCoord2f(0., 115./256.);
      glVertex3f(-length, buildingbaseHeight, width);
    // glTexCoord2f(1., 115./256.);
      glVertex3f(length, buildingbaseHeight,  width);
    // glTexCoord2f(1., 212./256.);
      glVertex3f(length, buildingbaseHeight, -width);
    // glTexCoord2f(0., 212./256.);
      glVertex3f(-length, buildingbaseHeight, -width);
	glEnd();

	// 2 small side triangles (left, right)
    // glDisable(GL_TEXTURE_2D);
	glColor4f(0.5, 0.5, 0.5, 1.0);
	glBegin(GL_TRIANGLES);
      glNormal3f(-1.0, 0.0, 0.0);   //Facing -x (Left side)
	  glVertex3f(-length, buildingbaseHeight, -width);
	  glVertex3f(-length, buildingbaseHeight, width);
	  glVertex3f(-length, height, width);

	  glNormal3f(1.0, 0.0, 0.0);   //Facing +x (Right side)
	  glVertex3f(length, buildingbaseHeight, -width);
	  glVertex3f(length, buildingbaseHeight, width);
	  glVertex3f(length, height, width);
	glEnd();
}

void slope2(float length, float width, float height)
{

    float buildingbaseHeight = 0.0;

	glColor4f(0.4, 0.3, 0.2, 1.0);

    // glEnable(GL_TEXTURE_2D);
    // glBindTexture(GL_TEXTURE_2D, txId);
    
	// 3 large polygons (front, back, top)
	glBegin(GL_QUADS);
      glNormal3f(0.0, 0.0, 1.0);   //Facing +z (Front side)
    //    glTexCoord2f(0., 0.);
      glVertex3f(-length, buildingbaseHeight, width);
    //    glTexCoord2f(1., 0.);
      glVertex3f(length, buildingbaseHeight, width);
    //    glTexCoord2f(1., 87./256.);
      glVertex3f(length, height, width);
    //    glTexCoord2f(0., 87./256.);
      glVertex3f(-length, height, width);

      glNormal3f(0.0, 0.0, -1.0);   //Facing -z (Back side)
    //    glTexCoord2f(0., 0.);
      glVertex3f(length, buildingbaseHeight, -width);
    //    glTexCoord2f(1., 0.);
      glVertex3f(-length, buildingbaseHeight,-width);
    //    glTexCoord2f(1., 87./256.);
      glVertex3f(-length, height, width);
    //    glTexCoord2f(0., 87./256.);
      glVertex3f(length, height, width);

      glNormal3f(0.0, 1.0, 0.0);   //Facing +y (bottom side)
    // glTexCoord2f(0., 115./256.);
      glVertex3f(-length, buildingbaseHeight, width);
    // glTexCoord2f(1., 115./256.);
      glVertex3f(length, buildingbaseHeight,  width);
    // glTexCoord2f(1., 212./256.);
      glVertex3f(length, buildingbaseHeight, -width);
    // glTexCoord2f(0., 212./256.);
      glVertex3f(-length, buildingbaseHeight, -width);
	glEnd();

	// 2 small side triangles (left, right)
    // glDisable(GL_TEXTURE_2D);
	glColor4f(0.5, 0.5, 0.5, 1.0);
	glBegin(GL_TRIANGLES);
      glNormal3f(-1.0, 0.0, 0.0);   //Facing -x (Left side)
	  glVertex3f(-length, buildingbaseHeight, -width);
	  glVertex3f(-length, buildingbaseHeight, width);
	  glVertex3f(-length, height, width);

	  glNormal3f(1.0, 0.0, 0.0);   //Facing +x (Right side)
	  glVertex3f(length, buildingbaseHeight, -width);
	  glVertex3f(length, buildingbaseHeight, width);
	  glVertex3f(length, height, width);
	glEnd();
}

void roof(float length, float width, float height)
{
    float buildingbaseHeight = 0.0;

	glColor4f(0.5, 0.2, 0.2, 1.0);

    // glEnable(GL_TEXTURE_2D);
    // glBindTexture(GL_TEXTURE_2D, txId);
    
	// 3 large polygons (front, back, top)
	glBegin(GL_QUADS);
      glNormal3f(0.0, 0.0, 1.0);   //Facing +z (Front top side)
    //    glTexCoord2f(0., 0.);
      glVertex3f(-length, buildingbaseHeight, width);
    //    glTexCoord2f(1., 0.);
      glVertex3f(length, buildingbaseHeight, width);
    //    glTexCoord2f(1., 87./256.);
      glVertex3f(length, height, 0);
    //    glTexCoord2f(0., 87./256.);
      glVertex3f(-length, height, 0);

      glNormal3f(0.0, 0.0, -1.0);   //Facing -z (Back top side)
    //    glTexCoord2f(0., 0.);
      glVertex3f(length, buildingbaseHeight, -width);
    //    glTexCoord2f(1., 0.);
      glVertex3f(-length, buildingbaseHeight,-width);
    //    glTexCoord2f(1., 87./256.);
      glVertex3f(-length, height, 0);
    //    glTexCoord2f(0., 87./256.);
      glVertex3f(length, height, 0);

      glNormal3f(0.0, 1.0, 0.0);   //Facing +y (bottom side)
    // glTexCoord2f(0., 115./256.);
      glVertex3f(-length, buildingbaseHeight, width);
    // glTexCoord2f(1., 115./256.);
      glVertex3f(length, buildingbaseHeight,  width);
    // glTexCoord2f(1., 212./256.);
      glVertex3f(length, buildingbaseHeight, -width);
    // glTexCoord2f(0., 212./256.);
      glVertex3f(-length, buildingbaseHeight, -width);
	glEnd();

	// 2 small side triangles (left, right)
    // glDisable(GL_TEXTURE_2D);
	glColor4f(0.5, 0.2, 0.1, 1.0);
	glBegin(GL_TRIANGLES);
      glNormal3f(-1.0, 0.0, 0.0);   //Facing -x (Left side)
	  glVertex3f(-length, buildingbaseHeight, -width);
	  glVertex3f(-length, buildingbaseHeight, width);
	  glVertex3f(-length, height, 0);

	  glNormal3f(1.0, 0.0, 0.0);   //Facing +x (Right side)
	  glVertex3f(length, buildingbaseHeight, -width);
	  glVertex3f(length, buildingbaseHeight, width);
	  glVertex3f(length, height, 0);
	glEnd();
}
