//  Vector.cpp
// This class is used to create vectors to store coordinates in. The class is used to define coordinates and velocities.


#include "Vector.hpp"

//Default constructor
Vector::Vector(){
    x = 0;
    y = 0;
    z = 0;
}

//Constructor
Vector::Vector(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
}

//Function to return the length of the vector
float Vector::length(){
    return (float) sqrt(x * x + y * y + z * z);
}



