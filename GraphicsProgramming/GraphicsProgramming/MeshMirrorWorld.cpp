#include "MeshMirrorWorld.h"


MeshMirrorWorld::MeshMirrorWorld()
{
	// initialise components
	last_shape_copy_id_ = 0;

}


MeshMirrorWorld::~MeshMirrorWorld()
{
	// delete all the pointers to the shape copies created
	for (auto shape_copy : shape_copy_container_)
	{
		delete shape_copy.second;
		shape_copy.second = nullptr;
	}
	// delete mirror component
	delete mirror_obj_;
	mirror_obj_ = nullptr;
}

void MeshMirrorWorld::initPlaneMirror(Facing facing, int height, int width)
{
	// control the mirror is created only once
	if (mirror_obj_ == nullptr)
	{
		facing_ = facing;
		mirror_obj_ = new MeshPlane(facing, height, width);
	}
}

void MeshMirrorWorld::initDiscMirror(Facing facing, float radius, int num_triangles)
{
	// control the mirror is created only once
	if (mirror_obj_ == nullptr)
	{
		facing_ = facing;

		// create the mirror
		mirror_obj_ = new MeshDisc(radius, num_triangles);

		if (facing == Facing::kForward)
		{
			mirror_obj_->setRotationAngles({ 0.0f, -180.0f, 0.0f }); // set the disc facing forward to the objct which is going to be reflected
		}
		else if (facing == Facing::kBackward)
		{
			// the disc is already facing backward (normal:+z)
		}
		// add more types facing if neccesary here
	}
}

void MeshMirrorWorld::createReflection(BaseMesh* original_shape_to_copy, bool invert_z, Texture* special_texture_for_copy)
{
	// create the pointer
	BaseMesh* shape_copy; // as all the base shapes extendes from base shape we declare this as pointer	
	// clone the original
	shape_copy = original_shape_to_copy->clone();

	// if there is a special texture for the copy, set it just to the copy
	if (special_texture_for_copy != nullptr)
	{
		shape_copy->setTexture(special_texture_for_copy);
	}

	// invert the copy
	if (invert_z)
	{
		shape_copy->invertZ();
	}
	else if (facing_ == Facing::kForward || facing_ == Facing::kBackward || facing_ == Facing::kLeft || facing_ == Facing::kRight)
	{
		shape_copy->invertHorizontally();
	}
	else if(facing_ == Facing::kUp || facing_ == Facing::kDown)
	{
		shape_copy->invertVertically();
	}
		
	// increment id
	last_shape_copy_id_ += 1;

	// save the copy linked to its id
	shape_copy_container_.insert(make_pair(last_shape_copy_id_, shape_copy));

	// save the original linked to the id of its copy
	original_shape_container_.insert(make_pair(last_shape_copy_id_, original_shape_to_copy));
}

void MeshMirrorWorld::update(float dt)
{
	float dist_mirror_to_original_shape; // distance between the mirror and the original shape, it has to be the same as the distance between the copy and the mirror

	// update the 
	for (auto shape_copy : shape_copy_container_)
	{
		// update the position of the copy, copying the position of the original shape
		shape_copy.second->copyMovementComponents(original_shape_container_[shape_copy.first]);

		// mirror world
		Vector3 mirror_translation = mirror_obj_->getTranslation();

		// set the position of the copy behind the mirror, the mirror is always looking at the original

		// the original is up and the copy is down
		if (facing_ == Facing::kUp)
		{
			dist_mirror_to_original_shape = abs(original_shape_container_[shape_copy.first]->getTranslation().y - mirror_translation.y);

			shape_copy.second->setTranslationY(-dist_mirror_to_original_shape + mirror_translation.y ); // set the copy under the mirror
		}
		// the original is down and the copy is up
		else if (facing_ == Facing::kDown)
		{
			dist_mirror_to_original_shape = abs(original_shape_container_[shape_copy.first]->getTranslation().y - mirror_translation.y);

			shape_copy.second->setTranslationY(dist_mirror_to_original_shape + mirror_translation.y); // set the copy above the mirror
		}
		// the original is left and the copy is right
		else if (facing_ == Facing::kLeft)
		{
			dist_mirror_to_original_shape = abs(original_shape_container_[shape_copy.first]->getTranslation().x - mirror_translation.x);

			shape_copy.second->setTranslationX(dist_mirror_to_original_shape + mirror_translation.x); // set the copy rigth to the mirror
		}
		// the original is right and the copy is left
		else if (facing_ == Facing::kRight)
		{
			dist_mirror_to_original_shape = abs(original_shape_container_[shape_copy.first]->getTranslation().x - mirror_translation.x);

			shape_copy.second->setTranslationX(-dist_mirror_to_original_shape + mirror_translation.x); // set the copy under the mirror
		}
		// the original is front and the copy is back
		else if (facing_ == Facing::kBackward)
		{
			dist_mirror_to_original_shape = abs(original_shape_container_[shape_copy.first]->getTranslation().z - mirror_translation.z);

			shape_copy.second->setTranslationZ(-dist_mirror_to_original_shape + mirror_translation.z); // set the copy under the mirror
		}
		// the original is back and the copy is front
		else if (facing_ == Facing::kForward)
		{
			dist_mirror_to_original_shape = abs(original_shape_container_[shape_copy.first]->getTranslation().z - mirror_translation.z);

			shape_copy.second->setTranslationZ(dist_mirror_to_original_shape + mirror_translation.z); // set the copy under the mirror
		}

	}

	// update the mirror obj itself
	mirror_obj_->update(dt);
}

void MeshMirrorWorld::setTranslation(Vector3 translation)
{
	mirror_obj_->setTranslation(translation);
}

void MeshMirrorWorld::setSharedContext(SharedContext* shared_context)
{
	mirror_obj_->setSharedContext(shared_context);
}

void MeshMirrorWorld::setColour(Colour4 colour)
{
	mirror_obj_->setColour(colour);
}

void MeshMirrorWorld::removeShapeCopy(int shape_copy_id)
{
	// remove the shape copy, the pointer object and from the collection
	for (auto shape_copy = shape_copy_container_.begin(); shape_copy != shape_copy_container_.end(); shape_copy++) //Look for the scene to remove
	{
		if (shape_copy->first == shape_copy_id) // If it has been found, then remove it
		{
			delete shape_copy->second; // delete pointer of the object
			shape_copy_container_.erase(shape_copy); // erase from the collection
			return;
		}
	}

	// remove the original shape copy just from the collection (no the pointer/object itself)
	for (auto original_shape = original_shape_container_.begin(); original_shape != original_shape_container_.end(); original_shape++) //Look for the scene to remove
	{
		if (original_shape->first == shape_copy_id) // If it has been found, then remove it
		{
			original_shape_container_.erase(original_shape); // erase just from the collection
			return;
		}
	}
}

void MeshMirrorWorld::render()
{
	// disable the depth test (we don't want to store depths values while writing to the stencil buffer)
	glDisable(GL_DEPTH_TEST);

	//  turn off writing to the frame buffer. Don't update colour
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	// enable the stencil test
	glEnable(GL_STENCIL_TEST);

	// set the stencil operation to replace values when the test passes
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

	// set the stencil function to always pass
	glStencilFunc(GL_ALWAYS, 1, 0xffffffff);

	// render the mirror shape, mirror pixels just get their stencil set to 1
	mirror_obj_->render();

	// turn on rendering to the frame buffer
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	// enable depth test
	glEnable(GL_DEPTH_TEST);

	// set stencil function to test if the value is equal to 1
	glStencilFunc(GL_EQUAL, 1, 0xffffffff);

	// set the stencil operation to keep all values (we don't want to change the stencil)
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	// draw the copies (reflections) 
	for (auto shape_copy : shape_copy_container_)
	{
		shape_copy.second->render(); // draw the copies
	}

	// disable stencil test (no longer needed)
	glDisable(GL_STENCIL_TEST);

	// enable alpha blending (to combine the floor object with the copy shape
	glEnable(GL_BLEND);

	// disable lighting (100% reflective object)
	glDisable(GL_LIGHTING);

	// draw floor object 
	mirror_obj_->render();

	// enable lighting (rest of the scene is lit correctly)
	glEnable(GL_LIGHTING);

	// disable blend (no longer blending)
	glDisable(GL_BLEND);

	// RENDER THE REAL OBJECT AFETER THIS FUNCTION
}