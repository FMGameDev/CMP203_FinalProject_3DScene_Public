#include "MeshDisc.h"

MeshDisc::MeshDisc(float radius, int num_triangles) 
	: r_(radius), num_triangles_(num_triangles)
{
	initVertexAndNormalCoords();
}

MeshDisc::~MeshDisc()
{
}


void MeshDisc::initVertexAndNormalCoords()
{
	// Values to calculate the vertex and indices_
	float x, y, z = 0; // x and y values
	unsigned int v0, v1;
	float angle = 0; // angle around circle

	// One disc segments is made of one triangle
	// v1 is always the center of the disc 0,0,0
	// The first v0 is an outer vertex, v0+1 is the next outer vertex
	//  v0__v0 + 1
	//   |  / 
	//   | /
	//   v1

	// set the type of dereference to use and the mode depending on how the vertices, normals and indices are set in this function
	dereference_method_ = DereferenceMethod::kMethod3;
	mode_ = GL_TRIANGLES;

	/* The centre of the disc (inner vertex v1) */

	// (0,0,0) centre of the disc
	addVertex(0.0f,0.0f,0.0f); //v1
	addNormal(0.0f, 0.0f, +1.0f); // +z looking to as (backward)
	v1 = (unsigned int) 0; // the first vertex is in index 0

	/* Create the rest of the vertices_ */

	// using parametric equations of a circle to find the vertices_
	for (int i = 0, v0 = 0; i <= num_triangles_; i++, v0++)
	{
		// calculate angle
		angle = (2.0f * M_PI) * ((float)i / (float)num_triangles_);

		// calculate x, y, z for the vertex
		x = r_ * cos(angle);
		y = r_ * sin(angle);
		z = 0;

		// add the new vertex and its normal to the collection
		addVertex(x, y, z); //v0
		addNormal(0.0f, 0.0f, 1.0f);

		//Create the triangle
		if (i < num_triangles_) // avoid the first and last triangle
		{
			addTriangleIndices(v0, v1, v0 + 1);
		}
		else // the last triangle will be connected with the first triangle
		{
			addTriangleIndices(v0, v1, (unsigned int)1); // use the first outer vertex to close the circle
		}
	}
}

void MeshDisc::initTextureCoords()
{
	float u, v;
	float angle = 0; // angle around circle

	// using parametric equations of a circle to find the outer coords of the texture_ image 
	// The default texture_ coords 'u' and 'v' lie from 0 to 1, so if we are drawing a circle in the midddle point of the
	// center of the image texture_ (u,v) = (0.5, 0.5) and drawing a circle of diameter 1 (getting the max surface of the texture_ image).
	// so this points of 'u' and 'v' we obtain will be matched with the vertex points in our disc as we are using the same num_triangles_ (number of vertices_)
	///// Texture Image and circle diagram
	//  0---- 0.5----1 u
	//	|  -	   - |
	//  |-     .    -|
	//	|  -	   - |
	//  1 -----------
	//  v

	/* Create the first coord textures for the first the centre of the MeshDisc */
	u = 0.5f;
	v = 0.5f;
	addTexCoord(u,v); // centre point

	/* Create the texture_ coords for the outer vertices_ */
	for (int i = 0; i < num_triangles_; i++) // the last outer vertex is the first outer vertex created in the disc so it has already assigned a text coords
	{
		// calculate angle
		angle = (2.0f * M_PI) * ((float)i / (float)num_triangles_);

		/* Triangle (calculate texture_ coord for its 3 vertices_) */
		// u,v based on angle plus interval (next outer vertex)
		u = (cos(angle) / 2.0f) + 0.5f;
		v = -(sin(angle) / 2.0f) + 0.5f;
		addTexCoord(u,v);
	}
}