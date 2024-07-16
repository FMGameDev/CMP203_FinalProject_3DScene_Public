
// Class for MeshPlane Object
// It extends from Mesh Rectangle
// Add the function of deciding where is facing originally the rectangle
// This function contain a Rectangle object which is the plane itself. This class allow translate/rotate it without alterating the original facing rectangle by using hierarchical
// @author Francisco Diaz (FMGameDev)

#pragma once
#include "MeshRectangle.h"

// normal facing of the plane
enum class Facing
{
	kUp, // looking up, it can be used as a roof, floor, etc (normal: +y)
	kDown, // looking down (normal: -y)
	kBackward, // looking at us (normal: +z)
	kForward, // normal looking inside the window (normal: -z)
	kLeft, // looking left (normal: -x)
	kRight // looking right (normal: +x)
};

class MeshPlane : public virtual MeshRectangle
{

public:
	// constructor
	// height is like the number of 'rows' and width is like the number of columns
	MeshPlane(Facing facing = Facing::kUp, int height = 2, int width = 2, RectangleBehaviourType rectangle_behaviour_type_ = RectangleBehaviourType::kSplitted);

	// destructor
	~MeshPlane();

	// render the real plane
	void render(bool is_shadow = false) override;

	// set the shared context, which can be used for the input, wireframe_mode, etc
	void setSharedContext(SharedContext* shared_context);

	// set the texture in the plane (by setting it in the rectangle object)
	void setTexture(Texture* texture, float starting_u = 0.0f, float starting_v = 0.0f, float ending_u = 1.0f, float ending_v = 1.0f) override;

	// set the colour (red, green, blue, alpha) in the rectangle object
	void setColour(Colour4 colour);

	// return the PQR vertices of the rectangle component
	// it set the vertices depending on the translation and the facing component
	// if the plane mesh plane (not rectangle) is rotate then this function will need to be updated to take into account the plane rotation
	vector<float> getPQRVertices();

protected:
	/** CHARACTERISTICS OF THE MeshPlane */
	Facing facing_; // component to detect/set the orientation of the plane (by the normal facing)
	MeshRectangle* rectangle_; // it contain the plane/rectangle itself
};

