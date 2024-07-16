// Shadow class
// This class contain functions to create Planar Shadows and Shadow Volume.
// The function "generateShadowMatrix" which is to create a planar shadow has been modified so instead of taking a full floor vertices it takes
// the PRQ coords (vertices) of a plane surface (the corners). This allow a more dynamic use of this function
// Proportioned by Paul Robertson and modifed by Francisco Diaz (@FMGameDev)

#pragma once

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>
#include "Vector3.h"

class Shadow
{
public:
	// Function to create a Planar Shadow
	static void generateShadowMatrix(float* shadowMatrix, float light_pos[4], GLfloat PRQ_vertices[9]);

	// Create a Shadow Volume
	static std::vector<float> buildShadowVolume(float lightPosit[4], std::vector<float> verts);
	static void extendVertex(float newVert[3], float lightPosit[4], float x, float y, float z, float ext); // used in shadow volume function
};