#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <math.h>

#include "config.h"
#include "display.h"
#include "amplitude.h"
#include "normals.h"


extern bool stop;
extern int ripple_x, ripple_y;
extern float WaveAmp[2][scene][scene];
int rotationX, rotationY, rotationZ;
extern GLfloat position[4];
extern GLfloat norm[3];
extern GLuint texID; 
extern int source, target;



void resetWaveAmp() {
    for (int i = 0; i <  scene - 1; i++) {
        for (int j = 0; j < scene - 1; j++) {
            WaveAmp[source][i][j] = 0;
        }
    }
}

void drawWave() {
    float w = scene/2;
    for(int i = 0; i < scene-1; i++) {
        for(int j = 0; j < scene-1; j++) {
            calculateNormals(j-w, i-w, WaveAmp[source][j][i],
                    j+1-w, i-w, WaveAmp[source][j+1][i],
                    j+1-w, i+1-w, WaveAmp[source][j+1][i+1]);

            glNormal3fv(norm);
            glTexCoord2f((float)j/scene, (float)i/scene); glVertex3f(j-scene/2, i-scene/2, WaveAmp[source][j][i]);
            glTexCoord2f((float)(j+1)/scene, (float)i/scene); glVertex3f(j+1-scene/2, i-scene/2, WaveAmp[source][j+1][i]);
            glTexCoord2f((float)(j+1)/scene, (float)(i+1)/scene); glVertex3f(j+1-scene/2, i+1-scene/2, WaveAmp[source][j+1][i+1]);

            calculateNormals(j-w, i+1-w, WaveAmp[source][j][i+1],
                    j-w, i-w, WaveAmp[source][j][i],
                    j+1-w, i+1-w, WaveAmp[source][j+1][i+1]);

            glNormal3fv(norm);
            glTexCoord2f((float)j/scene, (float)(i+1)/scene); glVertex3f(j-scene/2, i+1-scene/2, WaveAmp[source][j][i+1]);
            glTexCoord2f((float)j/scene, (float)i/scene); glVertex3f(j-scene/2, i-scene/2, WaveAmp[source][j][i]);
            glTexCoord2f((float)(j+1)/scene, (float)(i+1)/scene); glVertex3f(j+1-scene/2, i+1-scene/2, WaveAmp[source][j+1][i+1]);
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLightfv (GL_LIGHT0, GL_POSITION, position);

    glPushMatrix();
    glTranslatef (-1.0, 1.0, -10);

    glDisable (GL_LIGHTING);
    glColor3f (0.0, 1.0, 1.0);

    glEnable (GL_LIGHTING);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, rotationZ-200);
    glRotatef(rotationX, 0, 1, 0);
    glRotatef(rotationY-60, 1, 0, 0);

    calcAmplitude();
    glColor3f(0, 1, 1);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID);

    glBegin(GL_TRIANGLES);

    if (stop) {
        resetWaveAmp();
        stop = false;
    }

    drawWave();

    glEnd();

    std::swap(source, target);

    glPopMatrix();
    glutSwapBuffers();
}


void idle()
{
	glutPostRedisplay();
}