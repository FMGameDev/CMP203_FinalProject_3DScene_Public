
// Class for Disc Mesh
// It extends BaseMesh, creating a disc, creating different shapes depending on the number of triangles
// It can be used as a cover/lid, base, in the mirror world, etc
// @author Francisco Diaz (FMGameDev)

#pragma once
#ifndef DISC_H
#define DISC_H

// Include GLUT, openGL, input.
#include "BaseMesh.h"

class MeshDisc : public BaseMesh
{
public:
	// constructor
	MeshDisc(float radius = 1.0f, int num_triangles = 20);

	// destructor
	~MeshDisc();

private:

	/** SPECIFIC CHARACTERISTICS OF THIS MESH */
	
	// radius and number of triangles of the disc
	float r_; //radius
	int num_triangles_; // it is the same as the number of outer vertices it has (without counting the center of the disc)


	/* FUNCTIONS FOR GENERATE THE MESH [ARRAYS COORDS (VERTICES, NORMALS, TEXTURES AND INDICES)] */

	// initialise arrays of coords for vertex, coords and index
	void initVertexAndNormalCoords() override;

	// initialise arrays of textures coords
	void initTextureCoords() override;
};

#endif