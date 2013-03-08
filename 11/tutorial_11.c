#include <stdio.h>
#include <stdlib.h>
#include <GL/glfw.h>
#include <OpenGL/glu.h>
#include <SOIL.h>
#include <math.h>

int width, height;
GLfloat     xrot, yrot, zrot;
GLuint      texture[1];                 // Storage For One Texture ( NEW )
float points[45][45][3];
int wiggle_count = 0;
float hold;

float rad(float degrees)
{
    return degrees * 0.01745329252f;
}

float deg(float radians)
{
    return radians * 57.295779513f;
}

int load()
{
    /* load an image file directly as a new OpenGL texture */
    texture[0] = SOIL_load_OGL_texture
        (
         "data/Glass.bmp",
         SOIL_LOAD_AUTO,
         SOIL_CREATE_NEW_ID,
         SOIL_FLAG_INVERT_Y
        );

    if(texture[0] == 0)
        return 0;

    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    return 1;                                        // Return Success
}

int draw(GLvoid)                             // Here's Where We Do All The Drawing
{
    int x, y;                       // Loop Variables
    float float_x, float_y, float_xb, float_yb;     // Used To Break The Flag Into Tiny Quads

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear The Screen And The Depth Buffer
    glLoadIdentity();                           // Reset The Current Modelview Matrix
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And Depth Buffer   
    glLoadIdentity();                   // Reset The Current Matrix

    glTranslatef(0.0f,0.0f,-12.0f);             // Translate 12 Units Into The Screen

    glRotatef(xrot,1.0f,0.0f,0.0f);             // Rotate On The X Axis
    glRotatef(yrot,0.0f,1.0f,0.0f);             // Rotate On The Y Axis 
    glRotatef(zrot,0.0f,0.0f,1.0f);             // Rotate On The Z Axis

    glBindTexture(GL_TEXTURE_2D, texture[0]);       // Select Our Texture
    glBegin(GL_QUADS);                  // Start Drawing Our Quads
    for( x = 0; x < 44; x++ )                // Loop Through The X Plane (44 Points)
    {
        for( y = 0; y < 44; y++ )            // Loop Through The Y Plane (44 Points)
        {
            float_x = x/44.0f;       // Create A Floating Point X Value
            float_y = y/44.0f;       // Create A Floating Point Y Value
            float_xb = (x+1)/44.0f;        // Create A Floating Point Y Value+0.0227f
            float_yb = (y+1)/44.0f;        // Create A Floating Point Y Value+0.0227f
            glTexCoord2f( float_x, float_y);    // First Texture Coordinate (Bottom Left)
            glVertex3f( points[x][y][0], points[x][y][1], points[x][y][2] );

            glTexCoord2f( float_x, float_yb );  // Second Texture Coordinate (Top Left)
            glVertex3f( points[x][y+1][0], points[x][y+1][1], points[x][y+1][2] );

            glTexCoord2f( float_xb, float_yb ); // Third Texture Coordinate (Top Right)
            glVertex3f( points[x+1][y+1][0], points[x+1][y+1][1], points[x+1][y+1][2] );

            glTexCoord2f( float_xb, float_y );  // Fourth Texture Coordinate (Bottom Right)
            glVertex3f( points[x+1][y][0], points[x+1][y][1], points[x+1][y][2] );
        }
    }
    glEnd();                        // Done Drawing Our Quads
    if( wiggle_count == 2 )                 // Used To Slow Down The Wave (Every 2nd Frame Only)
    {
        for( y = 0; y < 45; y++ )            // Loop Through The Y Plane
        {
            hold=points[0][y][2];           // Store Current Value One Left Side Of Wave
            for( x = 0; x < 44; x++)     // Loop Through The X Plane
            {
                // Current Wave Value Equals Value To The Right
                points[x][y][2] = points[x+1][y][2];
            }
            points[44][y][2]=hold;          // Last Value Becomes The Far Left Stored Value
        }
        wiggle_count = 0;               // Set Counter Back To Zero
    }
    wiggle_count++;                     // Increase The Counter
    xrot+=0.3f;                     // Increase The X Rotation Variable
    yrot+=0.2f;                     // Increase The Y Rotation Variable
    zrot+=0.4f;                     // Increase The Z Rotation Variable


    return 1;
}

int init_gl(){
    if(!load()) return 0;
    glEnable(GL_TEXTURE_2D);
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
    glPolygonMode(GL_BACK, GL_FILL);
    glPolygonMode(GL_FRONT, GL_LINE);
    for(int x=0; x<45; x++)
    {
        // Loop Through The Y Plane
        for(int y=0; y<45; y++)
        {
            // Apply The Wave To Our Mesh
            points[x][y][0]=(x/5.0f)-4.5f;
            points[x][y][1]=(y/5.0f)-4.5f;
            points[x][y][2]=sin(rad((x/5.0f)*40.0f));
        }
    }

    return 1;
}

int init(){
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return 0;
    }
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // 4x antialiasing

    width = 1024;
    height = 768;

    // Open a window and create its OpenGL context
    if( !glfwOpenWindow( width, height, 0,0,0,0, 32,0, GLFW_WINDOW ) )
    {
        fprintf( stderr, "Failed to open GLFW window\n" );
        glfwTerminate();
        return 0;
    }
    glfwSetWindowTitle( "Tutorial" );
    glfwEnable( GLFW_STICKY_KEYS );
    init_gl();
    return 1;
}

int main(){

    if(!init()) return -1;

    do{
        // Draw nothing, see you in tutorial 2 !
        draw();
        // Swap buffers
        glfwSwapBuffers();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
            glfwGetWindowParam( GLFW_OPENED ) );
}
