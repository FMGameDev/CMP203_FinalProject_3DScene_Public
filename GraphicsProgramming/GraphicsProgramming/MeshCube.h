
// Class for MeshCube Object
// It cube is made from 6 planes, which have the same width and height
// Each plane is named as a face of the cube, and it is contained in the faces collection
// The cube face identified each face, this name are based on the cube without being rotated
// @author Francisco Diaz (FMGameDev)

#pragma once
#include "MeshPlane.h"
#include <map>

// enum used for naming/identifing the faces.
enum class CubeFace
{
	kFront,
	kBack,
	kLeft,
	kRight,
	kTop,
	kBottom
};

class MeshCube : public virtual BaseMesh
{

public:
	// constructor
	MeshCube(int dimension = 1, bool colored_by_default = false, RectangleBehaviourType rectangle_behaviour_type = RectangleBehaviourType::kSplitted);

	// destructor
	~MeshCube();


	/* FUNCTIONS TO MODIFY THE CHARACTERISTICS OF A MESH */

	// set the texture of a single face
	void setFaceTexture(CubeFace cube_face, Texture* texture);
	// set a texture to the full cube
	void setTexture(Texture* texture) override;

	// set the colour (red, green, blue, alpha) of a single face
	void setFaceColor(CubeFace cube_face, Colour4 colour);
	// set the same color to every face of the cube
	void setAColourToAllFaces(Colour4 colour);


	// return the dimension (width and height) of the cube 
	int getDimension() const;


	/* FUNCTIONS TO UPDATE AND RENDER THE MESH*/

	// draw the cube
	void render(bool is_shadow = false) override;

	// set the shared context, which can be used for the input, wireframe_mode, etc
	void setSharedContext(SharedContext* shared_context);

	// return a clone of this shape
	BaseMesh* clone() const;


private:
	/** CHARACTERISTICS OF THE CUBE */

	// dimension of this cube 
	// example: dimension=1 means 1*1 quad per face, dimension=2 means 2*2 quad per face
	int dimension_;

	// collection of six faces
	map<CubeFace, MeshPlane*> faces_;
};
