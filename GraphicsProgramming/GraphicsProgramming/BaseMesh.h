
// Class Base Mesh
// It is the base for all the shapes/meshes/models 
// @author Francisco Diaz (FMGameDev)

#pragma once

#ifndef BASEMESH_H
#define BASEMESH_H

#define _USE_MATH_DEFINES // for using pi

// Include GLUT, openGL, input.
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>
#include <list>
#include <cmath> // for cos() and sin()

#include "Texture.h"
#include "Vector3.h"
#include "SharedContext.h"
#include "Colour4.h"


using namespace std;

#ifndef DEREFERENCE_METHOD
#define DEREFERENCE_METHOD
// enum class to initialise the shape and draw them using one method or other
// it is useful in case of testing, by default will use the method 3 which reuse vertices using the index
enum class DereferenceMethod
{
	kMethod1, // Dereferencing single array elements. It lies within a glBegin and glEnd. It doesn't require an Index array
	kMethod2, // Accessing full arrays with a single function. It doesn't lies within a glBegin and glEnd. It doesn't require an Index array
	kMethod3  // Accessing full arrays with a single function. It doesn't lies within a glBegin and glEnd. It uses an index collection in order to reuse vertices. The collection of vertices, normals and texture coords are much smaller (aprox. 1/3 smaller) thus save memory.
};
#endif

#ifndef AXIS_LIMITS_METHOD
#define AXIS_LIMITS_METHOD
// struct to save the limits of movement along the axis of this mesh
// it is useful for objects are moving
struct AxisLimits
{
	// default constructor
	AxisLimits() : max({ 0.0f, 0.0f, 0.0f }), min({ 0.0f, 0.0f, 0.0f }){};
	// constructor for passing data
	AxisLimits(Vector3 maximum, Vector3 minimum) : max(maximum), min(minimum) {};

	Vector3 max; // max x,y,z values which this mesh can be
	Vector3 min; // min x,y,z values which this mesh can be
};
#endif

class BaseMesh
{
public:
	// constructor
	BaseMesh();

	// destructor
	~BaseMesh();


	/* FUNCTIONS TO MODIFY THE CHARACTERISTICS OF A MESH */

	// set the texture
	virtual void setTexture(Texture* texture);

	// set the colour (red, green, blue, alpha) for this mesh
	virtual void setColour(Colour4 colour);

	// set scale
	void setScale(Vector3 scale);

	// invert the shape vertically by setting inverse y
	void invertVertically();

	// invert the shape vertically by setting inverse x
	void invertHorizontally();

	// invert z-axis
	void invertZ();

	// set if the mesh has to be rendered using triangles, quads, etc..
	void setMode(GLenum mode);
	// set the type of dereference mething
	void setDereferenceMethod(DereferenceMethod dereference_method);


	/* FUNCTIONS TO MODIFY THE MOVEMENT COMPONENTS OF A MESH*/

	// set if the shape is rotating in x, y, z (automatically)
	void setIsRotating(bool rotation_x, bool rotation_y, bool rotation_z);
	void setIsRotating(vector<bool> is_rotating);

	// set  the angles of rotation
	virtual void setRotationAngles(Vector3 rotation_angles);
	// return rotation angles
	Vector3 getRotationAngles() const;

	// translation units of the shape
	virtual void setTranslation(Vector3 translation);
	void setTranslationX(float translation_x);
	void setTranslationY(float translation_y);
	void setTranslationZ(float translation_z);
	//return translation
	Vector3 getTranslation() const;
	// set if it is automatically moving along the axis
	void setIsMoving(vector<bool> is_moving);
	void setDirection(Vector3 direction);

	// speed of Rotation
	virtual void setSpeed(float speed);

	// set the axis limits of this mesh in case is moving
	void setAxisLimits(AxisLimits axis_limits);

	/* FUNCTIONS TO UPDATE AND RENDER A MESH*/

	// update the shape
	virtual void update(float dt);

	// draw the shape
	virtual void render(bool is_shadow = false);

	// return a clone of this shape
	virtual BaseMesh* clone() const;

	// copy translation and rotation angles
	void copyMovementComponents(BaseMesh* base_shape_to_copy);

	// set the shared context, which can be used for the input, wireframe_mode, etc
	virtual void setSharedContext(SharedContext* shared_context);

	// addding an external submesh to the system
	void addSubmesh(BaseMesh* submesh);

	// return a clone of this shape
	virtual BaseMesh* clone();

protected:
	/* CHARACTERISTICS OF A BASE MESH */

	// Texture component of this shape
	Texture* texture_;

	// colour component
	Colour4 colour_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// component to scale the shape
	Vector3 scale_ = { 1.0f, 1.0f, 1.0f }; // by default scale is '*1.0f' (the object is multiply by 1.0f, so it keeps the same size)


	/* MOVEMENTS COMPONENTS */

	// speed_ component for rotations
	float speed_ = 0.0f;

	// rotation components
	Vector3 rotation_angles_;
	vector<bool> is_rotating_ = { false, false, false }; // set if it is rotating in x, y or z axis automatically

	// translation components
	Vector3 translation_; // translation of the shape
	vector<bool> is_moving_ = { false, false, false }; // set if it is mving along x, y or z axis automatically
	Vector3 direction_; // component to move the mesh

	// contain the max and min values of x,y,z in which this mesh can move
	AxisLimits axis_limits_;


	/* ARRAYS COORDS (VERTICES, NORMALS, TEXTURES AND INDICES), COMPONENTS AND FUNCTIONS */

	// component to identify what method to use when the data is being rendered
	DereferenceMethod dereference_method_;
	GLenum mode_; // it is the geometry which is made of this shape

	// arrays
	vector<float> vertices_;
	vector<float> normals_;
	vector<float> texture_coords_;
	vector<unsigned int> indices_; // I have used unsigned int instead of GLubyte because GLubyte is limited to 255 so for big number of vertices/indices does not work. unsigned int solves this

	// function to add element into the vectors of vertices, texture coords and indices
	void addVertex(float x, float y, float z);
	void addNormal(float nx, float ny, float nz);
	void addTexCoord(float u, float v);
	void addTriangleIndices(unsigned int i0, unsigned int i1, unsigned int i2);

	// initialise arrays of coords for vertex, coords and index
	virtual void initVertexAndNormalCoords();

	// initialise arrays of textures coords
	virtual void initTextureCoords();

	/* OTHERS COMPONENTS */
	// shared context component
	SharedContext* shared_context_;

	// collection of submeshes created outside of this class, these can been added to the system: using of hierarchical
	vector<BaseMesh*> submeshes_;
};

#endif

