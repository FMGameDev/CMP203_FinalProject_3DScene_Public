// Colour4 structure
// Contain a structure with an array with the rgba colour in an openGL format
// It has two different constructors one for transforming from rgba and other for transforming from 8digits-hexadecimal color 
// For the possible alpha values can be used the guide: https://gist.github.com/lopspower/03fb1cc0ac9f32ef38f4
// The sintaxis for the 8-digits hexadecimal colour has been collected from: https://www.geeksforgeeks.org/css-8-digit-hex-color/
// @author Francisco Diaz

#ifndef _COLOUR4_H_
#define _COLOUR4_H_

#include <stdio.h>

using namespace std;

// Container the rgba values in openGL format (from 0 to 1)
// red = rgba[0], green = rgba[1], blue = rgba[2], alpha = rgba[3]
struct Colour4
{
	// constructor for rgba format
	// by default the parameter passed are in openGL format (values between 0-1) isOpenGLFormat == true
	// if those values are in normal rgba format (0-255 for rgb and 0-1 for alpha) then 'isOpenGLFormat' must be set to false
	Colour4(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f, bool isOpenGLFormat = true)
	{
		// translate to openGL format if necessary
		if (!isOpenGLFormat)
		{
			// rgb colours are divided by 255 because the normal values are between 0 to 255 and openGL works between 0 to 1
			// we don't need to transform alpha because the normal alpha value in rgba format is already between 0 to 1
			r /= 255.0f;
			g /= 255.0f;
			b /= 255.0f;
		}

		// save the rgba values in the array
		this->rgba[0] = r;
		this->rgba[1] = g;
		this->rgba[2] = b;
		this->rgba[3] = a;
	}
	// constructor for 8-digits hexadecimal. Syntax: 0xRRGGBBAA
	Colour4(unsigned int hex)
	{
		// Extract RR, GG, BB bytes and translate to the rgba usual format:
		// rgb values between 0 to 255 and alpha between 0 to 1
		this->rgba[0] = ((hex >> 24) & 0xFF);  // red
		this->rgba[1] = ((hex >> 16) & 0xFF);  // green
		this->rgba[2] = ((hex >> 8) & 0xFF);   // blue
		this->rgba[3] = ((hex) & 0xFF) / 255.0f; // alpha

		// Translate the rgb values to the opengl format dividing them by 255 
		// because the normal rgb values are between 0 to 255 and openGL works between 0 to 1
		this->rgba[0] /= 255.0f; // r
		this->rgba[1] /= 255.0f; // g
		this->rgba[2] /= 255.0f; // b
	}

	// rgba values container in openGL format (values from 0 to 1)
	float rgba[4]{ 1.0f };
};

#endif
