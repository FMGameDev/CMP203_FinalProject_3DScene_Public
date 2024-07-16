// Class for Materials
// It creates a material object to apply it on objects
// @author Francisco Diaz (FMGameDev)

#pragma once

// Include GLUT, openGL, input.
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Colour4.h"

class Material
{
public:

	// custom constructor
	Material(GLenum mat_face, Colour4 mat_ambient_colour, Colour4 mat_diffuse_colour, Colour4 mat_specular_colour, Colour4 emission_colour, GLfloat shininess);

	// destructor
	~Material();

	// set the material components
	void render();

	// decrease the shininess substracting the number passed;
	void decreaseShininessBy(GLfloat number_to_substract);

	// increase the shininess adding the number passed
	void increaseShininessBy(GLfloat number_to_add);

private:
	// face where it is going to be applied the material components
	GLenum material_face_ = GL_FRONT; // default

	// MATERIAL COMPONENTS

	// material ambient component
	Colour4 material_ambient_colour_ = { 0.2f, 0.2f, 0.2f, 1.0f }; // default value
	// material diffuse component
	Colour4 material_diffuse_colour_ = { 0.8f, 0.8f, 0.8f, 1.0f }; // default value
	// material specular component
	Colour4 material_specular_colour_ = { 0.0f, 0.0f, 0.0f, 1.0f }; // default value


	// emission component for the object
	Colour4 emission_colour_ = { 0.0f, 0.0f, 0.0f, 1.0f }; // default value
	// shininess component for the object
	GLfloat shininess_ = 0.0f; // default value

};