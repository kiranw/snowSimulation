#ifndef MESH_H
#define MESH_H

#include <vector>
#include <vecmath.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include "extra.h"
#include "tuples.h"

typedef tuples< unsigned, 3 > Tuple3u;

struct Mesh
{
	// list of vertices from the OBJ file
	// in the "bind pose"
	std::vector< Vector3f > bindVertices;

	// each face has 3 indices
	// referencing 3 vertices
	std::vector< Tuple3u > faces;

	// current vertex positions after animation
	std::vector< Vector3f > currentVertices;
	//list of colors 
	std::vector<Vector3f> colors;
	// list of vertex to joint attachments
	// each element of attachments is a vector< float > containing
	// one attachment weight per joint
	std::vector< std::vector< float > > attachments;

	// 2.1.1. load() should populate bindVertices, currentVertices, and faces
	void load(const char *filename);

	// 2.1.2. draw the current mesh.
	void draw();

	float getFromObject(float x, float z);
	float getFromSpace(float x, float z);
	void set(float x, float z, float y);
	// 2.2. Implement this method to load the per-vertex attachment weights
	// this method should update m_mesh.attachments
	void loadAttachments( const char* filename, int numJoints );

		float height[42][42]={
  {-10, -10},
  {-10, -10}
};
private:

};

#endif
