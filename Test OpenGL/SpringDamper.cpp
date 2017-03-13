//  Spring.cpp
// Class used to create and add the spring force and damper force of the two masses connected by the spring and
// damper.


#include "SpringDamper.hpp"

//Constructor
SpringDamper::SpringDamper(Mass* mass1, Mass* mass2, float springConstant, float springMax, float springMin, float springLength, float damperConstant){
    
    this->springConstant = springConstant;
    this->springLength = springLength;
    this->springMax = springMax;
    this->springMin = springMin;
    this->damperConstant = damperConstant;
    this->mass1 = mass1;
    this->mass2 = mass2;
    
}

//The function adds the spring and damper forces to the Vector force
void SpringDamper::addSDForce(){
    
    // Vector between the two masses
    Vector springVector;
    springVector.x = mass1->position.x - mass2->position.x;
    springVector.y = mass1->position.y - mass2->position.y;
    springVector.z = mass1->position.z - mass2->position.z;
    
    //The distance betwee the two masses
    distance = springVector.length();

    // The Spring Force is Added To the Force
    force.x -= springConstant * (distance - springLength) * (springVector.x / distance);
    force.y -= springConstant * (distance - springLength) * (springVector.y / distance);
    force.z -= springConstant * (distance - springLength) * (springVector.z / distance);
    
    //The Damping Force is added
    force.x -= (mass1->velocity.x - mass2->velocity.x) * damperConstant;
    force.y -= (mass1->velocity.y - mass2->velocity.y) * damperConstant;
    force.z -= (mass1->velocity.z - mass2->velocity.z) * damperConstant;
    
}

//The function uses the force to simulate the new velocities and positions of the masses connected to the spring and damper
//using the Euler method
void SpringDamper::simulateEuler(float dt){
    //Velocity and position of the first mass
    mass1->velocity.x += (force.x / mass1->weight) * dt;
    mass1->velocity.y += ((force.y-9.82*mass1->weight) / mass1->weight) * dt; //Gravity is added to the y-component
    mass1->velocity.z += (force.z / mass1->weight) * dt;
    mass1->position.x += mass1->velocity.x * dt;
    mass1->position.y += mass1->velocity.y * dt;
    mass1->position.z += mass1->velocity.z * dt;
    //Velocity and position of the second mass
    mass2->velocity.x -= (force.x / mass2->weight) * dt;
    mass2->velocity.y -= ((force.y+9.82*mass2->weight) / mass2->weight) * dt; //Gravity is added to the y-component
    mass2->velocity.z -= (force.z / mass2->weight) * dt;
    mass2->position.x += mass2->velocity.x * dt;
    mass2->position.y += mass2->velocity.y * dt;
    mass2->position.z += mass2->velocity.z * dt;
    
}



