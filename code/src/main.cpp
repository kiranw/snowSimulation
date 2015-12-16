//=========================================================
//  Snow Simulation
//=========================================================
//
//
//  Created by Kiran Wattamwar and Maria Karelina
//  6.837 - Computer Graphics 2015
//
//  Details on running in README and further information
//  in written report
//
//
//=========================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include <iostream>
#include "Particle.h"
#include "PerlinNoise.h"


using namespace std;



// Controls
#define N_PARTICLES 1000
#define FLOOR_X 42
#define FLOOR_Z 42
#define FLOOR -10

int num_particles = 1000;
float damping = 2000.0;
float zoom = -30.0;
bool wind = false;
int wind_direction = 1; // 1 = right, -1 = left
unsigned int i;

GLfloat light_pos[4] = {0,0,-1,0};
bool objInclude=true;


// For adjustable light positions
float light_x = 1.0;
float light_y = 34.0;
float light_increment = 0.5;


//vertices
float original_floor_mesh[FLOOR_X][FLOOR_Z][3];
float floor_mesh[FLOOR_X][FLOOR_Z][3];
float floor_mesh_copy[FLOOR_X][FLOOR_Z][3];

//normals
float normals[FLOOR_X][FLOOR_Z][3];

// color at floor
float snow_accumulation[FLOOR_X][FLOOR_Z][4];
float snow_accumulation_copy[FLOOR_X][FLOOR_Z][4];

float accum = -10.0;

Particle snowSystem[N_PARTICLES];


// for handling input from command line
void normal_keys(unsigned char key, int x, int y) {
    // toggle wind
    if (key == 'w')
    {
        if (wind) wind = false;
        else wind = true;
        cout << "toggling wind" << endl;
    }
    if (key == 'q')
    {
        cout << "toggling wind direction" << endl;
        wind_direction *= -1;
    }
}

// for handling input from command line
// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void special_keys( int key, int x, int y )
{
    switch ( key )
    {
        case GLUT_KEY_UP:
            light_y += light_increment;
            cout << "increasing light y " << endl;
            break;
        case GLUT_KEY_DOWN:
            light_y -= light_increment;
            cout << "decreasing light y " << endl;
            break;
        case GLUT_KEY_LEFT:
            light_x -= light_increment;
            cout << "increasing light x " << endl;
            break;
        case GLUT_KEY_RIGHT:
            light_x += light_increment;
            cout << "decreasing light x " << endl;
            break;
    }
    
    // this will refresh the screen so that the user sees the light position
    glutPostRedisplay();
}

// restore a particle number i and reset properties
void initParticles(int i) {
    snowSystem[i] = Particle();
}

// initialize particle system
void init( ) {
    int x, z;
    
    // makes snow look distributed
    glShadeModel(GL_SMOOTH);
    glClearColor(28.0/125.0, 35.0/125.0, 49.0/125.0, 0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    
    // floor mesh
    for (z = 0; z < FLOOR_Z; z++) {
        for (x = 0; x < FLOOR_X; x++) {
            floor_mesh[x][z][0] = x - 20.0;
            if (objInclude) floor_mesh[x][z][1] = 5*(float)perlinOctaveNoise(Vector3f(x/11.0f,1.3f,z/13.0f),2)-6;
            else floor_mesh[x][z][1] = accum;
            floor_mesh[x][z][2] = z - 20.0;
            
            original_floor_mesh[x][z][0] = x - 20.0;
            original_floor_mesh[x][z][1] = floor_mesh[x][z][1];
            original_floor_mesh[x][z][2] = z - 20.0;
            
            float m=3*(float)perlinOctaveNoise(Vector3f(x/13.0f,2.437f,z/7.0f),2);
            snow_accumulation[z][x][0] = 0.356*m+(1-m)*0.433; // r
            snow_accumulation[z][x][1] = 0.6454*m+(1-m)*0.56; // g
            snow_accumulation[z][x][2] = 0.252*m+(1-m)*0.222; // b -11 to -3
            snow_accumulation[z][x][3] = 0.0; // accumulation
        }
    }

    // particles
    for (i = 0; i < num_particles; i++) {
        snowSystem[i] = Particle();
    }
}

// account for wind by altering accumulation in wind's direction
void wind_smooth_accumulation() {
    // floor mesh
    int x, z, x1, x2;
    bool keep_going = true;
    if (wind)
    {
        for (z = 0; z < FLOOR_Z; z++) {
            for (x = 1; x < FLOOR_X; x++) {
                for (unsigned int j = 0; j < 4; j++) {
                    if (j != 4) {
                        floor_mesh_copy[x][z][j] = floor_mesh[x][z][j];
                    }
                    snow_accumulation_copy[z][x][j] = snow_accumulation[z][x][j];
                }
            }
        }
        for (x = 1; x < FLOOR_X - 1; x++) {
            //            keep_going = true;
            if (wind_direction == 1)
            {
                x1 = x;
                x2 = x + 1;
            }
            else
            {
                x1 = x + 1;
                x2 = x;
            }
            for (z = 0; z < FLOOR_Z; z++) {
                //                if (keep_going)
                {
                    if ((floor_mesh[x1][z][1] - original_floor_mesh[x1][z][1]) - (floor_mesh[x2][z][1] - original_floor_mesh[x2][z][1]) > 0.25) // how different does slope have to be
                    {
                        floor_mesh_copy[x1][z][1] -= 0.01; // transfer snow from one place to another
                        floor_mesh_copy[x2][z][1] += 0.01;
                        snow_accumulation_copy[z][x2][0] = min(1.0, 0.02 + snow_accumulation[z][x][0]); // adjust color
                        snow_accumulation_copy[z][x2][1] = min(1.0, 0.02 + snow_accumulation[z][x][1]); // adjust color
                        snow_accumulation_copy[z][x2][2] = min(1.0, 0.02 + snow_accumulation[z][x][2]);
                        snow_accumulation_copy[z][x2][3] += 0.02;
                        //                        keep_going = false;
                    }
                }
            }
        }
        
        for (z = 0; z < FLOOR_Z; z++) {
            for (x = 1; x < FLOOR_X; x++) {
                for (unsigned int j = 0; j < 4; j++) {
                    if (j != 4) {
                        floor_mesh[x][z][j] = floor_mesh_copy[x][z][j];
                    }
                    snow_accumulation[z][x][j] = snow_accumulation_copy[z][x][j];
                }
            }
        }
    }
}

//
void smooth_mesh() {
    float deltaGround;
    float deltaSnow;
    for (int z = 1; z < FLOOR_Z-2; z++) {
        for (int x = 1; x < FLOOR_X-2; x++) {
            for (int j = -1; j < 0; j++) {
                for (int i = -1; i < 0; i++){
                    deltaGround = original_floor_mesh[x-j][z-i][1] - original_floor_mesh[x][z][1];
                    deltaSnow = floor_mesh[x-j][z-i][1] - floor_mesh[x][z][1];
                    if (deltaSnow - deltaGround > 0.5){
                        floor_mesh[x-j][z-i][1]-=0.03;
                        floor_mesh[x][z][1]+=0.03;
                    }
                }
            }
        }
    }
}

// render particle system with OpenGl
void drawSystem() {
    float x, y, z;
    for (i = 0; i < num_particles; i=i+2) {
        if (snowSystem[i].alive == true) {
            x = snowSystem[i].xpos;
            y = snowSystem[i].ypos;
            z = snowSystem[i].zpos + zoom;
            
            // particles
            glColor4f(1.0, 1.0, 1.0, (rand()%20)/40.0+0.5);
            glPushMatrix();
            glTranslatef(x, y, z);
            float radius_jitter = (rand() % 20)/400.0;
            radius_jitter = 0;
            glutSolidSphere(0.08 + radius_jitter, 8, 8);
            glPopMatrix();
            
            // take step
            // translation and force integration
            snowSystem[i].ypos += snowSystem[i].velocity / (damping);
            snowSystem[i].xpos += (rand() % 20)/150.0 - 0.066;
            if (wind)
            {
                snowSystem[i].xpos += wind_direction * ((rand() % 20)/50.0 - 0.02);
            }
            snowSystem[i].zpos += (rand() % 20)/150.0 - 0.066;
            snowSystem[i].velocity += snowSystem[i].gravity;
            snowSystem[i].life -= snowSystem[i].fade;
            
            int zi = z - zoom + 20;
            int xi = x + 20;
            // accumulation
            if (snowSystem[i].ypos <= floor_mesh[xi][zi][1]) { // collision with ground
                snow_accumulation[zi][xi][0] = min(1.0, 0.25 + snow_accumulation[zi][xi][0]);
                snow_accumulation[zi][xi][1] = min(1.0, 0.25 + snow_accumulation[zi][xi][1]);
                snow_accumulation[zi][xi][2] = min(1.0, 0.25 + snow_accumulation[zi][xi][2]);
                snow_accumulation[zi][xi][3] += 0.25;
                if (snow_accumulation[zi][xi][3] > 1.0) {
                    floor_mesh[xi][zi][1] += 0.08;
                }
                snowSystem[i].life = -1.0;
            }
            
            // adjust accumulation for wind
            wind_smooth_accumulation();
            
            // smoothing
            smooth_mesh();
            
            // create new particle to replace old
            if (snowSystem[i].life < 0.0) {
                snowSystem[i] = Particle();
            }
        }
    }
}

void updateNormals() {
    int i, j;
    GLfloat Qx, Qy, Qz, Px, Py, Pz;
    for (i = 0; i+1 < FLOOR_X; i++) { // raised floor along z-y axis
        for (j = 0; j+1 < FLOOR_Z; j++) { // raised floor along x-y axis
            Qx = floor_mesh[j + 1][i][0]-floor_mesh[j][i][0];
            Qy = floor_mesh[j + 1][i][1]-floor_mesh[j][i][1];
            Qz = floor_mesh[j + 1][i][2]-floor_mesh[j][i][2];
            Px = floor_mesh[j + 1][i + 1][0]-floor_mesh[j][i][0];
            Py = floor_mesh[j + 1][i + 1][1]-floor_mesh[j][i][1];
            Pz = floor_mesh[j + 1][i + 1][2]-floor_mesh[j][i][2];
            normals[i][j][0] = Py*Qz - Pz*Qy;
            normals[i][j][1] = Pz*Qx - Px*Qz;
            normals[i][j][2] = Px*Qy - Py*Qx;
        }
    }
}

void displayInterpolatedNormal(int i, int j)
{
    if (i > 1 && i < 40 && j > 1 && j < 40)
    {
        float normal[3];
        normal[0] = (normals[i+1][j][0] + normals[i-1][j][0] + normals[i][j+1][0] + normals[i][j-1][0] + normals[i+1][j+1][0] + normals[i][j][0] + normals[i+1][j-1][0] + normals[i-1][j+1] [0]+ normals[i-1][j-1][0])/9.0;
        normal[1] = (normals[i+1][j][1] + normals[i-1][j][1] + normals[i][j+1][1] + normals[i][j-1][1] + normals[i+1][j+1][1] + normals[i][j][1] + normals[i+1][j-1][1] + normals[i-1][j+1] [1]+ normals[i-1][j-1][1])/9.0;
        normal[2] = (normals[i+1][j][2] + normals[i-1][j][2] + normals[i][j+1][2] + normals[i][j-1][2] + normals[i+1][j+1][2] + normals[i][j][2] + normals[i+1][j-1][2] + normals[i-1][j+1][2] + normals[i-1][j-1][2])/9.0;
        float len = normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2];
        len = pow(len, 0.5);
        normal[0]/= len;
        normal[1]/= len;
        normal[2]/= len;
        glNormal3fv(normal);
    }
    else
    {
        glNormal3fv(normals[i][j]);
    }
}

// draw the particle system in its current state
void drawScene( ) {
    int i, j;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Light color (RGBA)
    GLfloat Lt0diff[] = {0.97,1.0,1.0,1.0};
    // Light position
    GLfloat Lt0pos[] = {light_x, light_y, 5.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

    
    // raise floor mesh based on accumulation
    updateNormals();
    glColor3f(1.0, 0, 0);
    glBegin(GL_QUADS);
    int k = 0;
    for (i = 0; i+1 < FLOOR_X; i++) { // raised floor along z-y axis
        for (j = 0; j+1 < FLOOR_Z; j++) { // raised floor along x-y axis
            displayInterpolatedNormal(i,j);
            glColor3fv(snow_accumulation[i][j]);
            glVertex3f(floor_mesh[j][i][0], floor_mesh[j][i][1], floor_mesh[j][i][2] + zoom);
            displayInterpolatedNormal(i,j+1);
            glColor3fv(snow_accumulation[i][j + 1]);
            glVertex3f(floor_mesh[j + 1][i][0], floor_mesh[j + 1][i][1], floor_mesh[j + 1][i][2] + zoom);
            displayInterpolatedNormal(i+1,j+1);
            glColor3fv(snow_accumulation[i + 1][j + 1]);
            glVertex3f(floor_mesh[j + 1][i + 1][0], floor_mesh[j + 1][i + 1][1], floor_mesh[j + 1][i + 1][2] + zoom);
            displayInterpolatedNormal(i+1,j);
            glColor3fv(snow_accumulation[i + 1][j]);
            glVertex3f(floor_mesh[j][i + 1][0], floor_mesh[j][i + 1][1], floor_mesh[j][i + 1][2] + zoom);
        }
    }
    
    glEnd();
    drawSystem();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0)
    {
        h = 1;
    }
    
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(45, (float) w / (float) h, .1, 200);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void idle ( ) {
    glutPostRedisplay();
}

int main (int argc, char** argv) {
    glutInit(&argc, argv);
    cout << "6.837 Final Project 2015 - Kiran Wattamwar and Maria Karelina"<< endl;
    cout << " " << endl;
    cout << "General Controls:" << endl;
    cout << "w - toggle wind" << endl;
    cout << "q - toggle wind direction" << endl;
    cout << "left and right - adjust light's x-position" << endl;
    cout << "up and down - adjust light's y-position" << endl;
    cout << " " << endl;
    cout << " " << endl;
    cout << " " << endl;
    
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(640, 480);
    glutCreateWindow("6.837 Final Project 2015");
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0.
    glEnable(GL_COLOR_MATERIAL);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glNormal3f(0.0,0.0,1.0);
    init();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(normal_keys);
    glutSpecialFunc(special_keys);
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}
