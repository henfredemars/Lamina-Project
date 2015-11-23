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
    
    //Do initialization
    if( !initGL() ) {
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
