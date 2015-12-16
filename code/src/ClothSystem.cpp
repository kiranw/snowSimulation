#include "ClothSystem.h"
#include "forceHandler.h"
#include <iostream>



int indexOf(int i, int j, int r, int c)
{
    return i * c + j;
}

bool indexExists(int i, int j, int r, int c)
{
    if (i > r-1 || j > c-1 || i < 0 || j < 0) {
        return false;
    }
    return true;
}


//TODO: Initialize here
ClothSystem::ClothSystem()
{
    r = 10;
    c = 6;
    m_numParticles = r * c;
    unsigned i;
    unsigned j;
    
    // tuning the springs
    const float length = 0.25f;
    const float STRUCT = 20.0f;
    const float SHEAR = 20.0f;
    const float FLEX = 20.0f;
    const float VISCOUS_DRAG = 0.8f;
    const float G = -0.2f;
    
    vector<Vector3f> state_0(2 * m_numParticles);
    forces = new forceHandler();
    
    // fix the top corner particles
    forces->staticParticle(indexOf(0, 0, r, c));
    forces->staticParticle(indexOf(0, c-1, r, c));
    
    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
        {
            int index = indexOf(i, j, r, c);

            masses.push_back(1.0f);
            forces->damping(index, VISCOUS_DRAG);
            forces->gravity(index, Vector3f(0.0f, G, 0.0f));
            
            // STRUCTURAL SPRING
            if (indexExists(i, j+1, r, c)) // right
            {
                forces->spring(index, indexOf(i, j+1, r, c), length, STRUCT);
            }
            if (indexExists(i+1, j, r, c)) // bottom
            {
                forces->spring(index, indexOf(i+1, j, r, c), length, STRUCT);
            }
            
            
            // SHEAR SPRING
            if (indexExists(i-1, j+1, r, c)) // top right
            {
                forces->spring(index, indexOf(i-1, j+1, r, c), sqrt(2) * length, SHEAR);
            }
            if (indexExists(i+1, j+1, r, c)) // bottom right
            {
                forces->spring(index, indexOf(i+1, j+1, r, c), sqrt(2) * length, SHEAR);
            }
            
            // FLEX SPRING
            if (indexExists(i, j+2, r, c)) // 2 rights away
            {
                forces->spring(index, indexOf(i, j+2, r, c), 2 * length, FLEX);
            }
            if (indexExists(i+2, j, r, c)) // 2 bottoms away
            {
                forces->spring(index, indexOf(i+2, j, r, c), 2 * length, FLEX);
            }
            
            // set the initial position and velocity of this particle
            state_0[position_index(index)] = Vector3f(-2+0.25f*j, 2-0.25f*i, 0);
            state_0[velocity_index(index)] = Vector3f(0, 0, 0);
            
            // neeed to debug when theres time, probably something really stupid
            // FACES
//            Vector3f face1 = Vector3f((float)((i+1)*m_numParticles + (j+1)),
//                                      (float)((i)  *m_numParticles + (j+1)),
//                                      (float)((i+1)*m_numParticles + (j)  ));
//            faces.push_back(face1);
//            
//            Vector3f face2 = Vector3f((float)((i)  *m_numParticles + (j+1)),
//                                      (float)((i)  *m_numParticles + (j)  ),
//                                      (float)((i+1)*m_numParticles + (j)  ));
//            faces.push_back(face2);
//            Vector3f face3 = Vector3f((float)((i+1)*m_numParticles + (j+1)),
//                                      (float)((i+1)*m_numParticles + (j)  ),
//                                      (float)((i)  *m_numParticles + (j+1)  ));
//            faces.push_back(face3);
//            
//            Vector3f face4 = Vector3f((float)((i)  *m_numParticles + (j+1)),
//                                      (float)((i+1)*m_numParticles + (j)  ),
//                                      (float)((i)  *m_numParticles + (j)  ));
//            faces.push_back(face4);
        }
    }
    
    setState(state_0);

    
}


// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state)
{
    vector<Vector3f> f = forces->differentiate(state, masses);
    return f;
}


///TODO: render the system (ie draw the particles)
void ClothSystem::draw()
{
    vector<Vector3f> state = getState();
    for (int i = 0; i < m_numParticles; i++)
    {
        Vector3f pos = state[position_index(i)];
        glPushMatrix();
        glTranslatef(pos[0], pos[1], pos[2] );
        glutSolidSphere(0.075f,10.0f,10.0f);
        glPopMatrix();
    }
    
//    for (unsigned int f = 0; f < faces.size(); f++){
//        Vector3f face = faces[f];
//        Vector3f v1 = getState()[2*(int)face[0]];
//        Vector3f v2 = getState()[2*(int)face[1]];
//        Vector3f v3 = getState()[2*(int)face[2]];
//        Vector3f n = Vector3f::cross(v2-v1, v3-v1).normalized();
//        glBegin(GL_TRIANGLES);
//        glNormal3f(n[0], n[1], n[2]);
//        glVertex3f(v1[0], v1[1], v1[2]);
//        glVertex3f(v2[0], v2[1], v2[2]);
//        glVertex3f(v3[0], v3[1], v3[2]);
//        glEnd();
//    }
    
    // DRAW STRUCTURAL SPRINGS
    forces->drawSystem(state);
}

