//
//  forceHandler.cpp
//  
//
//  Created by Kiran Wattamwar on 10/24/15.
//
//

#include "forceHandler.h"
#include <iostream>

using namespace std;

// constructor
forceHandler::forceHandler()
{
}



// for indexing
int position_index(int i)
{
    return 2*i;
}

int velocity_index(int i)
{
    return (2*i) + 1;
}



// initializes diff types of forces
void forceHandler::spring(int particle_1, int particle_2, float r, float k)
{
    spring_forces.push_back(Vector4f(particle_1, particle_2, r, k));
}

void forceHandler::damping(int particle, float k)
{
    damping_forces.push_back(Vector2f(particle, k));
}

void forceHandler::gravity(int particle, Vector3f acceleration)
{
    gravity_forces.push_back(Vector4f(particle, acceleration));
}



// initializes a particle with no force acting on it
void forceHandler::staticParticle(int particle)
{
    statics.push_back(particle);
}


// the deriv of the system
vector<Vector3f> forceHandler::differentiate(vector<Vector3f> state, vector<float> masses)
{
    unsigned i;
    Vector3f force;
    vector<Vector3f> f(state.size());
    
    for (i = 0; i < state.size()/2; i++)
    {
        // switch x to v, v to 0
        f[position_index(i)] = state[velocity_index(i)];
        f[velocity_index(i)] = Vector3f(0, 0, 0);
    }
    
    for (i = 0; i < spring_forces.size(); i++)
    {
        Vector4f force = spring_forces[i];
        int particle1 = force[0];
        int particle2 = force[1];
        
        Vector3f diff = state[position_index(particle1)] - state[position_index(particle2)];
        float dist = diff.abs();
        Vector3f force1 = -1 * force[3] * (dist - force[2]) * diff.normalized();
        
        f[velocity_index(particle1)] = f[velocity_index(particle1)] + force1;
        f[velocity_index(particle2)] = f[velocity_index(particle2)] - force1;
    }
    
    for (i = 0; i < damping_forces.size(); i++)
    {
        int particle = damping_forces[i][0];
        force = -1 * damping_forces[i][1] * state[velocity_index(particle)];
        f[velocity_index(particle)] = f[velocity_index(particle)] + force;
    }
    
    for (i = 0; i < gravity_forces.size(); i++)
    {
        int particle = gravity_forces[i][0];
        force = masses[0] * gravity_forces[i].yzw();
        f[velocity_index(particle)] = f[velocity_index(particle)] + force;
    }
    
    for (i = 0; i < statics.size(); i++)
    {
        int particle = velocity_index(statics[i]);
        f[particle] = Vector3f(0.0, 0.0, 0.0);
    }
    
    return f;
}


// draw springs
void forceHandler::drawSystem(vector<Vector3f> state)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_LIGHTING);
    glLineWidth(3.0f); //customize settings on springs
    glPushMatrix();
    glBegin(GL_LINES);
    glColor4f(0.6f, 0.6f, 0.6f, 1.0f);
    
    for (int i = 0; i < spring_forces.size(); i++)
    {
        int particle1 = spring_forces[i][0];
        Vector3f pos1 = state[position_index(particle1)];
        glVertex3f(pos1[0], pos1[1], pos1[2]);

        int particle2 = spring_forces[i][1];
        Vector3f pos2 = state[position_index(particle2)];
        glVertex3f(pos2[0], pos2[1], pos2[2]);
    }
    glEnd();
    glPopMatrix();
    glPopAttrib();
}

