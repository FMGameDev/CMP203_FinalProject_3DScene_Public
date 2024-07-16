#include "BaseMesh.h"

BaseMesh::BaseMesh()
{
	// by default the object doesn't have texture and it doesn't use triangle, squads, etc...
	texture_ = nullptr;
	mode_ = GL_NONE;
}


BaseMesh::~BaseMesh()
{
}

void BaseMesh::setTexture(Texture* texture)
{

	// initialise the texture coords in case there wasn't initialised
	if (texture_ == nullptr)
	{
		// initialise the texture_ coords with the new texture_
		initTextureCoords();
	}

	// set the texture pointer to the object
	texture_ = texture;
}


void BaseMesh::setColour(Colour4 colour)
{
	colour_ = colour;
}


void BaseMesh::setScale(Vector3 scale)
{
	scale_ = scale;
}

void BaseMesh::invertVertically()
{
	scale_.y *= -1.0f;
}

void BaseMesh::invertHorizontally()
{
	scale_.x *= -1.0f;
}

void BaseMesh::invertZ()
{
	scale_.z *= -1.0f;
}

void BaseMesh::setMode(GLenum mode)
{
	mode_ = mode;
}

void BaseMesh::setDereferenceMethod(DereferenceMethod dereference_method)
{
	dereference_method_ = dereference_method;
}


void BaseMesh::setIsRotating(bool rotation_x, bool rotation_y, bool rotation_z)
{
	is_rotating_[0] = rotation_x; // set rotation flag for X Axis 
	is_rotating_[1] = rotation_y; // set rotation flag for Y Axis 
	is_rotating_[2] = rotation_z; // set rotation flag for Z Axis 
}

void BaseMesh::setIsRotating(vector<bool> is_rotating)
{
	is_rotating_ = is_rotating;
}


void BaseMesh::setRotationAngles(Vector3 rotation_angles)
{
	rotation_angles_ = rotation_angles;
}

Vector3 BaseMesh::getRotationAngles() const
{
	return rotation_angles_;
}

void BaseMesh::setTranslation(Vector3 translation)
{
	translation_ = translation;
}

void BaseMesh::setTranslationX(float translation_x)
{
	translation_.x = translation_x;
}

void BaseMesh::setTranslationY(float translation_y)
{
	translation_.y = translation_y;
}

void BaseMesh::setTranslationZ(float translation_z)
{
	translation_.z = translation_z;
}

Vector3 BaseMesh::getTranslation() const
{
	return translation_;
}

void BaseMesh::setIsMoving(vector<bool> is_moving)
{
	is_moving_ = is_moving;
}

void BaseMesh::setDirection(Vector3 direction)
{
	direction_ = direction;
}

void BaseMesh::setSpeed(float speed)
{
	speed_ = speed;
}

void BaseMesh::setAxisLimits(AxisLimits axis_limits)
{
	axis_limits_ = axis_limits;
}


void BaseMesh::update(float dt)
{

	/* rotate automatically around x, y and/or z axis */ 
	if (is_rotating_[0]) // on the x axis
	{
		rotation_angles_.x += (speed_ * dt);
	}
	if (is_rotating_[1]) // on the y axis
	{
		rotation_angles_.y += (speed_ * dt);
	}
	if (is_rotating_[2]) // on the z axis
	{
		rotation_angles_.z += (speed_ * dt);
	}

	/* move automatically along x, y and/or z axis */

	if (is_moving_[0]) // along x-axis
	{
		if (translation_.x >= axis_limits_.max.x && direction_.x == +1.0f)
		{
			rotation_angles_.y -= 180.0f; // turn around
			direction_.x = -1.0f; // change direction to the opposite way
		}
		else if (translation_.x <= axis_limits_.min.x && direction_.x == -1.0f)
		{
			rotation_angles_.y += 180.0f; // turn around
			direction_.x = 1.0f; // change direction to the opposite way
		}

		translation_.add(direction_, speed_ * dt);
	}
	if (is_moving_[1]) // along y-axis
	{
		if (translation_.y >= axis_limits_.max.y && direction_.y == +1.0f)
		{
			rotation_angles_.z -= 180.0f; // turn around
			direction_.y = -1.0f; // change direction to the opposite way
		}
		else if (translation_.y <= axis_limits_.min.y && direction_.y == -1.0f)
		{
			rotation_angles_.z += 180.0f; // turn around
			direction_.y = 1.0f; // change direction to the opposite way
		}

		translation_.add(direction_, speed_ * dt);
	}
	if (is_moving_[2]) // along z-axis
	{
		if (translation_.z >= axis_limits_.max.z && direction_.z == +1.0f)
		{
			rotation_angles_.y -= 180.0f; // turn around
			direction_.z = -1.0f; // change direction to the opposite way
		}
		else if (translation_.z <= axis_limits_.min.z && direction_.z == -1.0f)
		{
			rotation_angles_.y += 180.0f; // turn around
			direction_.z = 1.0f; // change direction to the opposite way
		}

		translation_.add(direction_, speed_ * dt);
	}
	
}

void BaseMesh::render(bool is_shadow)
{
	// To Turn on Wireframe to draw the lines (for test proposed) 
	if(*shared_context_->wireframe_mode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draw lines
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/* Apply colour */
	if (!is_shadow)
		glColor4f(colour_.rgba[0], colour_.rgba[1], colour_.rgba[2], colour_.rgba[3]); // (All the vertices_ will have the same colour)
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

		// scale the object
		//glScalef(scale_.x, scale_.y, scale_.z);
		
		// translate the square
		glTranslatef(translation_.x, translation_.y, translation_.z);

		// rotate on the x axis
		glRotatef(rotation_angles_.x, 1.0f, 0.0f, 0.0f);
		// rotate on the y axis
		glRotatef(rotation_angles_.y, 0.0f, 1.0f, 0.0f);
		// rotate on the z axis
		glRotatef(rotation_angles_.z, 0.0f, 0.0f, 1.0f);

		// scale the object
		glScalef(scale_.x, scale_.y, scale_.z);


		/* DRAW */
		// Method 1
		if (dereference_method_ == DereferenceMethod::kMethod1)
		{
			glBegin(mode_);
				for (GLint i = 0; i < (int)vertices_.size()/3; i++)
				{
					glArrayElement(i);
				}
			glEnd();
		}
		// Method 2
		else if (dereference_method_ == DereferenceMethod::kMethod2)
		{
			glDrawArrays(mode_, 0, (int)vertices_.size()/3);
		}
		// Method 3
		else if (dereference_method_ == DereferenceMethod::kMethod3)
		{
			glDrawElements(mode_, (unsigned int)indices_.size(), GL_UNSIGNED_INT, indices_.data()); // method 3 of dereference
		}

		// render all the submeshes of this mesh (hierarchical)
		for (BaseMesh* submesh : submeshes_)
		{
			submesh->render(is_shadow);
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


BaseMesh* BaseMesh::clone() const
{
	return new BaseMesh(*this);
}

void BaseMesh::initVertexAndNormalCoords()
{
}

void BaseMesh::initTextureCoords()
{
}

BaseMesh* BaseMesh::clone()
{
	return new BaseMesh(*this);
}


void BaseMesh::copyMovementComponents(BaseMesh* base_shape_to_copy)
{
	translation_ = base_shape_to_copy->getTranslation();

	rotation_angles_ = base_shape_to_copy->getRotationAngles();
}


void BaseMesh::addVertex(float x, float y, float z)
{
	vertices_.push_back(x);
	vertices_.push_back(y);
	vertices_.push_back(z);
}

void BaseMesh::addNormal(float nx, float ny, float nz)
{
	normals_.push_back(nx);
	normals_.push_back(ny);
	normals_.push_back(nz);
}

void BaseMesh::addTexCoord(float u, float v)
{
	texture_coords_.push_back(u);
	texture_coords_.push_back(v);
}

void BaseMesh::addTriangleIndices(unsigned int i0, unsigned int i1, unsigned int i2)
{
	indices_.push_back(i0);
	indices_.push_back(i1);
	indices_.push_back(i2);
}

void BaseMesh::setSharedContext(SharedContext* shared_context)
{
	shared_context_ = shared_context;
}

void BaseMesh::addSubmesh(BaseMesh* submesh)
{
	submeshes_.push_back(submesh);
}