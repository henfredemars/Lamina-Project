
#include "LUtil.h"
#include <cmath>
#include <iostream>
#include <vector>
#include "particles.h"

using namespace std;

//Camera position on XZ plane
GLfloat gCameraPositionX = 0.f, gCameraPositionY = 0.f, gCameraPositionZ = 0.f;
//Camera Direction on XZ plane (I won't allow changes of direction in Y)
//At first the camera looks into the -Z direction.
GLfloat gCameraDirectionX = 0.0f, gCameraDirectionZ = -1.0f;

//Different Angles for the Camera Manipulations
float angleCamera = 0.0f;      //Camera Rotations are controlled by keys: A and D

//Other useful variables
const float DEG2RAD = 3.14159/180;

//Vector of Particles (TEST ONLY)
vector <Particles> particles_vector;

//Init Function
bool initGL() {
    //Initializing Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    //Initializing Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    //Initializing lighting parameters
    GLfloat light_direction[] = { 0.0, 2.0, -1.0, 1.0 };
    GLfloat light_intensity[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat ambient_intensity[] = { 0.3, 0.3, 0.3, 1.0 };

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );
    
    //Setting the backgroung depth to farthest
    glClearDepth(1.0f);
    
    //Initialize Depth Test
    glEnable(GL_DEPTH_TEST);
    
    //Enabling lighting
    glEnable(GL_LIGHTING);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_intensity);
    
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_direction);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_intensity);
    
    //Enabling color material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    //Type of Depth Testing
    glDepthFunc(GL_LEQUAL);
    
    //Smooth Shading
    glShadeModel(GL_SMOOTH);

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR ) {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }
    
    //Creating the Initial Particles Vector
    /*This is temporary, test only.
    3 Particles will be added*/
    Particles p1(0.0f, 5.0f, -15.0f, 1.0f);
    Particles p2(6.0f, 4.0f, -10.0f, 1.0f);
    Particles p3(-5.0f, 5.0f, -5.0f, 3.0f);

    particles_vector.push_back(p1);
    particles_vector.push_back(p2);
    particles_vector.push_back(p3);
    
    
    return true;
}


void render() {
    //Clear color buffer and stencil buffer
    glClearStencil(0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
          
    //Defining Camera Position
    gluLookAt(	gCameraPositionX, gCameraPositionY, gCameraPositionZ,
			gCameraPositionX+gCameraDirectionX, 0.0f, gCameraPositionZ+gCameraDirectionZ,
			0.0f, 1.0f,  0.0f);	
			
	//Rendering the Floor
	glPushMatrix();
	
	//Enabling stencil operations before drawing
	glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);	
	
	glTranslatef(0.0f, -11.0f, 0.0f);
	
	glStencilFunc(GL_ALWAYS, 0, -1);
	glBegin(GL_QUADS);
	    //Drawing Floor
        glColor3f(0.9f, 0.9f, 0.9f);
        glVertex3f( 100.0f, 0.0f, 100.0f);
        glVertex3f(-100.0f, 0.0f, 100.0f);
        glVertex3f(-100.0f, 0.0f, -100.0f);
        glVertex3f( 100.0f, 0.0f, -100.0f);
    glEnd(); 
    glPopMatrix();
    
    //Particle Rendering
    drawParticles();
        
    //Swapping Back and Forth Render Buffers  
    glutSwapBuffers();    
}

void reshape(GLsizei size_x, GLsizei size_y) {

    //Aspect Ratio of the Resized Window
    if (size_y == 0) size_y = 1;
    GLfloat aspect = (GLfloat)size_x / (GLfloat)size_y;
    
    //Setting a viewport for the new resized window
    glViewport(0,0,size_x,size_y);
    
    //Setting aspect ratio of CLIPPING Space, matching VIEWPORT
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

//handleKeys Function
/*This function receives input from the keyboard.
Key A: Move Camera left
Key D: Move Camera Right
Key W: Move Camera Forward
Key S: Move Camera Backwards*/
void handleKeys( unsigned char key, int x, int y ) {

    //If the user pressed w/a/s/d, change camera position
    
    //Key W changes the Position of the Camera (forward translate) in cameraMode
    if( key == 'w') {
        gCameraPositionX += gCameraDirectionX * 0.1f;
        gCameraPositionZ += gCameraDirectionZ * 0.1f;
    }
    //Key S changes the Position of the Camera (backwards translate) in cameraMode
    else if( key == 's') {
        gCameraPositionX -= gCameraDirectionX * 0.1f;
        gCameraPositionZ -= gCameraDirectionZ * 0.1f;
    }
    //Key A Rotates camera to the Left in both Fly and Orbit Modes
    else if( key == 'a')  {
        angleCamera -= 0.01f;
        gCameraDirectionX = sin(angleCamera);
        gCameraDirectionZ = -cos(angleCamera);
    }
    //Key D Rotates camera to the right in both Fly and Orbit Modes
    else if( key == 'd') {
        angleCamera += 0.01f;
        gCameraDirectionX = sin(angleCamera);
        gCameraDirectionZ = -cos(angleCamera);
    }  
    //Reset Camera
    else if(key == 'r') {
        resetCamera();
    }
}

//handleMouse function. It handles Particle Selection using the mouse
void handleMouse(int button, int state, int x, int y) {
    //Return if the Button was not pressed (or if it recognizes a 'release' action)
    if (state != GLUT_DOWN) return;
    
    //Getting window size
    const int window_width = glutGet(GLUT_WINDOW_WIDTH);
    const int window_height = glutGet(GLUT_WINDOW_HEIGHT);
    
    //Setting variables used to recogize the objects which are clicked by the user
    //GLbyte color[4];
    //GLfloat depth;
    GLuint index;   //Only the stencil index will be used
    
    //Reading Pixels
    //glReadPixels(x, window_height - y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
    //glReadPixels(x, window_height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glReadPixels(x, window_height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
    
    if (index != 0) {
    printf("Clicked on stencil index %u\n",
          index);
    }
}

//Camera Reset Function
void resetCamera() {
    gCameraPositionX = 0.0f;
    gCameraPositionY = 0.0f;
    gCameraPositionZ = 0.0f;
    
    angleCamera = 0;
    
    gCameraDirectionX = 0.0f;
    gCameraDirectionZ = -1.0f;
}

//Particles Drawing Function
/*It draws a sphere for each particle in the particles vector*/
void drawParticles() {
    //Here I start drawing each particle
    for(int i =0; i< (int)particles_vector.size(); i++) {
    
        const float p_r = particles_vector[i].getRadius();
        const float p_x = particles_vector[i].getX();
        const float p_y = particles_vector[i].getY();
        const float p_z = particles_vector[i].getZ();
        
        glPushMatrix();
        //Moving to Particle Position
        glTranslatef(p_x,p_y,p_z);
        
        //Setting Particle color and drawing it as sphere
        glColor3f(0.9f, 0.5f, 0.5f);
        glStencilFunc(GL_ALWAYS, i + 1, -1);   
        glutSolidSphere(p_r,20,20);

        glPopMatrix();
    }    
}

