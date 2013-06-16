#include <iostream>
#include <stdlib.h>
#include <GL/glfw.h>
 
void Init(void);
void Shut_Down(int return_code);
void Main_Loop(void);
void Draw_Square(float red, float green, float blue);
void Draw(void);
 
float rotate_x = 0,
      rotate_y = 0,
      rotate_z = 0;
const float rotations_per_tick = .2;
 
double old_time = 0;

int main(void)
{
  Init();
  Main_Loop();
  Shut_Down(0);
}
 
void Init(void)
{
  const int window_width = 800,
            window_height = 600;
 
  if (glfwInit() != GL_TRUE)
    Shut_Down(1);
  // 800 x 600, 16 bit color, no depth, alpha or stencil buffers, windowed
  if (glfwOpenWindow(window_width, window_height, 5, 6, 5,
                     0, 0, 0, GLFW_WINDOW) != GL_TRUE)
    Shut_Down(1);
  glfwSetWindowTitle("The GLFW Window");

  glEnable(GL_CULL_FACE);

  // smoothing?
  //glEnable (GL_LINE_SMOOTH);
  //glEnable (GL_BLEND);
  //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
  glLineWidth (1.5);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  // set the projection matrix to a normal frustum with a max depth of 50
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspect_ratio = ((float)window_height) / window_width;
  glFrustum(.5, -.5, -.5 * aspect_ratio, .5 * aspect_ratio, 1, 50);
  glMatrixMode(GL_MODELVIEW);
}
 
void Shut_Down(int return_code)
{
  glfwTerminate();
  exit(return_code);
}

int Handle_Keyboard(void)
{
    // the time of the previous frame
    // calculate time elapsed, and the amount by which stuff rotates
    double current_time = glfwGetTime(),
           delta_rotate = (current_time - old_time) * rotations_per_tick * 360;

    old_time = current_time;

    if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
        return -1;

    if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS)
      rotate_y += delta_rotate;
    if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS)
      rotate_y -= delta_rotate;
    if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS)
        rotate_x -= delta_rotate;
    if (glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS)
        rotate_x += delta_rotate;

    return 0;
}

void Main_Loop(void)
{
    // this just loops as long as the program runs
    while(1)
    {

        // end program
        if(Handle_Keyboard() == -1) break;

        // clear the buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw the figure
        Draw();

        // swap back and front buffers
        glfwSwapBuffers();
    }
}

void Draw_Square(float red, float green, float blue)
{
  // Draws a square with a gradient color at coordinates 0, 10
  glBegin(GL_QUADS);
  {
    glColor3f(red, green, blue);
    glVertex2i(1, 11);
    glColor3f(red * .8, green * .8, blue * .8);
    glVertex2i(-1, 11);
    glColor3f(red * .5, green * .5, blue * .5);
    glVertex2i(-1, 9);
    glColor3f(red * .8, green * .8, blue * .8);
    glVertex2i(1, 9);
  }
  glEnd();
}

void Draw_Triangle(float red, float green, float blue)
{
    glBegin(GL_TRIANGLES);
        glColor3f(red, green, blue);
        glVertex2i(0, 11);
        glColor3f(red * .5, green * .5, blue * .5);
        glVertex2i(-1, 9);
        glColor3f(red * .8, green * .8, blue * .8);
        glVertex2i(1, 9);
    glEnd();
}

void Draw_Cube(float red, float green, float blue)
{
    // front
    glBegin(GL_POLYGON);
        glColor3f(1.0, 0.0, 0.0);

        glVertex3f( 1.0, 1.0, 1.0 );
        glVertex3f( 1.0,-1.0, 1.0 );
        glVertex3f(-1.0,-1.0, 1.0 );
        glVertex3f(-1.0, 1.0, 1.0 );
    glEnd();

    // back
    glBegin(GL_POLYGON);
        glColor3f(0.0, 1.0, 0.0);

        glVertex3f( 1.0, 1.0,-1.0 );
        glVertex3f(-1.0, 1.0,-1.0 );
        glVertex3f(-1.0,-1.0,-1.0 );
        glVertex3f( 1.0,-1.0,-1.0 );
    glEnd();

    // left 
    glBegin(GL_POLYGON);
        glColor3f(0.0, 0.0, 1.0);

        glVertex3f( 1.0,-1.0,-1.0 );
        glVertex3f( 1.0,-1.0, 1.0 );
        glVertex3f( 1.0, 1.0, 1.0 );
        glVertex3f( 1.0, 1.0,-1.0 );
    glEnd();

    // right 
    glBegin(GL_POLYGON);
        glColor3f(0.0, 1.0, 1.0);

        glVertex3f(-1.0,-1.0,-1.0 );
        glVertex3f(-1.0, 1.0,-1.0 );
        glVertex3f(-1.0, 1.0, 1.0 );
        glVertex3f(-1.0,-1.0, 1.0 );
    glEnd();
    
    
    //  top
    glBegin(GL_POLYGON);
        glColor3f(1.0, 0.0, 1.0);

        glVertex3f(-1.0, 1.0,-1.0 );
        glVertex3f( 1.0, 1.0,-1.0 );
        glVertex3f( 1.0, 1.0, 1.0 );
        glVertex3f(-1.0, 1.0, 1.0 );
    glEnd();

    //  bottom 
    glBegin(GL_POLYGON);
        glColor3f(1.0, 1.0, 0.0);

        glVertex3f(-1.0,-1.0,-1.0 );
        glVertex3f(-1.0,-1.0, 1.0 );
        glVertex3f( 1.0,-1.0, 1.0 );
        glVertex3f( 1.0,-1.0,-1.0 );
    glEnd();
}

/*
int LoadGLTextures()
{
    texture[0] = SOIL_load_OGL_
}
*/

void Draw_Pyramid(float red, float green,float blue)
{
    glBegin(GL_TRIANGLES);
        // Draw front of pyramid
        glColor3f(1.0f,1.0f,0.0f);      // Red
        glVertex3f(0.0f, 1.0f, 0.0f);   // Front / Top
        glVertex3f(-1.0f,-1.0f, 1.0f);  // Front / Left 
        glVertex3f(1.0f,-1.0f, 1.0f);   // Front / Right 

        // Draw left side of pyramid
        glColor3f(0.0f,1.0f,0.0f);      // Green
        glVertex3f(0.0f, 1.0f, 0.0f);   // Left / Top
        glVertex3f(-1.0f,-1.0f,-1.0f);  // Left / Left 
        glVertex3f(-1.0f,-1.0f, 1.0f);   // Left / Right 

        // Draw right side of pyramid
        glColor3f(0.0f,0.0f,1.0f);      // Blue
        glVertex3f(0.0f, 1.0f, 0.0f);   // Right / Top
        glVertex3f( 1.0f,-1.0f, 1.0f);  // Right / Left 
        glVertex3f( 1.0f,-1.0f,-1.0f);   // Right / Right 

        // Draw back side of pyramid
        glColor3f(1.0f,0.0f,0.0f);      // Red
        glVertex3f(0.0f, 1.0f, 0.0f);   // Back / Top
        glVertex3f( 1.0f,-1.0f,-1.0f);  // Back / Left 
        glVertex3f(-1.0f,-1.0f,-1.0f);   // Back / Right 

    glEnd();
}


void Draw(void)
{
  // reset view matrix
  glLoadIdentity();
  // move view back a bit
  glTranslatef(0, 0, -10);
  glRotatef(rotate_y, 0, 1, 0);
  glRotatef(rotate_x, 1, 0, 0);

  // apply the current rotation
  //glRotatef(rotate_z, 0, 0, 1);
  // by repeatedly rotating the view matrix during drawing, the
  // squares end up in a circle
//  int i = 0, squares = 15;
    float red = 0, blue = 1;
    Draw_Cube(red, 0.6, blue);
    glTranslatef(2.1, 0, 0);
    Draw_Cube(red, 0.6, blue);
    std::cout << glGetString(GL_VERSION) << std::endl;
}
