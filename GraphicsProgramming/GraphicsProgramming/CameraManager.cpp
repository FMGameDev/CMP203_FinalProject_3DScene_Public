#include "CameraManager.h"

CameraManager::CameraManager(SharedContext* shared_context)
	: shared_context_(shared_context)
{

	// create the floating/main camera
	cameras_.push_back(new Camera(1, shared_context_)); // id of the camera is the vector position + 1
	cameras_.back()->setPosition({ 10, 2, -1 });
	cameras_.back()->setRotationSpeed(80.0f);
	cameras_.back()->setMovementSpeed(80.0f);
	cameras_.back()->setZoomSpeed(400.0f);
	cameras_.back()->setPitchLimits(AngleLimits(89.0f, -89.0f));
	cameras_.back()->setYawLimits(AngleLimits(360.0f, -360.0f));
	cameras_.back()->setCameraType(CameraType::kFloating);

	// create the right front corner camera (simulate a security camera)
	cameras_.push_back(new Camera(2, shared_context_)); // id of the camera is the vector position + 1
	cameras_.back()->setPosition({19.5, 3, -1});
	cameras_.back()->setRotationSpeed(80.0f);
	cameras_.back()->setZoomSpeed(400.0f);
	cameras_.back()->setRotation({ -10.0f, -45.0f, 0.0f}); // rotate the camera to look at the center of the plane
	cameras_.back()->setPitchLimits(AngleLimits(-3.0f, -45.0f)); // it can turn 42 degrees vertically
	cameras_.back()->setYawLimits(AngleLimits(-32.0f, -58.0f)); // it can only turn 26 degrees horizontally to avoid look back the walls
	cameras_.back()->setCameraType(CameraType::kRotating);

	// create the right back corner camera (simulate a security camera)
	cameras_.push_back(new Camera(3, shared_context_)); // id of the camera is the vector position + 1
	cameras_.back()->setPosition({ 19.5, 3, -23.5});
	cameras_.back()->setRotationSpeed(80.0f);
	cameras_.back()->setZoomSpeed(400.0f);
	cameras_.back()->setRotation({ -10.0f, -135.0f, 0.0f });  // rotate the camera to look at the center of the plane
	cameras_.back()->setPitchLimits(AngleLimits(-3.0f, -45.0f)); // it can turn 42 degrees vertically
	cameras_.back()->setYawLimits(AngleLimits(-122.0f, -148.0f)); // it can only turn 26 degrees horizontally to avoid look back the walls
	cameras_.back()->setCameraType(CameraType::kRotating);

	// create the middle back corner camera (simulate a security camera)
	cameras_.push_back(new Camera(4, shared_context_)); // id of the camera is the vector position + 1
	cameras_.back()->setPosition({ 10, 3, -23.5 });
	cameras_.back()->setRotationSpeed(80.0f);
	cameras_.back()->setZoomSpeed(400.0f);
	cameras_.back()->setRotation({ -10.0f, -180.0f, 0.0f });  // rotate the camera to look at the center of the plane
	cameras_.back()->setPitchLimits(AngleLimits(-3.0f, -45.0f)); // it can turn 42 degrees vertically
	cameras_.back()->setYawLimits(AngleLimits(-90.0f, -270.0f)); // it can only turn 180 degrees horizontally to avoid look back the walls
	cameras_.back()->setCameraType(CameraType::kRotating);

	// create the left back corner camera (simulate a security camera)
	cameras_.push_back(new Camera(5, shared_context_)); // id of the camera is the vector position + 1
	cameras_.back()->setPosition({ 1, 3, -23.5 });
	cameras_.back()->setRotationSpeed(80.0f);
	cameras_.back()->setZoomSpeed(400.0f);
	cameras_.back()->setRotation({ -10.0f, +135.0f, 0.0f });  // rotate the camera to look at the center of the plane
	cameras_.back()->setPitchLimits(AngleLimits(-3.0f, -45.0f)); // it can turn 42 degrees vertically
	cameras_.back()->setYawLimits(AngleLimits(+148.0f, +122.0f)); // it can only turn 26 degrees horizontally
	cameras_.back()->setCameraType(CameraType::kRotating);

	// create the top camera (a fixed camera looking down)
	cameras_.push_back(new Camera(6, shared_context_)); // id of the camera is the vector position + 1
	cameras_.back()->setPosition({ 12, 33, -12 }); // set above the center of the plane
	cameras_.back()->setRotation({ -90.0f, 0.0f, 0.0f });  // rotate the camera to look down
	cameras_.back()->setCameraType(CameraType::kFixedPoint);

	// create a tracking camera (spaceship which is moving along z-axis)
	cameras_.push_back(new Camera(7, shared_context_)); // id of the camera is the vector position + 1
	cameras_.back()->setCameraType(CameraType::kTracking);

	// create other camera (shapeship2 which is moving along x-axis)
	cameras_.push_back(new Camera(8, shared_context_)); // id of the camera is the vector position + 1
	cameras_.back()->setCameraType(CameraType::kTracking);

	// set the current camera to the floating one by default
	current_camera_ = cameras_.at(0);
}


CameraManager::~CameraManager()
{
	// delete all the pointers created in this class
	for (Camera* camera : cameras_)
	{
		delete camera;
		camera = nullptr;
	}
}


void CameraManager::handleInput(float dt)
{
	/* Switching between cameras */

	// switch to the flight/main camera
	if (shared_context_->input->isKeyDown((int)'1'))
	{
		shared_context_->input->setKeyUp((int)'1');

		current_camera_ = cameras_.at(0);
	}
	// switch to the fixed front-right camera
	else if (shared_context_->input->isKeyDown((int)'2'))
	{
		shared_context_->input->setKeyUp((int)'2');

		current_camera_ = cameras_.at(1);
	}
	// switch to the fixed back-right camera
	else if (shared_context_->input->isKeyDown((int)'3'))
	{
		shared_context_->input->setKeyUp((int)'3');

		current_camera_ = cameras_.at(2);
	}
	// switch to the fixed back-middle camera
	else if (shared_context_->input->isKeyDown((int)'4'))
	{
		shared_context_->input->setKeyUp((int)'4');

		current_camera_ = cameras_.at(3);
	}
	// switch to the fixed back-left camera
	else if (shared_context_->input->isKeyDown((int)'5'))
	{
		shared_context_->input->setKeyUp((int)'5');

		current_camera_ = cameras_.at(4);
	}
	// switch to the fixed front-left camera
	else if (shared_context_->input->isKeyDown((int)'6'))
	{
		shared_context_->input->setKeyUp((int)'6');

		current_camera_ = cameras_.at(5);
	}
	// switch to the tracking camera (spaceship which is moving along z-axis)
	else if (shared_context_->input->isKeyDown((int)'7'))
	{
		shared_context_->input->setKeyUp((int)'7');

		current_camera_ = cameras_.at(6);
	}
	// switch to the tracking camera (spaceship2 which is moving along x-axis)
	else if (shared_context_->input->isKeyDown((int)'8'))
	{
		shared_context_->input->setKeyUp((int)'8');

		current_camera_ = cameras_.at(7);
	}

	// handle the current camera
	current_camera_->handleInput(dt);
}

void CameraManager::update(float dt)
{
	// update the camera is being used
	current_camera_->update(dt);
}

Vector3 CameraManager::getCurrentCameraPosition()
{
	return current_camera_->getPosition();
}

Vector3 CameraManager::getCurrentCameraLookAt()
{
	return current_camera_->getLookAt();
}

Vector3 CameraManager::getCurrentCameraUp()
{
	return current_camera_->getUp();
}

float CameraManager::getCurrentCameraFov()
{
	return current_camera_->getFov();
}

int CameraManager::getCurrentCameraId()
{
	return current_camera_->getId();
}

void CameraManager::linkObjToCamera(int cam_id, BaseMesh* object, Vector3 translation_to_obj_eye)
{
	for (Camera* camera : cameras_)
	{
		if (camera->getId() == cam_id) // when the cam id has been found
		{
			camera->setLinkedObject(object); // link cam-obj
			camera->setTranslationToObjEye(translation_to_obj_eye);
		}
	}
}

