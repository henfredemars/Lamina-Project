//*******************************************************
/*Lamina Project: Graphic Visualization of Particles

I would like to give credit to the tutorials found in the following webpages:
http://www.lighthouse3d.com/tutorials/glut-tutorial/
http://lazyfoo.net/tutorials/OpenGL/index.php#Hello OpenGL
https://www3.ntu.edu.sg/home/ehchua/programming/opengl/CG_Examples.html
https://en.wikibooks.org/wiki/OpenGL_Programming/
*/
//*******************************************************
#include "LUtil.h"
#include <vector>
#include <string>

using namespace std;

void runMainLoop( int val );

int main( int argc, char* args[] ) {
    //Initialize FreeGLUT
    glutInit( &argc, args );

    //Use OpenGL 2.1
    glutInitContextVersion( 2, 1 );

    //Create Double Buffered, Depth enabled, RGBA Window and Stencil enabled
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT );
    glutCreateWindow( "Project3" );

    //Set keyboard handler function
    glutKeyboardFunc( handleKeys );
    
    //Set Mouse handler function
    glutMouseFunc( handleMouse );
    
    //Set rendering function
    glutDisplayFunc( render );
    
    //Set window resize function
    glutReshapeFunc(reshape);

    //Set main loop
    glutTimerFunc( 1000 / SCREEN_FPS, runMainLoop, 0 );
    
    //Setting the Lamina Particles vectors
    //This is temporary
    vector <Particles> particles_vector;
    vector <string> statistics_vector;
    Particles p1(0.0f, 5.0f, -15.0f, 1.0f);
    Particles p2(6.0f, 4.0f, -10.0f, 1.0f);
    Particles p3(-5.0f, 5.0f, -5.0f, 3.0f);
    Particles p4(-3.0f, 5.0f, -15.0f, 1.0f);

    particles_vector.push_back(p1);
    particles_vector.push_back(p2);
    particles_vector.push_back(p3);
    particles_vector.push_back(p4);
    
    string s1 = "These are statistics for particle 1";
    string s2 = "Particle 2's statistics";
    string s3 = "Third particles's statistics are here";
    string s4 = "Statistics for the 4th particle";
    
    statistics_vector.push_back(s1);
    statistics_vector.push_back(s2);
    statistics_vector.push_back(s3);
    statistics_vector.push_back(s4);
    
    
    //Do initialization
    if( !initGL( particles_vector , statistics_vector) ) {
        printf( "Unable to run graphics application!\n" );
        return 1;
    }
    

    //Start GLUT main loop
    glutMainLoop();

    return 0;
}

void runMainLoop( int val ) {
    //Rendering...
    render();

    //Run frames every 1/60 os second
    glutTimerFunc( 1000 / SCREEN_FPS, runMainLoop, val );
}