
// Class for Lights
// It creates a light object and it can create a debug object in the position where it is the light
// It can be used for directional and not directional (point) lights
// For changing the colour over the time has been used the guide: https://learnopengl.com/Lighting/Materials
// @author Francisco Diaz (FMGameDev)

#pragma once
// Include GLUT, openGL, input.
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>

#include "MeshSphere.h"
#include "Vector3.h"
#include "Colour4.h"

using namespace std;

enum class LightType
{
	kDirectional,
	kPoint,
	kSpot
};

class Light
{
public:

	// constructor for Ambient + Diffuse + Specular components (directional or point light)
	Light(GLenum light_id, Colour4 light_ambient_colour, Colour4 light_diffuse_colour, Colour4 light_specular_colour, vector<GLfloat> light_position);

	// constructor for Ambient + Diffuse + Specular components (spot light)
	Light(GLenum light_id, Colour4 light_ambient_colour, Colour4 light_diffuse_colour, Colour4 light_specular_colour, vector<GLfloat> light_position,
		vector<GLfloat> spot_direction, float spot_cutoff, float spot_exponent);

	//destructor
	~Light();


	// FUNCTION FOR ADD MORE CHARACTERISTICS TO THE LIGHT

	// set the atttenuation for this light
	void setAttenuation(GLfloat constant_attenuation, GLfloat linear_attenuation, GLfloat quadratic_attenuation);

	// set the debug mode, if it is true it will be render an object in the position of the light
	void setDebugMode(bool debug_mode);


	// FUNCTIONS FOR TURNING ON/ OFF THE LIGHT

	// turn the light on or off depending on if it is currently on or not (it works like a switch)
	// if it is turned on -> turn off
	// if it is turned off -> turn on
	void changeLightState();

	// turn on light
	void turnOff();

	// turn off light
	void turnOn();

	// return the state of the light
	// if it is on it return true
	// else false
	bool getIsTurnedOn();


	// FUNCTIONS FOR ROTATION

	// function to control if the light rotate in x, y or z
	void setIsRotating(bool rotation_x, bool rotation_z, bool rotation_y);
	void setIsRotating(vector<bool> isRotating);

	// set the speed of the light (and debug object in case it exists)
	void setSpeed(float speed);

	// the light (and debug object) orbit around x-axis
	// for orbitate in positive way pass a positive dt (up effect)
	// for orbitate in negative way pass a negative dt (down effect)
	void orbitAroundXAxis(float dt);

	// the light (and debug object) orbit around y-axis
	// for orbitate in positive way pass a positive dt (right effect)
	// for orbitate in negative way pass a negative dt (left effect)
	void orbitAroundYAxis(float dt);

	// the light (and debug object) orbit around z-axis
	// for orbitate in positive way pass a positive dt (up effect)
	// for orbitate in negative way pass a negative dt (down effect)
	void orbitAroundZAxis(float dt);


	// FUCNTIONS FOR DISPLACEMENT

	// move the light along the x axis
	void moveAlongXAxis(float dt); // by passing +- dt move in posite or negative way

	// move the light along the y axis
	void moveAlongYAxis(float dt); // by passing +- dt move in posite or negative way

	// move the light along the z axis
	void moveAlongZAxis(float dt); // by passing +- dt move in posite or negative way

	// set the translation/position for the light
	void setPosition(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z);


	// UPDATE AND RENDER FUNCTIONS

	// function for updating the light and the debug object in case it exists
	void update(float dt);

	// function for rendering the light and the debug object in case it exists
	virtual void render();

	
	// return the id of the light
	GLenum getLightId();

	// return the position/direction of the light
	vector<GLfloat> getPosition();

	// function to set if change the light colour over the time or not
	void setLightColourChangingOverTime(bool changing_colour);

private:

	// IDENTIFICATION VARIABLES

	// ID of the light (Max 8 ids - From 0 to 7)
	GLenum light_id_;

	// type of the light
	LightType light_type_;


	// LIGHTS COMPONENT

	// Ambient component (a different colour to the main light)
	// Comes from everywhere
	Colour4 light_ambient_colour_; // default value

	// Diffuse component,it gives a coloured light
	// Comes from a particular direction
	Colour4 light_diffuse_colour_ = {(unsigned int)0xFFFFFFFF};

	// Specular component, it a light that sets highlights and often the shininess of an object
	// it comes from a particular direction
	Colour4 light_specular_colour_ = {(unsigned int)0xFFFFFFFF};

	// It sets the position of a light (when w == 1.0f)
	// or its direction (when w == 0.0f)
	//vector<GLfloat> light_position_ = { x, y, z, w };
	vector<GLfloat> light_position_ = { 0.0f, 0.0f, 1.0f, 0.0f };


	// VARIABLES FOR SPOT LIGHT
	vector<GLfloat> spot_direction_ = { 0.0f, 0.0f, -1.0f};

	GLfloat spot_cutoff_ = 180.0f; // default value
	GLfloat spot_exponent_ = 0.0f; // default value


	// ATTENUATION COMPONENTS
	GLfloat constant_attenuation_ = 1.0f; // default value
	GLfloat linear_attenuation_ = 0.0f; // default value
	GLfloat quadratic_attenuation_ = 0.0f; // default value


	// VARIABLES FOR MOVEMENT

	// variable to control if the light is rotating in x, y, z
	vector<bool> is_rotating_ = { false, false, false }; // default value
	Vector3 rotation_angles_ = { 0.0f, 0.0f, 0.0f }; // default value

	// speed of rotation
	float speed_ = 0.0f; // default value


	// VARIABLES FOR CONTROL OF STATE 

	// component to control the state of the light (If it is turned on or off)
	bool is_turned_on_ = true;

	// value to control if we are in deug mode (render object in position of the )
	bool debug_mode_ = true;


	// VARIABLES TO MODIFY THE LIGHT COLOUR

	// boolean to detect if the light colour (ambient and diffuse change over the time)
	bool light_colour_changing_ = false;

	// component to control the change of colour automatically
	float countdown_ = 0.0f;

	// function to change the colour over the time
	// it has been used as a guide the link: https://learnopengl.com/Lighting/Materials
	void changeColourOverTime();
};

