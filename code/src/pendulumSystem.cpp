#include "pendulumSystem.h"
#include "forceHandler.h"
#include <iostream>
using namespace std;


// for indexing
//int position_index(int i)
//{
//    return 2*i;
//}
//
//int velocity_index(int i)
//{
//    return (2*i) + 1;
//}
//

PendulumSystem::PendulumSystem(int numParticles):ParticleSystem(numParticles)
{
	m_numParticles = numParticles;
    vector<Vector3f> state_0(2*numParticles);
    
    forces = new forceHandler();
    
    // first pt fixed
    forces->staticParticle(0);
	
	// fill in code for initializing the state based on the number of particles
	for (int i = 0; i < m_numParticles; i++) {
		
		// for this system, we care about the position and the velocity
        masses.push_back(1.0f);
        
        // connect sequentially
        if (i != 0) {
            forces->spring(i-1, i, 0.25f, 2.0f);
            forces->gravity(i, Vector3f(0.0f, -0.5f, 0.0f));
            forces->damping(i, 0.2f);
        }
        
        state_0[position_index(i)] = Vector3f(0, -0.25*i, 0);
        state_0[velocity_index(i)] = Vector3f(0, 0, 0);
        
        if (i == m_numParticles - 1)
        {
            state_0[velocity_index(i)] = Vector3f(3, 0, 0);
        }
	}
    setState(state_0);
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> PendulumSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
	f = forces->differentiate(state, masses);
	return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw()
{
    vector<Vector3f> current_state = getState();
    
    // springs
    forces->drawSystem(current_state);
    
	for (int i = 0; i < m_numParticles; i++) {
		Vector3f pos = current_state[position_index(i)];//  position of particle i. YOUR CODE HERE
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
	}
}
