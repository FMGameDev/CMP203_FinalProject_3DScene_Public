
// Class for Mesh Rectangle Object
// It defines a rectangle which has a default size of 1*1 quadrangle,
// each quadrangle is composed for 10 triangles (two rows of 5 each triangles)
// There are 30 vertices per each quadrangle. So for a rectangle of height 2 and width 3 (six quadrangles in total) will have 180 vertices.
// By default the rectangle is made lying (like a floor) the width is along the x-axis and height is along z-axis
// This class is just a tool/base for the Mesh Plane and it should be used with it, which add more functionality and allow more movements
// @author Francisco Diaz (FMGameDev)

#pragma once
// Include GLUT, openGL, input.
#include "BaseMesh.h"

// enum used for detecting the behaviour of a rectangle used for texturing it.
enum class RectangleBehaviourType
{
	kUnit, // The rectangle will look like one full unit in case some texture is set it on it
	kSplitted, // The rectangle will look like it is splitted in height*width individual squares and their texture will be handled like that. It can be useful in case of using GL_Repeat etc
};

class MeshRectangle: public BaseMesh
{

public:
	// constructor
	MeshRectangle(int height = 1, int width = 1, RectangleBehaviourType rectangle_behaviour_type = RectangleBehaviourType::kSplitted);

	// destructor
	~MeshRectangle();

	// set the texture for this shape
	// min U/V img values are the points where the texture_ for this rectangle starts respect the image coords (being these from 0 to 1 for the full image)
	// max U/V img values are the points where the texture_ for this rectangle ends respect the image coords (being these from 0 to 1 for the full image)
	// for example: if we want to get the left half of the picture starting_u and starting_v would 0.0f, ending_u and ending_v would be 1.0f
	// this is used mainly when the texture_ img is mapped else by default will take from 0 to 1
	virtual void setTexture(Texture* texture, float starting_u = 0.0f, float starting_v = 0.0f, float ending_u = 1.0f, float ending_v = 1.0f);

	// return the PQR vertices (top left,bottom left and top right vertices) of this rectangle
	virtual vector<float> getPQRVertices();

	// initialise arrays of coords for vertex, coords and index
	// By default the rectangle is made lying (like a floor) the width is along the x-axis and height is along z-axis
	void initVertexAndNormalCoords();

protected:

	/** SPECIFIC CHARACTERISTICS OF THIS MESH */

	// component to detect how will be handle the texture_ coord
	RectangleBehaviourType rectangle_behaviour_type_;

	// dimension of this rectangle
	int width_;
	int height_;

	// Example: For a rectangle of width 2 and height 1 PQR are:
	//  ==> +x
	// P---------------------------------Q    
	// | \     /\      /| \     /\      /|	  ||
	// |  \   /  \    / |  \   /  \    / |    \/ -z 
	// |   \ /    \  /  |   \ /    \  /  |    
	// -----------------------------------
	// |   / \     /\   |   / \     /\   |
	// |  /   \   /  \  |  /   \   /  \  |
	// | /     \ /    \ | /     \ /    \ |
	// R---------------------------------
	vector<float> PQR_vertices_; // container for PQR vertices, it has a format {Px,Py,Pz, Rx,Rx,Rz, Qx,Qy,Qz,}


	/* FUNCTIONS FOR GENERATE THE MESH [ARRAYS COORDS (VERTICES, NORMALS, TEXTURES AND INDICES)] */
	// initialise arrays of textures coords
	void initTextureCoords(float starting_u, float starting_v, float ending_u, float ending_v);
};


