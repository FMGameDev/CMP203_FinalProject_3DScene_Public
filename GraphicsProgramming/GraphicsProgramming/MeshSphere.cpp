#include "MeshSphere.h"


MeshSphere::MeshSphere(float radius, int longitudinal_segments, int latitudinal_segments)
	: r_(radius), longitudinal_segments_(longitudinal_segments), latitudinal_segments_(latitudinal_segments)
{
	initVertexAndNormalCoords();
}

MeshSphere::~MeshSphere()
{

}

void MeshSphere::initVertexAndNormalCoords()
{
	float x, y, z; // x, y and z vertex coords
	float nx, ny, nz; // nx, ny and nz normals_
	float lengthInv = 1.0f / r_; // for calculate the normal
	float longitudeAngle; // longitude is vertically
	float latitudeAngle; // latitude is horizontally

	unsigned int v0, v1;

	// Each latitudinal Segment is made of two triangles:
	//
	// v0____v0 + 1
	//  |  / |
	//  | /  |
	// v1----v1 + 1
	// the first triangle is made of the points v0, v1 and v0+1
	// the second triangle is made of the points v0+1, v1 and v1+1

	// set the type of dereference to use and the mode depending on how the vertices, normals and indices are set in this function
	dereference_method_ = DereferenceMethod::kMethod3;
	mode_ = GL_TRIANGLES;

	// first loop for longitude
	for (int longiSeg = 0; longiSeg <= longitudinal_segments_; longiSeg++)
	{
		// calculate the longitudeAngle
		longitudeAngle = (M_PI / 2.0f) - (longiSeg * (M_PI / ((float)longitudinal_segments_))); // latitude is horizontally

		// calculate the indices_
		v0 = longiSeg * (latitudinal_segments_ + 1);
		v1 = v0 + (latitudinal_segments_ + 1);

		// 'y' and 'ny' values and the first part of 'x' and 'z' values ((r_ * cos(longitudeAngle)) could be calculated here to improve the performance of the program
		// I have decided to calculate it in the the second loop as it is more understandble for the propose of this proyect

		// second loop for latitude
		for (int latiSeg = 0; latiSeg <= latitudinal_segments_; latiSeg++, v0++, v1++)
		{
			latitudeAngle = latiSeg * ((2.0f * M_PI) / ((float)latitudinal_segments_)) ; // longitude is vertically

			// VERTEX - calculate x, y, and z values of vertex
			x = (r_ * cos(longitudeAngle)) * cos(latitudeAngle);
			y = r_ * sin(longitudeAngle);
			z = (r_ * cos(longitudeAngle)) * sin(latitudeAngle);
			addVertex(x, y, z);

			// NORMAL - normalized vertex normals_
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			addNormal(nx, ny, nz);

			// INDICES
			// first triangle
			if (longiSeg != 0 && longiSeg != longitudinal_segments_) // avoid the first and last longitudinal segments values
			{
				addTriangleIndices(v0, v1, v0 + 1);
			}
			// second triangle
			if(longiSeg < longitudinal_segments_ - 1) // avoid the last two longitudinal segments values
			{
				addTriangleIndices(v0 + 1, v1, v1 + 1);
			}
		}
	}
}

void MeshSphere::initTextureCoords()
{
	float u, v; //coords for texture_

	// first loop for longitude
	for (int longiSeg = 0; longiSeg <= longitudinal_segments_; longiSeg++)
	{
		v = (float)longiSeg / longitudinal_segments_;

		// second loop for latitude
		// as we switched z and y in the parametric equations we need this loop going in opossite way we created the vertex
		for (int latiSeg = latitudinal_segments_; latiSeg >= 0; latiSeg--)
		{
			// TEXTURE
			u = (float)latiSeg / latitudinal_segments_;
			addTexCoord(u, v);
		}
	}
}