

#ifndef LUTIL_H
#define LUTIL_H

#include "LOpenGL.h"
#include <stdio.h>



//Screen constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;



bool initGL();

void reshape(GLsizei size_x, GLsizei size_y);

void render();

void handleKeys( unsigned char key, int x, int y );

void handleMouse(int button, int state, int x, int y);

void resetCamera();

void drawParticles();

#endif
