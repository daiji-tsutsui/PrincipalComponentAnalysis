//
//  catalog.h
//  SupportVectorMachine
//
//  Created by 筒井 大二 on 2018/06/27.
//  Copyright © 2018年 筒井 大二. All rights reserved.
//
#include "linear.h"
#ifndef catalog_h
#define catalog_h

void sample_op_test(){
	//2.7.2018
	//Test for sample mean and variznce
	double mv[3] = {-0.2, -0.3, 0.1};
	vect m(mv, 3); m.print("strict mean");
	double varv[9] = {0.03, -0.015, 0.01,
					-0.015, 0.04, 0.02,
					0.01, 0.02, 0.06};
	mat var(varv, 3, 3); var.print("strict var");
	mat s; s = var.sqrt();
	
	mat X(5000, 3); X.rand(m, s);	//デザイン行列
	vect M; M = X.samp_mean(); M.print("mean(X)");
	mat V; V = X.samp_var(); V.print("var(X)");
	
//	vect l; mat U; V.eigendecomp(&l, &U);
//	l.print("lambda"); U.print("U");
}

void square_root_test(){
	//27.6.2018
	//Test for eigendecomp and sqrt
	mat C(4,4);
	
	C.rand(); C.print("C");
	C = C * C.transpose(); C.print("C C^t");
	
	vect l; mat U;
	C.eigendecomp(&l, &U);
	l.print("lambda");
	U.print("U");
	
	(U*(U.transpose()))->print("U U^t");
	(U * ((*l.diagonal()) * (*U.transpose())))->print("U D U^t");
	
	C = C.sqrt(); C.print("sqrt(C C^t)");
	C = (C * C); C.print("sqrt(C C^t)^2");
}

void destruvtive_op(){
	//27.6.2018
	//To save memory, introduce destructive operator
	
	vect A; A.rand();
	vect d;		//nothing -> memory used: 5.2MB
	for(int i=0; i<50000; i++) d = A + *(A + A);	// -> memory used: 6.7MB
	for(int i=0; i<50000; i++) d = A + (A + A);		// -> memory used: 5.2MB

	getchar();
}

#endif /* catalog_h */
