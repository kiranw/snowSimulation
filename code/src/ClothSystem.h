#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include "extra.h"
#include <vector>

#include "particleSystem.h"
#include "forceHandler.h"

class ClothSystem: public ParticleSystem
{
///ADD MORE FUNCTION AND FIELDS HERE
public:
	ClothSystem();
	vector<Vector3f> evalF(vector<Vector3f> state);
	
	void draw();

private:
    int r;
    int c;
    vector<Vector3f> faces;
    forceHandler *forces;
    vector<float> masses;
};

// suggested in pset
int indexOf(int i, int j, int r, int c);

bool indexExists(int i, int j, int r, int c);

#endif
