#include <stdio.h>
#include <stdlib.h>
#include <GL/glfw.h>
#include <OpenGL/glu.h>
#include <SOIL.h>

int width, height;
int light;                                  // Lighting ON / OFF
int filter;                                 // Which Filter To Use
int blend;
int twinkle;
GLfloat xrot;                                   // X Rotation
GLfloat yrot;                                   // Y Rotation
GLfloat xspeed;                                 // X Rotation Speed
GLfloat yspeed;                                 // Y Rotation Speed
GLfloat z=-5.0f;                                // Depth Into The Screen

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };                 // Ambient Light Values ( NEW )
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };              // Diffuse Light Values ( NEW )
GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };                 // Light Position ( NEW )

const int num = 50;

typedef struct                          // Create A Structure For Star
{
    int r, g, b;                        // Stars Color
    GLfloat dist;                       // Stars Distance From Center
    GLfloat angle;                      // Stars Current Angle
}
stars;                              // Structures Name Is Stars
stars star[num];                        // Make 'star' Array Of 'num' Using Info From The Structure 'stars'

GLfloat spin;                           // Spin Twinkling Stars
 
GLuint  loop;                           // General Loop Variable
GLuint  texture[1];                     // Storage For One Texture


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
    texture[0] = load_texture("data/Star.png");
    if(texture[0] == 0)
        return 0;

    // Create Linear Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    return 1;                                        // Return Success
}

int draw(GLvoid)                             // Here's Where We Do All The Drawing
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer
    glBindTexture(GL_TEXTURE_2D, texture[0]);       // Select Our Texture

    for (loop=0; loop<num; loop++)               // Loop Through All The Stars
    {
        glLoadIdentity();               // Reset The View Before We Draw Each Star
        glTranslatef(0.0f,0.0f,z);           // Zoom Into The Screen (Using The Value In 'zoom')
        glRotatef(yrot,0.0f,1.0f,0.0f);         // Tilt The View (Using The Value In 'tilt')
        glRotatef(xrot,1.0f,0.0f,0.0f);         // Tilt The View (Using The Value In 'tilt')
        glRotatef(star[loop].angle,0.0f,1.0f,0.0f); // Rotate To The Current Stars Angle
        glTranslatef(star[loop].dist,0.0f,0.0f);    // Move Forward On The X Plane
        glRotatef(-star[loop].angle,0.0f,1.0f,0.0f);    // Cancel The Current Stars Angle
        glRotatef(-xrot,1.0f,0.0f,0.0f);        // Cancel The Screen Tilt
        glRotatef(-yrot,0.0f,1.0f,0.0f);        // Cancel The Screen Tilt
        if (twinkle){
            // Assign A Color Using Bytes
            glColor4ub(star[(num-loop)-1].r,star[(num-loop)-1].g,star[(num-loop)-1].b,255);
            glBegin(GL_QUADS);          // Begin Drawing The Textured Quad
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
            glEnd();                // Done Drawing The Textured Quad
        }
        glRotatef(spin,0.0f,0.0f,1.0f);         // Rotate The Star On The Z Axis
        // Assign A Color Using Bytes
        glColor4ub(star[loop].r,star[loop].g,star[loop].b,255);
        glBegin(GL_QUADS);              // Begin Drawing The Textured Quad
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
        glEnd();                    // Done Drawing The Textured Quad
        spin+=0.02f;                    // Used To Spin The Stars
        star[loop].angle+=(float)loop/num;      // Changes The Angle Of A Star
        star[loop].dist-=0.01f;             // Changes The Distance Of A Star
        if (star[loop].dist<0.0f)            // Is The Star In The Middle Yet
        {
            star[loop].dist+=5.0f;          // Move The Star 5 Units From The Center
            star[loop].r=rand()%256;        // Give It A New Red Value
            star[loop].g=rand()%256;        // Give It A New Green Value
            star[loop].b=rand()%256;        // Give It A New Blue Value
        }
    }

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
        case 'T':{
            twinkle = !twinkle;
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
    //glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
    //glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Test To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective Calculations
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);             // Setup The Ambient Light
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);             // Setup The Diffuse Light
    glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);            // Position The Light
    glEnable(GL_LIGHT1);                            // Enable Light One
    glColor4f(1.0f,1.0f,1.0f,0.5f);         // Full Brightness, 50% Alpha ( NEW )
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);       // Blending Function For Translucency Based On Source Alpha Value ( NEW )
    for (loop=0; loop<num; loop++)               // Create A Loop That Goes Through All The Stars
    {
        star[loop].angle=0.0f;              // Start All The Stars At Angle Zero
        star[loop].dist=((float)loop/num)*5.0f;     // Calculate Distance From The Center
        star[loop].r=rand()%256;            // Give star[loop] A Random Red Intensity
        star[loop].g=rand()%256;            // Give star[loop] A Random Green Intensity
        star[loop].b=rand()%256;            // Give star[loop] A Random Blue Intensity
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
            //glDisable(GL_DEPTH_TEST);   // Turn Depth Testing Off
        }
        else{
            glDisable(GL_BLEND);        // Turn Blending Off
            //glEnable(GL_DEPTH_TEST);    // Turn Depth Testing On
        }

        xrot += xspeed;
        yrot += yspeed;

        draw();
        // Swap buffers
        glfwSwapBuffers();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
            glfwGetWindowParam( GLFW_OPENED ) );
}
