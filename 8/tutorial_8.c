#include <stdio.h>
#include <stdlib.h>
#include <GL/glfw.h>
#include <OpenGL/glu.h>
#include <SOIL.h>

int width, height;
int light;                                  // Lighting ON / OFF
int filter;                                 // Which Filter To Use
int blend;
GLfloat xrot;                                   // X Rotation
GLfloat yrot;                                   // Y Rotation
GLfloat xspeed;                                 // X Rotation Speed
GLfloat yspeed;                                 // Y Rotation Speed
GLfloat z=-5.0f;                                // Depth Into The Screen

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };                 // Ambient Light Values ( NEW )
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };              // Diffuse Light Values ( NEW )
GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };                 // Light Position ( NEW )

GLuint  texture[3];                             // Storage for 3 textures

GLuint load_texture(char *filename)
{
    return SOIL_load_OGL_texture
        (
         filename,
         SOIL_LOAD_AUTO,
         SOIL_CREATE_NEW_ID,
         SOIL_FLAG_INVERT_Y
        );
}
GLuint load_mipmap_texture(char *filename)
{
    return SOIL_load_OGL_texture
        (
         filename,
         SOIL_LOAD_AUTO,
         SOIL_CREATE_NEW_ID,
         SOIL_FLAG_MIPMAPS|SOIL_FLAG_INVERT_Y
        );
}

int load()
{
    /* load an image file directly as a new OpenGL texture */
    texture[0] = load_texture("data/Glass.bmp");
    texture[1] = load_texture("data/Glass.bmp");
    texture[2] = load_mipmap_texture("data/Glass.bmp");
    if(texture[0] == 0)
        return 0;

    // Create Nearest Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // ( NEW )
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // ( NEW )

    // Create Linear Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    // Create MipMapped Texture
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); // ( NEW )

    return 1;                                        // Return Success
}

int draw(GLvoid)                             // Here's Where We Do All The Drawing
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear The Screen And The Depth Buffer
    glLoadIdentity();                           // Reset The View
    glTranslatef(0.0f,0.0f,z);                      // Translate Into/Out Of The Screen By z

    glRotatef(xrot,1.0f,0.0f,0.0f);                     // Rotate On The X Axis By xrot
    glRotatef(yrot,0.0f,1.0f,0.0f);                     // Rotate On The Y Axis By yrot
    glBindTexture(GL_TEXTURE_2D, texture[filter]);              // Select A Texture Based On filter

    glBegin(GL_QUADS);                          // Start Drawing Quads

    // Front Face
    glNormal3f( 0.0f, 0.0f, 1.0f);                  // Normal Pointing Towards Viewer
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 1 (Front)
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 2 (Front)
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Front)
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 4 (Front)
    // Back Face
    glNormal3f( 0.0f, 0.0f,-1.0f);                  // Normal Pointing Away From Viewer
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Back)
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 2 (Back)
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 3 (Back)
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 4 (Back)
    // Top Face
    glNormal3f( 0.0f, 1.0f, 0.0f);                  // Normal Pointing Up
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 1 (Top)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 2 (Top)
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Top)
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 4 (Top)
    // Bottom Face
    glNormal3f( 0.0f,-1.0f, 0.0f);                  // Normal Pointing Down
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Bottom)
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 2 (Bottom)
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 3 (Bottom)
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 4 (Bottom)
    // Right face
    glNormal3f( 1.0f, 0.0f, 0.0f);                  // Normal Pointing Right
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 1 (Right)
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 2 (Right)
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Right)
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 4 (Right)
    // Left Face
    glNormal3f(-1.0f, 0.0f, 0.0f);                  // Normal Pointing Left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Left)
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 2 (Left)
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 3 (Left)
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 4 (Left)
    glEnd();                                // Done Drawing Quads

    xrot+=xspeed;                               // Add xspeed To xrot
    yrot+=yspeed;                               // Add yspeed To yrot
    glLoadIdentity();                           // Reset The View

    return 1;
}

void key_press(int key)
{
    switch(key){
        case 'L':{
            light = !light;
            break;
        }
        case 'F':{
            filter = (filter+1)%3;
            break;
        }
        case 'B':{
            blend = !blend;
            break;
        }
        default:{
            break;
        }
    }
}

void key_release(int key)
{
    return;
}

void GLFWCALL key_callback(int key, int action)
{
    switch(action){
        case GLFW_PRESS:{
            key_press(key);
            break;
        }
        case GLFW_RELEASE:{
            key_release(key);
            break;
        }
        default:{
            break;
        }
    }
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
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);             // Setup The Ambient Light
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);             // Setup The Diffuse Light
    glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);            // Position The Light
    glEnable(GL_LIGHT1);                            // Enable Light One
    glColor4f(1.0f,1.0f,1.0f,0.5f);         // Full Brightness, 50% Alpha ( NEW )
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);       // Blending Function For Translucency Based On Source Alpha Value ( NEW )

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
    //glfwEnable( GLFW_STICKY_KEYS );
    glfwSetKeyCallback(key_callback);
    if(!init_gl()) return 0;
    return 1;
}

int main(){

    if(!init()) return -1;

    do{
        if(glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS) xspeed-=.04f;
        if(glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS) xspeed+=.04f;
        if(glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) yspeed+=.04f;
        if(glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS) yspeed-=.04f;
        if(glfwGetKey('0') == GLFW_PRESS) z+=.04f;
        if(glfwGetKey('1') == GLFW_PRESS) z-=.04f;
        // Draw nothing, see you in tutorial 2 !
        if (!light) glDisable(GL_LIGHTING);     // Disable Lighting
        else glEnable(GL_LIGHTING);      // Enable Lighting
        if(blend){
            glEnable(GL_BLEND);     // Turn Blending On
            glDisable(GL_DEPTH_TEST);   // Turn Depth Testing Off
        }
        else{
            glDisable(GL_BLEND);        // Turn Blending Off
            glEnable(GL_DEPTH_TEST);    // Turn Depth Testing On
        }


        draw();
        // Swap buffers
        glfwSwapBuffers();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
            glfwGetWindowParam( GLFW_OPENED ) );
}
