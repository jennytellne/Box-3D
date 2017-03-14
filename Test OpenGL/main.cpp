
/*
 * A C++ framework for OpenGL programming in TNM046 for MT1 2014.
 * This is a small, limited framework, designed to be easy to use
 * for students in an introductory computer graphics course in
 * the first year of a M Sc curriculum. It uses custom code
 * for some things that are better solved by external libraries
 * like GLEW and GLM, but the emphasis is on simplicity and
 * readability, not generality.
 * For the window management, GLFW 3.0 is used for convenience.
 * This code is dependent only on
 * the GLFW and OpenGL libraries. OpenGL 3.3 or higher is required.
 * Author: Stefan Gustavson (stegu@itn.liu.se) 2013-2014
 * This code is in the public domain.
 *
 * The code has been modified and further developed by Sofia Linevik 
 * and Jenny Tellne for a project in the course TNM085.
 */



// File and console I/O for logging and error reporting
#include <iostream>

// In MacOS X, tell GLFW to include the modern OpenGL headers.
// Windows does not want this, so we make this Mac-only.
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#endif

// GLFW 3.x, to handle the OpenGL window
#include <GLFW/glfw3.h>
#include <GLUT/glut.h>
#include "Utilities.hpp"
#include "Shader.hpp"
#include "Vector.hpp"
#include "Mass.hpp"
#include "SpringDamper.hpp"
#include "TriangleSoup.hpp"

using namespace std;

//Function to check if a mass is colliding with the object placed at -0.9 in the y-direction
bool checkCollision(Mass* mass) {
    if (mass->position.y <= -0.9f) {
        return false;
    }
    else {
        return true;
    }
}

// Multiplies two matrices and stores the result in Mout
void mat4mult(float M1[], float M2[], float Mout[]) {
    float Mtemp [16];
    Mtemp [0] = M1[0]*M2[0]+M1[4]*M2[1]+M1[8]*M2[2]+M1[12]*M2[3];
    Mtemp [1] = M1[1]*M2[0]+M1[5]*M2[1]+M1[9]*M2[2]+M1[13]*M2[3];
    Mtemp [2] = M1[2]*M2[0]+M1[6]*M2[1]+M1[10]*M2[2]+M1[14]*M2[3];
    Mtemp [3] = M1[3]*M2[0]+M1[7]*M2[1]+M1[11]*M2[2]+M1[15]*M2[3];
    
    Mtemp [4] = M1[0]*M2[4]+M1[4]*M2[5]+M1[8]*M2[6]+M1[12]*M2[7];
    Mtemp [5] = M1[1]*M2[4]+M1[5]*M2[5]+M1[9]*M2[6]+M1[13]*M2[7];
    Mtemp [6] = M1[2]*M2[4]+M1[6]*M2[5]+M1[10]*M2[6]+M1[14]*M2[7];
    Mtemp [7] = M1[3]*M2[4]+M1[7]*M2[5]+M1[11]*M2[6]+M1[15]*M2[7];
    
    Mtemp [8] = M1[0]*M2[8]+M1[4]*M2[9]+M1[8]*M2[10]+M1[12]*M2[11];
    Mtemp [9] = M1[1]*M2[8]+M1[5]*M2[9]+M1[9]*M2[10]+M1[13]*M2[11];
    Mtemp [10] = M1[2]*M2[8]+M1[6]*M2[9]+M1[10]*M2[10]+M1[14]*M2[11];
    Mtemp [11] = M1[3]*M2[8]+M1[7]*M2[9]+M1[11]*M2[10]+M1[15]*M2[11];
    
    Mtemp [12] = M1[0]*M2[12]+M1[4]*M2[13]+M1[8]*M2[14]+M1[12]*M2[15];
    Mtemp [13] = M1[1]*M2[12]+M1[5]*M2[13]+M1[9]*M2[14]+M1[13]*M2[15];
    Mtemp [14] = M1[2]*M2[12]+M1[6]*M2[13]+M1[10]*M2[14]+M1[14]*M2[15];
    Mtemp [15] = M1[3]*M2[12]+M1[7]*M2[13]+M1[11]*M2[14]+M1[15]*M2[15];
    
    for ( int i =0; i <16; i ++) {
        Mout[i] = Mtemp[i];
    }
}
//Function creates an identity matrix and stores it in M
void mat4identity(float M[]) {
    float Mtemp [16];
    Mtemp [0] = 1.0f;
    Mtemp [1] = 0.0f;
    Mtemp [2] = 0.0f;
    Mtemp [3] = 0.0f;
    
    Mtemp [4] = 0.0f;
    Mtemp [5] = 1.0f;
    Mtemp [6] = 0.0f;
    Mtemp [7] = 0.0f;
    
    Mtemp [8] = 0.0f;
    Mtemp [9] = 0.0f;
    Mtemp [10] = 1.0f;
    Mtemp [11] = 0.0f;
    
    Mtemp [12] = 0.0f;
    Mtemp [13] = 0.0f;
    Mtemp [14] = 0.0f;
    Mtemp [15] = 1.0f;
    
    for ( int i =0; i <16; i ++) {
        M[i] = Mtemp[i];
    }
};

//Function that rotates the matrix around the x-axis and stores the result in M
void mat4rotx(float M[], float angle){
    float a = cos(angle);
    float b = sin(angle);
    float Mtemp [16];
    Mtemp [0] = 1.0f;
    Mtemp [1] = 0.0f;
    Mtemp [2] = 0.0f;
    Mtemp [3] = 0.0f;
    
    Mtemp [4] = 0.0f;
    Mtemp [5] = a;
    Mtemp [6] = b;
    Mtemp [7] = 0.0f;
    
    Mtemp [8] = 0.0f;
    Mtemp [9] = -b;
    Mtemp [10] = a;
    Mtemp [11] = 0.0f;
    
    Mtemp [12] = 0.0f;
    Mtemp [13] = 0.0f;
    Mtemp [14] = 0.0f;
    Mtemp [15] = 1.0f;
    
    for ( int i =0; i <16; i++) {
        M[i] = Mtemp[i];
    }
    
};

//Function that rotates the matrix around the y-axis and stores the result in M
void mat4roty(float M[], float angle){
    float a = cos(angle);
    float b = sin(angle);
    float Mtemp [16];
    Mtemp [0] = a;
    Mtemp [1] = 0.0f;
    Mtemp [2] = -b;
    Mtemp [3] = 0.0f;
    
    Mtemp [4] = 0.0f;
    Mtemp [5] = 1.0f;
    Mtemp [6] = 0.0f;
    Mtemp [7] = 0.0f;
    
    Mtemp [8] = b;
    Mtemp [9] = 0.0f;
    Mtemp [10] = a;
    Mtemp [11] = 0.0f;
    
    Mtemp [12] = 0.0f;
    Mtemp [13] = 0.0f;
    Mtemp [14] = 0.0f;
    Mtemp [15] = 1.0f;
    
    for ( int i =0; i <16; i++) {
        M[i] = Mtemp[i];
    }
};

//Function that rotates the matrix around the z-axis and stores the result in M
void mat4rotz(float M[], float angle){
    float a = cos(angle);
    float b = sin(angle);
    float Mtemp [16];
    Mtemp [0] = a;
    Mtemp [1] = b;
    Mtemp [2] = 0.0f;
    Mtemp [3] = 0.0f;
    
    Mtemp [4] = -b;
    Mtemp [5] = a;
    Mtemp [6] = 0.0f;
    Mtemp [7] = 0.0f;
    
    Mtemp [8] = 0.0f;
    Mtemp [9] = 0.0f;
    Mtemp [10] = 1.0f;
    Mtemp [11] = 0.0f;
    
    Mtemp [12] = 0.0f;
    Mtemp [13] = 0.0f;
    Mtemp [14] = 0.0f;
    Mtemp [15] = 1.0f;
    
    for ( int i =0; i <16; i++) {
        M[i] = Mtemp[i];
    }
};

//Function that scales the matrix and stores the result in M
void mat4scale(float M[], float scale){
    float Mtemp [16];
    Mtemp [0] = scale;
    Mtemp [1] = 0.0f;
    Mtemp [2] = 0.0f;
    Mtemp [3] = 0.0f;
    
    Mtemp [4] = 0.0f;
    Mtemp [5] = scale;
    Mtemp [6] = 0.0f;
    Mtemp [7] = 0.0f;
    
    Mtemp [8] = 0.0f;
    Mtemp [9] = 0.0f;
    Mtemp [10] = scale;
    Mtemp [11] = 0.0f;
    
    Mtemp [12] = 0.0f;
    Mtemp [13] = 0.0f;
    Mtemp [14] = 0.0f;
    Mtemp [15] = 1.0f;
    
    for ( int i =0; i <16; i++) {
        M[i] = Mtemp[i];
    }
};
//Function that translates the matrix and stores the result in M
void mat4translate(float M[], float x, float y, float z){
    float Mtemp [16];
    Mtemp [0] = 1.0f;
    Mtemp [1] = 0.0f;
    Mtemp [2] = 0.0f;
    Mtemp [3] = 0.0f;
    
    Mtemp [4] = 0.0f;
    Mtemp [5] = 1.0f;
    Mtemp [6] = 0.0f;
    Mtemp [7] = 0.0f;
    
    Mtemp [8] = 0.0f;
    Mtemp [9] = 0.0f;
    Mtemp [10] = 1.0f;
    Mtemp [11] = 0.0f;
    
    Mtemp [12] = x;
    Mtemp [13] = y;
    Mtemp [14] = z;
    Mtemp [15] = 1.0f;
    
    for ( int i =0; i <16; i++) {
        M[i] = Mtemp[i];
    }
};

// This function creates a perspective matrix.
// M is the matrix we want to create (an output argument )
// vfov is the vertical field of view (in the y direction )
// aspect is the aspect ratio of the viewport ( width / height )
// znear is the distance to the near clip plane ( znear > 0)
// zfar is the distance to the far clip plane ( zfar > znear )
void mat4perspective ( float M [] , float vfov , float aspect , float znear , float zfar ) {
    
    float f = cos(vfov/2)/sin(vfov/2);
    float fa = f/aspect;
    float A = -(zfar+znear)/(zfar-znear);
    float B = -(2*zfar*znear)/(zfar-znear);
    
    float Mtemp [16];
    Mtemp [0] = fa;
    Mtemp [1] = 0.0f;
    Mtemp [2] = 0.0f;
    Mtemp [3] = 0.0f;
    
    Mtemp [4] = 0.0f;
    Mtemp [5] = f;
    Mtemp [6] = 0.0f;
    Mtemp [7] = 0.0f;
    
    Mtemp [8] = 0.0f;
    Mtemp [9] = 0.0f;
    Mtemp [10] = A;
    Mtemp [11] = -1.0f;
    
    Mtemp [12] = 0.0f;
    Mtemp [13] = 0.0f;
    Mtemp [14] = B;
    Mtemp [15] = 0.0f;
    
    for ( int i =0; i <16; i++) {
        M[i] = Mtemp[i];
    }
}

int main(int argc, char *argv[])
{
    //Declaration of the matrices
    GLfloat M1[16];
    GLfloat M2[16];
    GLfloat M3[16];
    GLfloat M4[16];
    GLfloat MV[16];
    GLfloat P[16];
    
    //Window size
    int width, height;
    
    //Objects - myBox as the modell and myFloor as a collision object
    TriangleSoup myBox;
    myBox.createBox(0.3f, 0.3f, 0.3f, 0.0f, 0.0f, 0.0f);
    TriangleSoup myFloor;
    myFloor.createBox(2.0f, 0.1f, 2.0f, 0.0f, -1.0f, 0.0f);
   
    //Constants
    float springConstant = 20.0f;
    float springConstDiag = 20.0f;
    float damperConstant = 2.0f;
    float damperConstDiag = 2.0f;
    
    float springLength = 0.3f;
    float springLengthDiag = sqrt(springLength*springLength*2);
    float springMax = 0.6f;
    float springMin = 0.03f;
    float springMaxDiag = sqrt(springMax*springMax*2);
    float springMinDiag = sqrt(springMin*springMin*2);
    
    int nSprings = 24;
    float weight = 2.0f;
    float dt = 0.0001f;
    
    //Array to store the masses of the box
    Mass** masses;
    masses = new Mass*[myBox.nverts];
    //Array to store the springs and dampers
    SpringDamper** springs_dampers;
    springs_dampers = new SpringDamper*[nSprings];
    
    //Give each mass a weight and set their starting positions to the positions defined for the box
    for(int i = 0; i < myBox.nverts; i++){
        masses[i] = new Mass(weight);
        masses[i]->setStartPos(myBox.vertexarray[8*i],myBox.vertexarray[8*i+1], myBox.vertexarray[8*i+2]);
    }
    
    const GLFWvidmode *vidmode;  // GLFW struct to hold information about the display
    GLFWwindow *window;    // GLFW struct to hold information about the window
    
    // Declarations : the C++ variable and the location of its GLSL counterpart
    float time = 0.0;
    GLint location_time;
    Shader myShader;
    
    // Initialise GLFW
    glfwInit();
    
    // Determine the desktop size
    vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    // Make sure we are getting a GL context of at least version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Exclude old legacy cruft from the context. We don't need it, and we don't want it.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // Open a square window (aspect 1:1) to fill the screen height
    window = glfwCreateWindow(vidmode->height*0.8, vidmode->height*0.8, "GLprimer", NULL, NULL);
    if (!window) {
        cout << "Unable to open window. Terminating." << endl;
        glfwTerminate(); // No window was opened, so we can't continue in any useful way
        return -1;
    }
    
    // Make the newly created window the "current context" for OpenGL
    // (This step is strictly required, or things will simply not work)
    glfwMakeContextCurrent(window);
    // Load extensions (only needed in Microsoft Windows)
    Utilities::loadExtensions();
    
    myShader.createShader("vertex.glsl", "fragment.glsl");
    
    // Show some useful information on the GL context
    cout << "GL vendor:       " << glGetString(GL_VENDOR) << endl;
    cout << "GL renderer:     " << glGetString(GL_RENDERER) << endl;
    cout << "GL version:      " << glGetString(GL_VERSION) << endl;
    cout << "Desktop size:    " << vidmode->width << "x" << vidmode->height << " pixels" << endl;
    
    
    glfwSwapInterval(0); // Do not wait for screen refresh between frames
    
    
    //For the shader
    location_time = glGetUniformLocation(myShader.programID, "time");
    if(location_time == -1) { // If the variable is not found , -1 is returned
        cout << " Unable to locate variable ✬time ✬ in shader !" << endl;
    }
    
    //To pass variables to the shader
    GLint location_M1;
    location_M1 = glGetUniformLocation(myShader.programID,"M1");
    glUseProgram(myShader.programID); // Activate the shader to set its variables
    
    GLint location_M2;
    location_M2 = glGetUniformLocation(myShader.programID,"M2");
    glUseProgram(myShader.programID); // Activate the shader to set its variables
    
    GLint location_MV;
    location_MV = glGetUniformLocation(myShader.programID,"MV");
    glUseProgram(myShader.programID); // Activate the shader to set its variables
    
    GLint location_P;
    location_P = glGetUniformLocation(myShader.programID,"P");
    glUseProgram(myShader.programID); // Activate the shader to set its variables
    
    // ------------------------------------------------- Main loop ----------------------------------------------------------------------- //
    while(!glfwWindowShouldClose(window)) {

        //glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // --- Insert this line into the rendering loop .
        Utilities :: displayFPS ( window );
        // Get window size. It may start out different from the requested
        // size, and will change if the user resizes the window.
        glfwGetWindowSize( window, &width, &height );
        // Set viewport. This is the pixel rectangle we want to draw into.
        glViewport( 0, 0, width, height ); // The entire window
        // Set the clear color and depth, and clear the buffers for drawing
        glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Activate the vertex array object we want to draw (we may have several)
        glUseProgram (myShader.programID);
        
        /********************************* SHADER AND CAMERA ******************************/
        //Initialize identity matrices
        mat4identity(M1);
        mat4identity(M2);
        mat4identity(M3);
        mat4identity(MV);
        mat4identity(P);
        mat4identity(M4);
        
        mat4perspective(P, (3.14/4), 1.0f, 0.1f, 100.0f);
        mat4mult(M4,P,P);
        
        mat4roty(M2,time*0.1);
        mat4mult(M2,MV,MV);
        
        mat4translate(M1, 0.0f, 0.0f, -0.5f);
        mat4mult(M1,MV,MV);
        
        mat4rotx(M4, 0.6f);
        mat4mult(M4,MV,MV);
        
        mat4translate(M1, 0.0f, 0.0f, -3.0f);
        mat4mult(M1,MV,MV);
        
        mat4scale(M3, 0.5);
        mat4mult(M1, M3, M1);
        
        //Update the values for the shader
        location_M1 = glGetUniformLocation(myShader.programID,"M1");
        glUseProgram(myShader.programID); // Activate the shader to set its variables
        glUniformMatrix4fv(location_M1, 1, GL_FALSE,M1); // Copy the value
        
        location_M2 = glGetUniformLocation(myShader.programID,"M2");
        glUseProgram(myShader.programID); // Activate the shader to set its variables
        glUniformMatrix4fv(location_M2, 1, GL_FALSE,M2); // Copy the value
        
        location_MV = glGetUniformLocation(myShader.programID,"MV");
        glUseProgram(myShader.programID); // Activate the shader to set its variables
        glUniformMatrix4fv(location_MV, 1, GL_FALSE,MV); // Copy the value
        
        location_P = glGetUniformLocation(myShader.programID,"P");
        glUseProgram(myShader.programID); // Activate the shader to set its variables
        glUniformMatrix4fv(location_P, 1, GL_FALSE,P); // Copy the value
        
        /********************************* SHADER AND CAMERA ******************************/
        
        // ----------------------------------------------- Springs and dampers ---------------------------------------------------------- //
        //x-direction
        springs_dampers[0] = new SpringDamper(masses[0], masses[1], springConstant, springMax, springMin, springLength, damperConstant);
        springs_dampers[1] = new SpringDamper(masses[2], masses[3], springConstant, springMax, springMin, springLength, damperConstant);
        springs_dampers[2] = new SpringDamper(masses[4], masses[5], springConstant, springMax, springMin, springLength, damperConstant);
        springs_dampers[3] = new SpringDamper(masses[6], masses[7], springConstant, springMax, springMin, springLength, damperConstant);
        //y-dimension
        springs_dampers[4] = new SpringDamper(masses[0], masses[2], springConstant, springMax, springMin, springLength, damperConstant);
        springs_dampers[5] = new SpringDamper(masses[1], masses[3], springConstant, springMax, springMin, springLength, damperConstant);
        springs_dampers[6] = new SpringDamper(masses[4], masses[6], springConstant, springMax, springMin, springLength, damperConstant);
        springs_dampers[7] = new SpringDamper(masses[5], masses[7], springConstant, springMax, springMin, springLength, damperConstant);
        //z-dimension
        springs_dampers[8] = new SpringDamper(masses[0], masses[4], springConstant, springMax, springMin, springLength, damperConstant);
        springs_dampers[9] = new SpringDamper(masses[1], masses[5], springConstant, springMax, springMin, springLength, damperConstant);
        springs_dampers[10] = new SpringDamper(masses[2], masses[6], springConstant, springMax, springMin, springLength, damperConstant);
        springs_dampers[11] = new SpringDamper(masses[3], masses[7], springConstant, springMax, springMin, springLength, damperConstant);
        
        //Diagonals xy-plane
        springs_dampers[12] = new SpringDamper(masses[0], masses[3], springConstDiag,springMinDiag, springMaxDiag, springLengthDiag, damperConstDiag);
        springs_dampers[13] = new SpringDamper(masses[1], masses[2], springConstDiag,springMinDiag, springMaxDiag, springLengthDiag, damperConstDiag);
        springs_dampers[14] = new SpringDamper(masses[4], masses[7], springConstDiag,springMinDiag, springMaxDiag, springLengthDiag, damperConstDiag);
        springs_dampers[15] = new SpringDamper(masses[5], masses[6], springConstDiag,springMinDiag, springMaxDiag, springLengthDiag, damperConstDiag);
        //Diagonals xz-plane
        springs_dampers[16] = new SpringDamper(masses[0], masses[5], springConstDiag,springMinDiag, springMaxDiag, springLengthDiag, damperConstDiag);
        springs_dampers[17] = new SpringDamper(masses[1], masses[4], springConstDiag,springMinDiag, springMaxDiag, springLengthDiag, damperConstDiag);
        springs_dampers[18] = new SpringDamper(masses[2], masses[7], springConstDiag,springMinDiag, springMaxDiag, springLengthDiag, damperConstDiag);
        springs_dampers[19] = new SpringDamper(masses[3], masses[6], springConstDiag,springMinDiag, springMaxDiag, springLengthDiag, damperConstDiag);
        //Diagonals yz-plane
        springs_dampers[20] = new SpringDamper(masses[0], masses[6], springConstDiag,springMinDiag, springMaxDiag, springLengthDiag, damperConstDiag);
        springs_dampers[21] = new SpringDamper(masses[2], masses[4], springConstDiag,springMinDiag, springMaxDiag, springLengthDiag, damperConstDiag);
        springs_dampers[22] = new SpringDamper(masses[1], masses[7], springConstDiag,springMinDiag, springMaxDiag, springLengthDiag, damperConstDiag);
        springs_dampers[23] = new SpringDamper(masses[3], masses[5], springConstDiag,springMinDiag, springMaxDiag, springLengthDiag, damperConstDiag);
        
        // ------------ Add forces to masses -------------- //
        // --------- Simulate masses using Euler --------- //
        for(int i = 0; i<nSprings; i++) {
            springs_dampers[i]->addSDForce();
            springs_dampers[i]->simulateEuler(dt);
        }
 
        // --------- Update positions of vertices ---------- //
        // ------------- Check for collision -------------- //
        for(int i = 0; i < myBox.nverts; i++){
            //Update position of the vertices of the box with the new simulated positions of the masses
            myBox.updateVertexArray(8*i, masses[i]->position.x, masses[i]->position.y, masses[i]->position.z);
            if (!checkCollision(masses[i])){
                //If the mass collides with the floor, change direction of the velocity in the y-direction
                //Due to energy loss the resulting velocity will have a smaller amplitude
                masses[i]->setVelocityY(-0.9*masses[i]->velocity.y);
            }
        }
        
        // ---------- Bind buffers and render objects --------- //
        myBox.generateVAO();
        myBox.render();
        myFloor.generateVAO();
        myFloor.render();
        
        // Swap buffers, i.e. display the image and prepare for next frame.
        glfwSwapBuffers(window);
        // Poll events (read keyboard and mouse input)
        glfwPollEvents();
        // Exit if the ESC key is pressed (and also if the window is closed).
        if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        
        // Do this in the rendering loop to update the uniform variable " time "
        time = (float)glfwGetTime(); // Number of seconds since the program was started
        glUseProgram(myShader.programID); // Activate the shader to set its variables
        glUniform1f(location_time, time); // Copy the value to the shader program
    }
    
    // Close the OpenGL window and terminate GLFW.
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}




    
