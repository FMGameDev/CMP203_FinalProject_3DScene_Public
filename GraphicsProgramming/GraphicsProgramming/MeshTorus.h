
// Class for MeshTorus
// It extendes from BaseMesh and it create a torus.
// This shape characteristics/form depends on the parameter passed on the constructor.
// A torus can be used for creating donuts, cereals, some rings...
// For vertex and normals it has been used the equations of https://web.cs.ucdavis.edu/~amenta/s12/findnorm.pdf
// @author Francisco Diaz (FMGameDev)

#pragma once
#ifndef TORUS_H
#define TORUS_H

#define _USE_MATH_DEFINES // for using pi

#include "BaseMesh.h"

class MeshTorus : public BaseMesh
{
public:
	// constructor
	MeshTorus(float minor_radius = 1.1f, float major_radius = 2.1f, int num_tube_faces = 200, int num_rings = 400); // by default the shape only has the sides

	// destructor
	~MeshTorus();

	// draw the shape
	void render(bool is_shadow = false) override;

	BaseMesh* clone() override;

private:

	/** SPECIFIC CHARACTERISTICS OF THIS MESH */

	// radius and number of segments of the sphere the shape
	float r_; // radius of the tube (minor Radius)
	float R_; // distance from the center of the tube to the center of the torus

	// Number of segments it is divided the shape
	int num_rings_, num_tube_faces_;


	/* FUNCTIONS FOR GENERATE THE MESH [ARRAYS COORDS (VERTICES, NORMALS, TEXTURES AND INDICES)] */

	// initialise arrays of coords for vertex, coords and index
	void initVertexAndNormalCoords();

	// initialise arrays of textures coords
	void initTextureCoords();
};

#endif