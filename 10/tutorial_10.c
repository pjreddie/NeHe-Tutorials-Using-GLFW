#include <stdio.h>
#include <stdlib.h>
#include <GL/glfw.h>
#include <OpenGL/glu.h>
#include <SOIL.h>
#include <math.h>

int width, height;
int light;                                  // Lighting ON / OFF
int filter;                                 // Which Filter To Use
int blend;
GLfloat xrot;                                   // X Rotation
GLfloat yrot;                                   // Y Rotation
GLfloat xspeed;                                 // X Rotation Speed
GLfloat yspeed;                                 // Y Rotation Speed
GLfloat zspeed;                                 // Y Rotation Speed
GLfloat speed;                                 // Y Rotation Speed
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
GLuint  texture[3];                     // Storage For One Texture

typedef struct tagVERTEX                        // Build Our Vertex Structure
{
    float x, y, z;                          // 3D Coordinates
    float u, v;                         // Texture Coordinates
} VERTEX;                               // Call It VERTEX

typedef struct tagTRIANGLE                      // Build Our Triangle Structure
{
    VERTEX vertex[3];                       // Array Of Three Vertices
} TRIANGLE;                             // Call It TRIANGLE

typedef struct tagSECTOR                        // Build Our Sector Structure
{
    int numtriangles;                       // Number Of Triangles In Sector
    TRIANGLE* triangle;                     // Pointer To Array Of Triangles
} SECTOR;                               // Call It SECTOR

SECTOR sector1;
GLfloat walkbias = 0;
GLfloat walkbiasangle = 0;
GLfloat lookupdown = 0.0;
float heading, xpos, ypos, zpos;


float rad(float angle)
{
    return angle * 0.0174532925f;
}


void readstr(FILE *f, char *string)
{
    do {
        fgets(string, 255, f); // read the line
    } while ((string[0] == '/') || (string[0] == '\n'));
    return;
}

// loads the world from a text file.
void SetupWorld() 
{
    float x, y, z, u, v;
    int vert;
    int numtriangles;
    FILE *filein;        // file to load the world from
    char oneline[255];

    filein = fopen("data/world.txt", "rt");

    readstr(filein, oneline);
    sscanf(oneline, "NUMPOLLIES %d\n", &numtriangles);

    sector1.numtriangles = numtriangles;
    sector1.triangle = malloc(sizeof(TRIANGLE)*numtriangles);

    for (loop = 0; loop < numtriangles; loop++) {
        for (vert = 0; vert < 3; vert++) {
            readstr(filein,oneline);
            sscanf(oneline, "%f %f %f %f %f", &x, &y, &z, &u, &v);
            sector1.triangle[loop].vertex[vert].x = x;
            sector1.triangle[loop].vertex[vert].y = y;
            sector1.triangle[loop].vertex[vert].z = z;
            sector1.triangle[loop].vertex[vert].u = u;
            sector1.triangle[loop].vertex[vert].v = v;
        }
    }

    fclose(filein);
    return;
}


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
    texture[0] = load_texture("data/mud.bmp");
    texture[1] = load_texture("data/mud.bmp");
    texture[2] = load_mipmap_texture("data/mud.bmp");
    if(texture[0] == 0)
        return 0;

    glBindTexture(GL_TEXTURE_2D, texture[0]);   // 2d texture (x and y size)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // scale cheaply when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // scale cheaply when image smalled than texture

    glBindTexture(GL_TEXTURE_2D, texture[1]);   // 2d texture (x and y size)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

    glBindTexture(GL_TEXTURE_2D, texture[2]);   // 2d texture (x and y size)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); // scale mipmap when image smalled than texture


    return 1;                                        // Return Success
}

int update()
{
    xpos += sin(rad(yrot))*speed;
    zpos += cos(rad(yrot))*speed;
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
    return 1;
}

int draw()                             // Here's Where We Do All The Drawing
{
    glLoadIdentity();
    int i;
    GLfloat x_m, y_m, z_m, u_m, v_m;
    ypos = .1;

    float sceneroty = 360-yrot;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer
    glBindTexture(GL_TEXTURE_2D, texture[0]);       // Select Our Texture
    glRotatef(sceneroty, 0, 1.f, 0);

    glTranslatef(-xpos, -ypos, -zpos);    

    glBindTexture(GL_TEXTURE_2D, texture[filter]);

    for(i = 0; i < sector1.numtriangles; ++i){
        glBegin(GL_TRIANGLES);      
        glNormal3f( 0.0f, 0.0f, 1.0f);

        x_m = sector1.triangle[i].vertex[0].x;
        y_m = sector1.triangle[i].vertex[0].y;
        z_m = sector1.triangle[i].vertex[0].z;
        u_m = sector1.triangle[i].vertex[0].u;
        v_m = sector1.triangle[i].vertex[0].v;
        glTexCoord2f(u_m,v_m); 
        glVertex3f(x_m,y_m,z_m);

        x_m = sector1.triangle[i].vertex[1].x;
        y_m = sector1.triangle[i].vertex[1].y;
        z_m = sector1.triangle[i].vertex[1].z;
        u_m = sector1.triangle[i].vertex[1].u;
        v_m = sector1.triangle[i].vertex[1].v;
        glTexCoord2f(u_m,v_m); 
        glVertex3f(x_m,y_m,z_m);

        x_m = sector1.triangle[i].vertex[2].x;
        y_m = sector1.triangle[i].vertex[2].y;
        z_m = sector1.triangle[i].vertex[2].z;
        u_m = sector1.triangle[i].vertex[2].u;
        v_m = sector1.triangle[i].vertex[2].v;
        glTexCoord2f(u_m,v_m); 
        glVertex3f(x_m,y_m,z_m);    

        glEnd();    
    }
    glfwSwapBuffers();
    return 1;
}

void key_poll()
{
    speed = 0;
    if(glfwGetKey(GLFW_KEY_UP)      == GLFW_PRESS) speed=-.2f;
    if(glfwGetKey(GLFW_KEY_DOWN)    == GLFW_PRESS) speed=+.2f;
    if(glfwGetKey(GLFW_KEY_RIGHT)   == GLFW_PRESS) yrot-=1.5f;
    if(glfwGetKey(GLFW_KEY_LEFT)    == GLFW_PRESS) yrot+=1.5f;
    if(glfwGetKey('0') == GLFW_PRESS) z+=.04f;
    if(glfwGetKey('1') == GLFW_PRESS) z-=.04f;
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
    SetupWorld();
    return 1;
}

int main(){

    if(!init()) return -1;

    do{
        key_poll();
        update();
        draw();
        // Swap buffers

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
            glfwGetWindowParam( GLFW_OPENED ) );
}
