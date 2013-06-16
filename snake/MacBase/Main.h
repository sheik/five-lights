//
//  Main.h
//  MacBase
//
//  Created by Jeff Aigner on 6/11/13.
//  Copyright (c) 2013 Jeff Aigner. All rights reserved.
//

#ifndef __MacBase__Main__
#define __MacBase__Main__

#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <StopWatch.h>

#include <deque>
#include <unistd.h>

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif


#endif /* defined(__MacBase__Main__) */
