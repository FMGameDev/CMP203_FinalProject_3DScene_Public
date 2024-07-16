#include "MeshCube.h"

MeshCube::MeshCube(int dimension, bool coloured_by_default, RectangleBehaviourType rectangle_behaviour_type)
	: dimension_(dimension)
{
	/** create the six faces  with the normals looking outside of the cube*/
	// The default face plane is the bottom so the rest of the faces 
	// needs to be translate (set their position) here to define them as back, left etc

	// front face
	faces_[CubeFace::kFront] = new MeshPlane(Facing::kBackward, dimension, dimension, rectangle_behaviour_type); // it doesn't need to be translated

	// back face
	faces_[CubeFace::kBack] = new MeshPlane(Facing::kForward, dimension, dimension, rectangle_behaviour_type);
	faces_[CubeFace::kBack]->setTranslation({0.0f, (float)+dimension_, (float)-dimension_}); // translate it up and forward (to the back)
	
	// left face
	faces_[CubeFace::kLeft] = new MeshPlane(Facing::kLeft, dimension, dimension, rectangle_behaviour_type);
	faces_[CubeFace::kLeft]->setTranslation({ 0.0f, (float)+dimension_, 0.0f }); // translate it up

	// right face
	faces_[CubeFace::kRight] = new MeshPlane(Facing::kRight, dimension, dimension, rectangle_behaviour_type);
	faces_[CubeFace::kRight]->setTranslation({ (float)+dimension_, (float)+dimension_, (float)-dimension_ }); // translate it to the right, up and forward

	// top face
	faces_[CubeFace::kTop] = new MeshPlane(Facing::kUp, dimension, dimension, rectangle_behaviour_type);
	faces_[CubeFace::kTop]->setTranslation({ 0.0f, (float)+dimension_, 0.0f }); // translate it up

	// bottom face
	faces_[CubeFace::kBottom] = new MeshPlane(Facing::kDown, dimension, dimension, rectangle_behaviour_type); // by default, it doesn't need to be translated
	faces_[CubeFace::kBottom]->setTranslation({ 0.0f, 0.0f, (float)-dimension_ }); // translate it forward

	// set a color to each face if it is coloured by default (not white)
	if (coloured_by_default)
	{
		faces_[CubeFace::kFront]->setColour(Colour4{ 1.0f, 0.0f, 0.0f, 1.0f }); // red
		faces_[CubeFace::kBack]->setColour(Colour4{ 0.0f, 1.0f, 0.0f, 1.0f }); // green
		faces_[CubeFace::kLeft]->setColour(Colour4{ 0.0f, 0.0f, 1.0f, 1.0f }); // blue
		faces_[CubeFace::kRight]->setColour(Colour4{ 0.5f, 0.5f, 0.0f, 1.0f });
		faces_[CubeFace::kTop]->setColour(Colour4{ 0.5f, 0.0f, 0.5f, 1.0f });
		faces_[CubeFace::kBottom]->setColour(Colour4{ 0.0f, 0.5f, 0.5f, 1.0f });
	}
}

MeshCube::~MeshCube()
{
	// delete the pointers created in this class
	for (std::pair<CubeFace, MeshPlane*> face : faces_)
	{
		delete face.second;
		face.second = nullptr;
	}
}


void MeshCube::setFaceTexture(CubeFace cube_face, Texture* texture_)
{
	faces_[cube_face]->setTexture(texture_);
}

void MeshCube::setTexture(Texture* texture)
{
	// for mapped texture_
	if (texture != nullptr && texture->getTextureCoordsType() == TextureCoordsType::kMapped)
	{
		// front face
		faces_[CubeFace::kFront]->setTexture(texture, 0.25f, 0.25f, 0.5f, 0.5f);

		// back face
		faces_[CubeFace::kBack]->setTexture(texture, 0.75f, 0.25f, 1.0f, 0.5f);

		// left face
		faces_[CubeFace::kLeft]->setTexture(texture, 0.0f, 0.25f, 0.25f, 0.5f);

		// right face
		faces_[CubeFace::kRight]->setTexture(texture, 0.5f, 0.25f, 0.75f, 0.5f);

		// top face
		faces_[CubeFace::kTop]->setTexture(texture, 0.25f, 0.0f, 0.5f, 0.25f);

		// bottom face
		faces_[CubeFace::kBottom]->setTexture(texture, 0.25f, 0.5f, 0.5f, 0.75f);

	}
	else
	{
		// set the same texture_ to all the faces in the cube
		for (std::pair<CubeFace, MeshPlane*> face : faces_)
		{
			face.second->setTexture(texture);
		}
	}
	// set texture for the container
	texture_ = texture;
}

void MeshCube::setFaceColor(CubeFace cube_face, Colour4 colour)
{
	faces_[cube_face]->setColour(colour);
}

void MeshCube::setAColourToAllFaces(Colour4 colour)
{
	// set the color to all the faces in the cube
	for (std::pair<CubeFace, MeshPlane*> face : faces_)
	{
		face.second->setColour(colour);
	}
}

int MeshCube::getDimension() const
{
	return dimension_;
}


/*void MeshCube::setIsRotating(bool rotation_x, bool rotation_y, bool rotation_z)
{
	is_rotating_[0] = rotation_x; // set rotation flag for X Axis 
	is_rotating_[1] = rotation_y; // set rotation flag for Y Axis 
	is_rotating_[2] = rotation_z; // set rotation flag for Z Axis 
}

void MeshCube::setRotationAngles(Vector3 rotation_angles)
{
	rotation_angles_ = rotation_angles; 
}

void MeshCube::setTranslation(Vector3 translation)
{
	translation_ = translation;
}

void MeshCube::setSpeed(float speed)
{
	speed_ = speed;
}*/


void MeshCube::render(bool is_shadow)
{
	// remember where we are
	glPushMatrix();

		// translate the cube
		glTranslatef(translation_.x, translation_.y, translation_.z);

		// rotate on the x axis
		glRotatef(rotation_angles_.x, 1.0f, 0.0f, 0.0f);
		// rotate on the y axis
		glRotatef(rotation_angles_.y, 0.0f, 1.0f, 0.0f);
		// rotate on the z axis
		glRotatef(rotation_angles_.z, 0.0f, 0.0f, 1.0f);

		// scale the object
		glScalef(scale_.x, scale_.y, scale_.z);

		// remember where we are
		glPushMatrix();

			// set the point of gravitation in the center of the cube if the cube is rotating in y-axis
			// for rotating the cube itself, this will set the y-axis in the middle if the cube
			if (is_rotating_[1])
				glTranslatef(-dimension_ / 2.0f, 0.0f, dimension_ / 2.0f);

			// draw all the faces of the cube
			for (const std::pair<CubeFace, MeshPlane*> face : faces_)
			{	
				face.second->render(is_shadow);
			}

		// go back where we were
		glPopMatrix();

	// go back where we were
	glPopMatrix();
}

void MeshCube::setSharedContext(SharedContext* shared_context)
{
	/* Set shared context in all the faces*/
	for (const std::pair<CubeFace, MeshPlane*> face : faces_)
	{
		face.second->setSharedContext(shared_context);
	}

	// set context the cube itself
	shared_context_ = shared_context;
}

BaseMesh* MeshCube::clone() const
{
	return new MeshCube(*this);
}