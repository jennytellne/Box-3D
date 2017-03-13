//  Mass.cpp
// This class creates a mass with a weight, position and velocity.

#include "Mass.hpp"

//Constructor
Mass::Mass(float weight){
    this->weight = weight;
    this->velocity.x = 0;
    this->velocity.y = 0;
    this->velocity.z = 0;
}

//Function used to set starting position of mass
void Mass::setStartPos(float x, float y, float z){
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
    
}
//Function used to set velocity of the mass
void Mass::setVelocity(float vel_x, float vel_y, float vel_z){
    this->velocity.x = vel_x;
    this->velocity.y = vel_y;
    this->velocity.z = vel_z;
}
//Function used to set velocity in the x-direction of the mass
void Mass::setVelocityX(float vel_x){
    this->velocity.x = vel_x;
}
//Function used to set velocity in the y-direction of the mass
void Mass::setVelocityY(float vel_y){
    this->velocity.y = vel_y;
}
//Function used to set velocity in the z-direction of the mass
void Mass::setVelocityZ(float vel_z){
    this->velocity.z = vel_z;
}
//Function used to add to the velocity in the x-direction of the mass
void Mass::addVelocityX(float vel_x){
    this->velocity.x += vel_x;
}
//Function used to add to the velocity in the y-direction of the mass
void Mass::addVelocityY(float vel_y){
    this->velocity.y += vel_y;
}
//Function used to add to the velocity in the z-direction of the mass
void Mass::addVelocityZ(float vel_z){
    this->velocity.z += vel_z;
}

