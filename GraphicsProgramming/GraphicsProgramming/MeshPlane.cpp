#include "MeshPlane.h"

MeshPlane::MeshPlane(Facing facing, int height, int width, RectangleBehaviourType rectangle_behaviour_type)
	: facing_(facing), MeshRectangle(height, width, rectangle_behaviour_type)
{
	// declare the rectangle object
	rectangle_ = new MeshRectangle(height, width, rectangle_behaviour_type);

	// up facing
	if (facing_ == Facing::kUp)
	{
		// n(0, +1, 0)
		//				 R---------------------------------- v(x= +width, y=0, z= -height)   
		//				 | \     /\      /| \     /\      /| 
		//				 |  \   /  \    / |  \   /  \    / |
		//				 |   \ /    \  /  |   \ /    \  /  |
		//				 -----------------------------------
		//				 |   / \     /\   |   / \     /\   |
		//				 |  /   \   /  \  |  /   \   /  \  |   /\ -z
		//				 | /     \ /    \ | /     \ /    \ |   ||
		// v(x=0,y=0,z=0)P---------------------------------Q 
		//			      ==> +x
	}

	// down facing
	else if (facing_ == Facing::kDown)
	{
		// n(0, -1, 0)
		//					==> +x
		// v(x=0,y=0,z=0)P---------------------------------Q  ||
		//				 | \     /\      /| \     /\      /|  \/ +z
		//				 |  \   /  \    / |  \   /  \    / |
		//				 |   \ /    \  /  |   \ /    \  /  |
		//				 -----------------------------------
		//				 |   / \     /\   |   / \     /\   |
		//				 |  /   \   /  \  |  /   \   /  \  |
		//				 | /     \ /    \ | /     \ /    \ |   
		//				 R---------------------------------- v(x= +width, y= 0, z= +height)  
		rectangle_->setRotationAngles({ -180.0f, 0.0f, 0.0f });
	}

	// backward facing (looking at us)
	else if (facing_ == Facing::kBackward)
	{
		// n(0, 0, +1)
		//				 R---------------------------------- v(x= +width, y= +height, z=0)   
		//				 | \     /\      /| \     /\      /| 
		//				 |  \   /  \    / |  \   /  \    / |
		//				 |   \ /    \  /  |   \ /    \  /  |
		//				 -----------------------------------
		//				 |   / \     /\   |   / \     /\   |
		//				 |  /   \   /  \  |  /   \   /  \  |   /\ +y
		//				 | /     \ /    \ | /     \ /    \ |   ||
		// v(x=0,y=0,z=0) P---------------------------------Q 
		//				   ==> +x
		rectangle_->setRotationAngles({ 90.0f, 0.0f, 0.0f });
	}

	// forward facing (inside the window)
	else if (facing_ == Facing::kForward)
	{
		// n(0, 0, -1)
		//					==> +x
		// v(x=0,y=0,z=0)P---------------------------------Q  ||
		//				 | \     /\      /| \     /\      /|  \/ -y
		//				 |  \   /  \    / |  \   /  \    / |
		//				 |   \ /    \  /  |   \ /    \  /  |
		//				 -----------------------------------
		//				 |   / \     /\   |   / \     /\   |
		//				 |  /   \   /  \  |  /   \   /  \  |
		//				 | /     \ /    \ | /     \ /    \ |   
		//				 R---------------------------------- v(x= +width, y= -height, z=0)   

		rectangle_->setRotationAngles({ -90.0f, 0.0f, 0.0f });
	}

	// left facing
	else if (facing_ == Facing::kLeft)
	{
		// n(-1, 0, 0)
		//					==> -z
		// v(x=0,y=0,z=0)P---------------------------------Q  ||
		//				 | \     /\      /| \     /\      /|  \/ -y
		//				 |  \   /  \    / |  \   /  \    / |
		//				 |   \ /    \  /  |   \ /    \  /  |
		//				 -----------------------------------
		//				 |   / \     /\   |   / \     /\   |
		//				 |  /   \   /  \  |  /   \   /  \  |
		//				 | /     \ /    \ | /     \ /    \ |   
		//				 R---------------------------------- v(x= 0, y= -height, z= -width)   
		rectangle_->setRotationAngles({ -90.0f, 0.0f, +90.0f });
	}

	// right facing
	else if (facing_ == Facing::kRight)
	{
		// n(+1, 0, 0)
		//					==> +z
		// v(x=0,y=0,z=0)P---------------------------------Q  ||
		//				 | \     /\      /| \     /\      /|  \/ -y
		//				 |  \   /  \    / |  \   /  \    / |
		//				 |   \ /    \  /  |   \ /    \  /  |
		//				 -----------------------------------
		//				 |   / \     /\   |   / \     /\   |
		//				 |  /   \   /  \  |  /   \   /  \  |
		//				 | /     \ /    \ | /     \ /    \ |   
		//				 R---------------------------------- v(x= 0, y= -height, z= +width)  

		rectangle_->setRotationAngles({ -90.0f, 0.0f, -90.0f });
	}

	// create vertex, coords, etc 
	rectangle_->initVertexAndNormalCoords();
}


MeshPlane::~MeshPlane()
{
	delete rectangle_;
	rectangle_ = nullptr;
}

void MeshPlane::render(bool is_shadow)
{
	// remember where we are
	glPushMatrix();

		// translate the plane
		glTranslatef(translation_.x, translation_.y, translation_.z);

		// rotate on the x axis
		glRotatef(rotation_angles_.x, 1.0f, 0.0f, 0.0f);
		// rotate on the y axis
		glRotatef(rotation_angles_.y, 0.0f, 1.0f, 0.0f);
		// rotate on the z axis
		glRotatef(rotation_angles_.z, 0.0f, 0.0f, 1.0f);

		// scale the object
		glScalef(scale_.x, scale_.y, scale_.z);

		// render the rectangle itself
		rectangle_->render(is_shadow);

	// go back where we were
	glPopMatrix();
}

void MeshPlane::setSharedContext(SharedContext* shared_context)
{
	/* Set shared context in the rectangle*/
	rectangle_->setSharedContext(shared_context);

	// set context for the plane
	shared_context_ = shared_context;
}

void MeshPlane::setTexture(Texture* texture, float starting_u, float starting_v, float ending_u, float ending_v)
{
	rectangle_->setTexture(texture, starting_u, starting_v, ending_u, ending_v);
}

void MeshPlane::setColour(Colour4 colour)
{
	rectangle_->setColour(colour);
}

vector<float> MeshPlane::getPQRVertices()
{
	vector<float> PQRVectices;

	switch (facing_)
	{
	case Facing::kDown:
		PQRVectices = { translation_.x, translation_.y, translation_.z,		// Px, Py, Pz
			translation_.x, translation_.y, +height_ + translation_.z,				// Rx, Ry, Rz
			+width_ + translation_.x, translation_.y, +height_ + translation_.z };	// Qx, Qy, Qz
		break;
	case Facing::kBackward:
		PQRVectices = { translation_.x, translation_.y, translation_.z,		// Px, Py, Pz
			translation_.x, +height_ + translation_.y, translation_.z,				// Rx, Ry, Rz
			+width_ + translation_.x, translation_.y, translation_.z };	// Qx, Qy, Qz
		break;
	case Facing::kForward:
		PQRVectices = { translation_.x, translation_.y, translation_.z,		// Px, Py, Pz
			translation_.x, -height_ + translation_.y, translation_.z,				// Rx, Ry, Rz
			+width_ + translation_.x, translation_.y, translation_.z };	// Qx, Qy, Qz
		break;
	case Facing::kLeft:
		PQRVectices = { translation_.x, translation_.y, translation_.z, // Px, Py, Pz
			translation_.x, -height_ + translation_.y, translation_.z,		 // Rx, Ry, Rz
			translation_.x, translation_.y, -width_ + translation_.z };		 // Qx, Qy, Qz
		break;
	case Facing::kRight:
		PQRVectices = { translation_.x, translation_.y, translation_.z, // Px, Py, Pz
			translation_.x, -height_ + translation_.y, translation_.z,		 // Rx, Ry, Rz
			translation_.x, translation_.y, -width_ + translation_.z };		 // Qx, Qy, Qz
		break;
	case Facing::kUp:
	default:
		PQRVectices = { translation_.x, translation_.y, translation_.z,		// Px, Py, Pz
			translation_.x, translation_.y, -height_ + translation_.z,				// Rx, Ry, Rz
			+width_ + translation_.x, translation_.y, -height_ + translation_.z };	// Qx, Qy, Qz
		break;
	}
	return PQRVectices;
}