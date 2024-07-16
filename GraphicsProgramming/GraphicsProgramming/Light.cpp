#include "Light.h"

Light::Light(GLenum light_id, Colour4 light_ambient_colour, Colour4 light_diffuse_colour, Colour4 light_specular_colour, vector<GLfloat> light_position)
	: light_id_(light_id), light_ambient_colour_(light_ambient_colour), light_diffuse_colour_(light_diffuse_colour), light_specular_colour_(light_specular_colour), light_position_(light_position)
{
	// set the type of light
	if (light_position_[3] == 1.0f) // point light
	{
		light_type_ = LightType::kPoint;
	}
	else if (light_position_[3] == 0.0f) // directional light
	{
		light_type_ = LightType::kDirectional;
	}

	if (debug_mode_)
	{

	}
}

Light::Light(GLenum light_id, Colour4 light_ambient_colour, Colour4 light_diffuse_colour, Colour4 light_specular_colour, vector<GLfloat> light_position,
	vector<GLfloat> spot_direction, float spot_cutoff, float spot_exponent)
	: light_id_(light_id), light_ambient_colour_(light_ambient_colour), light_diffuse_colour_(light_diffuse_colour), light_specular_colour_(light_specular_colour), light_position_(light_position),
	spot_direction_(spot_direction), spot_cutoff_(spot_cutoff), spot_exponent_(spot_exponent)
{

	// set the type of light
	if (light_position_[3] == 1.0f) // point light (spot light as it has been used this constructor)
	{
		light_type_ = LightType::kSpot;
	}
	else if (light_position_[3] == 0.0f) // directional light (cannot be a spot light)
	{
		light_type_ = LightType::kDirectional;
	}

}


Light::~Light()
{

}


void Light::setAttenuation(GLfloat constant_attenuation, GLfloat linear_attenuation, GLfloat quadratic_attenuation)
{
	constant_attenuation_= constant_attenuation;
	linear_attenuation_ = linear_attenuation;
	quadratic_attenuation_ = quadratic_attenuation;
}


void Light::setDebugMode(bool debug_mode)
{
	debug_mode_ = debug_mode;
}


bool Light::getIsTurnedOn()
{
	return is_turned_on_;
}


void Light::changeLightState()
{
	if (is_turned_on_) // if it is on then turn it off
	{
		turnOff();
	}
	else // if it is on then turn it off
	{
		turnOn();
	}
}


void Light::turnOff()
{
	is_turned_on_ = false;
}


void Light::turnOn()
{
	is_turned_on_ = true;
}


void Light::setIsRotating(bool rotation_x, bool rotation_y, bool rotation_z)
{
	is_rotating_[0] = rotation_x; // set rotation flag for X Axis 
	is_rotating_[1] = rotation_y; // set rotation flag for Y Axis 
	is_rotating_[2] = rotation_z; // set rotation flag for Z Axis 
}

void Light::setIsRotating(vector<bool> is_rotating)
{
	is_rotating_ = is_rotating;
}

void Light::setSpeed(float speed)
{
	speed_ = speed;
}

void Light::update(float dt)
{
	// move light by the time if it is rotating (moving)
	if (is_rotating_[0]) // on the x axis
	{
		rotation_angles_.x += (speed_ * dt);
	}
	if (is_rotating_[1]) // on the y axis
	{
		rotation_angles_.y += (speed_ * dt);
	}
	if (is_rotating_[2]) // on the z axis
	{
		rotation_angles_.z += (speed_ * dt);
	}

	// if the light is automatically changing over the time
	if (light_colour_changing_)
	{
		// substract the time passed
		countdown_ -= dt;

		if (countdown_ <= 0)
		{
			changeColourOverTime();
			countdown_ = 5; // change colour light each 30s
		}


	}
}


void Light::render()
{
	glPushMatrix(); // REMEMBER WHERE WE ARE

		// rotate on the x axis
		glRotatef(rotation_angles_.x, 1.0f, 0.0f, 0.0f);		
		// rotate on the y axis
		glRotatef(rotation_angles_.y, 0.0f, 1.0f, 0.0f);	
		// rotate on the z axis
		glRotatef(rotation_angles_.z, 0.0f, 0.0f, 1.0f);

		// Set Light Ambient colour
		glLightfv(light_id_, GL_AMBIENT, light_ambient_colour_.rgba);
		// Set Light Diffuse colour
		glLightfv(light_id_, GL_DIFFUSE, light_diffuse_colour_.rgba);
		// Set Light Specular colour
		glLightfv(light_id_, GL_SPECULAR, light_specular_colour_.rgba);

		// Set Light Position
		glLightfv(light_id_, GL_POSITION, light_position_.data());

		// set Spot components depending if it is a spot light (it has been used the spot light constructor)
		if (light_type_ == LightType::kSpot)
		{
			// set all values for spot light
			glLightf(light_id_, GL_SPOT_CUTOFF, spot_cutoff_);
			glLightfv(light_id_, GL_SPOT_DIRECTION, spot_direction_.data());
			glLightf(light_id_, GL_SPOT_EXPONENT, spot_exponent_);
		}

		// Set attenuation
		glLightfv(light_id_, GL_CONSTANT_ATTENUATION, &constant_attenuation_);
		glLightfv(light_id_, GL_LINEAR_ATTENUATION, &linear_attenuation_);
		glLightfv(light_id_, GL_QUADRATIC_ATTENUATION, &quadratic_attenuation_);

		// set enable/disable depending on if it is turned on
		if (is_turned_on_)
		{
			glEnable(light_id_);
		}
		else
		{
			glDisable(light_id_);
		}

	glPopMatrix(); // Go back where we were

	
	// OBJECT (situated where the light it is)

	if (debug_mode_)
	{
		// same colour as the diffuse light
		glColor4f(light_diffuse_colour_.rgba[0], light_diffuse_colour_.rgba[1], light_diffuse_colour_.rgba[2], light_diffuse_colour_.rgba[3]); 

		glPushMatrix(); // REMEMBER WHERE WE ARE
			
			glTranslatef(light_position_[0], light_position_[1], light_position_[2]); // same position as the light plus the radius to keep the light in the center of the sphere
			
			glRotatef(rotation_angles_.x, 1.0f, 0.0f, 0.0f);
			glRotatef(rotation_angles_.y, 0.0f, 1.0f, 0.0f);
			glRotatef(rotation_angles_.z, 0.0f, 0.0f, 1.0f);

			glDisable(GL_LIGHTING); // disable lighting to apply the color of the diffuse to the sphere
				gluSphere(gluNewQuadric(), 0.20, 20, 20); // draw the sphere where the light it is
			glEnable(GL_LIGHTING);

		glPopMatrix();	// Go back where we were

		// reset colour
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

}


GLenum Light::getLightId()
{
	return light_id_;
}

vector<GLfloat> Light::getPosition()
{
	return light_position_;
}

void Light::setLightColourChangingOverTime(bool changing_colour)
{
	light_colour_changing_ = changing_colour;
}

void Light::changeColourOverTime()
{
	Vector3 lightColor;
	lightColor.x = abs(sinf(glutGet(GLUT_ELAPSED_TIME) * 2.0f));
	lightColor.y = abs(sinf(glutGet(GLUT_ELAPSED_TIME) * 0.7f));
	lightColor.z = abs(sinf(glutGet(GLUT_ELAPSED_TIME) * 1.3f));

	light_diffuse_colour_ = { lightColor.x * 0.5f, lightColor.y * 0.5f, lightColor.z * 0.5f, 1.0f};
	light_ambient_colour_ = { lightColor.x * 0.2f, lightColor.y * 0.2f, lightColor.z * 0.2f, 1.0f };
}

void Light::orbitAroundXAxis(float dt)
{
	rotation_angles_.x += (speed_ * dt);
}


void Light::orbitAroundYAxis(float dt)
{
	rotation_angles_.y += (speed_ * dt);
}


void Light::orbitAroundZAxis(float dt)
{
	rotation_angles_.z += (speed_ * dt);
}



void Light::moveAlongXAxis(float dt)
{
	light_position_[0] += (speed_* dt);
}


void Light::moveAlongYAxis(float dt)
{
	light_position_[1] += (speed_* dt);
	 
	// control pos
	if (light_position_[1] <= 1.0f) // set min in y
		light_position_[1] = 1.0f;
	else if(light_position_[1] >= 30.0f) // set max in y
		light_position_[1] = 30.0f;
}


void Light::moveAlongZAxis(float dt)
{
	light_position_[2] += (speed_* dt);
}


void Light::setPosition(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z)
{
	light_position_[0] = pos_x;
	light_position_[1] = pos_y;
	light_position_[2] = pos_z;
	// the last light position (w) or pos (3), it is not changed in running time
}
