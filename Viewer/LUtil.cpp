
#include "LUtil.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

//Camera position on XZ plane
GLfloat gCameraPositionX = 0.f, gCameraPositionY = 0.f, gCameraPositionZ = -80.f;
//Camera Direction XZ plane
//At first the camera looks into the -Z direction.
GLfloat gCameraDirectionX = 0.0f, gCameraDirectionZ = 1.0f, gCameraDirectionY = 0.0f;

//Different Angles for the Camera Manipulations
float angleCameraXZ = 3.14159f;      //Camera Rotations are controlled by keys: A and D
float angleCameraXY = 0;
bool camRight = false;
bool camLeft = false;
bool camUp = false;
bool camDown = false;
float delta_xy = 0;
float delta_xz = 0;

//Other useful variables
const float DEG2RAD = 3.14159/180;

//Vector of Vectors of Strings for global and particle statistics.
vector < string> global_statistics_vector;
vector < vector <string>> particle_statistics_vector;

//Particles Size ratio
const int size_ratio = 1;

//Statistics Flag and particle number
bool statistics_flag = false;
GLuint stencil_index = 0;

//Lamina database, lamina particles and source particles global variables
Database database("test1.db");
Lamina lamina;
vector<LaminaParticle> lamina_particles_vector;
Source source;
vector<SourceParticle> source_particles_vector;
int currentGenerationNumber = 0;
int maxGenerationNumber;
int time_step = 1;
bool runFlag = false;


//Init Function
bool initGL( string filename_in) {
    //Initializing Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    //Initializing Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    //Initializing lighting parameters
    GLfloat light_direction[] = { 0.0, 0.0, 1.0, 1.0 };
    GLfloat light_intensity[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat ambient_intensity[] = { 0.4, 0.4, 0.4, 1.0 };
    GLfloat specular_intensity[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat shininess[] = {40.0};

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
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_intensity);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_intensity);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    
    //Enabling color material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    //glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
    
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
    
    //Getting the initial lamina particles and source particles
    Database database_in(filename_in);
    database = std::move(database_in);
    
    maxGenerationNumber = database.getMaxGenerationNumber();
    
    lamina = database.getLaminaParticlesForGeneration(0);
    lamina_particles_vector = lamina.asVector();
    
    source = database.getSourceParticles();
    source_particles_vector = source.asVector();
    
    updateStatistics(0);
    
    return true;
}

//Rendering Function
void render() {
    //Clear color buffer and stencil buffer
    glClearStencil(0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
                  
    //Defining Camera Position
    handleCamera();
    gluLookAt(	gCameraPositionX, gCameraPositionY, gCameraPositionZ,
			gCameraPositionX+gCameraDirectionX, gCameraPositionY+gCameraDirectionY, gCameraPositionZ+gCameraDirectionZ,
			0.0f, 1.0f,  0.0f);	
			
	//Rendering the Floor
	glPushMatrix();
	
	//Enabling stencil operations before drawing
	glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);	
	
	//glTranslatef(0.0f, -11.0f, 0.0f);
	
	glStencilFunc(GL_ALWAYS, 0, -1);
	
	//Drawing Walls
    drawWalls(1, 30, 30,  300,  300,  -300,  300, -300);
    drawWalls(1, 30, 30, -300, -300,  300, -300,  300);
    
    drawWalls(2, 30, 30,  300, -300,  300,  300, -300);
    drawWalls(2, 30, 30, -300, -300,  300, -300,  300);
    
    drawWalls(3, 30, 30,  300, -300,  300,  300, -300);
    drawWalls(3, 30, 30, -300, -300,  300, -300,  300);
    
    //Checking if automatic animation is ON
    if (runFlag && currentGenerationNumber+time_step<=maxGenerationNumber) {
        currentGenerationNumber += time_step;
        updateCurrentDatabase(currentGenerationNumber);
    }
    
    //Particle Rendering
    drawParticles();
    
    //Drawing text
    //if (statistics_flag) printText(-1, 0.9, statistics_vector[(int) stencil_index-1]);
    printText(-1, 0.9, 0.17, -0.7);
      
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
    gluPerspective(45.0f, aspect, 0.1f, 800.0f);
}

//handleCamera Function: It moves the camera according to the mouse position.
void handleCamera() {
    if (camRight) delta_xz = 0.03;
    else if (camLeft) delta_xz = -0.03;
    else delta_xz = 0;
    
    if (camUp) delta_xy = 0.03;
    else if (camDown) delta_xy = -0.03;
    else delta_xy = 0;
    
    if (camUp || camDown || camLeft || camRight) {
        angleCameraXZ += delta_xz;
        gCameraDirectionX = sin(angleCameraXZ);
        gCameraDirectionZ = -cos(angleCameraXZ);
        
        angleCameraXY += delta_xy;
        gCameraDirectionY = sin(angleCameraXY);
        //gCameraDirectionY = -cos(angleCameraXY);
    }
}

//handleKeys Function
/*This function receives input from the keyboard.
Key W: Move Camera Forward
Key S: Move Camera Backwards
Key F: Go forward in Time by one time_step
Key B: Go backwards in Time by one time_step
Key O: Go forward in Time automatically
Key P: Stop the Automatic Animation
Key R: Reset the Camera*/
void handleKeys( unsigned char key, int x, int y ) {

    //If the user pressed w/a/s/d, change camera position
    
    //Key W changes the Position of the Camera (forward translate) in cameraMode
    if( key == 'w') {
        //if (gCameraPositionX + gCameraDirectionX * 0.4f > 280 || gCameraPositionX + gCameraDirectionX * 0.4f < -280)
            gCameraPositionX += gCameraDirectionX * 0.4f;
        //if (gCameraPositionZ + gCameraDirectionZ * 0.4f > 280 || gCameraPositionZ + gCameraDirectionZ * 0.4f < -280)
            gCameraPositionZ += gCameraDirectionZ * 0.4f;
    }
    //Key S changes the Position of the Camera (backwards translate) in cameraMode
    else if( key == 's') {
        //if (gCameraPositionX - gCameraDirectionX * 0.4f > 280 || gCameraPositionX - gCameraDirectionX * 0.4f < -280)
            gCameraPositionX -= gCameraDirectionX * 0.4f;
        //if (gCameraPositionZ - gCameraDirectionZ * 0.4f > 280 || gCameraPositionZ - gCameraDirectionZ * 0.4f < -280)
            gCameraPositionZ -= gCameraDirectionZ * 0.4f;
    }
    //Key F goes forwards one time step
    else if( key == 'f')  {
        if (currentGenerationNumber + time_step <= maxGenerationNumber) {
            currentGenerationNumber += time_step;
            updateCurrentDatabase(currentGenerationNumber);
        }   
        updateGlobalStatistics(); 
    }
    //Key B goes backwards one time step
    else if( key == 'b') {
        if (currentGenerationNumber - time_step >= 0) {
            currentGenerationNumber -= time_step;
            updateCurrentDatabase(currentGenerationNumber);
        }   
    }
    else if( key == 't') {
        if (time_step == 1) time_step = 2;
        else if (time_step == 2) time_step = 5;
        else if (time_step == 5) time_step = 10;
        else if (time_step == 10) time_step = 20;
        else time_step = 1;
        
        updateCurrentDatabase(currentGenerationNumber);
    }
    //Key O: Starts automatic animation. Key P: Pauses automatic animation
    else if ( key == 'o') {
        runFlag = true;
    }
    else if ( key == 'p') {
        runFlag = false;
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
    //GLuint stencil_index;   //Only the stencil stencil_index will be used
    
    //Reading Pixels
    //glReadPixels(x, window_height - y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
    //glReadPixels(x, window_height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glReadPixels(x, window_height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &stencil_index);
    
    //Setting the statistics flag, if a particle was selected, then the flag is set to true
    if (stencil_index != 0) {
        statistics_flag = true;
    }
    else {
        statistics_flag = false;
    }
}

//mouseMovement Functions: It tracks the movement of the mouse and moves the camera accordingly
void mouseMovement(int x, int y) {
    int window_width = glutGet(GLUT_WINDOW_WIDTH);
    int window_height = glutGet(GLUT_WINDOW_HEIGHT);
    
    if (x < window_width/15) {
        camLeft = true;
        camRight = false;
    }
    else if (x > window_width*14/15) {
        camLeft = false;
        camRight = true;
    }
    else {
        camLeft = false;
        camRight = false;
    }
    
    if (y > window_height*14/15) {
        camDown = true;
        camUp = false;
    }
    else if (y < window_height/15) {
        camDown = false;
        camUp = true;
    }
    else {
        camDown = false;
        camUp = false;
    }
    
}

//printText Function
/*This function prints text on screen*/
void printText(float x_particle, float y_particle, float x_global, float y_global) {
    
    glPushMatrix(); //Pushing Modelview Matrix
    glMatrixMode( GL_PROJECTION );
    glPushMatrix(); //Pushing Projection Matrix
    
    //Resetting Projection and Modelview Matrices
    //This way, the text will have different coordinate system than the rest of the scene
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    //Disabling Lighting, so the text will be properly rendered
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos3f(x_global, y_global, 0);  //Text Position 

    for (int i=0; i< (int)global_statistics_vector.size(); i++) {
        string input_string = global_statistics_vector[i];
        int l;
        const char *st = input_string.c_str();

        l=(int)input_string.size();
        
        //Resetting the stencil
        glStencilFunc(GL_ALWAYS, 0, -1);    
        for(int j=0; j < l; j++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[j]); // Printing a character on screen
        }
        y_global -= 0.1;
        glRasterPos3f(x_global, y_global, 0);
    }
    
    if (statistics_flag) {
        glRasterPos3f(x_particle, y_particle, 0);
        vector < string> vec_string = particle_statistics_vector[stencil_index-1];
        
        for (int i=0; i< (int)vec_string.size(); i++) {

            string input_string = vec_string[i];
            int l;
            const char *st = input_string.c_str();

            l=(int)input_string.size();
            
            //Resetting the stencil
            glStencilFunc(GL_ALWAYS, 0, -1);    
            for(int j=0; j < l; j++) {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[j]); // Printing a character on screen
            }
            y_particle -= 0.1;
            glRasterPos3f(x_particle, y_particle, 0);
        }
    }
    
    //Reeabling Lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
        
    //Recovering Projection and ModelView Matrices    
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
    
}

//Camera Reset Function
void resetCamera() {
    gCameraPositionX = 0.0f;
    gCameraPositionY = 0.0f;
    gCameraPositionZ = -80.0f;
    
    angleCameraXZ = 3.1452f;
    angleCameraXY = 0;
    
    gCameraDirectionX = 0.0f;
    gCameraDirectionZ = 1.0f;
    gCameraDirectionY = 0.0f;
    
    camRight = false;
    camLeft = false;
    camUp = false;
    camDown = false;
    delta_xz = 0;
    delta_xy = 0;
}

//Particles Drawing Function
/*It draws a sphere for each particle in the particles vector*/
void drawParticles() {

    //Here I start drawing each lamina particle
    for(int i =0; i< (int)lamina_particles_vector.size(); i++) {
    
        const double p_rl = lamina_particles_vector[i].getRadius()*size_ratio;
        const double p_xl = lamina_particles_vector[i].getX();
        const double p_yl = lamina_particles_vector[i].getY();
        const double p_zl = lamina_particles_vector[i].getZ();
        
        glPushMatrix();
        //Moving to Particle Position
        glTranslatef(p_xl,p_yl,p_zl);
        
        //Setting Particle color and drawing it as sphere
        if ((int)stencil_index==i+1) glColor3f(0.9f, 0.5f, 0.5f);
        else glColor3f(0.1f, 0.1f, 0.95f);
        glStencilFunc(GL_ALWAYS, i + 1, -1);   
        glutSolidSphere(p_rl,20,20);

        glPopMatrix();
    }    
    
    
    //Here I start drawing each source particle
    for(int i =0; i< (int)source_particles_vector.size(); i++) {
    
        const double p_rs = source_particles_vector[i].getRadius()*size_ratio;
        const double p_xs = source_particles_vector[i].getX();
        const double p_ys = source_particles_vector[i].getY();
        const double p_zs = source_particles_vector[i].getZ();
        
        glPushMatrix();
        //Moving to Particle Position
        glTranslatef(p_xs,p_ys,p_zs);
        
        //Setting Particle color and drawing it as sphere
        if ((int)stencil_index==i+1+(int)lamina_particles_vector.size()) glColor3f(0.5f, 0.5f, 0.1f);
        else glColor3f(0.1f, 0.5f, 0.5f);
        glStencilFunc(GL_ALWAYS, i + 1 +(int)lamina_particles_vector.size(), -1);   
        glutSolidSphere(p_rs,20,20);

        glPopMatrix();
    }  
    
    
}

//Walls Drawing Function
/*It draws walls*/
void drawWalls(const int type, const int rows, const int cols, const float p0, const float p1, const float p2, const float p3, const float p4) {
    //Here I start drawing the wall
    const float step_row = (p2-p1)/rows;
    const float step_col = (p4-p3)/cols;
        
    if (type == 1) {
        for (int i =0; i<rows; i++) {
            for (int j=0; j<cols; j++) {
                if ((j+i)%2==0) glColor3f(0.5f,0.0f,0.0f);
                else glColor3f(0.82f,0.52f,0.25f);
                glBegin(GL_QUADS);
                glVertex3f( p0,  p3+step_row*i    , p1+step_col*j);
                glVertex3f( p0,  p3+step_row*(i+1), p1+step_col*j);
                glVertex3f( p0,  p3+step_row*(i+1), p1+step_col*(j+1));
                glVertex3f( p0,  p3+step_row*i    , p1+step_col*(j+1));
                glEnd();
            }
        }
    }
    else if (type == 2) {
        for (int i =0; i<rows; i++) {
            for (int j=0; j<cols; j++) {
                if ((j+i)%2==0) glColor3f(0.5f,0.0f,0.0f);
                else glColor3f(0.82f,0.52f,0.25f);
                glBegin(GL_QUADS);
                glVertex3f(p1+step_row*i    , p0, p3+step_col*j);
                glVertex3f(p1+step_row*(i+1), p0, p3+step_col*j);
                glVertex3f(p1+step_row*(i+1), p0, p3+step_col*(j+1));
                glVertex3f(p1+step_row*i    , p0, p3+step_col*(j+1));
                glEnd();
            }
        }
    }   
    else if (type == 3) {
        for (int i =0; i<rows; i++) {
            for (int j=0; j<cols; j++) {
                if ((j+i)%2==0) glColor3f(0.5f,0.0f,0.0f);
                else glColor3f(0.82f,0.52f,0.25f);
                glBegin(GL_QUADS);
                glVertex3f(p1+step_row*i    , p3+step_col*j, p0);
                glVertex3f(p1+step_row*(i+1), p3+step_col*j, p0);
                glVertex3f(p1+step_row*(i+1), p3+step_col*(j+1), p0);
                glVertex3f(p1+step_row*i    , p3+step_col*(j+1), p0);
                glEnd();
            }
        }
    }
}

//This function updates the Current lamina by setting a new Generation Number
void updateCurrentDatabase(int genNumber) {
    currentGenerationNumber = genNumber;
    lamina = database.getLaminaParticlesForGeneration(currentGenerationNumber);
    lamina_particles_vector.clear();
    lamina_particles_vector = lamina.asVector();
    updateStatistics(genNumber);
}

//This function updates the statistics vector for displaying information
void updateStatistics(int genNumber) {

    particle_statistics_vector.clear();
    global_statistics_vector.clear();
    vector <string> string_line_temp;
    string string_temp;
    
    double rad, x_c, y_c, z_c, q;
    
    for(int i =0; i<(int)lamina_particles_vector.size(); i++) {
        string_line_temp.clear();
        string_temp = "Lamina Particle #: " + to_string(i);
        string_line_temp.push_back(string_temp);
        
        rad = lamina_particles_vector[i].getRadius();
        string_temp = "Radius: " + to_string(rad);
        string_line_temp.push_back(string_temp);
        
        x_c = lamina_particles_vector[i].getX();
        y_c = lamina_particles_vector[i].getY();
        z_c = lamina_particles_vector[i].getZ();
        string_temp = "Position: (" + to_string(x_c) + "," + to_string(y_c) + "," + to_string(z_c) + ")";
        string_line_temp.push_back(string_temp);
        
        particle_statistics_vector.push_back(string_line_temp);
    }

    for(int i =0; i<(int)source_particles_vector.size(); i++) {
        string_line_temp.clear();
        string_temp = "Source Particle #: " + to_string(i);
        string_line_temp.push_back(string_temp);
        
        rad = source_particles_vector[i].getRadius();
        string_temp = "Radius: " + to_string(rad);
        string_line_temp.push_back(string_temp);
        
        x_c = source_particles_vector[i].getX();
        y_c = source_particles_vector[i].getY();
        z_c = source_particles_vector[i].getZ();
        string_temp = "Position: (" + to_string(x_c) + "," + to_string(y_c) + "," + to_string(z_c) + ")";
        string_line_temp.push_back(string_temp);
        
        q = source_particles_vector[i].getQ();
        string_temp = "Charge: " + to_string(q);
        string_line_temp.push_back(string_temp);
        
        particle_statistics_vector.push_back(string_line_temp);
    }
    
    //Now Updating Global Statistics
    string_temp = "Max Gen Number: " + to_string(maxGenerationNumber);
    global_statistics_vector.push_back(string_temp);
    
    string_temp = "Current Gen Number: " + to_string(currentGenerationNumber);
    global_statistics_vector.push_back(string_temp);
    
    string_temp = "# of Particles: " + to_string((int)lamina_particles_vector.size()+(int)source_particles_vector.size());
    global_statistics_vector.push_back(string_temp);
    
    string_temp = "Time Step: " + to_string(time_step);
    global_statistics_vector.push_back(string_temp);
        
}

void updateGlobalStatistics() {
    //Now Updating Global Statistics
    string string_temp;
    string_temp = "Max Gen Number: " + to_string(maxGenerationNumber);
    global_statistics_vector.push_back(string_temp);
    
    string_temp = "Current Gen Number: " + to_string(currentGenerationNumber);
    global_statistics_vector.push_back(string_temp);
    
    string_temp = "# of Particles: " + to_string((int)lamina_particles_vector.size()+(int)source_particles_vector.size());
    global_statistics_vector.push_back(string_temp);
    
    string_temp = "Time Step: " + to_string(time_step);
    global_statistics_vector.push_back(string_temp);
}
