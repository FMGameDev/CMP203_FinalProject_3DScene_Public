// Mirror world
// It create a parallel world with copies of the shapes which we want to reflect in this world
// These copies are inverted in x, y or z-axis respect the original shape depending on the position of the original shape and the mirror
// The copies are made copying the original object by using the copy() function of the mesh which return a copy of the object. It is created faster than the original as it doesn't have to create vertices or load model again.
// The advantage of using copies for renderng instead of the original copy itself it is that we could move the copy when the original is not moving and play with this
// The copy also can also use different texture from the original.
// @author Francisco Diaz (FMGameDev)

#pragma once

// types of mirror shapes
#include "MeshPlane.h"
#include "MeshDisc.h"

// all the meshes classes to reflect
#include "Model.h"
#include "MeshTorus.h"

// includes for using in the containers
#include <unordered_map>
#include <vector>
#include <functional>


class MeshMirrorWorld
{
public:
	// constructor
	// the mirror must be facing the objects to reflect
	MeshMirrorWorld();

	// destructor
	~MeshMirrorWorld();

	// create the mirror with a plane rectangular form
	void initPlaneMirror(Facing facing = Facing::kUp, int height = 20, int width = 20);

	// create the mirror with a circular form
	void initDiscMirror(Facing facing = Facing::kBackward, float radius = 1.0f, int num_triangles = 20);

	// function to make a copy of the shape passed as a parameter
	void createReflection(BaseMesh* shape_to_copy, bool invert_z = false, Texture* special_texture_for_copy = nullptr);

	// update the mirror world
	void update(float dt);

	// set translation to the mirror
	void setTranslation(Vector3 translation);

	// set shared context to the mirror
	void setSharedContext(SharedContext* shared_context);

	// set colour to the mirror
	void setColour(Colour4 colour);

	// render mirror
	void render();

private:
	Facing facing_; // position real objet respect the mirror world and normal facing 
	BaseMesh* mirror_obj_;

	// container with all the shapes with linked to its id
	using ShapeContainer = unordered_map<int, BaseMesh*>; // <shape ID, Copy>
	ShapeContainer original_shape_container_; // container with all the original shapes
	ShapeContainer shape_copy_container_; // container with all the shapes copies

	// container with all the shape IDs to remove later
	using IDShapeContainer = vector<int>; // <ID of the copy to remove>
	IDShapeContainer shape_copy_to_remove_;

	// function to remove a game scene object by its game scene name
	void removeShapeCopy(int shape_copy_id);

	// component to save the last id assigned to a shape
	int last_shape_copy_id_;
};

