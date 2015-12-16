#ifndef PENDULUMSYSTEM_H
#define PENDULUMSYSTEM_H

#include "extra.h"
#include <vector>
#include "particleSystem.h"
#include "forceHandler.h"

class PendulumSystem: public ParticleSystem
{
public:
	PendulumSystem(int numParticles);
	
	vector<Vector3f> evalF(vector<Vector3f> state);
    
	void draw();
    
protected:
    forceHandler *forces;
    vector<float> masses;
	
};

int position_index(int i);
int velocity_index(int i);

#endif
