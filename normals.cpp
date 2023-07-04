#include <math.h>
#include "normals.h"
#include <GL/glut.h>
#include "config.h"
#include <glm/glm.hpp>


extern GLfloat norm[3];


void calculateNormals(float x1, float y1, float z1, 
                      float x2, float y2, float z2, 
                      float x3, float y3, float z3) 
{
    glm::vec3 point1 = glm::vec3(x1, y1, z1);
    glm::vec3 point2 = glm::vec3(x2, y2, z2);
    glm::vec3 point3 = glm::vec3(x3, y3, z3);

    glm::vec3 v1 = point1 - point2;
    glm::vec3 v2 = point2 - point3;

    glm::vec3 glmNorm = glm::cross(v1, v2);
    if (glm::length(glmNorm) == 0.0f) {
    } else {
        glmNorm = glm::normalize(glmNorm);
    }

    norm[0] = glmNorm.x;
    norm[1] = glmNorm.y;
    norm[2] = glmNorm.z;
}


