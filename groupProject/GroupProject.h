/*
 * GroupProject.h
 *
 *  Created on: Mar 28, 2014
 *  Created on: Mar 28, 2014
 *      Author: Dr. Booth
 */

#ifndef GroupProject_H_
#define GroupProject_H_
#include "GLUT_Plotter.h"


class GroupProject{
private:
    GLUT_Plotter *g;

public:
    //constructor
    GroupProject(GLUT_Plotter* g);

    //Main GAIM FUNCTION
    void Play(void);
};

#endif /* GroupProject_H_ */
