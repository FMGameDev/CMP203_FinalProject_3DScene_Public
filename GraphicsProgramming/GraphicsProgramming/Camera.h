// Class Camera
// It is a camera object which can have different scheme movements depending in the camera type
// Camera movement names has been based on the real camera movements: https://csawesome.runestone.academy/runestone/books/published/learnwebgl2/07_cameras/03_camera_movement.html
// The idea of zoom has been adquired from https://learnopengl.com/Getting-started/Camera
// @author Francisco Diaz (FMGameDev)

#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include <vector>
#include "Vector3.h"

#include "SharedContext.h"
#include "BaseMesh.h" // for linking the camera to an object in kFirstPerson, kTracking etc

// types of camera
enum class CameraType
{
	kFixedPoint, // Doesn't move/rotate/scroll. The fixed point camera change as the level does.
	kRotating, // It doesn't move, it can only rotate. Used for tracking a character from a fixed point (e.g. security cameras, gun sight..)
	kScrolling, // It moves horizontally and vertically. The player cannot control the camera (it moves when the character does, or automatically by the game). Mainly in 2D games.
	kMoveable, // Moves horizontally and vertically. The player can control the camera. Mainly in 2D games
	kFloating, // Full control, the player moves and rotate with not fixed rotation. (Usually the z-axis rotation is locked) - for this project I have habilitate it 
	kTracking, // Tracks an object (usually a character) in 3D space. It may move, rotate, move ahead of the character or fall behind. The player has little or no control over it.
	kPusable, // (3rd person). It has a default position (usually behind the character). Camera rotate around the character (Player can move the camera)
	kFirstPerson // Camera linked to the character. Movement and rotation applied to the character and camera.
};

// structure to control the max values for the rotation angles: pitch, yaw, roll
struct AngleLimits
{
	AngleLimits() : max(360.0f), min(-360.0f) {}; // default constructor
	AngleLimits(float maximum, float minimum) : max(maximum), min(minimum) {};

	float max;
	float min;
};

class Camera
{
public:
	// constructor
	Camera(int id, SharedContext* shared_context);

	// destructor
	~Camera();

	// update the vector
	void update(float dt);

	// update the vector
	void handleInput(float dt);

	// return the position of the camera
	Vector3 getPosition();

	// set the position of the camera and call to update the look at point
	void setPosition(Vector3 position);

	// set the rotation of the camera and call to update the vectors
	void setRotation(Vector3 rotation_angles);

	// return the look at point
	void setLookAt(Vector3 look_at);

	// return the look at point
	Vector3 getLookAt();

	// return the "forward" vector
	Vector3 getForward();

	// return the "up" vector
	Vector3 getUp();

	// return the "right" vector
	Vector3 getRight();

	/* camera movement along axis*/

	// move the camera position closer to or further from the localitation it is looking at
	void dollyIn(float dt);
	void dollyOut(float dt);

	// move the camera position vertically (up or down)
	void pedestalUp(float dt);
	void pedestalDown(float dt);

	// move the camera position laterally (left or right)
	void truckRight(float dt);
	void truckLeft(float dt);


	/* camera rotation around axis*/

	// Cant: Rotate the camera sideways, left or right, around z-axis with a fixed position and lookAt direction
	void cantRight(float dt);
	void cantLeft(float dt);

	// Pan: Rotate the camera view horizontally, left or right, around y-axis with a fixed position
	void panRight(float dt);
	void panLeft(float dt);

	// Tilt: Rotate the camera view vertically, up or down, around x-axis with a fixed position
	void tiltDown(float dt);
	void tiltUp(float dt);

	// set speed of rotation around axis
	void setRotationSpeed(float rot_speed);

	// set speed of movement along the axis
	void setMovementSpeed(float mov_speed);

	// set the speed for using zoom
	void setZoomSpeed(float zoom_speed);

	// return the field of view (for zoom)
	float getFov();

	// set the camera type
	void setCameraType(CameraType camera_type);

	// set linked object for some type of cameras such as first person
	void setLinkedObject(BaseMesh* obj);

	// set the translation necessary to set the camera to the eye objct position
	void setTranslationToObjEye(Vector3 translation_to_obj_eye);

	// set the limits for the angles
	void setPitchLimits(AngleLimits pitch_limits);
	void setYawLimits(AngleLimits yaw_limits);
	void setRollLimits(AngleLimits roll_limits);


	// return the identifier
	int getId();

private:
	// identifier (it will be the vector position in the collection of camera manager)
	int id_;

	// define the type of camera
	CameraType camera_type_;

	// mesh object linked to the camera, it is set in some cameras types such as first person, tracking, etc
	BaseMesh* linked_object_;

	// shared context component
	SharedContext* shared_context_;

	Vector3 position_; // position of the camera in world space
	Vector3 look_at_; // a point in the world the camera is looking at
	Vector3 translation_to_obj_eye_; // component to add to the linked object position translating this camera to the eye position of the object (ex: for a tracking camera, first person, etc)

	Vector3 right_; // x-axis
	Vector3 up_; // y-axis
	Vector3 forward_; // z-axis

	// speeds for different uses
	float rot_speed_; // speed for rotation around the axis
	float mov_speed_; // speed of movement along the axis
	float zoom_speed_; // speed for using zoom

	float fov_; // field of view (used for zoom)

	// rotations angles
	float pitch_; // Lateral Axis Rotation(Around x-axis)
	float yaw_; // Vertical Axis Rotation (Around y-axis) 
	float roll_; // Logitudinal Axis Rotation (Around z-axis)

	// limits values for the rotation angle
	AngleLimits pitch_limits_, yaw_limits_, roll_limits_;

	// update the vectors forward, up and right and using the rotations values (pitch, yaw, roll)
	void recalculateUpForwardRightVectors();

	// update the look at point after recalculating the vectors and after setting the new position using some of the vectors (right, up, forward)
	// the recalculation is the result of sum of the forward point plus the position
	void recalculateLookAtPoint();

	// change the yaw and pitch angles in function of the mouse movement
	// after it has been set the new angles, it set the mouse position to the middle
	void handleMousePosInput(float dt);

	// used for zoom (changing the fov value)
	void handleMouseWheelInput(float dt);

	// rotations around axis and movement along axis by keyboard
	void handleKeyboardInput(float dt);

	// functions to control that rotation angles (pitch, yaw and roll) doesn't pass their limits
	void controlPitch();
	void controlYaw();
	void controlRoll();
};


