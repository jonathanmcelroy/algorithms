/*
 * GLUT_Plotter.cpp
 *
 *  Created on: Mar 21, 2014
 *      Author: Dr. Booth
 */

#include <string.h>
#include "GLUT_Plotter.h"

GLUT_Plotter::GLUT_Plotter(int w, int h){

    width = w+PADDING;
    height = h+PADDING;
    buffer = new char[width*height*3];
    historyON = false;
    init();
}

void GLUT_Plotter::init(){
    int *argc=new int(1);
    char **argv = new char*[1];
    argv[0] = new char[10];
    argv[0][0] = '\0';

    glutInitWindowSize(width+PADDING, height+PADDING);
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("Dr. Booth's Plotter Class");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

}

void GLUT_Plotter::init(int *argc, char **argv){

    glutInitWindowSize(width, height);
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("Dr. Booth's Plotter Class");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

}

void GLUT_Plotter::MainLoop(){
    glutMainLoop();
}

void GLUT_Plotter::KeyboardFunc(void (*func)(unsigned char key, int x, int y)){
    glutKeyboardFunc(func);
}

void GLUT_Plotter::DisplayFunc(void (*func)(void)){
    glutDisplayFunc(func);
}

void GLUT_Plotter::Clear(){
    memset(buffer,0,width*height*3);
    glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);
    glutSwapBuffers();
}

void GLUT_Plotter::Draw(int a[]){
     int x,y;
     if(!historyON)
         memset(buffer,0,width*height*3);

        for (int i = 0; i < getWidth(); i++) {
          x = i + PADDING;
          y = a[i] + PADDING;

          setpixel(buffer, x, y, 255, 255, 255);

          if(x>0)       setpixel(buffer, x-1, y, 255, 255, 255);
          if(x<width-1) setpixel(buffer, x+1, y, 255, 255, 255);
          if(y>0)       setpixel(buffer, x, y-1, 255, 255, 255);
          if(y<width-1) setpixel(buffer, x, y+1, 255, 255, 255);

          if(x > 0 && y > 0)           setpixel(buffer, x-1, y-1, 255, 255, 255);
          if(x > 0 && y<width-1)       setpixel(buffer, x-1,y+1, 255, 255, 255);
          if(x < width-1 && y<width-1) setpixel(buffer, x+1, y+1, 255, 255, 255);
          if(x < width-1 && y>0 )      setpixel(buffer, x+1, y-1, 255, 255, 255);


        }


      // drawpixels draws the rgb data stored in 'buffer' to the screen
      glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);


      // in double buffer mode so we swap to avoid a flicker
      glutSwapBuffers();

      // instruct event system to call 'drawfunc' again
      //glutPostRedisplay();

}

int GLUT_Plotter::getWidth(){
  return width-PADDING;
}

int GLUT_Plotter::getHeight(){
  return height-PADDING;
}


char* GLUT_Plotter::getBuffer(){
    return buffer;
}

void GLUT_Plotter::setpixel(char *buf, int x, int y, int r, int g, int b) {

  buf[(y*width+x)*3+0] = r;
  buf[(y*width+x)*3+1] = g;
  buf[(y*width+x)*3+2] = b;

}

void GLUT_Plotter::ShowHistory(){
    historyON = !historyON;
}
