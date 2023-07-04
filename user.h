#ifndef USER_H
#define USER_H

void exitProgram(int exitCode);
void updateRotation(int x, int y, bool updateZ);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
double getCurrentTime();
void handleKeyPress(unsigned char key, int x, int y);
void handleSpecialKeyPress(int key, int x, int y);


#endif