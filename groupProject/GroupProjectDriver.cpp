//============================================================================
// Name        : Tetris Driver.cpp
// Author      : Dr. Booth
// Version     :
// Copyright   : Your copyright notice
// Description : Main Driver For Group Project
//============================================================================

#include <iostream>
#include "GroupProject.h"
#include "Matrix.h"
#include "strassenAlgorithm.h"
using namespace std;

void Player(void);
GroupProject* app;

int main() {
    Matrix m1(2), m2(2);
    m1.set(0,0,1);
    m1.set(0,1,0);
    m1.set(1,0,0);
    m1.set(1,1,1);
    m2.set(0,0,1);
    m2.set(0,1,2);
    m2.set(1,0,3);
    m2.set(1,1,4);

    cout << strassen(m1, m2) << endl;

    GLUT_Plotter* g = new GLUT_Plotter();   //Create plotter 
    app = new GroupProject(g);              //Create Game (Tetris)
    g->RegisterIdleFunc(Player);            //Register Callback 
    g->MainLoop();                          //Enter main loop
    return 0;
}

void Player(void){
    app->Play();
}
