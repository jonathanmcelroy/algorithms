 /*
 * GroupProject.cpp
 *
 *  Created on: Mar 31, 2014
 *  Modified on: Mar 31, 2014
 *  Author: D. Booth
 */

#include "GroupProject.h"

//GroupProject Constructor

GroupProject::GroupProject(GLUT_Plotter* g){
    this->g = g;
}


//GroupProject Main Game Loop
void GroupProject::Play(void){

    //Check for Keyboard Hit
    while(g->kbhit()){
        int k = g->getKey();
        switch (k){
             case 27: exit(1); //ESC key
                      break;
        }
    }


    //Check for mouse click
    while(g->click()){

        Click c;
        c = g->getClick();
    }

    // Update screen - draw game
    g->Draw();
}
