//  Vector.hpp
// This class is used to create vectors to store coordinates in. The class is used to define coordinates and velocities.

#ifndef Vector_hpp
#define Vector_hpp


#include <iostream>
#include <cmath>

using namespace std;

class Vector {
public:
    float x; // the x value of the Vector
    float y; // the y value of the Vector
    float z; // the z value of the Vector
    
    //Constructors
    Vector();
    Vector(float x, float y, float z);
    //Function to return the length of the vector
    float length();

};

#endif /* Vector_hpp */
