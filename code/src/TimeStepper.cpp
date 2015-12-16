#include "TimeStepper.h"

///TODO: implement Explicit Euler time integrator here
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    vector<Vector3f> state_0 = particleSystem->getState();
    
    // deriv on current state
    vector<Vector3f> d = particleSystem->evalF(state_0);
    
    // update
    vector<Vector3f> state_1(state_0.size());
    for (unsigned i = 0; i < state_1.size(); i++)
    {
        state_1[i] = state_0[i] + (stepSize * d[i]);
    }
    
    particleSystem->setState(state_1);
}

///TODO: implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    vector<Vector3f> state_0 = particleSystem->getState();
    
    // deriv on current state
    vector<Vector3f> d_0 = particleSystem->evalF(state_0);
    
    // update last state
    vector<Vector3f> state_1(state_0.size());
    for (unsigned i = 0; i < state_1.size(); i++)
    {
        state_1[i] = state_0[i] + (stepSize * d_0[i]);
    }
    
    // deriv on state_1
    vector<Vector3f> d_1 = particleSystem->evalF(state_1);
    
    // final update
    vector<Vector3f> state_final(state_0.size());
    for (unsigned j = 0; j < state_final.size(); j++)
    {
        state_final[j] = state_0[j] + (stepSize * (d_0[j] + d_1[j]) / 2);
    }
    
    particleSystem->setState(state_final);
}
