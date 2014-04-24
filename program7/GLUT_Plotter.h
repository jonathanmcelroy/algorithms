/*
 * GLUT_Plotter.h
 *
 *  Created on: Mar 19, 2014
 *      Author: booth
 */

#ifndef GLUT_PLOTTER_H_
#define GLUT_PLOTTER_H_

#include <iostream>
#include <cstdlib>
#include <GL/glut.h>

using namespace std;

const int PADDING = 4;

class GLUT_Plotter{
private:
    int width;
    int height;
    char *buffer;
    bool historyON;

    void setpixel(char *buf, int x, int y, int r, int g, int b);

public:
    GLUT_Plotter(int width = 500, int height = 500);
    void DisplayFunc(void (*func)(void));
    void Draw(int []);
    void Clear();
    void KeyboardFunc(void (*func)(unsigned char key, int x, int y));
    void MainLoop();
    void init(int *argc, char **argv);
    void init();
    int getWidth();
    int getHeight();
    char* getBuffer();
    void ShowHistory();
};

#endif /* GLUT_PLOTTER_H_ */
