// Class sphere
// It extends BaseMesh and it creates a sphere depending on the parameters passed in the constructor
// The geometry used for creating this sphere are two triangles which form a quad
// To create this class I have followed the guide: http://www.songho.ca/opengl/gl_sphere.html by Songo Ho Ahn @email song.ahn@gmail.com
// @author Francisco Diaz (FMGameDev)

#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#define _USE_MATH_DEFINES // for using pi

// Class for MeshSphere Mesh
// @author Francisco Diaz (FMGameDev)

#include "BaseMesh.h"

class MeshSphere : public BaseMesh
{
public:
	// constructor
	MeshSphere(float radius = 1.0f, int longitudinal_segments = 50, int latitudinal_segments = 50);

	// destructor
	~MeshSphere();

private:

	/** SPEFICIC CHARACTERISTICS OF THIS SQUARE */

	// radius and number of segments of the sphere the shape
	float r_;

	// Number of segments it has the sphere, it is used to create the vertices_, normals_, texture_ coords and indices_. 
	int longitudinal_segments_; // Vertically speaking
	int latitudinal_segments_; // Horizontally speaking


	/* ARRAYS COORDS (VERTICES, NORMALS, TEXTURES AND INDICES), COMPONENTS AND FUNCTIONS */

	// initialise arrays of coords for vertex, coords and index
	void initVertexAndNormalCoords();

	// initialise arrays of textures coords
	void initTextureCoords();
};

#endif

