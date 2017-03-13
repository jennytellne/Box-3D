
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

//Function to check if a mass is colliding with the object placed at -0.8 in the y-direction
bool checkCollision(Mass* mass) {
    if (mass->position.y <= -0.8f) {
        return false;
    }
    else {
        return true;
    }
}

int main(int argc, char *argv[])
{
    int width, height;
    GLuint colorBufferID ; // Vertex colors
    
    //Objects - myBox as the modell and myFloor as a collision object
    TriangleSoup myBox;
    myBox.createBox(0.2f, 0.2f, 0.2f, 0.0f, 0.0f, 0.0f);
    TriangleSoup myFloor;
    myFloor.createBox(1.0f, 0.2f, 0.1f, 0.0f, -1.0f, 0.0f);
   
    //Constants
    float springConstant = 20.0f;
    float springConstDiag = 20.0f;
    float damperConstant = 5.0f;
    float damperConstDiag = 5.0f;
    
    float springLength = 0.2f;
    float springLengthDiag = sqrt(springLength*springLength*2);
    float springMax = 0.6f;
    float springMin = 0.08f;
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
    }
    
    // Close the OpenGL window and terminate GLFW.
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}




    
