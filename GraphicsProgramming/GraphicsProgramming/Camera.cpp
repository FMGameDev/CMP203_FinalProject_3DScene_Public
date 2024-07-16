#include "Camera.h"

Camera::Camera(int id, SharedContext* shared_context)
	: shared_context_(shared_context), id_(id)
{
	/** initialization by default */

	position_.set(0.0f, 0.0f, 8.0f);
	look_at_.set(0.0f, 0.0f, 0.0f);

	// three vectors which depends on rotation angles
	up_.set(0.0f, 0.0f, 0.0f);
	forward_.set(0.0f, 0.0f, 0.0f);
	right_.set(0.0f, 0.0f, 0.0f);

	// rotations
	roll_ = 0.0f;
	yaw_ = 0.0f;
	pitch_ = 0.0f;

	// fov used for zoom (no zoom by default)
	fov_ = 45.0f;

	// speed by default
	mov_speed_ = 0.0f;
	zoom_speed_ = 0.0f;
	rot_speed_ = 0.0f;

	recalculateUpForwardRightVectors();
	recalculateLookAtPoint();
}


Camera::~Camera()
{

}


Vector3 Camera::getPosition()
{
	return position_;
}

void Camera::setPosition(Vector3 position)
{
	position_ = position;
	
	// update the look at point
	recalculateLookAtPoint();
}

void Camera::setRotation(Vector3 rotation_angles)
{
	// set the new values
	pitch_ = rotation_angles.x;
	yaw_ = rotation_angles.y;
	roll_ = rotation_angles.z;

	// update all the vectors and look at point in the camera
	recalculateUpForwardRightVectors();
	recalculateLookAtPoint();
}


void Camera::setRotationSpeed(float rot_speed)
{
	rot_speed_ = rot_speed;
}

void Camera::setMovementSpeed(float mov_speed)
{
	mov_speed_ = mov_speed;
}

void Camera::setZoomSpeed(float zoom_speed)
{
	zoom_speed_ = zoom_speed;
}


float Camera::getFov()
{
	return fov_;
}

void Camera::setCameraType(CameraType camera_type)
{
	camera_type_ = camera_type;
}

void Camera::setLinkedObject(BaseMesh* obj)
{
	linked_object_ = obj;
}

void Camera::setTranslationToObjEye(Vector3 translation_to_obj_eye)
{
	translation_to_obj_eye_ = translation_to_obj_eye;
}

void Camera::setPitchLimits(AngleLimits pitch_limits)
{
	pitch_limits_ = pitch_limits;
}

void Camera::setYawLimits(AngleLimits yaw_limits)
{
	yaw_limits_ = yaw_limits;
}

void Camera::setRollLimits(AngleLimits roll_limits)
{
	roll_limits_ = roll_limits;
}

int Camera::getId()
{
	return id_;
}


void Camera::update(float dt)
{
	// update position and camera angles and vectors in case it is tracking an object 
	if (linked_object_ != nullptr && camera_type_ == CameraType::kTracking)
	{
		// rotate the camera the same angles as the object
		Vector3 rotation_angles = linked_object_->getRotationAngles();
		pitch_ = rotation_angles.x;
		yaw_ = rotation_angles.y;
		roll_ = rotation_angles.z;

		// set the position the same to the translation of the object
		// it would need to be modified in the future
		position_ = linked_object_->getTranslation() + translation_to_obj_eye_;
		
		// recalculate all the pointers and look at vector
		recalculateUpForwardRightVectors();
		recalculateLookAtPoint();
	}
}

void Camera::handleInput(float dt)
{
	/* Handle all the Mouse inputs*/
	// If I use more camera types in the future then add them here

	// roating camera using mouse pos
	if (camera_type_ == CameraType::kRotating || camera_type_ == CameraType::kFloating)
	{
		handleMousePosInput(dt);
	}

	if (camera_type_ == CameraType::kRotating) // set that security cameras, sight guns, etc (rotating type) are the only one which can zoom
	{
		handleMouseWheelInput(dt);
	}

	/* Handle keyboard input */
	if (camera_type_ == CameraType::kRotating || camera_type_ == CameraType::kFloating)
	{
		handleKeyboardInput(dt);
	}
}

void Camera::handleMousePosInput(float dt)
{
	// get current mouse position
	int x_pos = shared_context_->input->getMouseX();
	int y_pos = shared_context_->input->getMouseY();

	// calculate the middle window
	int middle_window_x = *shared_context_->window_width / 2;
	int middle_window_y = *shared_context_->window_height / 2;

	// set the mouse to the center of the window
	glutWarpPointer(middle_window_x, middle_window_y);

	// detect if the mouse has been moved, if so them update yaw_ and pitch_ angles and recalculate all the vectors
	if (!*shared_context_->first_mouse_click && (x_pos != middle_window_x || y_pos != middle_window_y))
	{
		// calculate the new yaw_ and pitch_ adding the distance between the last mouse position_ (middle_window) and the current mouse pos scaled to the speed and delta time.
		yaw_ += (float)(x_pos - middle_window_x) * (rot_speed_ * dt);
		pitch_ += (float)(middle_window_y - y_pos) * (rot_speed_ * dt); // reversed since y-coordinates range from bottom to top

		// control the angles are in their limits
		controlYaw();
		controlPitch();

		// update all right_, up_, and forward_ vectors
		recalculateUpForwardRightVectors();

		// update the look at point with using the new position_
		recalculateLookAtPoint();
	}

	// set to false first mouse click so next time the user move the camera will rotate
	*shared_context_->first_mouse_click = false;
}

void Camera::handleMouseWheelInput(float dt)
{
	bool fov_updated = false;

	// detect if the wheel has been used
	if (shared_context_->input->isScrollUp()) // scroll up_ to decrease zoom
	{
		shared_context_->input->setScrollUp(false);
		fov_ -= zoom_speed_ * dt;
		fov_updated = true;
	}
	else if (shared_context_->input->isScrollDown()) // scroll down to increase zoom
	{
		shared_context_->input->setScrollDown(false);
		fov_ += zoom_speed_ * dt;
		fov_updated = true;
	}

	// if the wheel has been used (fov updated) then control the fov value
	if (fov_updated)
	{
		// control the fov value, setting a min and a max values
		if (fov_ < 1.0f)
		{
			fov_ = 1.0f;
		}
		else if (fov_ > 45.0f)
		{
			fov_ = 45.0f;
		}
	}
}

void Camera::handleKeyboardInput(float dt)
{
	////////////////////////////////////// rotation around axis

	if (camera_type_ == CameraType::kFloating) // allow rotating z-axis just for this project propose (Usually the z-axis rotation is locked) 
	{
		/** Rotation around z-axis */
		if (shared_context_->input->isKeyDown((int)'i'))
		{
			shared_context_->input->setKeyUp((int)'i');
			Camera::cantRight(dt);
		}
		else if (shared_context_->input->isKeyDown((int)'y'))
		{
			shared_context_->input->setKeyUp((int)'y');
			Camera::cantLeft(dt);
		}
	}

	if (camera_type_ == CameraType::kRotating || camera_type_ == CameraType::kFloating)
	{
		/** Rotation around y-axis */
		if (shared_context_->input->isKeyDown((int)'k'))
		{
			shared_context_->input->setKeyUp((int)'k');
			Camera::panRight(dt);
		}
		else if (shared_context_->input->isKeyDown((int)'h'))
		{
			shared_context_->input->setKeyUp((int)'h');
			Camera::panLeft(dt);
		}

		/** Rotation around x-axis */
		if (shared_context_->input->isKeyDown((int)'u'))
		{
			shared_context_->input->setKeyUp((int)'u');
			Camera::tiltUp(dt);
		}
		else if (shared_context_->input->isKeyDown((int)'j'))
		{
			shared_context_->input->setKeyUp((int)'j');
			Camera::tiltDown(dt);
		}
	}


	////////////////////////////////////// Moving along axis
	// moving along axis is after rotation because it depends on the right, up, forward vectors which are modified with the rotation angles

	if (camera_type_ == CameraType::kFloating)
	{
		/** Move forward_ or backward (z-axis) */
		if (shared_context_->input->isKeyDown((int)'w'))
		{
			shared_context_->input->setKeyUp((int)'w');
			Camera::dollyIn(dt);
		}
		else if (shared_context_->input->isKeyDown((int)'s'))
		{
			shared_context_->input->setKeyUp((int)'s');
			Camera::dollyOut(dt);
		}

		/** Move up_ or down (y-axis) */
		if (shared_context_->input->isKeyDown((int)'q'))
		{
			shared_context_->input->setKeyUp((int)'q');
			Camera::pedestalUp(dt);
		}
		else if (shared_context_->input->isKeyDown((int)'e'))
		{
			shared_context_->input->setKeyUp((int)'e');
			Camera::pedestalDown(dt);
		}

		/** Move Left or Right (x-axis) */
		if (shared_context_->input->isKeyDown((int)'d'))
		{
			shared_context_->input->setKeyUp((int)'d');
			Camera::truckRight(dt);
		}
		else if (shared_context_->input->isKeyDown((int)'a'))
		{
			shared_context_->input->setKeyUp((int)'a');
			Camera::truckLeft(dt);
		}
	}
}


void Camera::controlPitch()
{
	// set a limit to the pitch angle for not rotate more than 89 grades as at 90 degrees we would get the LookAt vector flip
	// this ensures the user will be able to look up to the sky or below to his feet but not further.
	if (pitch_limits_.max > 89.0f && pitch_ > 89.0f)
		pitch_ = 89.0f;
	else if (pitch_limits_.min < -89.0f && pitch_ < -89.0f)
		pitch_ = -89.0f;
	// for pitch limits between 89 and -89
	else if (pitch_ > pitch_limits_.max)
		pitch_ = pitch_limits_.max;
	else if (pitch_ < pitch_limits_.min)
		pitch_ = pitch_limits_.min;
}

void Camera::controlYaw()
{
	// control yaw speed
	if (yaw_limits_.max != 360.0f && yaw_limits_.min != -360.0f)
	{
		if (yaw_ > yaw_limits_.max)
			yaw_ = yaw_limits_.max;
		else if (yaw_ < yaw_limits_.min)
			yaw_ = yaw_limits_.min;
	}
	else if (yaw_ > yaw_limits_.max || yaw_ < yaw_limits_.min)
		yaw_ = 0.0f; // reset it
}

void Camera::controlRoll()
{
	// control roll speed
	if (roll_limits_.max != 360.0f && roll_limits_.min != -360.0f)
	{
		if (roll_ > roll_limits_.max)
			roll_ = roll_limits_.max;
		else if (roll_ < roll_limits_.min)
			roll_ = roll_limits_.min;
	}
	else if (roll_ > yaw_limits_.max || roll_ < roll_limits_.min)
		roll_ = 0.0f; // reset it
}


void Camera::recalculateUpForwardRightVectors()
{
	// temp values for sin/cos
	float cos_pitch, cos_roll, cos_yaw;
	float sin_pitch, sin_roll, sin_yaw;

	// handle rotations
	cos_pitch = cosf(pitch_ * M_PI / 180.0f);
	cos_yaw = cosf(yaw_ * M_PI / 180.0f);
	cos_roll = cosf(roll_ * M_PI / 180.0f);

	sin_pitch = sinf(pitch_ * M_PI / 180.0f);
	sin_yaw = sinf(yaw_ * M_PI / 180.0f);
	sin_roll = sinf(roll_ * M_PI / 180.0f);

	// calculate Forward vector (z-direction)
	// using parametric equation of a sphere
	forward_.setX(sin_yaw * cos_pitch);
	forward_.setY(sin_pitch);
	forward_.setZ(cos_pitch * (-cos_yaw));
	forward_.normalise(); // to keep a consistent movement

	// calculate Up vector (y-direction)
	// using parametric equation of a sphere
	up_.setX((-cos_yaw * sin_roll) - (sin_yaw * sin_pitch * cos_roll));
	up_.setY(cos_pitch * cos_roll);
	up_.setZ((-sin_yaw * sin_roll) - (sin_pitch * cos_roll * (-cos_yaw)));
	up_.normalise(); // to keep a consistent movement

	// calculate Right vector (x-direction)
	// using cross product between the forward_ and up_ vector
	right_ = forward_.cross(up_);
	right_.normalise(); // to keep a consistent movement
}

void Camera::recalculateLookAtPoint()
{
	// calculate Look At vector using the new position_ of the camera and the forward_ position_
	look_at_ = position_ + forward_;
}


void Camera::setLookAt(Vector3 look_at)
{
	look_at_ = look_at;
}

Vector3 Camera::getLookAt()
{
	return look_at_;
}


Vector3 Camera::getForward()
{
	return forward_;
}

Vector3 Camera::getUp()
{
	return up_;
}

Vector3 Camera::getRight()
{
	return right_;
}




/** Movement along z-axis */

void Camera::dollyIn(float dt)
{
	// set the new position_ of the camera
	position_.add(forward_, mov_speed_ * dt); // move positively through forward_ (controlling the speed)

	// update the look at point with the new position
	recalculateLookAtPoint();
}

void Camera::dollyOut(float dt)
{
	// set the new position_ of the camera
	position_.subtract(forward_, mov_speed_ * dt); // move negatively through forward_ (controlling the speed)

	// update the look at point with the new position
	recalculateLookAtPoint();
}


/** Movement along y-axis */

void Camera::pedestalUp(float dt)
{
	// set the new position_ of the camera
	position_.add(up_, mov_speed_ * dt); // move positively through up_ (controlling the speed)

	// update the look at point with the new position
	recalculateLookAtPoint();
}

void Camera::pedestalDown(float dt)
{
	// set the new position_ of the camera
	position_.subtract(up_, mov_speed_ * dt); // move negatively through up_ (controlling the speed)

	// update the look at point with the new position
	recalculateLookAtPoint();
}


/** Movement along x-axis */

void Camera::truckRight(float dt)
{
	// set the new position_ of the camera
	position_.add(right_, mov_speed_ * dt); // move positively through right_ (controlling the speed)

	// update the look at point with the new position
	recalculateLookAtPoint();
}

void Camera::truckLeft(float dt)
{
	// set the new position_ of the camera
	position_.subtract(right_, mov_speed_ * dt); // move negatively through right_ (controlling the speed)

	// update the look at point with the new position
	recalculateLookAtPoint();
}


/** Rotation around z-axis */

void Camera::cantRight(float dt)
{
	// control the speed_ of movement
	roll_ += (rot_speed_ * dt);

	// control the angle is between its limits
	controlRoll();

	// update all the vectors and look at point in the camera
	recalculateUpForwardRightVectors();
	recalculateLookAtPoint();
}

void Camera::cantLeft(float dt)
{
	// control the speed_ of movement
	roll_ -= (rot_speed_ * dt);

	// control the angle is between its limits
	controlRoll();

	// update all the vectors and look at point in the camera
	recalculateUpForwardRightVectors();
	recalculateLookAtPoint();
}


/** Rotation around y-axis */

void Camera::panRight(float dt)
{
	// control the speed_ of movement
	yaw_ += (rot_speed_ * dt);

	// control the angle is between its limits
	controlYaw();

	// update all the vectors and look at point in the camera
	recalculateUpForwardRightVectors();
	recalculateLookAtPoint();
}

void Camera::panLeft(float dt)
{
	// control the speed_ of movement
	yaw_ -= (rot_speed_ * dt);

	// control the angle is between its limits
	controlYaw();

	// update all the vectors and look at point in the camera
	recalculateUpForwardRightVectors();
	recalculateLookAtPoint();
}


/** Rotation around x-axis */

void Camera::tiltDown(float dt)
{
	// control the speed_ of movement
	pitch_ += (rot_speed_ * dt);

	// control the angle is between its limits
	controlPitch();

	// update all the vectors and look at point in the camera
	recalculateUpForwardRightVectors();
	recalculateLookAtPoint();
}

void Camera::tiltUp(float dt)
{
	// control the speed_ of movement
	pitch_ -= (rot_speed_ * dt);

	// control the angle is between its limits
	controlPitch();

	// update all the vectors and look at point in the camera
	recalculateUpForwardRightVectors();
	recalculateLookAtPoint();
}

