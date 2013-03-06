#include <stdio.h>
#include <stdlib.h>
#include <GL/glfw.h>
#include <OpenGL/glu.h>

int width, height;

void init_gl(){
    glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();

    float aspect_ratio = ((float)height) / width;
    gluPerspective(45.0f,(1.f/aspect_ratio),0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);

    glShadeModel(GL_SMOOTH);                        // Enables Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                   // Black Background
    glClearDepth(1.0f);                         // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Test To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective Calculations
}

int init(){
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // 4x antialiasing

    width = 1024;
    height = 768;

    // Open a window and create its OpenGL context
    if( !glfwOpenWindow( width, height, 0,0,0,0, 32,0, GLFW_WINDOW ) )
    {
        fprintf( stderr, "Failed to open GLFW window\n" );
        glfwTerminate();
        return -1;
    }
    glfwSetWindowTitle( "Tutorial" );
    glfwEnable( GLFW_STICKY_KEYS );
    init_gl();
    return 0;
}

void draw(GLvoid)                             // Here's Where We Do All The Drawing
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear The Screen And The Depth Buffer
    glLoadIdentity();                           // Reset The Current Modelview Matrix
    glTranslatef(-1.5f,0.0f,-6.0f);                 // Move Left 1.5 Units And Into The Screen 6.0
    glBegin(GL_TRIANGLES);                      // Drawing Using Triangles

        glVertex3f( 0.0f, 1.0f, 0.0f);          // Move Up One Unit From Center (Top Point)
        glVertex3f(-1.0f,-1.0f, 0.0f);          // Left And Down One Unit (Bottom Left)
        glVertex3f( 1.0f,-1.0f, 0.0f);          // Right And Down One Unit (Bottom Right)

    glEnd();                            // Finished Drawing The Triangle
    glTranslatef(3.0f,0.0f,0.0f);                   // Move Right 3 Units

    glBegin(GL_QUADS);                      // Draw A Quad
        glVertex3f(-1.0f, 1.0f, 0.0f);              // Top Left
        glVertex3f( 1.0f, 1.0f, 0.0f);              // Top Right
        glVertex3f( 1.0f,-1.0f, 0.0f);              // Bottom Right
        glVertex3f(-1.0f,-1.0f, 0.0f);              // Bottom Left
    glEnd();                            // Done Drawing The Quad
    glLoadIdentity();                           // Reset The Current Modelview Matrix

}


int main(){

    int status = init();
    if(status) return status;

    do{
        // Draw nothing, see you in tutorial 2 !
        draw();
        // Swap buffers
        glfwSwapBuffers();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
            glfwGetWindowParam( GLFW_OPENED ) );
}
