//
//  Main.cpp
//  Snake!
//
//  Created by Jeff Aigner on 6/11/13.
//  Copyright (c) 2013 Jeff Aigner. All rights reserved.
//


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


const int GRID_SIZE = 24; // grid will be 32 x 32
const GLfloat CELL_WIDTH = 1.0f;

GLShaderManager		shaderManager;			// Shader Manager
GLMatrixStack		modelViewMatrix;		// Modelview Matrix
GLMatrixStack		projectionMatrix;		// Projection Matrix
GLFrustum			viewFrustum;			// View Frustum
GLGeometryTransform	transformPipeline;		// Geometry Transform Pipeline

GLTriangleBatch		torusBatch;
GLBatch				floorBatch;
GLBatch             cubeBatch;

GLFrame             cameraFrame;

// some colors
const GLfloat WHITE[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat RED[] = { 1.0f, 0.0f, 0.0f, 1.0f };
const GLfloat GREEN[] = { 0.0f, 1.0f, 0.0f, 1.0f };
const GLfloat BLUE[] = { 0.0f, 0.0f, 1.0f, 1.0f };
const GLfloat BLACK[] = { 0.0f, 0.0f, 0.0f, 1.0f };

const GLfloat GRAY[] = { 0.5f, 0.5f, 0.5f, 0.5f };
const GLfloat DARKGRAY[] = { 0.15f, 0.15f, 0.15f, 1.0f };

bool DEBUG_CONTROLS = false;

struct coords { GLfloat x, y; };

std::deque<coords> snake_coords;

enum direction_t { UP, DOWN, LEFT, RIGHT };

direction_t direction = RIGHT;


//////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering
// context.
void SetupRC()
{
	// Initialze Shader Manager
	shaderManager.InitializeStockShaders();
	
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // make cube
    gltMakeCube(cubeBatch, 0.5f);
    snake_coords.push_back({0.0,0.0});
    snake_coords.push_back({1.0,0.0});
    snake_coords.push_back({2.0,0.0});
    
    // set up initial camera
    cameraFrame.MoveForward(-((GRID_SIZE / 2.0)+(GRID_SIZE / 3.0)));
    cameraFrame.MoveUp(37.0f);
    cameraFrame.RotateWorld(m3dDegToRad(-64.0f), 1.0f, 0.0f, 0.0f);
    cameraFrame.MoveForward(7.0f);
    
    
    floorBatch.Begin(GL_LINES, 500);
    
    for(int i = 0; i <= GRID_SIZE; i++) {
        floorBatch.Vertex3f(i, -1.0f, 0.0f);
        floorBatch.Vertex3f(i, -1.0f, CELL_WIDTH * GRID_SIZE);
        
        floorBatch.Vertex3f(0.0f, -1.0f, i);
        floorBatch.Vertex3f(CELL_WIDTH * GRID_SIZE, -1.0f, i);
    }
    
    floorBatch.End();
}


///////////////////////////////////////////////////
// Screen changes size or is initialized
void ChangeSize(int nWidth, int nHeight)
{
	glViewport(0, 0, nWidth, nHeight);
	
    // Create the projection matrix, and load it on the projection matrix stack
	viewFrustum.SetPerspective(35.0f, float(nWidth)/float(nHeight), 1.0f, 100.0f);
	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    
    // Set the transformation pipeline to use the two matrix stacks
	transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

void UpdateSnakePos(int x) {
    if(direction == RIGHT)
        snake_coords.push_front({snake_coords.front().x+1.0f, snake_coords.front().y});
    if(direction == LEFT)
        snake_coords.push_front({snake_coords.front().x-1.0f, snake_coords.front().y});
    if(direction == UP)
        snake_coords.push_front({snake_coords.front().x, snake_coords.front().y-1});
    if(direction == DOWN)
        snake_coords.push_front({snake_coords.front().x, snake_coords.front().y+1});
    
    snake_coords.pop_back();
    glutTimerFunc(100, UpdateSnakePos, 0);
}


// Called to draw scene
void RenderScene(void)
{
    // Time Based animation
	static CStopWatch	rotTimer;
	//float yRot = rotTimer.GetElapsedSeconds() * 60.0f;

	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    
    // Save the current modelview matrix (the identity matrix)
	modelViewMatrix.PushMatrix();
    
    // set up camera
    M3DMatrix44f mCamera;
    cameraFrame.GetCameraMatrix(mCamera);
    modelViewMatrix.PushMatrix(mCamera);
    
    // set up light source
    M3DVector4f vLightPos = { 0.0f, 10.0f, 5.0f, 1.0f };
    M3DVector4f vLightEyePos;
    m3dTransformVector4(vLightEyePos, vLightPos, mCamera);
    
	// Draw the ground
    modelViewMatrix.PushMatrix();
    modelViewMatrix.Translate(-GRID_SIZE/2.0f, 0.0f, -GRID_SIZE/2.0f);
    
	shaderManager.UseStockShader(GLT_SHADER_FLAT,
								 transformPipeline.GetModelViewProjectionMatrix(),
								 GRAY);

	floorBatch.Draw();
    modelViewMatrix.PopMatrix();
    
    // Draw Mr. Snake
    for( struct coords i : snake_coords) {
        modelViewMatrix.PushMatrix();
        modelViewMatrix.Translate(i.x-0.5f, -0.5f, i.y-0.5f);
        shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF,
                                     transformPipeline.GetModelViewMatrix(),
                                     transformPipeline.GetProjectionMatrix(),
                                     vLightEyePos, GREEN);
        
        cubeBatch.Draw();
        modelViewMatrix.PopMatrix();
    }
    
	// Restore the previous modleview matrix (the idenity matrix)
	modelViewMatrix.PopMatrix();
    modelViewMatrix.PopMatrix();
    
    // Do the buffer Swap
    glutSwapBuffers();
    
    // Tell GLUT to do it again
    glutPostRedisplay();
}


void SpecialKeys(int key, int x, int y)
{
    float linear = 1.0f;
    float angular = float(m3dDegToRad(5.0f));
    
    if(DEBUG_CONTROLS) {
        if(key == GLUT_KEY_UP)
            cameraFrame.MoveForward(linear);
        
        if(key == GLUT_KEY_DOWN)
            cameraFrame.MoveForward(-linear);
        
        if(key == GLUT_KEY_LEFT)
            cameraFrame.RotateWorld(angular, 0.0f, 1.0f, 0.0f);
        
        if(key == GLUT_KEY_RIGHT)
            cameraFrame.RotateWorld(-angular, 0.0f, 1.0f, 0.0f);
        
    } else {
        if(key == GLUT_KEY_RIGHT && direction != LEFT)
            direction = RIGHT;
        if(key == GLUT_KEY_LEFT && direction != RIGHT)
            direction = LEFT;
        if(key == GLUT_KEY_DOWN && direction != UP)
            direction = DOWN;
        if(key == GLUT_KEY_UP && direction != DOWN)
            direction = UP;
    }
    
    if(key == 'j')
        cameraFrame.RotateWorld(-angular, 1.0f, 0.0f, 0.0f);
    
    if(key == 'k')
        cameraFrame.RotateWorld(angular, 1.0f, 0.0f, 0.0f);
    
    if(key == 'i')
        DEBUG_CONTROLS = !DEBUG_CONTROLS;
    
}

int main(int argc, char* argv[])
{
	gltSetWorkingDirectory(argv[0]);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(800,600);
    
    glutCreateWindow("Snake!");
    
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutSpecialFunc(SpecialKeys);
    
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    
    SetupRC();
    glutTimerFunc(100, UpdateSnakePos, 0);
    glutMainLoop();
}
