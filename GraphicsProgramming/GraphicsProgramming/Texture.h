// Texture Class 
// It is used to load a texture(image) and create a texture object with it
// @author Francisco Diaz (FMGameDev)

#pragma once

// Include GLUT, openGL, input.
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <fstream> // printf

#include "SOIL.h"

// Define the type of texture coords this texture has.
// It is mainly used for the cube (planes-rectangles) based on this parameter the classes initialise their texture coords
// If a texture is loaded from a model then use kDefault as the model has already the texture coords 
enum class TextureCoordsType
{
	kDefault, // the coords for the texture will take the full image coords (from 0 to 1) for 'u' and 'v'
	kMapped, // The texture is mapped so coord must follow a specific coords (point of start and ending are not 0 to 1), the texture_ takes only a part of the full texture_ img
};


class Texture
{
public:
	// constructor
	Texture(const char texture_url[], TextureCoordsType texture_coords_type = TextureCoordsType::kDefault, bool y_inverted = false); // by default the texture_ will take the full image coords

	// destructor
	~Texture();

	// return the texture_ coords type
	TextureCoordsType getTextureCoordsType() const;

	// function for using this texture (bind texture)
	void use();

	// function to stop using this texture
	void stopUsing();

	// fucntion for set the wrap_s and wrap_t
	void setWrapST(GLint wrap_s, GLint wrap_t);

private:
	// texture component
	GLuint texture_;

	// type of coords must follow the texture, respect the image
	TextureCoordsType texture_coords_type_;

	// components to identified if the texture_ is repeat, mirror, clamp
	// Possible values:
	// GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP, GL_CALMP_TO_EDGE, GL_CLAMP_TO_BORDER..
	GLint wrap_s_ = GL_CLAMP; // by default repeat ('s' is like 'x')
	GLint wrap_t_ = GL_CLAMP; // by default repeat ('t' is like 'y')
};

