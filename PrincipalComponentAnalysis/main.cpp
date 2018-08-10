//
//  main.cpp
//  PrincipalComponentAnalysis
//
//  Created by 筒井 大二 on 2018/06/30.
//  Copyright © 2018年 筒井 大二. All rights reserved.
//

#include <iostream>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <random>
//#include "linear.h"
#include "catalog.h"

using namespace std;

void drawCircle(double x0, double y0, double r, int div=50);
void drawTriangle(double x0, double y0, double r);
double hinge(double u);
double zero_one(double u);

random_device rnd;     // 非決定的な乱数生成器
mt19937 mt(rnd());  // メルセンヌ・ツイスタの32ビット版、引数は初期シード
uniform_real_distribution<> unif(0.0, 1.0);   // [0.0,1.0]上一様に分布させる
normal_distribution<> gauss(0.0, 1.0);   // 平均0.0、標準偏差1.0で分布させる

double winsize = 1000.0;
int datanum = 5000;
int cnt = 0;
mat X(datanum, 3);		//デザイン行列
double m0[3] = {-0.2, -0.3, 0.1};
double m1[2] = {0.1, 0.2};
vect** m = (vect**)calloc(2, sizeof(vect*));
double var0[9] = {0.03, -0.01, 0.0,
				-0.01, 0.08, -0.0,
				0.0, -0.0, 0.03};
double var1[4] = {0.05, -0.01,
				-0.01, 0.02};
mat** s = (mat**)calloc(2, sizeof(mat*));
mat** var = (mat**)calloc(2, sizeof(mat*));
mat Y;			//Principal components
int max_id[2] = {0, 0};

/*--For OpenGL-------------------------------------------------------------------------*/
void idle(void){
	glutPostRedisplay();
}
void setup(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);       //White
}
void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,
				   (double)width/height,
				   0.1,
				   100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 2.5,       //Position of Camera
			  0.0, 0.0, 0.0,        //Position of Object
			  0.0, 1.0, 0.0);       //Upward direction of Camera
}


/*--Display func-------------------------------------------------------------------------*/
void display(void){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	glutWireTeapot(0.5);
	
	//	glPointSize(3.0);
	for(int i=0; i<datanum; i++){
		glColor3d(1.0, 0.0, 0.0);	//Red
		drawCircle(X.a[i * 3 + 0], X.a[i * 3 + 1], 0.004);
		glColor3d(0.0, 0.0, 0.0);	//Black
		drawCircle(Y.a[i * 3 + max_id[0]], Y.a[i * 3 + max_id[1]], 0.004);
//		drawTriangle(X.a[i * 3 + 0], X.a[i * 3 + 1], 0.01);
	}
	
	glFlush();
}


/*--Main func-------------------------------------------------------------------------*/
int main(int argc, char * argv[]) {
	/*--Initialize-------*/
	m[0] = new vect(m0, 3); m[1] = new vect(m1, 2);
	var[0] = new mat(var0,3,3); var[1] = new mat(var1,2,2);
	s[0] = new mat; s[1] = new mat;
//	var[0]->print("var0"); var[1]->print("var1");
	(*s[0]) = var[0]->sqrt(); (*s[1]) = var[1]->sqrt();
	
	X.rand(*m[0], *s[0]);
	
	/*--Calculation--------*/
	mat V; V = X.samp_var(); V.print("var(X)");
	vect l; mat U; V.eigendecomp(&l, &U);
	l.print("lambda"); U.print("U");
	
	sort_with_id(l.v, l.size, max_id, 2);
	//cout << max_id[0] << ", " << max_id[1] << endl;
	Y = X * U;
	
//		getchar();
	
	/*--Main loop-------*/
	glutInit(&argc, argv);
	glutInitWindowSize(winsize, winsize);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Principal Component Analysis");
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	setup();
	glutMainLoop();
	
	for(int i=0; i<2; i++){
		delete m[i]; delete s[i]; delete var[i];
	}
	free(m); free(s); free(var);
	return 0;
}


/*--Other func-------------------------------------------------------------------------*/
void drawCircle(double x0, double y0, double r, int div){
	double x, y;
	glBegin(GL_POLYGON);
	for (int i=0; i<div; i++) {
		x = x0 + r * cos(2.0 * M_PI * ((double)i/div));
		y = y0 + r * sin(2.0 * M_PI * ((double)i/div));
		glVertex2d(x, y);
	}
	glEnd();
}
void drawTriangle(double x0, double y0, double r){
	double x, y;
	glBegin(GL_POLYGON);
	for (int i=0; i<4; i++) {
		x = x0 + r * cos(2.0 * M_PI * (0.25 + i/3.0));
		y = y0 + r * sin(2.0 * M_PI * (0.25 + i/3.0));
		glVertex2d(x, y);
	}
	glEnd();
}


