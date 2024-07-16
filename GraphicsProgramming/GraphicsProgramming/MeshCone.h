
// Class for MeshCone
// This class can create the shapes: Cones, pyramids, prisms(triangular, rectangular, octgonal, hexagonal...) depending on the parameters passed on the constructor
// This shapes can also contain a top (lid, cover) and a base, it is also decided on the constructor by the parameters passed
// The top and base can have different colours and texture from the side of the shape, or they can have the same texture depending on the function used for that
// @author Francisco Diaz (FMGameDev)

#pragma once
#ifndef MESH_CONE_H
#define MESH_CONE_H

#include "BaseMesh.h"
#include "MeshDisc.h"

class MeshCone : public virtual BaseMesh
{
public:
	// general/customisable constructor (prisms, pyramids, cones, cylinders) 
	MeshCone(float base_radius, float top_radius, float height, int longitudinal_segments = 200, int latitudinal_segments = 200,
		bool has_top_disc = true, bool has_base_disc = true); // by default the shape only has the sides

	// destructor
	~MeshCone();


	/* SPEFICIC FUNCTIONS TO MODIFY THE CHARACTERISTICS OF THIS MESH */

	// set the same texture for the side, base and top parts (in case the cylinder has these discs)
	void setTexture(Texture* common_texture);

	// set the textures for the discs of this object (if the object was created with that discs)
	// it set the same texture_ for top and base discs
	void setTopAndBaseDiscTexture(Texture* base_top_texture);

	// set the texture_ for base disc of this object (if the object was created with that disc)
	void setBaseTexture(Texture* base_texture);

	// set the texture_ for the top disc of this object (if the object was created with that disc)
	void setTopTexture(Texture* top_texture);

	// set only the side texture for this object
	void setSideTexture(Texture* side_texture);

	// set the colour for the side, base and top part
	void setColour(Colour4 colour);

	// set the colour for the side, base and top part
	void setTopAndBaseColour(Colour4 colour);

	// set the colour for the side, base and top part
	void setBaseColour(Colour4 colour);

	// set the colour for the side, base and top part
	void setTopColour(Colour4 colour);

	// set the colour for the side, base and top part
	void setSideColour(Colour4 colour);


	/* SPEFIFIC FUNCTIONS TO UPDATE AND RENDER THIS MESH*/

	// update the shape
	void update(float dt);

	// draw the shape
	void render(bool is_shadow);

	// set the shared context, which can be used for the input, wireframe_mode, etc
	void setSharedContext(SharedContext* shared_context);


private:

	/** SPEFIFIC CHARACTERISTICS OF THIS MESH */

	// base and top of this shape
	MeshDisc* base_disc_;
	MeshDisc* top_disc_;

	// radius and number of segments of the sphere the shape
	float base_r_, top_r_;
	float h_; // height
 
	int longitudinal_segments_; // Number of segments vertically speaking
	int latitudinal_segments_; // Number of segments horizontally speaking

	
	/* ARRAYS COORDS (vertices_, normals_, TEXTURES AND indices_), COMPONENTS AND FUNCTIONS */

	// initialise arrays of coords for vertex, coords and index
	void initVertexAndNormalCoords(bool has_top_disc, bool has_base_disc);

	// initialise arrays of textures coords
	void initSideTextureCoords();
};

#endif
