//  Mass.hpp
// This class creates a mass with a weight, position and velocity.

#ifndef Mass_hpp
#define Mass_hpp


#include "Vector.hpp"
#include "TriangleSoup.hpp"


class Mass {
public:
    float weight;           // The weight
    Vector position;		// Position in space
    Vector velocity;        // Velocity
    
    //Constructor
    Mass(float m);
    
    //Function used to set starting position of mass
    void setStartPos(float x, float y, float z);
    //Function used to set velocity of the mass
    void setVelocity(float vel_x, float vel_y, float vel_z);
    //Function used to set velocity in the x-direction of the mass
    void setVelocityX(float vel_x);
    //Function used to set velocity in the y-direction of the mass
    void setVelocityY(float vel_y);
    //Function used to set velocity in the z-direction of the mass
    void setVelocityZ(float vel_z);
    //Function used to add to the velocity in the x-direction of the mass
    void addVelocityX(float vel_x);
    //Function used to add to the velocity in the y-direction of the mass
    void addVelocityY(float vel_y);
    //Function used to add to the velocity in the z-direction of the mass
    void addVelocityZ(float vel_z);
    
};


#endif /* Mass_hpp */
