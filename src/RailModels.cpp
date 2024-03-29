//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//  Assignment 1
//  FILE NAME: RailModels.cpp
//  Author: MENG ZHANG
//  Student ID: 71682325
//  ========================================================================

#include <cmath>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "RailModels.h"

#include <iostream>
#include <fstream>
#include <climits>
#include <math.h>
#include "loadBMP.h"
#include "loadTGA.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////////
//                                TEXTURE LOADER                             //
///////////////////////////////////////////////////////////////////////////////

GLuint txWagon;
GLuint txRoof;
GLuint txShade;
GLuint txConcrete;
GLuint txHouseFront;
void loadWagonTexture()
{
	glGenTextures(1, &txWagon); 				// Create a Texture object
	glBindTexture(GL_TEXTURE_2D, txWagon);		//Use this texture
    loadBMP("WagonTexture.bmp");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void loadRoofTexture()
{
    glGenTextures(1, &txRoof); 				// Create a Texture object
	glBindTexture(GL_TEXTURE_2D, txRoof);		//Use this texture
    loadBMP("roofTexture.bmp");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void loadShadeTexture()
{
    glGenTextures(1, &txShade); 				// Create a Texture object
	glBindTexture(GL_TEXTURE_2D, txShade);		//Use this texture
    loadBMP("shadeTexture.bmp");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void loadConcreteTexture()
{
    glGenTextures(1, &txConcrete); 				// Create a Texture object
	glBindTexture(GL_TEXTURE_2D, txConcrete);		//Use this texture
    loadBMP("concreteTexture.bmp");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void loadHouseFrontTexture()
{
    glGenTextures(1, &txHouseFront); 				// Create a Texture object
	glBindTexture(GL_TEXTURE_2D, txHouseFront);		//Use this texture
    loadBMP("house.bmp");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

///////////////////////////////////////////////////////////////////////////////
//                                   FLOOR                                   //
///////////////////////////////////////////////////////////////////////////////

void floor()
{
	float white[4] = {1., 1., 1., 1.};
	float black[4] = {0};
	glColor4f(0.0, 0.2, 0.0, 1.0);  //The floor is gray in colour
	glNormal3f(0.0, 1.0, 0.0);

    // lab02.3 Mkae floors diffuse surfaces.
    // Supress specular reflections from the floor
    // by temporaily setting the specular colour to
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);

    // // set floor size to be 200 x 200
    int floorSize = 200;
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

void tracks(const float width, const int nvert, const float* x, const float* z)
{
    float p1x,p1z, p2x,p2z, p3x, p3z;
    float quadWidth = 2;
    glColor4f(0.2, 0.3, 0.5, 1.0);
    glBegin(GL_QUADS);
    for (int i = 0; i < nvert; i++)
    {
        if (i == nvert - 2)
        {
            p1x = x[i]; p1z = z[i];
            p2x = x[i+1]; p2z = z[i+1];
            p3x = x[0]; p3z = z[0];
        } else if (i == nvert - 1)
        {
            p1x = x[i]; p1z = z[i];
            p2x = x[0]; p2z = z[0];
            p3x = x[1]; p3z = z[1];
        } else
        {
            p1x = x[i]; p1z = z[i];
            p2x = x[i+1]; p2z = z[i+1];
            p3x = x[i+2]; p3z = z[i+2];
        }

        glm::vec3 p1(p1x, 1, p1z);
        glm::vec3 p2(p2x, 1, p2z);
        glm::vec3 p3(p3x, 1, p3z);

        glm::vec3 p1p2 = p2 - p1;
        glm::vec3 p1p2U = glm::normalize(p1p2);
        glm::vec3 v1(p1p2U[2], p1p2U[1], -p1p2U[0]);
        glm::vec3 a1 = p1 + v1 * width;
        glm::vec3 a2 = p1 + v1 * (width + quadWidth);
        glm::vec3 c1 = p1 + (-v1) * width;
        glm::vec3 c2 = p1 + (-v1) * (width + quadWidth);

        glm::vec3 p2p3 = p3 - p2;
        glm::vec3 p2p3U = glm::normalize(p2p3);
        glm::vec3 v2(p2p3U[2], p2p3U[1], -p2p3U[0]);
        glm::vec3 b1 = p2 + v2 * width;
        glm::vec3 b2 = p2 + v2 * (width + quadWidth);
        glm::vec3 d1 = p2 + (-v2) * width;
        glm::vec3 d2 = p2 + (-v2) * (width + quadWidth);

        glNormal3f(0, 1, 0);
        // outer track facing up
        glVertex3f(a1[0], 1, a1[2]);
        glVertex3f(a2[0], 1, a2[2]);
        glVertex3f(b2[0], 1, b2[2]);
        glVertex3f(b1[0], 1, b1[2]);

        // inter track facing up
        glVertex3f(c1[0], 1, c1[2]);
        glVertex3f(d1[0], 1, d1[2]);
        glVertex3f(d2[0], 1, d2[2]);
        glVertex3f(c2[0], 1, c2[2]);

        glNormal3f(v1[0], v1[1], v1[2]);
        // outer facing outward
        glVertex3f(a2[0], 1, a2[2]);
        glVertex3f(b2[0], 1, b2[2]);
        glVertex3f(b2[0], 0, b2[2]);
        glVertex3f(a2[0], 0, a2[2]);
        // inner facing outward
        glVertex3f(c1[0], 1, c1[2]);
        glVertex3f(d1[0], 1, d1[2]);
        glVertex3f(d1[0], 0, d1[2]);
        glVertex3f(c1[0], 0, c1[2]);

        glNormal3f(-v1[0], -v1[1], -v1[2]);
        // outer facing inward
        glVertex3f(a1[0], 1, a1[2]);
        glVertex3f(b1[0], 1, b1[2]);
        glVertex3f(b1[0], 0, b1[2]);
        glVertex3f(a1[0], 0, a1[2]);
        // inner facing inward
        glVertex3f(c2[0], 1, c2[2]);
        glVertex3f(d2[0], 1, d2[2]);
        glVertex3f(d2[0], 0, d2[2]);
        glVertex3f(c2[0], 0, c2[2]);
    }
    glEnd();
}

void sleepers(const int nvert, const float* x, const float* z)
{
    float p1x,p1z, p2x,p2z;
    float length = 8;
    float widthScaler = 1.5;
    glColor4f(0.3, 0.1, 0.0, 1.0);
    glBegin(GL_QUADS);
    for (int i = 0; i < nvert; i += 4)
    {

        if (i == nvert - 1)
        {
            p1x = x[i]; p1z = z[i];
            p2x = x[0]; p2z = z[0];
        } else
        {
            p1x = x[i]; p1z = z[i];
            p2x = x[i+1]; p2z = z[i+1];
        }

        glm::vec3 p1(p1x, 1, p1z);
        glm::vec3 p2(p2x, 1, p2z);

        glm::vec3 p1p2 = p2 - p1;
        glm::vec3 p1p2U = glm::normalize(p1p2);
        glm::vec3 v1(p1p2U[2], p1p2U[1], -p1p2U[0]);
        glm::vec3 s1 = p1 + v1 * length;
        glm::vec3 s2 = s1 + p1p2U * widthScaler;
        glm::vec3 s4 = p1 + (-v1) * length;
        glm::vec3 s3 = s4 + p1p2U * widthScaler;

        // sleeper
        glNormal3f(0, 1, 0);
        glVertex3f(s1[0], 0.1, s1[2]);
        glVertex3f(s2[0], 0.1, s2[2]);
        glVertex3f(s3[0], 0.1, s3[2]);
        glVertex3f(s4[0], 0.1, s4[2]);
    }
    glEnd();
}

///////////////////////////////////////////////////////////////////////////////
//                                   TUNNEL                                  //
///////////////////////////////////////////////////////////////////////////////

void tunnel(const int start, const int length, const int nvert, const float* x, const float* z)
{
    float p1x,p1z, p2x,p2z, p3x, p3z;
    float width = 15;
    float quadWidth = 5;
    float basequadWidth = 10;
    float height = 25;
    float roofHeight = 2.5;
    // glColor4f(0.2, 0.3, 0.6, 1.0);
    glColor4f(0.5, 0.5, 0.5, 1.0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txConcrete);

    glBegin(GL_QUADS);
    for (int i = start; i < start+length; i++)
    {
        if (i == nvert - 2)
        {
            p1x = x[i]; p1z = z[i];
            p2x = x[i+1]; p2z = z[i+1];
            p3x = x[0]; p3z = z[0];
        } else if (i == nvert - 1)
        {
            p1x = x[i]; p1z = z[i];
            p2x = x[0]; p2z = z[0];
            p3x = x[1]; p3z = z[1];
        } else
        {
            p1x = x[i]; p1z = z[i];
            p2x = x[i+1]; p2z = z[i+1];
            p3x = x[i+2]; p3z = z[i+2];
        }

        glm::vec3 p1(p1x, 1, p1z);
        glm::vec3 p2(p2x, 1, p2z);
        glm::vec3 p3(p3x, 1, p3z);

        glm::vec3 p1p2 = p2 - p1;
        glm::vec3 p1p2U = glm::normalize(p1p2);
        glm::vec3 v1(p1p2U[2], p1p2U[1], -p1p2U[0]);
        glm::vec3 a1 = p1 + v1 * width;
        glm::vec3 e1 = p1 + v1 * width;
        glm::vec3 a2 = p1 + v1 * (width + quadWidth);
        glm::vec3 r1 = a2;
        glm::vec3 e2 = p1 + v1 * (width + basequadWidth);
        glm::vec3 c1 = p1 + (-v1) * width;
        glm::vec3 g1 = p1 + (-v1) * width;
        glm::vec3 c2 = p1 + (-v1) * (width + quadWidth);
        glm::vec3 r4 = c2;
        glm::vec3 g2 = p1 + (-v1) * (width + basequadWidth);

        glm::vec3 p2p3 = p3 - p2;
        glm::vec3 p2p3U = glm::normalize(p2p3);
        glm::vec3 v2(p2p3U[2], p2p3U[1], -p2p3U[0]);
        glm::vec3 b1 = p2 + v2 * width;
        glm::vec3 f1 = p2 + v2 * width;
        glm::vec3 b2 = p2 + v2 * (width + quadWidth);
        glm::vec3 r2 = b2;
        glm::vec3 f2 = p2 + v2 * (width + basequadWidth);
        glm::vec3 d1 = p2 + (-v2) * width;
        glm::vec3 h1 = p2 + (-v2) * width;
        glm::vec3 d2 = p2 + (-v2) * (width + quadWidth);
        glm::vec3 r3 = d2;
        glm::vec3 h2 = p2 + (-v2) * (width + basequadWidth);

        glNormal3f(0, 1, 0);
        // inner quad facing up
        glTexCoord2f(0., 0.);
        glVertex3f(a1[0], height, a1[2]);
        glTexCoord2f(1., 0.);
        glVertex3f(a2[0], height, a2[2]);
        glTexCoord2f(1., 1.);
        glVertex3f(b2[0], height, b2[2]);
        glTexCoord2f(0., 1.);
        glVertex3f(b1[0], height, b1[2]);

        // outer track facing up
        glTexCoord2f(0., 0.);
        glVertex3f(c1[0], height, c1[2]);
        glTexCoord2f(1., 0.);
        glVertex3f(d1[0], height, d1[2]);
        glTexCoord2f(1., 1.);
        glVertex3f(d2[0], height, d2[2]);
        glTexCoord2f(0., 1.);
        glVertex3f(c2[0], height, c2[2]);

        glNormal3f(v1[0], 1, v1[2]);
        // inner facing outward
        glTexCoord2f(0., 0.);
        glVertex3f(a2[0], height, a2[2]);
        glTexCoord2f(1., 0.);
        glVertex3f(b2[0], height, b2[2]);
        glTexCoord2f(1., 1.);
        glVertex3f(f2[0], 0, f2[2]);
        glTexCoord2f(0., 1.);
        glVertex3f(e2[0], 0, e2[2]);

        // outer facing outward
        glTexCoord2f(0., 0.);
        glVertex3f(c1[0], height, c1[2]);
        glTexCoord2f(1., 0.);
        glVertex3f(d1[0], height, d1[2]);
        glTexCoord2f(1., 1.);
        glVertex3f(h1[0], 0, h1[2]);
        glTexCoord2f(0., 1.);
        glVertex3f(g1[0], 0, g1[2]);

        // inner facing inward
        glTexCoord2f(0., 0.);
        glVertex3f(a1[0], height, a1[2]);
        glTexCoord2f(1., 0.);
        glVertex3f(b1[0], height, b1[2]);
        glTexCoord2f(1., 1.);
        glVertex3f(f1[0], 0, f1[2]);
        glTexCoord2f(0., 1.);
        glVertex3f(e1[0], 0, e1[2]);
        // outer facing inward
        glTexCoord2f(0., 0.);
        glVertex3f(c2[0], height, c2[2]);
        glTexCoord2f(1., 0.);
        glVertex3f(d2[0], height, d2[2]);
        glTexCoord2f(1., 1.);
        glVertex3f(h2[0], 0, h2[2]);
        glTexCoord2f(0., 1.);
        glVertex3f(g2[0], 0, g2[2]);

        // inner vertical quads
        glNormal3f(1, 0, 1);
        glTexCoord2f(0., 0.);
        glVertex3f(a1[0], height, a1[2]);
        glTexCoord2f(1., 0.);
        glVertex3f(a2[0], height, a2[2]);
        glTexCoord2f(1., 1.);
        glVertex3f(e2[0], 0, e2[2]);
        glTexCoord2f(0., 1.);
        glVertex3f(e1[0], 0, e1[2]);

        glTexCoord2f(0., 0.);
        glVertex3f(b1[0], height, b1[2]);
        glTexCoord2f(1., 0.);
        glVertex3f(b2[0], height, b2[2]);
        glTexCoord2f(1., 1.);
        glVertex3f(f2[0], 0, f2[2]);
        glTexCoord2f(0., 1.);
        glVertex3f(f1[0], 0, f1[2]);

        // outer vertical quads
        glTexCoord2f(0., 0.);
        glVertex3f(c1[0], height, c1[2]);
        glTexCoord2f(1., 0.);
        glVertex3f(c2[0], height, c2[2]);
        glTexCoord2f(1., 1.);
        glVertex3f(g2[0], 0, g2[2]);
        glTexCoord2f(0., 1.);
        glVertex3f(g1[0], 0, g1[2]);

        glTexCoord2f(0., 0.);
        glVertex3f(d1[0], height, d1[2]);
        glTexCoord2f(1., 0.);
        glVertex3f(d2[0], height, d2[2]);
        glTexCoord2f(1., 1.);
        glVertex3f(h2[0], 0, h2[2]);
        glTexCoord2f(0., 1.);
        glVertex3f(h1[0], 0, h1[2]);

        // roof quads
        // top upper quad
        glNormal3f(0, 1, 0);
        glTexCoord2f(0., 0.);
        glVertex3f(r1[0], height+roofHeight, r1[2]);
        glTexCoord2f(1., 0.);
        glVertex3f(r2[0], height+roofHeight, r2[2]);
        glTexCoord2f(1., 1.);
        glVertex3f(r3[0], height+roofHeight, r3[2]);
        glTexCoord2f(0., 1.);
        glVertex3f(r4[0], height+roofHeight, r4[2]);
        // front quad
        glNormal3f(0, 0, 1);
        glTexCoord2f(0., 0.);
        glVertex3f(r1[0], height+roofHeight, r1[2]);
        glTexCoord2f(1., 0.);
        glVertex3f(r4[0], height+roofHeight, r4[2]);
        glTexCoord2f(1., 1.);
        glVertex3f(c2[0], height, c2[2]);
        glTexCoord2f(0., 1.);
        glVertex3f(a2[0], height, a2[2]);
        // back quad
        glNormal3f(1, 0, 0);
        glTexCoord2f(0., 0.);
        glVertex3f(r2[0], height+roofHeight, r2[2]);
        glTexCoord2f(1., 0.);
        glVertex3f(r3[0], height+roofHeight, r3[2]);
        glTexCoord2f(1., 1.);
        glVertex3f(d2[0], height, d2[2]);
        glTexCoord2f(0., 1.);
        glVertex3f(b2[0], height, b2[2]);

        // outer quad
        glNormal3f(0, 1, 0);
        glTexCoord2f(0., 0.);
        glVertex3f(r4[0], height+roofHeight, r4[2]);
        glTexCoord2f(1., 0.);
        glVertex3f(r3[0], height+roofHeight, r3[2]);
        glTexCoord2f(1., 1.);
        glVertex3f(d2[0], height, d2[2]);
        glTexCoord2f(0., 1.);
        glVertex3f(c2[0], height, c2[2]);
        // innder quad
        glTexCoord2f(0., 0.);
        glVertex3f(r1[0], height+roofHeight, r1[2]);
        glTexCoord2f(1., 0.);
        glVertex3f(r2[0], height+roofHeight, r2[2]);
        glTexCoord2f(1., 1.);
        glVertex3f(b2[0], height, b2[2]);
        glTexCoord2f(0., 1.);
        glVertex3f(a2[0], height, a2[2]);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
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
    glColor4f(0.4, 0.0, 0.1, 1.0);    //Wheel color
	GLUquadric *q = gluNewQuadric();   //Disc

	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		glTranslatef(wx[i], 2.0, wz[i]);
		gluDisk(q, 0.0, 2.0, 20, 2);
        glScalef(1, 1, 2);
        glutSolidCube(0.5);
		glPopMatrix();
	}

    /*
    	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		glTranslatef(wx[i], 2.0, wz[i]+1);
		gluDisk(q, 0.0, 2.0, 20, 2);
        glScalef(1, 1, 2);
        glutSolidCube(0.5);
		glPopMatrix();
	}
    */
}

//--------------- LOCOMOTIVE --------------------------------
// This simple model of a locomotive consists of the base,
// cabin and boiler
//-----------------------------------------------------------
void engine()
{
    base();

    //Cab
    glColor4f(0.3, 0.2, 0.5, 1.0);
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
      glColor4f(0.2, 0.2, 0.2, 1.0);
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

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txWagon);

	//3 large polygons (front, back, top)
	glBegin(GL_QUADS);
    glNormal3f(0.0, 0.0, 1.0);   //Facing +z (Front side)
      glTexCoord2f(0., 0.);
      glVertex3f(-wagonLength, 5.0, 6.0);
      glTexCoord2f(1., 0.);
      glVertex3f(wagonLength, 5.0, 6.0);
      glTexCoord2f(1., 87./256.);
      glVertex3f(wagonLength, 17.0, 6.0);
      glTexCoord2f(0., 87./256.);
      glVertex3f(-wagonLength, 17.0, 6.0);

      glNormal3f(0.0, 0.0, -1.0);   //Facing -z (Back side)
      glTexCoord2f(0., 0.);
      glVertex3f(wagonLength, 5.0, -6.0);
      glTexCoord2f(1., 0.);
      glVertex3f(-wagonLength, 5.0,-6.0);
      glTexCoord2f(1., 87./256.);
      glVertex3f(-wagonLength, 17.0,-6.0);
      glTexCoord2f(0., 87./256.);
      glVertex3f(wagonLength, 17.0, -6.0);

      glNormal3f(0.0, 1.0, 0.0);   //Facing +y (Top side)
      glTexCoord2f(0., 115./256.);
      glVertex3f(-wagonLength  , 17.0, 6.0);
      glTexCoord2f(1., 115./256.);
      glVertex3f(wagonLength  , 17.0,  6.0);
      glTexCoord2f(1., 212./256.);
      glVertex3f(wagonLength, 17.0, -6.0);
      glTexCoord2f(0., 212./256.);
      glVertex3f(-wagonLength, 17.0, -6.0);
	glEnd();

	// 2 small side polygons (left, right)
    glDisable(GL_TEXTURE_2D);

	glColor4f(0.3, 0.3, 0.3, 1.0);
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
    blockBase(45, 70, 5);
    glPopMatrix();

    // Quads building-1
    glPushMatrix();
    glTranslatef(-15.0, 4, 30.0);
    glRotatef(90, 0, 1, 0);
    block1(30, 15, 35);
    glPopMatrix();

    // // building-1 roof
    glPushMatrix();
    glTranslatef(-15.0, 39, 30.0);
    glRotatef(90, 0, 1, 0);
    roof(30, 15, 20);
    glPopMatrix();

    // // building-1 shade cover
    glPushMatrix();
    glTranslatef(10.0, 36.8, 30.0);
    glRotatef(-90, 0, 1, 0);
    slope2(28.5, 20, 2);
    glPopMatrix();

    // // building-1 shade cover sticks
    glPushMatrix();
    glColor4f(0.1, 0.4, 0.5, 1.0);
    glTranslatef(10, 30, 58);
    glRotatef(25, 0, 0, 1);
    glRotatef(180, 0, 1, 0);
    glScalef(65, 1, 1);
    glutSolidCube(0.5);
    glPopMatrix();

    glPushMatrix();
    glColor4f(0.1, 0.4, 0.5, 1.0);
    glTranslatef(10, 30, 5);
    glRotatef(25, 0, 0, 1);
    glRotatef(180, 0, 1, 0);
    glScalef(65, 1, 1);
    glutSolidCube(0.5);
    glPopMatrix();

    // Quads building-2
    glPushMatrix();
    glTranslatef(-15.0, 4, -12.0);
    glRotatef(90, 0, 1, 0);
    block2(12, 15, 20);
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

	glColor4f(0.6, 0.6, 0.6, 1.0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txConcrete);

	// 3 large polygons (front, back, top)
	glBegin(GL_QUADS);
      glNormal3f(0.0, 0.0, 1.0);   //Facing +z (Front side)
      //      glTexCoord2f(0., 0.);
      glVertex3f(-length, buildingbaseHeight, width);
      //      glTexCoord2f(1., 0.);
      glVertex3f(length, buildingbaseHeight, width);
      //      glTexCoord2f(1., 87./256.);
      glVertex3f(length, height, width);
      //      glTexCoord2f(0., 87./256.);
      glVertex3f(-length, height, width);

      glNormal3f(0.0, 0.0, -1.0);   //Facing -z (Back side)
      glTexCoord2f(0., 0.);
      glVertex3f(length, buildingbaseHeight, -width);
      glTexCoord2f(1.0, 0.);
      glVertex3f(-length, buildingbaseHeight,-width);
      glTexCoord2f(1., 1.);
      glVertex3f(-length, height,-width);
      glTexCoord2f(0., 1.);
      glVertex3f(length, height, -width);

      glNormal3f(0.0, 1.0, 0.0);   //Facing +y (Top side)
      glTexCoord2f(0., 0.);
      glVertex3f(-length, height, width);
      glTexCoord2f(1.0, 0.);
      glVertex3f(length, height,  width);
      glTexCoord2f(1., 1.);
      glVertex3f(length, height, -width);
      glTexCoord2f(0., 1.);
      glVertex3f(-length, height, -width);
	glEnd();

	// 2 small side polygons (left, right)
	glColor4f(0.5, 0.5, 0.5, 1.0);
	glBegin(GL_QUADS);
      glNormal3f(1.0, 0.0, 0.0);   //Facing -x (Left side)
      glTexCoord2f(0., 0.);
	  glVertex3f(-length, buildingbaseHeight, -width);
      glTexCoord2f(1.0, 0.);
	  glVertex3f(-length, buildingbaseHeight, width);
      glTexCoord2f(1., 1.);
	  glVertex3f(-length, height, width);
      glTexCoord2f(0., 1.);
	  glVertex3f(-length, height, -width);

      glNormal3f(1.0, 0.0, 0.0);   //Facing +x (Right side)
      glTexCoord2f(0., 0.);
	  glVertex3f(length, buildingbaseHeight, width);
      glTexCoord2f(1.0, 0.);
	  glVertex3f(length, buildingbaseHeight, -width);
      glTexCoord2f(1., 1.);
	  glVertex3f(length, height, -width);
      glTexCoord2f(0., 1.);
	  glVertex3f(length, height, width);
	glEnd();

    glDisable(GL_TEXTURE_2D);
}

void block1(float length, float width, float height)
{
    float buildingbaseHeight = 0.01;

	glColor4f(0.8, 0.8, 0.8, 1.0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txHouseFront);

	// 3 large polygons (front, back, top)
	glBegin(GL_QUADS);
      glNormal3f(0.0, 0.0, 1.0);   //Facing +z (Front side)
      glTexCoord2f(0., 0.);
      glVertex3f(-length, buildingbaseHeight, width);
      glTexCoord2f(1., 0.);
      glVertex3f(length, buildingbaseHeight, width);
      glTexCoord2f(1., 1.);
      glVertex3f(length, height, width);
      glTexCoord2f(0., 1.);
      glVertex3f(-length, height, width);

      glNormal3f(0.0, 0.0, -1.0);   //Facing -z (Back side)
      glTexCoord2f(0.7, 0.);
      glVertex3f(length, buildingbaseHeight, -width);
      glTexCoord2f(1., 0.);
      glVertex3f(-length, buildingbaseHeight,-width);
      glTexCoord2f(1., 1);
      glVertex3f(-length, height,-width);
      glTexCoord2f(0.7, 1);
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

	glColor4f(0.8, 0.8, 0.8, 1.0);
	glBegin(GL_QUADS);
      glNormal3f(1.0, 0.0, 0.0);   //Facing -x (Left side)
      glTexCoord2f(0.7, 0.);
	  glVertex3f(-length, buildingbaseHeight, -width);
      glTexCoord2f(1., 0.);
	  glVertex3f(-length, buildingbaseHeight, width);
      glTexCoord2f(1., 1);
	  glVertex3f(-length, height, width);
      glTexCoord2f(0.7, 1);
	  glVertex3f(-length, height, -width);

      glNormal3f(1.0, 0.0, 0.0);   //Facing +x (Right side)
      glTexCoord2f(0.7, 0.);
	  glVertex3f(length, buildingbaseHeight, width);
      glTexCoord2f(1., 0.);
	  glVertex3f(length, buildingbaseHeight, -width);
      glTexCoord2f(1., 1);
	  glVertex3f(length, height, -width);
      glTexCoord2f(0.7, 1);
	  glVertex3f(length, height, width);

      glDisable(GL_TEXTURE_2D);
	glEnd();
}

void block2(float length, float width, float height)
{
    float buildingbaseHeight = 0.01;

	glColor4f(0.8, 0.8, 0.8, 1.0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txHouseFront);

	// 3 large polygons (front, back, top)
	glBegin(GL_QUADS);
      glNormal3f(0.0, 0.0, 1.0);   //Facing +z (Front side)
      glTexCoord2f(0.7, 0.);
      glVertex3f(-length, buildingbaseHeight, width);
      glTexCoord2f(1., 0.);
      glVertex3f(length, buildingbaseHeight, width);
      glTexCoord2f(1., 0.5);
      glVertex3f(length, height, width);
      glTexCoord2f(0.7, 0.5);
      glVertex3f(-length, height, width);

      glNormal3f(0.0, 0.0, -1.0);   //Facing -z (Back side)
      glTexCoord2f(0.7, 0.);
      glVertex3f(length, buildingbaseHeight, -width);
      glTexCoord2f(1., 0.);
      glVertex3f(-length, buildingbaseHeight,-width);
      glTexCoord2f(1., 0.5);
      glVertex3f(-length, height,-width);
      glTexCoord2f(0.7, 0.5);
      glVertex3f(length, height, -width);

      glNormal3f(0.0, 1.0, 0.0);   //Facing +y (Top side)
      glTexCoord2f(0.7, 0.);
      glVertex3f(-length, height, width);
      glTexCoord2f(1., 0.);
      glVertex3f(length, height,  width);
      glTexCoord2f(1., 0.5);
      glVertex3f(length, height, -width);
      glTexCoord2f(0.7, 0.5);
      glVertex3f(-length, height, -width);
	glEnd();

	// 2 small side polygons (left, right)
	glColor4f(0.5, 0.5, 0.5, 1.0);
	glBegin(GL_QUADS);
      glNormal3f(-1.0, 0.0, 0.0);   //Facing -x (Left side)
	  glVertex3f(-length, buildingbaseHeight, -width);
	  glVertex3f(-length, buildingbaseHeight, width);
	  glVertex3f(-length, height, width);
	  glVertex3f(-length, height, -width);

      glNormal3f(0.0, 0.0, 1.0);   //Facing +x (Right side)
      glTexCoord2f(0.7, 0.);
	  glVertex3f(length, buildingbaseHeight, width);
      glTexCoord2f(1., 0.);
	  glVertex3f(length, buildingbaseHeight, -width);
      glTexCoord2f(1., 0.5);
	  glVertex3f(length, height, -width);
      glTexCoord2f(0.7, 0.5);
	  glVertex3f(length, height, width);

      glDisable(GL_TEXTURE_2D);
	glEnd();
}

void slope1(float length, float width, float height)
{

    float buildingbaseHeight = 0.0;

	glColor4f(0.55, 0.55, 0.55, 1.0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txConcrete  );

	// 3 large polygons (front, back, top)
	glBegin(GL_QUADS);
      glNormal3f(0.0, 1.0, 0.0);   //Facing +z (Front side)
      glTexCoord2f(0., 0.);
      glVertex3f(-length, buildingbaseHeight, width);
      glTexCoord2f(1., 0.);
      glVertex3f(length, buildingbaseHeight, width);
      glTexCoord2f(1., 1.);
      glVertex3f(length, height, width);
      glTexCoord2f(0., 1.);
      glVertex3f(-length, height, width);

      glNormal3f(0.0, 1.0, 0.0);   //Facing -z (Back side)
      glTexCoord2f(0., 0.);
      glVertex3f(length, buildingbaseHeight, -width);
      glTexCoord2f(1., 0.);
      glVertex3f(-length, buildingbaseHeight,-width);
      glTexCoord2f(1., 1.);
      glVertex3f(-length, height, width);
      glTexCoord2f(0., 1.);
      glVertex3f(length, height, width);

      glNormal3f(0.0, 1.0, 0.0);   //Facing +y (bottom side)
      glTexCoord2f(0., 0.);
      glVertex3f(-length, buildingbaseHeight, width);
      glTexCoord2f(1., 0.);
      glVertex3f(length, buildingbaseHeight,  width);
      glTexCoord2f(1., 1.);
      glVertex3f(length, buildingbaseHeight, -width);
      glTexCoord2f(0., 1.);
      glVertex3f(-length, buildingbaseHeight, -width);
	glEnd();

	// 2 small side triangles (left, right)
	glColor4f(0.5, 0.5, 0.5, 1.0);
	glBegin(GL_TRIANGLES);
      glNormal3f(0.0, 0.0, 1.0);   //Facing -x (Left side)
      glTexCoord2f(0., 0.);
	  glVertex3f(-length, buildingbaseHeight, -width);
      glTexCoord2f(1., 0.);
	  glVertex3f(-length, buildingbaseHeight, width);
      glTexCoord2f(1., 1.);
	  glVertex3f(-length, height, width);

	  glNormal3f(0.0, 0.0, 1.0);   //Facing +x (Right side)
      glTexCoord2f(0., 0.);
	  glVertex3f(length, buildingbaseHeight, -width);
      glTexCoord2f(1., 0.);
	  glVertex3f(length, buildingbaseHeight, width);
      glTexCoord2f(1., 1.);
	  glVertex3f(length, height, width);
	glEnd();

    glDisable(GL_TEXTURE_2D);
}

void slope2(float length, float width, float height)
{

    float buildingbaseHeight = 0.0;

	glColor4f(0.9, 0.9, 0.9, 1.0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txShade);

	// 3 large polygons (front, back, top)
	glBegin(GL_QUADS);
      glNormal3f(0.0, 1.0, 0.0);   //Facing +z (Front side)
      //      glTexCoord2f(0., 0.);
      glVertex3f(-length, buildingbaseHeight, width);
      //      glTexCoord2f(1., 0.);
      glVertex3f(length, buildingbaseHeight, width);
      //      glTexCoord2f(1., 87./256.);
      glVertex3f(length, height, width);
      //      glTexCoord2f(0., 87./256.);
      glVertex3f(-length, height, width);

      glNormal3f(0.0, 1.0, 0.0);   //Facing -z (Top side)
      glTexCoord2f(0., 0.);
      glVertex3f(length, buildingbaseHeight, -width);
      glTexCoord2f(1., 0.);
      glVertex3f(-length, buildingbaseHeight,-width);
      glTexCoord2f(1., 0.5);
      glVertex3f(-length, height, width);
      glTexCoord2f(0., 0.5);
      glVertex3f(length, height, width);

      glNormal3f(0.0, 1.0, 0.0);   //Facing +y (bottom side)
      glTexCoord2f(0., 0.);
      glVertex3f(-length, buildingbaseHeight, width);
      glTexCoord2f(1., 0.);
      glVertex3f(length, buildingbaseHeight,  width);
      glTexCoord2f(1., 0.5);
      glVertex3f(length, buildingbaseHeight, -width);
      glTexCoord2f(0., 0.5);
      glVertex3f(-length, buildingbaseHeight, -width);
	glEnd();

	// 2 small side triangles (left, right)
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

    glDisable(GL_TEXTURE_2D);
}

void roof(float length, float width, float height)
{
    float buildingbaseHeight = 0.0;

	glColor4f(0.5, 0.2, 0.2, 1.0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txRoof);

	// 3 large polygons (front, back, top)
	glBegin(GL_QUADS);
      glNormal3f(0.0, 0.0, 1.0);   //Facing +z (Front top side)
      glTexCoord2f(0., 0.);
      glVertex3f(-length, buildingbaseHeight, width);
      glTexCoord2f(1., 0.);
      glVertex3f(length, buildingbaseHeight, width);
      glTexCoord2f(1., 87./256.);
      glVertex3f(length, height, 0);
      glTexCoord2f(0., 87./256.);
      glVertex3f(-length, height, 0);

      glNormal3f(0.0, 0.0, -1.0);   //Facing -z (Back top side)
      glTexCoord2f(0., 0.);
      glVertex3f(length, buildingbaseHeight, -width);
      glTexCoord2f(1., 0.);
      glVertex3f(-length, buildingbaseHeight,-width);
      glTexCoord2f(1., 87./256.);
      glVertex3f(-length, height, 0);
      glTexCoord2f(0., 87./256.);
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

    glDisable(GL_TEXTURE_2D);
}

///////////////////////////////////////////////////////////////////////////////
//                              Signaling Lights                             //
///////////////////////////////////////////////////////////////////////////////
void signalLights(int timer)
{
    // lights-1
	GLUquadric *q = gluNewQuadric();   //Cylinder
    glPushMatrix();
    if ((timer <= 20) ||
        ((timer > 40) && (timer <= 60)) ||
        ((timer > 80) && (timer <= 100)) ||
        ((timer > 120) && (timer <= 140)) ||
        ((timer > 160) && (timer <= 180)) ||
        ((timer > 200) && (timer <= 220)) ||
        ((timer > 240) && (timer <= 260)) ||
        ((timer >280) && (timer <= 300)))
    {
        glColor4f(0.3, 1, 0.1, 1.0);
    } else {
        glColor4f(1, 0, 0, 1.0);
    }
    glTranslatef(0.0, 25.0, -0.7);
    gluDisk(q, 0.0, 2.0, 20,2);
    glPopMatrix();

    // lights-2
    glPushMatrix();
    if ((timer <= 20) ||
        ((timer > 40) && (timer <= 60)) ||
        ((timer > 80) && (timer <= 100)) ||
        ((timer > 120) && (timer <= 140)) ||
        ((timer > 160) && (timer <= 180)) ||
        ((timer > 200) && (timer <= 220)) ||
        ((timer > 240) && (timer <= 260)) ||
        ((timer >280) && (timer <= 300)))
    {
        glColor4f(0.3, 1, 0.1, 1.0);
    } else {
        glColor4f(0.9, 1.0, 0.1, 1.0);
    }
    glTranslatef(0.0, 20.0, -0.7);
    gluDisk(q, 0.0, 2.0, 20,2);
    glPopMatrix();

    // lights-3
    glPushMatrix();
    if ((timer <= 20) ||
        ((timer > 40) && (timer <= 60)) ||
        ((timer > 80) && (timer <= 100)) ||
        ((timer > 120) && (timer <= 140)) ||
        ((timer > 160) && (timer <= 180)) ||
        ((timer > 200) && (timer <= 220)) ||
        ((timer > 240) && (timer <= 260)) ||
        ((timer >280) && (timer <= 300))) {
        glColor4f(0.3, 1.0, 0.1, 1.0);
    } else {
        glColor4f(0.1, 0.0, 0.3, 1.0);
    }
    glTranslatef(0.0, 15.0, -0.7);
    gluDisk(q, 0.0, 2.0, 20,2);
    glPopMatrix();

    // lights pols
    glPushMatrix();
    glColor4f(0.0, 0.2, 0.2, 1.0);
    glTranslatef(0.0, 0.0, -2);
    glScalef(1, 60, 1);
    glutSolidCube(1.0);
    glPopMatrix();

    // lights box
    glPushMatrix();
    glColor4f(0.2, 0.2, 0.2, 1.0);
    glTranslatef(0.0, 20.0, -2);
    glScalef(5.5, 15.5, 2.5);
    glutSolidCube(1.0);
    glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////
//                                 Air Plane                                 //
///////////////////////////////////////////////////////////////////////////////
void airPlane(int timer)
{
    // wings
    glNormal3f(0.0, 1.0, 0.0);
    glColor4f(0.8, 0.8, 0.0, 1.0);
    glPushMatrix();
      glTranslatef(-4.0, 9.5, 0.0);
      glScalef(4.0, 1.0, 35.0);
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(6.0, 9.5, 0.0);
      glScalef(4.0, 1.0, 15.0);
      glutSolidCube(1.0);
    glPopMatrix();

    glColor4f(0.4, 0.4, 0.4, 1.0);
    glPushMatrix();
      glTranslatef(6.0, 14.0, 0.0);
      glScalef(4.0, 8.0, 1.0);
      glutSolidCube(1.0);
    glPopMatrix();

    //plane body
	GLUquadric *q = gluNewQuadric();
    glPushMatrix();
      glColor4f(0.5, 0., 0., 1.0);
      glTranslatef(4.0, 10.0, 0.0);
      glRotatef(-90.0, 0., 1., 0.);
      gluCylinder(q, 4.0, 2.0, 14.0, 30, 5);
      glTranslatef(0.0, 0.0, 14.0);
      gluDisk(q, 0.0, 2.0, 20, 3);
      if ((timer <= 20) ||
          ((timer > 40) && (timer <= 60)) ||
          ((timer > 80) && (timer <= 100)) ||
          ((timer > 120) && (timer <= 140)) ||
          ((timer > 160) && (timer <= 180)) ||
          ((timer > 200) && (timer <= 220)) ||
          ((timer > 240) && (timer <= 260)) ||
          ((timer >280) && (timer <= 300))) {
          glColor4f(0.3, 1.0, 0.1, 1.0);
      } else {
          glColor4f(0.1, 0.0, 0.3, 1.0);
      }
      glTranslatef(0.2, 0.2, 0.1);
      gluDisk(q, 0.0, 1.0, 20,2);  //headlight!
    glPopMatrix();

}
