//
//  Particle.cpp
//  
//
//  Created by Kiran Wattamwar on 12/6/15.
//
//
#include <vector>
#include <vecmath.h>
#include "Particle.h"
#include <iostream>

using namespace std;

Particle::Particle()
{
    alive = true;
    life = 1.0;
    fade = float(rand()%100)/1000.0f+0.003f;
    pos = Vector3f((float) (rand() % 21) - 10, 10.0, (float) (rand() % 21) - 10);
    xpos = (float) (rand() % 21) - 10;
    ypos = 10.0;
    zpos = (float) (rand() % 21) - 10;
    velocity = 0.0;
    gravity = -1.6 * 15;
}