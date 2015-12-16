
#include "simpleSystem.h"
#include <iostream>

using namespace std;

SimpleSystem::SimpleSystem()
{
    // set up particle system
    m_numParticles = 0;
    vector<Vector3f> states(1);
    states[0] = Vector3f(1, 1, 1);
    setState(states);
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> SimpleSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f(state.size());
    
    for (unsigned i = 0; i < state.size(); i++)
    {
        f[i] = Vector3f(-1 * state[i][1], state[i][0], 0);
    }
    return f;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{
		Vector3f pos =  getState()[0]; //YOUR PARTICLE POSITION
        glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
}
