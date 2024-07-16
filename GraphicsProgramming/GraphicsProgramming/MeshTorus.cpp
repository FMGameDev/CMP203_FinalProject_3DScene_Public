#include "MeshTorus.h"


MeshTorus::MeshTorus(float minor_radius, float major_radius, int tubeFaces, int num_rings)
	: r_(minor_radius), R_(major_radius), num_tube_faces_(tubeFaces), num_rings_(num_rings)
{
	initVertexAndNormalCoords();
}


MeshTorus::~MeshTorus()
{
}


void MeshTorus::render(bool is_shadow)
{
	// To Turn on Wireframe to draw the lines (for test proposed) 
	if (*shared_context_->wireframe_mode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draw lines
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/* Apply colour */
	if (!is_shadow)
		glColor4f(colour_.rgba[0], colour_.rgba[1], colour_.rgba[2], colour_.rgba[3]); // (All the vertices will have the same colour)
	else
		glColor4f(0.1f, 0.1f, 0.1f, 1.0f);

	/* Apply texture_ if this object has one */
	if (texture_ != nullptr && !is_shadow)
	{
		texture_->use();
	}

	/* Enable Arrays of coords (vertices_, normals_, textures and index) and link them */

	// 1st: Tell openGL what information we have (enable arrays)
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	if (texture_ != nullptr && !is_shadow)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	// 2nd: Tell openGL what data and how it is arranged
	glVertexPointer(3, GL_FLOAT, 0, vertices_.data());
	glNormalPointer(GL_FLOAT, 0, normals_.data());
	if (texture_ != nullptr && !is_shadow)
	{
		glTexCoordPointer(2, GL_FLOAT, 0, texture_coords_.data());
	}

	/* Draw the shape using the arrays */

	// remember where we are
	glPushMatrix();

		// translate the shape
		glTranslatef(translation_.x, translation_.y, translation_.z);

		// rotate on the x axis
		glRotatef(rotation_angles_.x, 1.0f, 0.0f, 0.0f);
		// rotate on the y axis
		glRotatef(rotation_angles_.y, 0.0f, 1.0f, 0.0f);
		// rotate on the z axis
		glRotatef(rotation_angles_.z, 0.0f, 0.0f, 1.0f);

		// scale the shape
		glScalef(scale_.x, scale_.y, scale_.z);

		/* DRAW */
		glDrawElements(GL_TRIANGLES, (unsigned int)indices_.size(), GL_UNSIGNED_INT, indices_.data()); // method 3 of dereference

	// go back where we were
	glPopMatrix();


	/* Diable Arrays of coords (vertices_, normals_, textures and index)*/

	// 4rd: Tell openGL don't use more the information (disable arrays)
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	if (texture_ != nullptr && !is_shadow) // disable texture_ coords if the sahape has one assigned
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	/* Stop using the texture_ if this object has one*/
	if (texture_ != nullptr && !is_shadow)
	{
		texture_->stopUsing();
	}

	/* reset to white colour */
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

BaseMesh* MeshTorus::clone()
{
	return new MeshTorus(*this);
}

void MeshTorus::initVertexAndNormalCoords()
{
	float x, y, z; // x, y and z vertex coords
	float nx, ny, nz; // nx, ny and nz normals_
	float angleRing = 0; // angle around circle
	float angleTube = 0;

	float tx, ty, tz; // tangent vector with respect angleRing
	float sx, sy, sz; // tangent vector with respect angle tube
	float length; // used to normalised normals_

	float u,v;
	float s,t;

	unsigned int v0, v1;

	// Each latitudinal Segment is made of two triangles:
	//
	// v0____v0 + 1
	//  |  / |
	//  | /  |
	// v1----v1 + 1
	// the first triangle is made of the points v0, v1 and v0+1
	// the second triangle is made of the points v0+1, v1 and v1+1

	// first loop for num_rings
	for (int ring = 0; ring <= num_rings_; ring++)
	{
		// calculate angle
		angleRing = static_cast<float>((2.0f * M_PI) * ((float)ring / (float)num_rings_));

		// calculate the indices_
		v0 = ring * (num_tube_faces_ + 1);
		v1 = v0 + (num_tube_faces_ + 1); // it will be the next v0

		// second loop a circle around the ring
		for (int tubeFace = 0; tubeFace <= num_tube_faces_; tubeFace++, v0++, v1++)
		{
			// calculate angle the tube
			angleTube = static_cast<float>((2.0f * M_PI) * ((float)tubeFace/ (float)num_tube_faces_));

			// VERTEX - calculate x,y,z
			// It has set negative cosines of 'x' and 'y' order to start the torus from the middle left of the circle
			// so the texture_ limits will be in the inner circle instead of the outer circle of the torus (the line of the texture_ is almost hidden inside of the torus)
			// and the texture_ will go from left to bottom to right to top and close in left (horizontally speaking)
			x = (R_ + r_ * cos(angleTube)) * cos(angleRing); // x = (R_ + r_ * -cos(angleTube)) * -cos(angleRing);
			y = (R_ + r_ * cos(angleTube)) * sin(angleRing); // y = (R_ + r_ * -cos(angleTube)) * sin(angleRing);
			z = r_ * sin(angleTube);
			addVertex(x, y, z);

			// NORMAL - normalized vertex normals_
			// calculate tx,ty,tz (tangent respect ring circle)
			tx = -sin(angleRing);
			ty = cos(angleRing);
			tz = 0;
			// calculate sx,sy,sz (tangent respect tube circle)
			sx = cos(angleRing) * (-sin(angleTube));
			sy = sin(angleRing) * (-sin(angleTube));
			sz = cos(angleTube);
			// calculate normal which is cross-product of tangents (t,s)
			nx = ty*sz - tz*sy;
			ny = tz*sx - tx*sz;
			nz = tx*sy - ty*sx;
			// normalize normal
			length = sqrt((nx * nx) + (ny * ny) + (nz * nz));
			nx = nx / length;
			ny = ny / length;
			nz = nz / length;
			addNormal(nx, ny, nz);

			// INDICES - For side part of this shape
			if (ring < num_rings_ && tubeFace < num_tube_faces_) // avoid the first and last longitudinal segments values
			{
				// first triangle
				addTriangleIndices(v0, v1, v0 + 1);
				// second triangle
				addTriangleIndices(v0 + 1, v1, v1 + 1);
			}
			
		}
	}

}

void MeshTorus::initTextureCoords()
{
	float u, v; //coords for texture_

	/* Create texture_ coords*/

	// first loop for the ring
	for (int ring = 0; ring <= num_rings_; ring++)
	{
		//for (int ring = num_rings_; ring >= 0; ring--) // for inverting v values
		//{
		// calculate v
		u = 1.0f - (float)ring / (float)num_rings_;

		//for (int tubeFace = 0; tubeFace <= num_tube_faces_; tubeFace++) // for inverting u values
		//{
		// second loop for the tube
		for (int tubeFace = num_tube_faces_; tubeFace >= 0; tubeFace--) // loop in case of the circle made from right to left
		{
			// calculate u
			v = (float)tubeFace / num_tube_faces_;

			// add texture_
			addTexCoord(u, v);
		}
	}
}