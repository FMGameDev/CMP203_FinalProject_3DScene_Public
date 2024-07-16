// Model class
// It extends from base mesh
// It handles the loading/processing of the model file.
// The class model has been proportioned by Paul Robertson and
// It has been modified by Francisco Diaz (@FMGameDev) completing the function of loadModel for sorting the vertices, normals, textures, saving and rendering them
// It has been added the functionality of loading models with different geometry GL_TRIANGLES AND/OR GL_QUADS, 
// so if a model is loaded if this has only triangles, only quads or a both (a mixture of quads and triangles)

#ifndef _MODEL_H_
#define _MODEL_H_

#include "BaseMesh.h"
#include <list>

class Model : public BaseMesh
{

public:
	// general constructor
	Model(char* modelFilename);

	// constructor used for models are copies of other models 
	// this model doesn't use the loadModel function
	//Model();

	// destructor
	~Model();

	/* FUNCTIONS TO UPDATE AND RENDER A MESH*/

	// draw the shape
	void render(bool is_shadow = false) override;

	// set a texture for this model
	void setTexture(Texture* texture) override;

	// return a clone of this shape
	BaseMesh* clone() override;

private:
	// Load the model using the url parameter and save the vertices, tex coords and indices in the right format to be rendered
	// Modified from a multi-threaded version by Mark Ropper.
	bool loadModel(char* file_name);


	// Component to tracker the order of the vertices.
	// It is initialised in loadModel() and used later in render().
	// The first element of this component will save the type of mode which use during the drawing,
	// The second element will save the last vertex position which use that type of mode (GL_TRIANGLE, GL_QUADS)
	list<pair<GLenum, int>> vertices_tracker_;

};

#endif