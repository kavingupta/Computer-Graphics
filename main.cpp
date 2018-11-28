
#define _CRT_SECURE_NO_DEPRECATE
// Windows includes (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.
#include <cassert>
// OpenGL includes
#include<GL\glew.h>
#include <GL\freeglut.h>


#include <assert.h>
#include <fstream>
#include <GL/glut.h>


// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "GL/maths_funcs.h"
#define STB_IMAGE_IMPLEMENTATION
#include <GL\stb_image.h>

#define M_PI 3.14

#include <iostream>
#include <stdlib.h>
#include <math.h>

#include <GL/glut.h>



#include <cmath>
#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>
#include <ctime>
#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;





//****BEGIN GLOBAL VARIBLES******
//	SNOWMAN TERRAIN










//WIREFRAME MODE
int w = 0;

//LIGHTING
int l = 0;

//CIRCLE ALGORITHM
int temp = 0;

//RESET
int temp1 = 0;

//WINDOWS
int window_1, window_2;

//snowman functionality
//SNOWMAN POSITION
float pos[3] = { 1, 1, 1 };
//SNOWMAN ROTATION
float rot[3] = { 0, 0, 0 };
//SNOWMAN HEAD ROTATION
float headRot[3] = { 0, 0, 0 };

//CAMERA POSITION
float CamPos[3] = { -29, 22, -29 };

//TERRAIN AREA
float terrainArea;

float terrainGuard;

//HEIGHMAP TO STORE HEIGHT VALUES
float HeightMap[500][500];

//NORMAL VECTOR STORAGE ARRAYS
float FaceNormalStorage[500][500][3];
float VertexNormalStorage[500][500][3];

//LIGHT POSITIONS, FAULT ALGORITHM
float light_pos[4] = { -5, 5, -5, 0 };
float secondlight_pos[4] = { 5, 5, 5, 0 };

//LIGHT POSITIONS, CIRCLE ALGORITHM
float light_posCircle[4];
float secondlight_posCircle[4];

//SHININESS
const float shiny = 1.0f;

// *****END GLOBAL VARIBLES******

//THIS FUNCTION IMPLEMENTS AN ALGORITHM WHICH AVERAGES HEIGHT VALUES FOR SMOOTHER TERRAINS TO EVEN OUT SPIKES IN LARGE VALUES
void average() {
	for (int z = 0; z < terrainArea + 1; z++) {
		for (int x = 0; x < terrainArea + 1; x++) {
			HeightMap[x + 1][z + 1] = (HeightMap[x][z] + HeightMap[x + 1][z]
				+ HeightMap[x + 2][z] + HeightMap[x][z + 1]
				+ HeightMap[x + 2][z + 1] + HeightMap[x][z + 2]
				+ HeightMap[x + 1][z + 2] + HeightMap[x + 2][z + 2]) / 8;
		}
	}
	for (int z = 0; z < terrainArea + 1; z++) {
		HeightMap[0][z] = HeightMap[1][z + 1];
	}
	for (int x = 0; x < terrainArea + 1; x++) {
		HeightMap[x][0] = HeightMap[x + 1][1];
	}
}

//THIS FUNCTION IMPLEMENTS THE FAULT ALGORITHM;
void CreateTerrain() {
	for (int z = 0; z < terrainArea + 1; z++) {
		for (int x = 0; x < terrainArea + 1; x++) {
			float k = rand() % 100 + 1;
			float a = sin(k);
			float b = cos(k);
			float distance = sqrt(a * a + b * b);
			float c = (rand() % 2) * distance - (distance / 2);
			if ((a * x) + (b * z) - c > 0) {
				HeightMap[x][z] += (rand() % 75 + 1) * 0.8;
			}
			else {
				HeightMap[x][z] -= (rand() % 48 + 1) * 0.8;
			}
		}
	}
}

//THIS FUNCTION IMPLEMENTS THE CIRCLE ALGORITHM; 
void CircleAlgorithm() {
	float radius = 0.79;
	float pd;
	float circleSize = 3.14 * radius * radius;
	for (int z = 0; z < terrainArea + 1; z++) {
		for (int x = 0; x < terrainArea + 1; x++) {
			pd = (radius * 2) / circleSize;
			if (fabs(pd) <= 1.0) {
				HeightMap[x][z] += ((rand() % 35 + 1))
					+ cos(pd * 3.14) * (((rand() % 35 + 1)));
			}
			else {
				HeightMap[x][z] -= ((rand() % 35 + 1) * 1)
					+ cos(pd * 3.14) * (((rand() % 35 + 1)));
			}
		}
	}
}

//THIS FUNCTION DRAWS THE BASE OF THE TERRAIN TO WHICH ALL OTHER ALGORITHMS AND FUNCTIONS ARE APPLIED
void DrawBaseTerrain() {

	//set values for light0
	float amb0[4] = { 0.01, 0.2, 0.01, 1.0 };
	float diff0[4] = { 0.184314, 0.509804, 0.184314, 1.0 };
	float spec0[4] = { 0.01, 0.01, 0.01, 1.0 };

	// set material colour properties
	float m_amb[4] = { 0.01, 0.2, 0.01, 1.0 };
	float m_diff[4] = { 0.184314, 0.509804, 0.184314, 1.0 };	//Lime Green
	float m_spec[4] = { 0.01, 0.01, 0.01, 1.0 };

	//assign material colour properties
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

	//assign lighting colour properties, light 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);

	//set values for light0
	float amb1[4] = { 0.01, 0.2, 0.01, 1.0 };
	float diff1[4] = { 0.576471, 0.858824, 0.439216, 1.0 };
	float spec1[4] = { 0.01, 0.01, 0.01, 1.0 };

	//assign lighting colour properties, light1
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diff1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec1);

	int x;
	int z;
	for (z = 0; z < terrainArea; z++) {
		for (x = 0; x < terrainArea; x++) {
			//draw the terrain with filled quads
			if (w == 0) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBegin(GL_QUADS);

				//SET NORMALS FOR EACH VERTEX USING THE VERTEX NORMAL ALGORITHM
				//ASSIGN COLOURS FOR EACH VERTEX ACCORDING TO HEIGHTMAP VALUES

				//VERTEX 1
				glNormal3f(VertexNormalStorage[x + 1][z][0],
					VertexNormalStorage[x + 1][z][1],
					VertexNormalStorage[x + 1][z][2]);
				glColor3f((HeightMap[x + 1][z] / 10) + 0,
					(HeightMap[x + 1][z] / 10) + 0,
					(HeightMap[x + 1][z] / 10) + 0);
				glVertex3f(x + 1, HeightMap[x + 1][z], z);

				//VERTEX 2
				glNormal3f(VertexNormalStorage[x][z][0],
					VertexNormalStorage[x][z][1],
					VertexNormalStorage[x][z][2]);
				glColor3f((HeightMap[x][z] / 10) + 0,
					(HeightMap[x][z] / 10) + 0, (HeightMap[x][z] / 10) + 0);
				glVertex3f(x, HeightMap[x][z], z);

				//VERTEX 3
				glNormal3f(VertexNormalStorage[x][z + 1][0],
					VertexNormalStorage[x][z + 1][1],
					VertexNormalStorage[x][z + 1][2]);
				glColor3f((HeightMap[x][z + 1] / 10) + 0,
					(HeightMap[x][z + 1] / 10) + 0,
					(HeightMap[x][z + 1] / 10) + 0);
				glVertex3f(x, HeightMap[x][z + 1], z + 1);

				//VERTEX 4
				glNormal3f(VertexNormalStorage[x + 1][z + 1][0],
					VertexNormalStorage[x + 1][z + 1][1],
					VertexNormalStorage[x + 1][z + 1][2]);
				glColor3f((HeightMap[x + 1][z + 1] / 10) + 0,
					(HeightMap[x + 1][z + 1] / 10) + 0,
					(HeightMap[x + 1][z + 1] / 10) + 0);
				glVertex3f(x + 1, HeightMap[x + 1][z + 1], z + 1);
				glEnd();
			}

			//draw the terrain in wireframe mode
			if (w == 1) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBegin(GL_QUADS);

				//SET NORMALS FOR EACH VERTEX USING THE VERTEX NORMAL ALGORITHM
				//ASSIGN COLOURS FOR EACH VERTEX ACCORDING TO HEIGHTMAP VALUES

				//VERTEX 1
				glNormal3f(VertexNormalStorage[x + 1][z][0],
					VertexNormalStorage[x + 1][z][1],
					VertexNormalStorage[x + 1][z][2]);
				glColor3f(HeightMap[x + 1][z] / 10, HeightMap[x + 1][z] / 10,
					HeightMap[x + 1][z] / 10);
				glVertex3f(x + 1, HeightMap[x + 1][z], z);

				//VERTEX 2
				glNormal3f(VertexNormalStorage[x][z][0],
					VertexNormalStorage[x][z][1],
					VertexNormalStorage[x][z][2]);
				glColor3f(HeightMap[x][z] / 10, HeightMap[x][z] / 10,
					HeightMap[x][z] / 10);
				glVertex3f(x, HeightMap[x][z], z);

				//VERTEX 3
				glNormal3f(VertexNormalStorage[x][z + 1][0],
					VertexNormalStorage[x][z + 1][1],
					VertexNormalStorage[x][z + 1][2]);
				glColor3f(HeightMap[x][z + 1] / 10, HeightMap[x][z + 1] / 10,
					HeightMap[x][z + 1] / 10);
				glVertex3f(x, HeightMap[x][z + 1], z + 1);

				//VERTEX 4
				glNormal3f(VertexNormalStorage[x + 1][z + 1][0],
					VertexNormalStorage[x + 1][z + 1][1],
					VertexNormalStorage[x + 1][z + 1][2]);
				glColor3f(HeightMap[x + 1][z + 1] / 10,
					HeightMap[x + 1][z + 1] / 10,
					HeightMap[x + 1][z + 1] / 10);
				glVertex3f(x + 1, HeightMap[x + 1][z + 1], z + 1);
				glEnd();
			}

			//draw the terrain with filled quads, overlaid with wireframe mode
			if (w == 2) {

				//wireframe
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBegin(GL_QUADS);

				//SET NORMALS FOR EACH VERTEX USING THE VERTEX NORMAL ALGORITHM
				//ASSIGN COLOURS FOR EACH VERTEX ACCORDING TO HEIGHTMAP VALUES

				//VERTEX 1
				glNormal3f(VertexNormalStorage[x + 1][z][0],
					VertexNormalStorage[x + 1][z][1],
					VertexNormalStorage[x + 1][z][2]);
				glColor3f(HeightMap[x + 1][z] / 10, HeightMap[x + 1][z] / 10,
					HeightMap[x + 1][z] / 10);
				glVertex3f(x + 1, HeightMap[x + 1][z], z);

				//VERTEX 2
				glNormal3f(VertexNormalStorage[x][z][0],
					VertexNormalStorage[x][z][1],
					VertexNormalStorage[x][z][2]);
				glColor3f(HeightMap[x][z] / 10, HeightMap[x][z] / 10,
					HeightMap[x][z] / 10);
				glVertex3f(x, HeightMap[x][z], z);

				//VERTEX 3
				glNormal3f(VertexNormalStorage[x][z + 1][0],
					VertexNormalStorage[x][z + 1][1],
					VertexNormalStorage[x][z + 1][2]);
				glColor3f(HeightMap[x][z + 1] / 10, HeightMap[x][z + 1] / 10,
					HeightMap[x][z + 1] / 10);
				glVertex3f(x, HeightMap[x][z + 1], z + 1);

				//VERTEX 4
				glNormal3f(VertexNormalStorage[x + 1][z + 1][0],
					VertexNormalStorage[x + 1][z + 1][1],
					VertexNormalStorage[x + 1][z + 1][2]);
				glColor3f(HeightMap[x + 1][z + 1] / 10,
					HeightMap[x + 1][z + 1] / 10,
					HeightMap[x + 1][z + 1] / 10);
				glVertex3f(x + 1, HeightMap[x + 1][z + 1], z + 1);
				glEnd();

				//filled quads
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBegin(GL_QUADS);

				//SET NORMALS FOR EACH VERTEX USING THE VERTEX NORMAL ALGORITHM
				//ASSIGN COLOURS FOR EACH VERTEX ACCORDING TO HEIGHTMAP VALUES

				//VERTEX 1
				glNormal3f(VertexNormalStorage[x + 1][z][0],
					VertexNormalStorage[x + 1][z][1],
					VertexNormalStorage[x + 1][z][2]);
				glColor3f((HeightMap[x + 1][z] / 10) + 0.5,
					(HeightMap[x + 1][z] / 10) + 0.5,
					(HeightMap[x + 1][z] / 10) + 0.5);
				glVertex3f(x + 1, HeightMap[x + 1][z], z);

				//VERTEX 2
				glNormal3f(VertexNormalStorage[x][z][0],
					VertexNormalStorage[x][z][1],
					VertexNormalStorage[x][z][2]);
				glColor3f((HeightMap[x][z] / 10) + 0.5,
					(HeightMap[x][z] / 10) + 0.5,
					(HeightMap[x][z] / 10) + 0.5);
				glVertex3f(x, HeightMap[x][z], z);

				//VERTEX 3
				glNormal3f(VertexNormalStorage[x][z + 1][0],
					VertexNormalStorage[x][z + 1][1],
					VertexNormalStorage[x][z + 1][2]);
				glColor3f((HeightMap[x][z + 1] / 10) + 0.5,
					(HeightMap[x][z + 1] / 10) + 0.5,
					(HeightMap[x][z + 1] / 10) + 0.5);
				glVertex3f(x, HeightMap[x][z + 1], z + 1);

				//VERTEX 4
				glNormal3f(VertexNormalStorage[x + 1][z + 1][0],
					VertexNormalStorage[x + 1][z + 1][1],
					VertexNormalStorage[x + 1][z + 1][2]);
				glColor3f((HeightMap[x + 1][z + 1] / 10) + 0.5,
					(HeightMap[x + 1][z + 1] / 10) + 0.5,
					(HeightMap[x + 1][z + 1] / 10) + 0.5);
				glVertex3f(x + 1, HeightMap[x + 1][z + 1], z + 1);
				glEnd();

			}
		}
	}
	glutPostRedisplay();
}

//THIS FUNCTION CALCULATES FACE NORMALS 
void CalculateFaceNormal() {
	for (int z = 0; z < terrainArea; z++) {
		for (int x = 0; x < terrainArea; x++) {
			float p1[3] = { (float)x + 1, HeightMap[x + 1][z], (float)z };//1st point
			float p2[3] = { (float)x, HeightMap[x][z], (float)z };//2nd point
			float p3[3] = { (float)x, HeightMap[x][z + 1], (float)z + 1 };//3rd point

			float vec1[3] =
			{ (p1[0] - p2[0]), (p1[1] - p2[1]), (p1[2] - p2[2]) };//1st vector from p1 and p2
			float vec2[3] =
			{ (p3[0] - p2[0]), (p3[1] - p2[1]), (p3[2] - p2[2]) };//2nd vector from p2 and p3

			float normal[3] = { (vec1[1] * vec2[2]) - (vec1[2] * vec2[1]),
				(vec1[2] * vec2[0]) - (vec1[0] * vec2[2]), (vec1[0]
					* vec2[1]) - (vec1[1] * vec2[0]) };	//normal from vec1 and vec2 CROSS PRODUCT

														//length used to make veectors unit length
			float len = sqrt(
				(normal[0] * normal[0]) + (normal[1] * normal[1])
				+ (normal[2] * normal[2]));

			//store face normals
			FaceNormalStorage[x][z][0] = normal[0] / len;
			FaceNormalStorage[x][z][1] = normal[1] / len;
			FaceNormalStorage[x][z][2] = normal[2] / len;
		}
	}
}

//THIS FUNCTION CALCULATES FACE NORMALS
void CalculateVertexNormal() {
	for (int z = 0; z < terrainArea; z++) {
		for (int x = 0; x < terrainArea; x++) {
			//define points
			float p1[3] = { (float)x, HeightMap[x][z - 1], (float)z - 1 };//1st point
			float p2[3] = { (float)x - 1, HeightMap[x - 1][z], (float)z };//2nd point
			float p3[3] = { (float)x + 1, HeightMap[x + 1][z], (float)z };//3rd point
			float p4[3] = { (float)x, HeightMap[x][z + 1], (float)z + 1 };//4th point
			float p5[3] = { (float)x, HeightMap[x][z], (float)z };//centre point
																  //calculate vectors from centre point
			float vec1[3] =
			{ (p1[0] - p5[0]), (p1[1] - p5[1]), (p1[2] - p5[2]) };//1st vector from p1 and p5
			float vec2[3] =
			{ (p2[0] - p5[0]), (p2[1] - p5[1]), (p2[2] - p5[2]) };//2nd vector from p2 and p5
			float vec3[3] =
			{ (p3[0] - p5[0]), (p3[1] - p5[1]), (p3[2] - p5[2]) };//3rd vector from p3 and p5
			float vec4[3] =
			{ (p4[0] - p5[0]), (p4[1] - p5[1]), (p4[2] - p5[2]) };//4th vector from p4 and p5
																  //calculate first normals
			float normal12[3] = { (vec1[1] * vec2[2]) - (vec1[2] * vec2[1]),
				(vec1[2] * vec2[0]) - (vec1[0] * vec2[2]), (vec1[0]
					* vec2[1]) - (vec1[1] * vec2[0]) };	//normal from vec1 and vec2 CROSS PRODUCT
			float normal24[3] = { (vec2[1] * vec4[2]) - (vec2[2] * vec4[1]),
				(vec2[2] * vec4[0]) - (vec2[0] * vec4[2]), (vec2[0]
					* vec4[1]) - (vec2[1] * vec4[0]) };	//normal from vec2 and vec4 CROSS PRODUCT
			float normal43[3] = { (vec4[1] * vec3[2]) - (vec4[2] * vec3[1]),
				(vec4[2] * vec3[0]) - (vec4[0] * vec3[2]), (vec4[0]
					* vec3[1]) - (vec4[1] * vec3[0]) };	//normal from vec4 and vec3 CROSS PRODUCT
			float normal31[3] = { (vec3[1] * vec1[2]) - (vec3[2] * vec1[1]),
				(vec3[2] * vec1[0]) - (vec3[0] * vec1[2]), (vec3[0]
					* vec1[1]) - (vec3[1] * vec1[0]) };	//normal from vec4 and vec1 CROSS PRODUCT
														//length used to make vectors unit length
			float len1 = sqrt(
				(normal12[0] * normal12[0]) + (normal12[1] * normal12[1])
				+ (normal12[2] * normal12[2]));
			float len2 = sqrt(
				(normal24[0] * normal24[0]) + (normal24[1] * normal24[1])
				+ (normal24[2] * normal24[2]));
			float len3 = sqrt(
				(normal43[0] * normal43[0]) + (normal43[1] * normal43[1])
				+ (normal43[2] * normal43[2]));
			float len4 = sqrt(
				(normal31[0] * normal31[0]) + (normal31[1] * normal31[1])
				+ (normal31[2] * normal31[2]));

			//normalize first normals
			float NormalizedNormal12[3];
			NormalizedNormal12[0] = normal12[0] / len1;
			NormalizedNormal12[1] = normal12[1] / len1;
			NormalizedNormal12[2] = normal12[2] / len1;

			float NormalizedNormal24[3];
			NormalizedNormal24[0] = normal24[0] / len2;
			NormalizedNormal24[1] = normal24[1] / len2;
			NormalizedNormal24[2] = normal24[2] / len2;

			float NormalizedNormal43[3];
			NormalizedNormal43[0] = normal43[0] / len3;
			NormalizedNormal43[1] = normal43[1] / len3;
			NormalizedNormal43[2] = normal43[2] / len3;

			float NormalizedNormal31[3];
			NormalizedNormal31[0] = normal31[0] / len4;
			NormalizedNormal31[1] = normal31[1] / len4;
			NormalizedNormal31[2] = normal31[2] / len4;
			//normalized vector!!

			float sum[3] = { (NormalizedNormal12[0] + NormalizedNormal24[0]
				+ NormalizedNormal43[0] + NormalizedNormal31[0]),
				(NormalizedNormal12[1] + NormalizedNormal24[1]
					+ NormalizedNormal43[1] + NormalizedNormal31[1]),
					(NormalizedNormal12[2] + NormalizedNormal24[2]
						+ NormalizedNormal43[2] + NormalizedNormal31[2]) };
			float length = sqrt(
				(sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

			//store vertex normals
			VertexNormalStorage[x][z][0] = sum[0] / length;
			VertexNormalStorage[x][z][1] = sum[1] / length;
			VertexNormalStorage[x][z][2] = sum[2] / length;
		}
	}
}

//THIS FUNCTION DRAWS THE SNOWMAN
void DrawSnowman(float* pos, float* rot) {
	glPushMatrix();
	glTranslatef(pos[0], (HeightMap[(int)(pos[0])][(int)(pos[2])]) + 1,
		pos[2]);
	glRotatef(rot[1], 0, 1, 0);

	//draw body
	glColor3f(1, 1, 1);
	glutSolidSphere(1, 16, 16);

	//draw buttons
	glPushMatrix();
	glTranslatef(0, 0.35, 0.9);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.15, 0.95);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.05, 0.95);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	//translate relative to body, and draw head
	glTranslatef(0, 1.25, 0);
	glRotatef(headRot[1], 0, 1, 0); //turn the head relative to the body
	glColor3f(1, 1, 1);
	glutSolidSphere(0.5, 16, 16);

	//translate and draw right eye
	glPushMatrix();
	glTranslatef(0.2, 0.15, 0.45);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	//translate and draw left eye
	glPushMatrix();
	glTranslatef(-0.2, 0.15, 0.45);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	//translate and draw nose
	glPushMatrix();
	glTranslatef(0, 0, 0.5);
	glColor3f(1, 0.4, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPopMatrix(); //body
	glPopMatrix(); //snowman
}

//DISPLAY FUNCTION FOR THE MAIN WINDOW WHICH DISPLAYS THE TERRAIN AND SNOWMAN CHARACTER
void display() {
	glutSetWindow(window_1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color and depth buffer, the color buffer enabled for color writing
	glMatrixMode(GL_MODELVIEW); //what kind of matrix is the current matrix, which maps from object's local coordinate to the world space coordinate
	glLoadIdentity(); //Loads the identity matrix to which all transformations are applied
	gluLookAt(CamPos[0], CamPos[1], CamPos[2], 0, 0, 0, 0, 1, 0); //This defines the viewing transformation of the camera
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	DrawBaseTerrain();
	DrawSnowman(pos, rot);
	glutSwapBuffers(); //Swap buffers of the current window
}

//CREATES THE 2D OVERVIEW OF THE TERRAIN DISPLAYS HIGHER VALUES CLOSER TO WHITE AND LOWER HEIGHT VALUES CLOSER TO BLACK COLOUR VALUES BASED ON HEIGHTMAP
void twoDrep() {
	for (int z = 0; z < terrainArea; z++) {
		for (int x = 0; x < terrainArea; x++) {
			glBegin(GL_POINTS);
			glColor3f((HeightMap[x][z] / 10), (HeightMap[x][z] / 10),
				(HeightMap[x][z] / 10));
			glVertex2f(x * 0.004, z * 0.004);
			glEnd();
		}
	}

}

//DISPLAY FUNCTION FOR THE SECOND WINDOW 
void overviewdisplay() {
	glutSetWindow(window_2);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	twoDrep();
	glutSwapBuffers();
}

//The following the the reshape function which will be used in Main to do a callback for the current window
void reshape(int w, int h) {
	glMatrixMode(GL_PROJECTION); //this defines the projection matrix as the current matrix
	glLoadIdentity(); //Loads the identity matrix
	if (temp == 0) {
		gluPerspective(65,
			glutGet(GLUT_SCREEN_WIDTH) / glutGet(GLUT_SCREEN_HEIGHT), 1,
			1000);
	} //sets up a perspective projection matrix, where the parameters are for
	  //field of view, aspect ratio, near clipping plane, far clipping plane
	else if (temp == 1) {
		gluPerspective(45,
			glutGet(GLUT_SCREEN_WIDTH) / glutGet(GLUT_SCREEN_HEIGHT), 1,
			1000);
	}
	glMatrixMode(GL_MODELVIEW);	//this defines the modelview matrix as the current matrix
}

//The following is a special key function for the arrow keys, which are used to move the camera around the scene
void special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		CamPos[0] += 0.8;
		break;

	case GLUT_KEY_RIGHT:s
		CamPos[0] -= 0.8;
		break;

	case GLUT_KEY_UP:z
		CamPos[2] += 0.8;
		break;

	case GLUT_KEY_DOWN:
		CamPos[2] -= 0.8;
		break;

	case GLUT_KEY_HOME:
		CamPos[1] -= 0.8;
		break;

	case GLUT_KEY_END:
		CamPos[1] += 0.8;
		break;

	}
	glutPostRedisplay();
}



void mouse2(int button, int state, int x, int z) {
	int r = rand() % (int)round(terrainArea / 10) + 5; //random radius (between 5 and a tenth of the map size)
	int disp = rand() % 5; //random number between 0 and 5
	x -= round(terrainArea);
	z -= round(terrainArea);

	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {  //if left button clicked
		CircleAlgorithm(); //add hill to that point
		CalculateFaceNormal();
		glutSetWindow(window_1);
		glutPostRedisplay();
	}
	if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) { //if right button clicked
		CircleAlgorithm(); //decrease hill at that point
		CalculateFaceNormal();
		glutSetWindow(window_1);
		glutPostRedisplay();
	}
}
void kbd(unsigned char key, int x, int y) {	//The keyboard defines the functionality of the keyboard keys, if pressed
	switch (key) {
		//QUIT THE PROGRAM, FROM MAIN WINDOW
	case 'q':
	case 'Q':
	case 27:
		exit(0);
		break;

		//CHANGE FROM FAULT ALGORITHM TO CIRCLE ALGORITHM AND BACK AGAIN
	case '1':
		if (temp == 0) {
			temp = 1;
			//SETS THE POSITION OF THE FIRST LIGHT WHICH LIGHTS THE TERRAIN WHEN IT IS RENDERED USING THE CIRCLE ALGORITHM
			light_posCircle[0] = -5;
			light_posCircle[1] = -65;
			light_posCircle[2] = -5;
			light_posCircle[3] = 0;

			//SETS THE POSITION OF THE SECOND LIGHT WHICH LIGHTS THE TERRAIN WHEN IT IS RENDERED USING THE CIRCLE ALGORITHM
			secondlight_posCircle[0] = 5;
			secondlight_posCircle[1] = -65;
			secondlight_posCircle[2] = 5;
			secondlight_posCircle[3] = 0;

			//SETS THE POSITION OF THE CAMERA WHICH LIGHTS THE TERRAIN WHEN IT IS RENDERED USING THE CIRCLE ALGORITHM
			CamPos[0] = -95;
			CamPos[1] = 85;
			CamPos[2] = -95;

			//CALLS THE FUNCTION TO USE THE CIRCLE ALGORITHM TO CREATE THE TERRAIN
			CircleAlgorithm();
			//CALL THE AVERAGE FUNCTION A NUMBER OF TIMES TO SMOOTH OUT THE TERRAIN
			average();
			average();
			average();
			average();
			average();
			//CALCULATE NORMALS
			CalculateFaceNormal();
			CalculateVertexNormal();
			reshape(1, 1);
			//CALL THE 2D OVERVIEW DISPLAY
			overviewdisplay();
		}
		else if (temp == 1) {
			temp = 0;
			for (int z = 0; z < terrainArea + 1; z++) {
				for (int x = 0; x < terrainArea + 1; x++) {
					HeightMap[x][z] = 0;
				}
			}
			//CREATE THE TERRAIN USING THE FAULT ALGORITM
			CreateTerrain();
			//CALL THE AVERAGE FUNCTION A NUMBER OF TIMES TO SMOOTH OUT THE TERRAIN
			average();
			average();
			average();
			average();
			//CALCULATE NORMALS
			CalculateFaceNormal();
			CalculateVertexNormal();
			//CALL THE 2D OVERVIEW DISPLAY
			reshape(1, 1);
			overviewdisplay();
		}
		break;

		//ENABLE FLAT SHADING
	case 'f':
	case 'F':
		glShadeModel(GL_FLAT);
		glutPostRedisplay();
		break;

		//ENABLE GOURAURD SHADING
	case 'g':
	case 'G':
		glShadeModel(GL_SMOOTH);
		glutPostRedisplay();
		break;

		//FIRST PRESS DISPLAYS WIREFRAME MODE; SECOND PRESS DISPLAYS WIREFRAME AND QUADS; THIRD PRESS GOES BACK TO DISPLAYING ONLY QUADS
	case 'w':
	case 'W':
		if (w == 0) {
			w = 1;
		}
		else if (w == 1) {
			w = 2;
		}
		else {
			w = 0;
		}
		break;

		//RESETS THE HEIGHMAP AND RECREATES ANOTHER RANDOME TERRAIN
	case 'r':
	case 'R':
		if (temp1 == 0) {
			temp1 = 1;
			if (temp == 0) {
				for (int z = 0; z < terrainArea + 1; z++) {
					for (int x = 0; x < terrainArea + 1; x++) {
						HeightMap[x][z] = 0;
					}
				}
				//CREATE THE TERRAIN USING THE FAULT ALGORITM
				CreateTerrain();
				//CALL THE AVERAGE FUNCTION A NUMBER OF TIMES TO SMOOTH OUT THE TERRAIN
				average();
				average();
				average();
				average();
				//CALCULATE NORMALS
				CalculateFaceNormal();
				CalculateVertexNormal();
				//CALL THE 2D OVERVIEW DISPLAY
				overviewdisplay();

			}
			else if (temp == 1) {
				for (int z = 0; z < terrainArea + 1; z++) {
					for (int x = 0; x < terrainArea + 1; x++) {
						HeightMap[x][z] = 0;
					}
				}
				//CALLS THE FUNCTION TO USE THE CIRCLE ALGORITHM TO CREATE THE TERRAIN
				CircleAlgorithm();
				//CALL THE AVERAGE FUNCTION A NUMBER OF TIMES TO SMOOTH OUT THE TERRAIN
				average();
				average();
				average();
				average();
				average();
				//CALCULATE NORMALS
				CalculateFaceNormal();
				CalculateVertexNormal();
				//CALL THE 2D OVERVIEW DISPLAY
				overviewdisplay();
			}
		}
		else if (temp1 == 1) {
			temp1 = 0;
			if (temp == 0) {
				for (int z = 0; z < terrainArea + 1; z++) {
					for (int x = 0; x < terrainArea + 1; x++) {
						HeightMap[x][z] = 0;
					}
				}
				//CREATE THE TERRAIN USING THE FAULT ALGORITM
				CreateTerrain();
				//CALL THE AVERAGE FUNCTION A NUMBER OF TIMES TO SMOOTH OUT THE TERRAIN
				average();
				average();
				average();
				average();
				//CALCULATE NORMALS
				CalculateFaceNormal();
				CalculateVertexNormal();
				//CALL THE 2D OVERVIEW DISPLAY
				overviewdisplay();

			}
			else if (temp == 1) {
				for (int z = 0; z < terrainArea + 1; z++) {
					for (int x = 0; x < terrainArea + 1; x++) {
						HeightMap[x][z] = 0;
					}
				}
				//CALLS THE FUNCTION TO USE THE CIRCLE ALGORITHM TO CREATE THE TERRAIN
				CircleAlgorithm();
				//CALL THE AVERAGE FUNCTION A NUMBER OF TIMES TO SMOOTH OUT THE TERRAIN
				average();
				average();
				average();
				average();
				average();
				//CALCULATE NORMALS
				CalculateFaceNormal();
				CalculateVertexNormal();
				//CALL THE 2D OVERVIEW DISPLAY
				overviewdisplay();
			}
		}
		break;

		//MOVE SNOWMAN FORWARD
	case 'u':
	case 'U':
		if (abs(pos[2]) < terrainArea) {
			pos[2]++;
			rot[1] = 0;
		}
		break;

		//MOVE SNOWMAN BACKWARD
	case 'j':
	case 'J':
		if (abs(pos[2]) > 0) {
			pos[2]--;
			rot[1] = 180;
		}
		break;

		//MOVE SNOWMAN LEFT
	case 'h':
	case 'H':
		if (abs(pos[0]) < terrainArea) {
			pos[0]++;
			rot[1] = 90;
		}

		break;

		//MOVE SNOWMAN RIGHT
	case 'k':
	case 'K':
		if (abs(pos[0]) > 0) {
			pos[0]--;
			rot[1] = -90;
		}
		break;

		//SWITCH LIGHTING ON AND OFF
	case 'l':
	case 'L':
		l = !l;
		if (l) {
			glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
			glLightfv(GL_LIGHT1, GL_POSITION, secondlight_pos);
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHT1);
			glEnable(GL_COLOR_MATERIAL);
		}
		else {
			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
			glDisable(GL_LIGHT1);
			glDisable(GL_COLOR_MATERIAL);
		}
		break;

		//MOVE LIGHT LEFT
	case 'z':
	case 'Z':
		if (light_pos[0] < 5) {
			light_pos[0] += 2;
			if (temp == 0) {
				light_posCircle[0] += 2;
			}
		}
		if (secondlight_pos[0] > -5) {
			secondlight_pos[0] -= 2;
			if (temp == 0) {
				secondlight_posCircle[0] -= 2;
			}
		}
		break;

		//MOVE LIGHT FORWARD
	case 'x':
	case 'X':
		if (light_pos[2] > -5) {
			light_pos[2] -= 2;
			if (temp == 0) {
				light_posCircle[2] -= 2;
			}
		}
		if (secondlight_pos[2] < 5) {
			secondlight_pos[2] += 2;
			if (temp == 0) {
				secondlight_posCircle[2] += 2;
			}
		}
		break;

		//MOVE LIGHT RIGHT
	case 'c':
	case 'C':

		if (light_pos[0] > -5) {
			light_pos[0] -= 2;
			if (temp == 0) {
				light_posCircle[0] -= 2;
			}
		}
		if (secondlight_pos[0] < 5) {
			secondlight_pos[0] += 2;
			if (temp == 0) {
				secondlight_posCircle[0] += 2;
			}
		}
		break;

		//MOVE LIGHT FORWARD
	case 'v':
	case 'V':

		if (light_pos[2] < 5) {
			light_pos[2] += 2;
			if (temp == 0) {
				light_posCircle[2] += 2;
			}
		}
		if (secondlight_pos[2] > -5) {
			secondlight_pos[2] -= 2;
			if (temp == 0) {
				secondlight_posCircle[2] -= 2;
			}

		}
		break;
	}
}

void kbd_2(unsigned char key, int x, int y) {
	switch (key) {
		//RESETS THE VALUES IN HEIGHTMAP AND RECALLS DISPLAY FOR THE SECOND WINDOW
	case 'r':
	case 'R':
		for (int z = 0; z < terrainArea + 1; z++) {
			for (int x = 0; x < terrainArea + 1; x++) {
				HeightMap[x][z] = 0;
			}
		}
		glutDisplayFunc(overviewdisplay);
		glutPostRedisplay();
		break;
	}
}

//MAIN FUNCTION
int main(int argc, char** argv) {
	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	//INSTRUCTIONS
	printf("----------------------------------------------------\n");
	printf("----------------------Welcome-----------------------\n");
	printf("This is a Terrain generator, generating a terrain. \n");
	printf("The terrain size is determined by user input\n");
	printf("The range of the terrain must be greater than 50\n");
	printf("and less than 300\n");
	printf("The terrain by default runs in solid polygon Mode\n");
	printf("The terrain by default has Gouraud Shading activated\n");
	printf("The terrain supports the following algorithms:\n");
	printf("Fault Algorithm(by Default) && Circle Algorithm\n");
	

	printf("----------------------------------------------------\n");
	printf("----------------------------------------------------\n");
	printf("--------------------Instructions--------------------\n");
	printf("----------------------------------------------------\n");
	printf("----------------------------------------------------\n");
	printf("Left/right arrows - rotate scene about y-axis\n");
	printf("up/down arrows - rotate scene about x-axis \n");
	printf("R/r - Regenerates a new terrain \n");
	printf("f/F - Activates Flat Shading\n");
	printf("g/G - Activates Gouraud Shading\n");
	printf("q/Q/Esc - Quits the program.\n");
	printf("u/U - Moves the snowman forward with respect to terrain\n");
	printf("h/H - Moves the snowman left with respect to terrain\n");
	printf("j/J - Moves the snowman backward with respect to terrain\n");
	printf("k/K - Moves the snowman right with respect to terrain\n");
	printf("w/W - First press activates wireframe mode\n");
	printf("w/W - Second press activates wireframe and solid polygon mode\n");
	printf("w/W - Third press activates solid polygon mode\n");
	printf("l/L - Lighting is turned on\n");
	printf("z/Z - Moves the light sources left\n");
	printf("c/C - Moves the light sources right\n");
	printf("x/X - Moves the light sources backward\n");
	printf("v/V - Moves the light sources forward\n");
	printf("Number 1 key - Switch between fault algorithm and circle algorithm\n");

	cout << "Please enter the size of the terrain you wish to see: ";
	cin >> terrainGuard;
	while (!(terrainGuard >= 50 && terrainGuard <= 300)) {
		printf("INVALID INPUT!!\n");
		cout << "Please enter a number between 50 and 300: ";
		cin >> terrainGuard;
	}
	terrainArea = terrainGuard;

	//*******MAIN WINDOW************************
	glutInitWindowSize(800, 800);		//This creates the window
	glutInitWindowPosition(100, 100);//This places the window at the position given
	window_1 = glutCreateWindow("Terrain");		//This names the window

	glutDisplayFunc(display);		//This a callback to the display function
	glutKeyboardFunc(kbd);		//This is a callback to the keyboard function
	glutSpecialFunc(special);	//This a callback to the speacial key function
	glEnable(GL_DEPTH_TEST);		//This enables depth test for 3D scene
	glEnable(GL_CULL_FACE);	//This enables Backface culling, since backface is by default
	glutReshapeFunc(reshape);	//This is a callback to the reshape function
	CreateTerrain();
	average();
	average();
	average();
	average();
	average();
	CalculateFaceNormal();
	CalculateVertexNormal();

	//********SECOND WINDOW*********
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(1000, 100);
	//glutMouseFunc(mouse2);

	window_2 = glutCreateWindow("2D Overview");
	glutDisplayFunc(overviewdisplay);
	glutKeyboardFunc(kbd_2);
	glutSpecialFunc(special);

	glutMainLoop();
	return 0;
}
