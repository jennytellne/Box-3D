//  Spring.hpp
// Class used to add the spring force and damper force of the two masses connected by the spring and
// damper. The class contains a function used to simulate the system.


#ifndef Spring_hpp
#define Spring_hpp

#include "Mass.hpp"
#include "TriangleSoup.hpp"

class SpringDamper {
public:
    
    Mass* mass1;            //One mass connected to the spring and damper
    Mass* mass2;            //Another mass connected to the spring and damper.
    Vector force;           //Vector to store the resulting force from the spring and damper acting on the masses
    float springConstant;   //The spring constant
    float springLength;     //The rest length of the spring
    float springMax;        //The maximum length of the spring
    float springMin;        //The minimum length of the spring
    float damperConstant;   //The damper constant
    float distance;         //The distance between two masses
    
    //Constructor
    SpringDamper(Mass* mass1, Mass* mass2, float springConstant, float springMax, float springMin, float springLength, float damperConstant);
    
    //Function to add the spring force and damper force to the Vector force
    void addSDForce();

    //Function to simulate the cubes position and velocity using the Euler method
    void simulateEuler(float dt);
    
};

#endif /* Spring_hpp */
