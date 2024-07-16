
// Manager Class for Lights
// It manage the colection of Lights created for a scene.
// it sets all the lights in the initlights function
// @author Francisco Diaz (FMGameDev)

#pragma once
#include "Light.h"
#include "SharedContext.h"

#include <vector>

using namespace std;

class LightManager
{
public:
	// constructor
	LightManager(SharedContext *shared_context);

	// destructor
	~LightManager();

	// handle input function for light
	void handleInput(float dt);

	// function for update the Lights
	void update(float dt);

	// function for render the Lights
	void render();

	// return the light position by its identifier
	vector<GLfloat> getLightPosition(GLenum light_id);

private:

	// initialise the lights for a scene
	void initLights();

	// Shared Context component
	SharedContext* shared_context_;

	// collection of Lights
	vector<Light*> lights_;
};

