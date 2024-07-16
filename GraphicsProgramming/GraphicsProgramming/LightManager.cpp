#include "LightManager.h"

// constructor
LightManager::LightManager(SharedContext* shared_context) 
	: shared_context_(shared_context)
{
	// initialise lights
	initLights();
}

// destructor
LightManager::~LightManager()
{
	for (Light* light : lights_)
	{
		delete light;
		light = nullptr;
	}
}


// handle input function for light
void LightManager::handleInput(float dt)
{
	/* MOVE THE LIGHTS AROUND AXIS */ // It has been disabled for this project

	/*if (shared_context_->input->isKeyDown((int)'w'))
	{
		shared_context_->input->setKeyUp((int)'w');

		// move the light up (around x-axis)
		lights_.at(0)->orbitAroundZAxis(dt);
	}
	else if(shared_context_->input->isKeyDown((int)'s'))
	{
		shared_context_->input->setKeyUp((int)'s');

		// move the light down (around x-axis)
		lights_.at(0)->orbitAroundXAxis(-dt);
	}
	else if (shared_context_->input->isKeyDown((int)'a'))
	{
		shared_context_->input->setKeyUp((int)'a');

		// move the light to the left (around y-axis)
		lights_.at(0)->orbitAroundYAxis(-dt);
	}
	else if (shared_context_->input->isKeyDown((int)'d'))
	{
		shared_context_->input->setKeyUp((int)'d');

		// move the light to the right (around y-axis)
		lights_.at(0)->orbitAroundYAxis(dt);

	}
	else if (shared_context_->input->isKeyDown((int)'q'))
	{
		shared_context_->input->setKeyUp((int)'q');

		// move the light down (around z-axis)
		lights_.at(0)->orbitAroundZAxis(-dt);
	}
	else if (shared_context_->input->isKeyDown((int)'e'))
	{
		shared_context_->input->setKeyUp((int)'e');

		// move the light up (around z-axis)
		lights_.at(0)->orbitAroundZAxis(dt);
	}
	*/


	/* MOVE THE LIGHTS ALONG AXIS */

	/*if (shared_context_->input->isKeyDown((int)'i'))
	{
		shared_context_->input->setKeyUp((int)'i');

		// move the light along z-axis on negative way - away from us
		lights_.at(0)->moveAlongZAxis(-dt);
	}
	else if (shared_context_->input->isKeyDown((int)'k'))
	{
		shared_context_->input->setKeyUp((int)'k');

		//move the light along z - axis on positive way - closer to us
		lights_.at(0)->moveAlongZAxis(dt);
		
	}
	else if (shared_context_->input->isKeyDown((int)'j'))
	{
		shared_context_->input->setKeyUp((int)'j');

		// move the light along x-axis on negative way - to the left
		lights_.at(0)->moveAlongXAxis(-dt);
	}
	else if (shared_context_->input->isKeyDown((int)'l'))
	{
		shared_context_->input->setKeyUp((int)'l');

		// move the light along x-axis on positive way - to the right
		lights_.at(0)->moveAlongXAxis(dt);
	
	}else*/

	// AMBIENT+DIFFUSE LIGHT
	if (shared_context_->input->isKeyDown((int)'z'))
	{
		shared_context_->input->setKeyUp((int)'z');

		// move the light along y-axis on positive way - up
		lights_.at(0)->moveAlongYAxis(dt);

	}
	else if (shared_context_->input->isKeyDown((int)'c'))
	{
		shared_context_->input->setKeyUp((int)'c');

		// move the light along y-axis on negative way - down
		lights_.at(0)->moveAlongYAxis(-dt);
	}

	// SPOT LIGHT
	else if (shared_context_->input->isKeyDown((int)'v'))
	{
		shared_context_->input->setKeyUp((int)'v');
		
		// move the light along y-axis on positive way - up
		lights_.at(1)->moveAlongYAxis(dt);
		
	}
	else if (shared_context_->input->isKeyDown((int)'n'))
	{
		shared_context_->input->setKeyUp((int)'n');

		// move the light along y-axis on negative way - down
		lights_.at(1)->moveAlongYAxis(-dt);
	}

	// Change the state of the SPOT LIGHT (TURN ON/ TURN OFF)
	if (shared_context_->input->isKeyDown((int)'b'))
	{
		shared_context_->input->setKeyUp((int)'b');
		// Change light state
		lights_.at(1)->changeLightState();
	}
	
}

// function for update the Lights
void LightManager::update(float dt)
{
	// update each light in the collection
	for (Light* light : lights_)
	{
		if(light->getIsTurnedOn() == true)
			light->update(dt);
	}

}

// function for render the Lights
void LightManager::render()
{
	// render each light of the collection
	for (Light* light : lights_)
	{
		light->render();
	}
}

vector<GLfloat> LightManager::getLightPosition(GLenum light_id)
{
	for (Light* light : lights_)
	{
		if (light->getLightId() == light_id)
		{
			return light->getPosition();
		}
	}

}

void LightManager::initLights()
{
	// enable lighting
	glEnable(GL_LIGHTING);

	// Components to create the Lights

	// Light ambient components
	Colour4 light_ambient_colour = { 0.2f, 0.2f, 0.2f, 1.0f}; // Ambient colours SHOULD NOT BE MORE THAN { 0.4f, 0.4f, 0.4f, 1.0f} in total (all the ambient colours light) because it could be too bright. Between 0.2 to 0.4 is right

	// Light diffuse components
	Colour4 light_diffuse_white_colour = { (unsigned int)0xFFFFFFFF };// = {1.0f, 1.0f, 1.0f, 1.0f} it has used hexadecimal just to show it is admited
	Colour4 light_diffuse_pomegranate_colour = { 240, 52, 52, 1, false }; // pomegranate colour (rgba will be transformed to opengl glut format by the constructure of the struct colour4)
	Colour4 light_diffuse_cabaret_colour = { 210, 82, 127, 1, false }; // cabaret colour
	Colour4 light_diffuse_froly_colour = { 255, 148, 120, 1, false }; // froly colour

	// Light specular components
	Colour4 light_specular_colour = {1.0f, 1.0f, 1.0f, 1.0f};
	Colour4 no_light_colour = { 0.0f, 0.0f, 0.0f, 1.0f };

	// Light emission components
	vector<GLfloat> light_position0 = { 7.0f, 7.0f, -9.0f, 1.0f };   // ambient + diffuse light
	vector<GLfloat> light_position1 = { 12.0f, 7.5f, -17.0f, 1.0f }; // spot light
	vector<GLfloat> light_position2 = { 18.0f, 5.5f, -9.0f, 1.0f };  // specular light

	// Values for spot light
	vector<GLfloat> spot_direction = { 0.0f, -1.0f, 0.0f }; // looking down down
	float spot_cutoff = 90.0f;
	float spot_exponent = 50.0f;

	// Attenuation components
	const GLfloat constant_attenuation = 1.0f; // default 1
	GLfloat linear_attenuantion = 0.25f; // default 0
	GLfloat quadratic_attenuantion = 1.0f; // default 0


	// Create lights:

	// Diffuse white light + ambient light
	lights_.push_back(new Light(GL_LIGHT0, light_ambient_colour, light_diffuse_white_colour, no_light_colour, light_position0));
	lights_.back()->setSpeed(10.5f);

	// spot light
	lights_.push_back(new Light(GL_LIGHT1, light_ambient_colour, light_diffuse_white_colour, no_light_colour, light_position1, spot_direction, spot_cutoff, spot_exponent));
	lights_.back()->setLightColourChangingOverTime(true);
	lights_.back()->setSpeed(10.5f);

	// light specular + attenuation
	lights_.push_back(new Light(GL_LIGHT2, light_ambient_colour, light_diffuse_white_colour, light_specular_colour, light_position2));
	lights_.back()->setAttenuation(constant_attenuation, linear_attenuantion, quadratic_attenuantion);
}
