// Class Camera Manager
// Contain the collection of cameras of a scene
// author Francisco Diaz @FMGameDev

#include <Vector>
#include "Camera.h"

using namespace std;

#pragma once
class CameraManager
{
public:
	// constructor
	CameraManager(SharedContext* shared_context);

	// destructor
	~CameraManager();

	// For switching between cameras 
	// and handle the input for the current camera
	void handleInput(float dt);

	// update the current camera
	void update(float dt);

	// return the position of the current camera
	Vector3 getCurrentCameraPosition();

	// return the look at point of the current camera
	Vector3 getCurrentCameraLookAt();

	// return the "up" vector of the current camera
	Vector3 getCurrentCameraUp();

	// return the fov value of the current camera
	float getCurrentCameraFov();

	// return the identifier of the current camera
	int getCurrentCameraId();

	// link an object to a camera by its id, it can be used for a tracking camera type for example
	void linkObjToCamera(int cam_id, BaseMesh* object, Vector3 translation_to_obj_eye = {0.0f,0.0f,0.0f});


private:
	// shared context component (input, window size etc)
	SharedContext* shared_context_;

	// camera is being currently showed
	Camera* current_camera_;

	// collection of vectors
	vector<Camera*> cameras_;
};

