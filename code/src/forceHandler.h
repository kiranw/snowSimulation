//
//  forceHandler.h
//  
//
//  Created by Kiran Wattamwar on 10/24/15.
//
//

#ifndef FORCEHANDLER_h
#define FORCEHANDLER_h

#include <vector>
#include <vecmath.h>
#include <GL/glut.h>
#include <string>

using namespace std;

// manage the forces in a system separately, isolated for debugging
class forceHandler
{
public:
    forceHandler();
    
    void spring(int particle_1, int particle_2, float r, float k);
    void damping(int particle, float k);
    void gravity(int particle, Vector3f acceleration);
    
    void staticParticle(int particle);
    
    vector<Vector3f> differentiate(vector<Vector3f> state, vector<float> masses);
    
    void drawSystem(vector<Vector3f> state);
    
protected:
    
    vector<Vector2f> damping_forces; //drag/damping
    vector<Vector4f> gravity_forces; //acceleration from gravity
    vector<Vector4f> spring_forces; //springs
    vector<int> statics; //particles with no force
};

int position_index(int i);

int velocity_index(int i);

#endif /* forceHandler_h */
