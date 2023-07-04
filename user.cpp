#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <math.h>
#include <algorithm>
#include <chrono>


#include "config.h"
#include "amplitude.h"
#include "user.h"


int ripple_x = scene/2;
int ripple_y = scene/2;


extern float WaveAmp[2][scene][scene];
extern int rotationX, rotationY, rotationZ;
extern int mX, mY, mZ;
extern GLfloat position[4];
extern GLfloat norm[3];
extern int source, target;



// Function to handle program exit
void exitProgram(int exitCode) {
    exit(exitCode);
}

// Function to update rotation variables
void updateRotation(int x, int y, bool updateZ) {
    if (updateZ) {
        rotationZ = y - mY;
    } else {
        rotationX = x - mX;
        rotationY = y - mY;
    }
    glutPostRedisplay();
}

// Mouse button handler
void mouse(int button, int state, int x, int y) {
    if (button == 0) {
        mX = x - rotationX;
        mY = y - rotationY;
    } else if (button == 2) {
        mY = y - rotationZ;
        mZ = (mZ ? 0 : 1);
    }
    glutPostRedisplay();
}

// Mouse motion handler
void motion(int x, int y) {
    updateRotation(x, y, mZ);
}

// Timer functions
std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
double getCurrentTime() {
    std::chrono::duration<double> elapsed_seconds = std::chrono::steady_clock::now() - start_time;
    return elapsed_seconds.count();
}

// Function to handle key presses
void handleKeyPress(unsigned char key, int x, int y) {
    static int old_x = 50;
    static int old_y = 50;
    static int old_width = 512;
    static int old_height = 512;
    float frequency = 0.1f;
    float speed = 1.0f;

    switch (key) {
    case 27:
        exitProgram(0);
        break;
    case 'w':
        glutPositionWindow(old_x, old_y);
        glutReshapeWindow(old_width, old_height);
        break;
    case 'f':
        if (glutGet(GLUT_WINDOW_WIDTH) < glutGet(GLUT_SCREEN_WIDTH)) {
            old_x = glutGet(GLUT_WINDOW_X);
            old_y = glutGet(GLUT_WINDOW_Y);
            old_width = glutGet(GLUT_WINDOW_WIDTH);
            old_height = glutGet(GLUT_WINDOW_HEIGHT);
            glutFullScreen();
        }
        break;
    case ' ':
        WaveAmp[target][ripple_x][ripple_y] = -10;
        break;
    case 'k': 
        start_time = std::chrono::steady_clock::now();
        calcWaveAmplitude(getCurrentTime(), frequency, speed);
        break;
    case 'p':
        start_time = std::chrono::steady_clock::now();
        calcWaveAmplitudeWithPerlinNoise(getCurrentTime(), frequency, speed);
        break;
    case 'm':
        start_time = std::chrono::steady_clock::now();
        calcMovementWithPerlinNoise(getCurrentTime(), frequency, speed);
        break;
    case 's':
        stopAllWaves();
        break;
    }
}

// Function to handle special key presses
void handleSpecialKeyPress(int key, int x, int y) {
    switch(key) {
    case GLUT_KEY_LEFT:
        ripple_x = std::max(0, ripple_x - 1); 
        break;
    case GLUT_KEY_RIGHT:
        ripple_x = std::min(scene - 1, ripple_x + 1); 
        break;
    case GLUT_KEY_UP:
        ripple_y = std::max(0, ripple_y - 1); 
        break;
    case GLUT_KEY_DOWN:
        ripple_y = std::min(scene - 1, ripple_y + 1); 
        break;
    }
}



