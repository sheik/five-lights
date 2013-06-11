#include <getopt.h>
#include <stdlib.h>
#include <GLUT/glut.h>

// The Following Directive Fixes The Problem With Extra Console Window
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

// Global Variables
bool g_gamemode;       // GLUT GameMode ON/OFF
bool g_fullscreen;     // Fullscreen Mode ON/OFF (When g_gamemode Is OFF)

GLfloat rtri = 0;   // angle for triangle
GLfloat rquad = 0;  // angle for quad

// Our GL Specific Initializations
bool init(void)
{
	glShadeModel(GL_SMOOTH);                // Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);   // Black Background
	glClearDepth(1.0f);                     // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);                // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                 // The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	return true;
}

// Our Rendering Is Done Here
void render(void)   
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
    glLoadIdentity(); // Reset The Current Modelview Matrix
    glTranslatef(-1.5f, 0.0f, -6.0f); // left 1.5, then into the screen 6 units

    rtri += 0.2f;
    rquad -=0.15f;

    glRotatef(rtri,0.0f,1.0f,0.0f);

    glBegin(GL_TRIANGLES);
        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f(-1.0f,-1.0f, 0.0f);
        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f( 1.0f,-1.0f, 0.0f);
    glEnd();

    glLoadIdentity(); // reset after rotation
    glTranslatef(1.5f, 0.0f, -6.0f); 
    glRotatef(rquad,1.0f,0.0f,0.0f);
    glRotatef(rquad,0.0f,1.0f,0.0f);

    glColor3f(0.5f,0.5f,1.0f);
    glBegin(GL_QUADS);
        glVertex3f(-1.0f, 1.0f, 0.0f);
        glVertex3f( 1.0f, 1.0f, 0.0f);
        glVertex3f( 1.0f,-1.0f, 0.0f);
        glVertex3f(-1.0f,-1.0f, 0.0f);
    glEnd();

    // Swap The Buffers To Become Our Rendering Visible
    glutSwapBuffers ( );

    glutPostRedisplay();
}

// Our Reshaping Handler (Required Even In Fullscreen-Only Modes)
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);     // Select The Projection Matrix
	glLoadIdentity();                // Reset The Projection Matrix
	// Calculate The Aspect Ratio And Set The Clipping Volume
	if (h == 0) h = 1;
	gluPerspective(80, (float)w/(float)h, 1.0, 5000.0);
	glMatrixMode(GL_MODELVIEW);      // Select The Modelview Matrix
	glLoadIdentity();                // Reset The Modelview Matrix
}

// Our Keyboard Handler (Normal Keys)
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 27:        // When Escape Is Pressed...
                case 'e':
                    exit(0);
                    break;
		default:        // Now Wrap It Up
		break;
	}
}

// Our Keyboard Handler For Special Keys (Like Arrow Keys And Function Keys)
void special_keys(int a_keys, int x, int y)
{
	switch (a_keys) {
		case GLUT_KEY_UP:
			// We Can Switch Between Windowed Mode And Fullscreen Mode Only
			if (!g_gamemode) {
				g_fullscreen = !g_fullscreen;       // Toggle g_fullscreen Flag
				if (g_fullscreen) glutFullScreen(); // We Went In Fullscreen Mode
				else glutReshapeWindow(500, 500);   // We Went In Windowed Mode
			}
		break;
		default:
		break;
	}
}

// Main Function For Bringing It All Together.
int main(int argc, char** argv)
{
    // parse program arguments
    int c;
    while((c = getopt(argc, argv, "f")) != -1)
        switch(c) {
            case 'f':
                g_gamemode = true;
                break;
            default:
                break;
        }

    g_fullscreen = false;
    glutInit(&argc, argv);                           // GLUT Initializtion
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);     // Display Mode (Rgb And Double Buffered)
    if (g_gamemode) {
        glutGameModeString("640x480:16");            // Select The 640x480 In 16bpp Mode
        if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
            glutEnterGameMode();                     // Enter Full Screen
        else g_gamemode = false;                     // Cannot Enter Game Mode, Switch To Windowed
    }
    if (!g_gamemode) {
        glutInitWindowSize(500, 500);                // Window Size If We Start In Windowed Mode
        glutCreateWindow("OpenGL Framework");        // Window Title 
    }
    init();                                          // Our Initialization
    glutDisplayFunc(render);                         // Register The Display Function
    glutReshapeFunc(reshape);                        // Register The Reshape Handler
    glutKeyboardFunc(keyboard);                      // Register The Keyboard Handler
    glutSpecialFunc(special_keys);                   // Register Special Keys Handler
    glutMainLoop();                                  // Go To GLUT Main Loop
    return 0;
}

