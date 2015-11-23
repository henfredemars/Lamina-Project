

#ifndef LUTIL_H
#define LUTIL_H

#include "LOpenGL.h"
#include "particles.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

//Screen constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;



bool initGL(vector <Particles> particles_vec_in , vector <string> statistics_vec_in);

void reshape(GLsizei size_x, GLsizei size_y);

void render();

void handleKeys( unsigned char key, int x, int y );

void handleMouse(int button, int state, int x, int y);

void printText(float x, float y, string input_string);

void resetCamera();

void drawParticles();

#endif
