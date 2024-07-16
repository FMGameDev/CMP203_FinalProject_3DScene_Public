// Scene class. Configures a basic 3D scene.
// Interfaces with the Input class to handle user input
// Calculates and outputs Frames Per Second (FPS) rendered.
// Important functions are the constructor (initialising the scene), 
// update (for process user input and updating scene objects) and render (renders scene).
// The class scene has been proportioned by Paul Robertson and modified by Francisco Diaz (@FmGameDev) adding the different objects to the scene

#ifndef _SCENE_H
#define _SCENE_H

// Include GLUT, openGL, input.
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "SharedContext.h"
#include <stdio.h>
// Further includes should go here:
#include "SOIL.h"
#include <vector>

// include all the own meshes
#include "MeshSphere.h"
#include "MeshPlane.h"
#include "MeshCube.h"
#include "MeshCone.h"
#include "MeshTorus.h"
// model
#include "Model.h"
// shadow
#include "Shadow.h"
#include "MeshMirrorWorld.h"

// others
#include "CameraManager.h"
#include "LightManager.h"
#include "Material.h"

#include <unordered_map>

using namespace std;

// enum with all the meshes/models of this class, it is used to easily identify and access to the right mesh
enum class MeshesType
{
	kSphere,
	kFloor,
	kCube,
	kCone,
	kCylinder,
	kPyramid,
	kPentagonal,
	kHexagonal,
	kOctagonal,
	kTorus,
	kSword,
	kSpaceship,
	kSpaceship2,
	kWallBack,
	kWallRight,
	kPlaneMirror,
	kDiscMirror
};

// enum with all the textures names of this class, it is used to easily identify and access to the right texture
enum class TextureName
{
	kDiceMap,
	kEarth,
	kDonut,
	kDarkGrayWood,
	kMetal,
	kSword,
	kBronzeSword,
	kSpaceship,
	kSpaceship2,
};

class Scene{

public:
	// constructor
	Scene(SharedContext *shared_context);
	// destructor
	~Scene();
	// Main render function
	void render();
	// Handle input function that receives delta time from parent.
	void handleInput(float dt);
	// Update function receives delta time from parent (used for frame independent updating).
	void update(float dt);
	// Resizes the OpenGL output based on new window size.
	void resize(int w, int h);

protected:
	// configure opengl render pipeline
	void initialiseOpenGL();
	// initiliase the meshes of this scene
	void initialiseMeshes();
	// initiliase the textures for the scene
	void initialiseTextures();
	// initialise material for walls
	void initialiseMaterials();

	// Renders text (x, y positions, RGB colour of text, string of text to be rendered)
	void displayText(float x, float y, float r, float g, float b, char* string);
	// A function to collate all text output in a single location
	void renderTextOutput();
	void calculateFPS();

	// For access to user input.
	SharedContext *shared_context_;
		
	// For Window and frustum calculation.
	//int width, height;
	float nearPlane, farPlane;

	// component to stop updating the scene
	bool paused = false;

	// For FPS counter and mouse coordinate output.
	int frame = 0, time, timebase = 0;
	char fps[40];
	char mouseText[40];
	char cameraText[40]; // text to print the id of the camera is being used
	char pausedText[40] = " PAUSED"; // text to print the id of the camera is being used

	// camera and light managers
	CameraManager* camera_mgr_;
	LightManager* light_mgr_;
	Material* metal_material_;

	// collection of the shapes/meshes I have created
	unordered_map<MeshesType, BaseMesh*> my_geometry_; // meshes created by me (not loaded from a model)
	unordered_map<MeshesType, MeshPlane*> floor_and_walls_; // collection of planes which we will use for printing the shadows of the rest of meshes/models 
	unordered_map<MeshesType, MeshMirrorWorld*> mirror_worlds_;

	// collection of models (meshes loaded from a file)
	unordered_map<MeshesType, Model*> models_;

	// collection of textures, each scene have different textures in that all the scenes have the same textures them this object would be passed throught the SharedContext
	unordered_map<TextureName, Texture*> textures;

};

#endif