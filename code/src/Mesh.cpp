#include "Mesh.h"

using namespace std;
#define MAX_BUFFER_SIZE 0x10000
void Mesh::load( const char* filename )
{
	// // 2.1.1. load() should populate bindVertices, currentVertices, and faces

	for (int i=0; i < 42; i ++){
		for  (int j=0; j<42; j++){
			height[i][j]=-10;
		}
	}
	char buffer[MAX_BUFFER_SIZE];
	string s;
	char f1[1];
	Vector3f v;
	Vector3f colorTemp(0.5,0.5,0.5);
	unsigned int i1, i2, i3, i4, i5, i6,i7, i8, i9;
	ifstream in(filename);
	while(in.getline(buffer, MAX_BUFFER_SIZE)){
	 	stringstream ss(buffer);
	 	colors.push_back(colorTemp);
		ss >> s;	
		if(s=="v"){
			ss >> v[0] >> v[1] >> v[2];
			bindVertices.push_back(v);
			//cout <<v[1];
			if (v[1]>getFromObject(v[0],v[2])){
				//cout << v[0];
				set(v[0],v[2],v[1]);
			}
		}
		
		else if (s=="f"){
			Tuple3u tup;
		
				ss >> tup[0] >> tup[1] >>tup [2];
		
			faces.push_back(tup);
			
		}
	}



	// make a copy of the bind vertices as the current vertices
	currentVertices = bindVertices;

}	

void Mesh::draw()
{
	// Since these meshes don't have normals
	// be sure to generate a normal per triangle.
	// Notice that since we have per-triangle normals
	// rather than the analytical normals from
	// assignment 1, the appearance is "faceted".

//	glEnable( GL_COLOR_MATERIAL );
     glPushMatrix();
    
     glTranslatef(0,-8,-30);
    glScalef(2,2,2);
   glTranslatef(1,0,1);
	glBegin(GL_TRIANGLES);
	for (int i=0; i< faces.size();i++){
		Vector3f normalV= Vector3f::cross(currentVertices[faces[i][1]-1]-currentVertices[faces[i][0]-1],currentVertices[faces[i][2]-1]-currentVertices[faces[i][0]-1]);
			normalV=normalV.normalized();
			glColor3d(colors[faces[i][2]-1].x(),colors[faces[i][2]-1].y(),colors[faces[i][2]-1].z());
			for (int j=0; j< 3; j++){
				glNormal3d(normalV[0],normalV[1],normalV[2]);
				glVertex3d(currentVertices[faces[i][j]-1].x(),currentVertices[faces[i][j]-1].y(),currentVertices[faces[i][j]-1].z());
				glColor3d(0,1,0);
				
			}
	}
	glEnd();
	glPopMatrix();
	//le(GL_COLOR_MATERIAL);
}

void Mesh::loadAttachments( const char* filename, int numJoints )
{
	// 2.2. Implement this method to load the per-vertex attachment weights
	// this method should update m_mesh.attachments

	char buffer[MAX_BUFFER_SIZE];
	string s;
	float f;
	
	ifstream in(filename);
	while(in.getline(buffer, MAX_BUFFER_SIZE)){
	 	stringstream ss(buffer);
	 	vector<float> innerV;
	 	while (ss>> f){
	 		innerV.push_back(f);
	 	}
	 	attachments.push_back(innerV);			
			
	}

}
float Mesh::getFromObject(float x, float z){
	float x2= (x+1)*2+20;
	float z2= (z+1)*2+20;
	//cout << x2<<endl;
	if (x2<=0 || z2<=0){
		return -10;
	}

	return height[(int)x2][(int)z2];
}
void Mesh::set(float x, float z, float y){
	float x2= (x+1)*2+20;
	float z2= (z+1)*2+20;
	//cout << x2<<endl;
	if (!(x2<=0 || z2<=0)){		
		//cout<<y<<endl;
		for (int i= -2; i<=1;i++){
			for (int j =-2; j<= 1; j++)
			height[(int)x2+i][(int)z2+j]=2*y-8;
		}
	}
}
float Mesh::getFromSpace(float x, float z){
	return height[(int)x][(int)z];
}
