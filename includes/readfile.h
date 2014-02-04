#pragma once

// Readfile definitions 
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "typeDefs.h"
#include "Scene.h"

using namespace std;

void rightmultiply(const mat4 & M, stack<mat4> &transfstack) 
{
    mat4 &T = transfstack.top(); 
    // Right multiply M, but do this left to account for row/column major 
    T = M * T; 
}

mat4 TranslationMatrix(float tx, float ty, float tz)
{
	return mat4(1.f, 0.f, 0.f, tx,
				0.f, 1.f, 0.f, ty,
				0.f, 0.f, 1.f, tz,
				0.f, 0.f, 0.f, 1.f);
}

mat4 ScalingMatrix(float sx, float sy, float sz)
{
	return mat4(sx, 0.f, 0.f, 0.f,
				0.f, sy, 0.f, 0.f,
				0.f, 0.f, sz, 0.f,
				0.f, 0.f, 0.f, 1.f);
}

mat4 RotationMatrix(float degrees, vec3 axis)
{
	vec3 ax = glm::normalize(axis);
	float a = degrees*pi/180.0;
	float c = cos(a);
	float s = sin(a);
	float x = ax.x;
	float y = ax.y;
	float z = ax.z;
	return mat4(c+x*x*(1-c), x*y*(1-c)-z*s, x*z*(1-c)+y*s, 0.f,
				x*y*(1-c)+z*s, c+y*y*(1-c), y*z*(1-c)-x*s, 0.f,
				z*x*(1-c)-y*s, z*y*(1-c)+x*s, c+z*z*(1-c), 0.f,
				0.f, 0.f, 0.f, 1.f);
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool readvals(stringstream &s, const int numvals, float* values) 
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i]; 
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n"; 
            return false;
        }
    }
    return true; 
}

void readfile(const char* filename, Scene* sc) 
{
    string str, cmd; 
    ifstream in;
    in.open(filename); 
    if (in.is_open()) {

        // I need to implement a matrix stack to store transforms.  
        // This is done using standard STL Templates 
        stack <mat4> transfstack; 
		int w,h;
		float fovy;
		float att_c = 1.0, att_l = 0.0, att_q = 0.0;
		int maxVerts = 0;
		int maxNVerts = 0;
		int maxD = 5;
		vector<Point> vertices;
		vector<vec3> indices;
		vector<Point> Nvertices;
		vector<vec3> Nnormals;
		vector<vec3> Nindices;
		BRDF current = BRDF();
		current.setAmbient(Color(0.2,0.2,0.2));
		current.setEmissive(Color(0.0,0.0,0.0));
		current.setDiffuse(Color(0.0,0.0,0.0));
		current.setSpecular(Color(0.0,0.0,0.0));
		current.setMirror(Color(0.0,0.0,0.0));
		current.setRefraction(Color(0.0,0.0,0.0));
		current.setShininess(0.0);
        transfstack.push(mat4(1.0));  // identity
        getline (in, str); 
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines 

                stringstream s(str);
                s >> cmd; 
                float values[10]; // Position and color for light, colors for others
                                    // Up to 10 params for cameras.  
                bool validinput; // Validity of input 

                if (cmd == "directional") {
                    validinput = readvals(s,6,values);
					if(validinput)
					{
						Color l = Color(values[3],values[4],values[5]);
						Point p = Point(values[0], values[1], values[2]);
						sc->addLight(new DirectionalLight(p,l));
					}
                }
				else if (cmd == "point") {
                    validinput = readvals(s,6,values);
					if(validinput)
					{
						Color l = Color(values[3],values[4],values[5]);
						Point p = Point(values[0], values[1], values[2]);
						sc->addLight(new PointLight(p,l,vec3(att_c, att_l, att_q)));
					}
                }


                else if (cmd == "ambient") {
                    validinput = readvals(s, 3, values); // colors 
                    if (validinput) {
						current.setAmbient(Color(values[0],values[1],values[2]));
                    }
                } else if (cmd == "diffuse") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
                        current.setDiffuse(Color(values[0],values[1],values[2]));
                    }
                } else if (cmd == "specular") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
                        current.setSpecular(Color(values[0],values[1],values[2]));
                    }
                } else if (cmd == "refraction") {
                    validinput = readvals(s, 4, values); 
                    if (validinput) {
						current.setRefraction(Color(values[0],values[1],values[2]));
						current.setTheta(values[3]);
                    }
                } else if (cmd == "emission") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
						current.setEmissive(Color(values[0],values[1],values[2]));
                    }
                } else if (cmd == "shininess") {
                    validinput = readvals(s, 1, values); 
                    if (validinput) {
						current.setShininess(values[0]);
                    }
                } else if(cmd == "attenuation")
				{
					validinput = readvals(s, 3, values); 
                    if (validinput) {
						att_c = values[0];
						att_l = values[1];
						att_q = values[2];
                    }
				}
				else if (cmd == "size") {
                    validinput = readvals(s,2,values); 
                    if (validinput) { 
                        w = (int) values[0]; h = (int) values[1]; 
                    } 
                } else if (cmd == "camera") {
                    validinput = readvals(s,10,values); // 10 values eye cen up fov
                    if (validinput) {
						glm::vec3 lookFrom = glm::vec3(values[0], values[1], values[2]);
						glm::vec3 lookTo = glm::vec3(values[3], values[4], values[5]);
						glm::vec3 UP = glm::vec3(values[6], values[7], values[8]);
						//glm::vec3 zVec = lookFrom-lookTo;

						fovy = values[9];
						//TO-DO
						Camera cam = Camera(lookFrom,lookTo,UP,fovy);
						cam.setViewPort(w,h);
						sc->setParameters(cam,maxD);
                    }
                }

				else if(cmd == "output")
				{
					string file;
					s>>file;
					sc->setOutput(file);
				}

				else if(cmd == "maxdepth")
				{
					validinput = readvals(s, 1, values); 
                    if (validinput) {
						maxD = (int)values[0];
					}
				}

                // I've left the code for loading objects in the skeleton, so 
                // you can get a sense of how this works.  
                // Also look at demo.txt to get a sense of why things are done this way.
                else if (cmd == "sphere") {
                    validinput = readvals(s, 4, values); 
                    if (validinput) {
                           Point p = Point(values[0],values[1],values[2]);
						   float r = values[3];
						   sc->addPrimitive(new SphereObj(p,r,Material(current), Transformation(transfstack.top())));
                    }
                }

				else if(cmd == "maxverts")
				{
					validinput = readvals(s, 1, values); 
                    if (validinput) {
						vertices.clear();
						indices.clear();
						maxVerts = (int)values[0];
					}
				}

				else if(cmd == "maxvertnorms")
				{
					validinput = readvals(s, 1, values); 
                    if (validinput) {
						Nvertices.clear();
						Nindices.clear();
						Nnormals.clear();
						maxNVerts = (int)values[0];
					}
				}

				else if(cmd == "vertex")
				{
					validinput = readvals(s, 3, values); 
                    if (validinput) {
						if(vertices.size()<maxVerts)
						{
							vertices.push_back(Point(values[0],values[1],values[2]));
						}
					}
				}

				else if(cmd == "tri")
				{
					validinput = readvals(s, 3, values); 
                    if (validinput) {
						int k1 = (int)values[0];
						int k2 = (int)values[1];
						int k3 = (int)values[2];
						Point p0 = vertices[k1];
						Point p1 = vertices[k2];
						Point p2 = vertices[k3];
						sc->addPrimitive(new TriangleObj(p0,p1,p2,Material(current), Transformation(transfstack.top())));
					}
				}


				else if(cmd == "vertexnormal")
				{
					validinput = readvals(s, 6, values); 
                    if (validinput) {
						if(Nvertices.size()<maxNVerts)
						{
							Nvertices.push_back(Point(values[0],values[1],values[2]));
							Nnormals.push_back(vec3(values[3],values[4],values[5]));
						}
					}
				}

				else if(cmd == "trinormal")
				{
					validinput = readvals(s, 3, values); 
                    if (validinput) {
						int k1 = (int)values[0];
						int k2 = (int)values[1];
						int k3 = (int)values[2];
						Point p0 = Nvertices[k1];
						Point p1 = Nvertices[k2];
						Point p2 = Nvertices[k3];
						vec3 n0 = Nnormals[k1];
						vec3 n1 = Nnormals[k2];
						vec3 n2 = Nnormals[k3];
						sc->addPrimitive(new TriangleNormalObj(p0,p1,p2,n0,n1,n2,Material(current), Transformation(transfstack.top())));
					}
				}


                else if (cmd == "translate") {
                    validinput = readvals(s,3,values); 
                    if (validinput) {
						mat4 M = TranslationMatrix(values[0], values[1], values[2]);
						rightmultiply(M, transfstack);

                    }
                }
                else if (cmd == "scale") {
                    validinput = readvals(s,3,values); 
                    if (validinput) {

						mat4 M = ScalingMatrix(values[0], values[1], values[2]);
						rightmultiply(M, transfstack);
                    }
                }
                else if (cmd == "rotate") {
                    validinput = readvals(s,4,values); 
                    if (validinput) {
						vec3 axis = vec3(values[0], values[1], values[2]);
						//axis = glm::normalize(axis);
						mat4 M = RotationMatrix(values[3], axis);
						rightmultiply(M, transfstack);

                    }
                }

                // I include the basic push/pop code for matrix stacks
                else if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top()); 
                } else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        cerr << "Stack has no elements.  Cannot Pop\n"; 
                    } else {
                        transfstack.pop(); 
                    }
                }

                else {
                    cerr << "Unknown Command: " << cmd << " Skipping \n"; 
                }
            }
            getline (in, str); 
        }
    } else {
        cerr << "Unable to Open Input Data File " << filename << "\n"; 
        throw 2; 
    }
}
