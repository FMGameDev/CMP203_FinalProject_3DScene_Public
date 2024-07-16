#include "MeshCone.h"

MeshCone::MeshCone(float base_radius, float top_radius, float height, int longitudinal_segments, int latitudinal_segments,
	bool has_top_disc, bool has_base_disc)
	: base_r_(base_radius), top_r_(top_radius), h_(height), longitudinal_segments_(longitudinal_segments), latitudinal_segments_(latitudinal_segments)
{
	initVertexAndNormalCoords(has_top_disc, has_base_disc);

}


MeshCone::~MeshCone()
{
	delete base_disc_;
	base_disc_ = nullptr;

	delete top_disc_;
	top_disc_ = nullptr;
}


void MeshCone::setTexture(Texture* common_texture)
{
	setSideTexture(common_texture);
	setTopAndBaseDiscTexture(common_texture);
}

void MeshCone::setTopAndBaseDiscTexture(Texture* base_top_texture)
{
	setBaseTexture(base_top_texture);
	setTopTexture(base_top_texture);
}

void MeshCone::setBaseTexture(Texture* base_texture)
{
	// check the shape has a base disc and if so set the texture_
	if (base_disc_ != nullptr)
	{
		base_disc_->setTexture(base_texture);
	}
}

void MeshCone::setTopTexture(Texture* top_texture)
{
	// check the shape has a top disc and if so set the texture_
	if (top_disc_ != nullptr)
	{
		top_disc_->setTexture(top_texture);
	}
}

void MeshCone::setSideTexture(Texture* side_texture)
{
	// initialise the texture_ coords in case there wasn't initialised
	if (texture_ == nullptr)
	{
		initSideTextureCoords();
	}

	// set the texture_ pointer to the object
	texture_ = side_texture;
}


void MeshCone::setColour(Colour4 colour)
{
	setSideColour(colour);
	setTopAndBaseColour(colour);
}

void  MeshCone::setTopAndBaseColour(Colour4 colour)
{
	setBaseColour(colour);
	setTopColour(colour);
}

void  MeshCone::setBaseColour(Colour4 colour)
{
	// check the shape has a base disc and if so set the texture_
	if (base_disc_ != nullptr)
	{
		base_disc_->setColour(colour);
	}
}

void  MeshCone::setTopColour(Colour4 colour)
{
	// check the shape has a top disc and if so set the texture_
	if (top_disc_ != nullptr)
	{
		top_disc_->setColour(colour);
	}
}

void  MeshCone::setSideColour(Colour4 colour)
{
	colour_ = colour;
}



void MeshCone::update(float dt)
{

	// move the side by the time if it is rotating (moving)
	if (is_rotating_[0]) // on the x axis
	{
		rotation_angles_.x += speed_ * dt;
	}
	if (is_rotating_[1]) // on the y axis
	{
		rotation_angles_.y += speed_ * dt;
	}
	if (is_rotating_[2]) // on the z axis
	{
		rotation_angles_.z += speed_ * dt;
	}

	// update the base disc if it has it
	if (base_disc_ != nullptr)
	{
		base_disc_->update(dt);
	}

	// update the top disc if it has it
	if (top_disc_ != nullptr)
	{
		top_disc_->update(dt);
	}
}

void MeshCone::render(bool is_shadow)
{
	// To Turn on Wireframe to draw the lines (for test proposed) 
	if (*shared_context_->wireframe_mode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draw lines
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/* Apply colour */
	if(!is_shadow)
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

		/* DRAWING THE SIDE*/
		glDrawElements(GL_TRIANGLES, (unsigned int)indices_.size(), GL_UNSIGNED_INT, indices_.data()); // method 3 of dereference

		/* DRAWING THE BASE DISC*/
		if (base_disc_ != nullptr)
		{
			base_disc_->render(is_shadow);
		}

		/* DRAWING THE BASE DISC*/
		if (top_disc_ != nullptr)
		{
			top_disc_->render(is_shadow);
		}

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


void MeshCone::setSharedContext(SharedContext* shared_context)
{
	/* Set shared context in the base disc*/
	if (base_disc_ != nullptr)
	{
		base_disc_->setSharedContext(shared_context);
	}

	/* Set shared context in the top disc*/
	if (top_disc_ != nullptr)
	{
		top_disc_->setSharedContext(shared_context);
	}

	// set wireframe for the laterals
	shared_context_ = shared_context;
}

void MeshCone::initVertexAndNormalCoords(bool has_top_disc, bool has_base_disc)
{
	float x, y=0, z; // x, y and z vertex coords
	float nx, ny, nz; // nx, ny and nz normals_
	float angle = 0; // angle around circle
	float r = base_r_; // start by the base radius
	float radiusInterval = abs(top_r_ - base_r_) / longitudinal_segments_;
	float longiInterval =  h_ / longitudinal_segments_; // for calculate the normal

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
		
		// calculate the indices_
		v0 = longiSeg * (latitudinal_segments_ + 1);
		v1 = v0 + (latitudinal_segments_ + 1); // it will be the next v0

		// second loop for latitude
		for (int latiSeg = 0; latiSeg <= latitudinal_segments_; latiSeg++, v0++, v1++)
		{
			// calculate angle
			angle = (2.0f * M_PI) * ((float)latiSeg / (float)latitudinal_segments_);

			// VERTEX - calculate x, y, and z values of vertex
			// x, y, z based on angle of 0 (first outer vertex)
			x = r * cos(angle); // x = r_*cos(0) = 1
			y = longiSeg * longiInterval; // x = r_*sin(0) = 0
			z = r * sin(angle);
			addVertex(x, y, z);

			// NORMAL - normalized vertex normals_
			nx = x / r;
			ny = y / r;
			nz = z / r;
			addNormal(nx, ny, nz);

			// indices_ - For side part of this shape
			if (longiSeg < longitudinal_segments_ && latiSeg < latitudinal_segments_) // avoid the first and last longitudinal segments values
			{
				// first triangle
				addTriangleIndices(v0, v1, v0 + 1);
				// second triangle
				addTriangleIndices(v0+1, v1, v1 + 1);		
			}
		}
		// calculate the next radius
		if (base_r_ > top_r_)
		{
			r -= radiusInterval;
		}
		else if (base_r_ < top_r_)
		{
			r += radiusInterval;
		}
	}

	// Create Base MeshDisc
	if (has_base_disc)
	{
		base_disc_ = new MeshDisc(base_r_, latitudinal_segments_);
		base_disc_->setRotationAngles({ 90.0f, 0.0f, 0.0f });
	}

	// Create Top MeshDisc
	if (has_top_disc)
	{
		top_disc_ = new MeshDisc(top_r_, latitudinal_segments_);
		top_disc_->setTranslation({ 0.0f, h_, 0.0f });
		top_disc_->setRotationAngles({ -90.0f, 0.0f, 0.0f });
	}
}

void MeshCone::initSideTextureCoords()
{
	float u, v; //coords for texture_

	/* Create texture_ for side coords*/
	// first loop for longitude
	for (int longiSeg = 0; longiSeg <= longitudinal_segments_; longiSeg++)
	{
		v = 1.0f - (float)longiSeg / longitudinal_segments_;

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