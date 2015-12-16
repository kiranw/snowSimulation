//
//  Particle.h
//  
//
//  Created by Kiran Wattamwar on 12/6/15.
//
//
#ifndef PARTICLE_H
#define PARTICLE_H

#include <stdio.h>
#include <vector>

class Particle
{
public:
    
    Particle();
    // position
    Vector3f pos;
//    float xpos;
//    float ypos;
//    float zpos;
    
    // particle properties
    bool alive;
    float life;
    float fade;
    
    // color
    Vector3f color;
//    float red;
//    float green;
//    float blue;
    
    // forces
    float vel;
    float gravity;
};

#endif
