

#ifndef LUTIL_H
#define LUTIL_H

#include "LOpenGL.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

#include "../lib/sim/include/Database.h"
#include "../lib/sim/include/Particle.h"
#include "../lib/sim/include/LaminaParticle.h"
#include "../lib/sim/include/SourceParticle.h"

using namespace std;

//Screen constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;



bool initGL(string filename_in);

void reshape(GLsizei size_x, GLsizei size_y);

void render();

void handleKeys( unsigned char key, int x, int y );

void handleMouse(int button, int state, int x, int y);

void mouseMovement(int x, int y);

void printText(float x_particle, float y_particle, float x_global, float y_global);

void resetCamera();

void drawParticles();

void drawWalls(const int type, const int rows, const int cols, const float p0, const float p1, const float p2, const float p3, const float p4);

void updateStatistics(int genNumber);

void updateGlobalStatistics();

void updateCurrentDatabase(int genNumber);

void handleCamera();

#endif
