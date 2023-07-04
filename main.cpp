#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "amplitude.h"
#include "normals.h"
#include "display.h"
#include "user.h"
#include "config.h"

GLuint texID; 
int texWidth, texHeight, texChannels; 
unsigned char* texData; 


int mY,mX,mZ;
float WaveAmp[2][scene][scene];
int source = 0, target = 1;


GLfloat position[] = { 0.0, 0.0, 1.5, 3.0 };
GLfloat norm[3];



void loadTexture(const char* filename) {
    texData = stbi_load(filename, &texWidth, &texHeight, &texChannels, 0);
    if (!texData) {
        fprintf(stderr, "ERROR: could not load %s\n", filename);
        exit(1);
    }
}

void generateTexture() {
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    if (texChannels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    else if (texChannels == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    stbi_image_free(texData);
}

void init()
{
    int i, j;
    GLfloat diffuseMaterial[] = { 0.0, 0.467, 0.845, 1.0 };
    GLfloat specularMaterial[] = { 0.0, 0.467, 0.745, 1.0 };
    GLfloat shininessMaterial[] = { 100.0 };

    // OpenGL state
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininessMaterial);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LEQUAL);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);

    //wave amplitude
    for( i = 0; i < scene; i++)
    {
        for( j = 0; j < scene; j++)
        {
            WaveAmp[0][j][i] = 0;
            WaveAmp[1][j][i] = 0;
        }
    }
}

// Reshape callback
void reshape(int width, int height)
{

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (GLfloat) width/(GLfloat) height, 1.0, 2000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Water physics");

    // Register GLUT callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeyPress);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutSpecialFunc(handleSpecialKeyPress);
    glutIdleFunc(idle);

    glEnable(GL_DEPTH_TEST);
    loadTexture("water1.png");
    generateTexture();
    init();
    glutMainLoop();
    return 0;
}