// Below ifdef required to remove warnings for unsafe version of fopen and fscanf.
// Secure version won't work cross-platform, forcing this small hack.
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "model.h"


Model::Model(char* modelFilename)
	: BaseMesh()
{
	bool result;

	// Load in the model data,
	result = loadModel(modelFilename);
	if (!result)
	{
		MessageBox(NULL, "Model failed to load", "Error", MB_OK);
	}
}

Model::~Model()
{

}

bool Model::loadModel(char* model_file_name)
{
	// Components where we will temporally store the data collected from the file 
	vector<Vector3> tmp_vertices;
	vector<Vector3> tmp_normals;
	vector<Vector3> tmp_texture_coords;
	vector<unsigned int> faces; // faces provide the indices of vertices, texture coordinates and normals. Format: v/t/n

	int face_vertices_counter = 0; // counter of vertices found, it is used for tracking the order of the vertices, if these will be rendered/used in a triangle or in a quad

	// Attempted to open the file
	FILE* file = fopen(model_file_name, "r");

	// Check if the file exists
	if (file == NULL)
	{
		printf("Impossible to open the file !\n");
		return false;
	}
	// Attempted to parse the file
	while (true)
	{
		// component where save the characters of the line is being read
		char lineHeader[128]; 

		// Read first word of the line
		int res = fscanf(file, "%s", lineHeader);

		if (res == EOF) // EOF = End Of File
		{
			// taking into account the last group of triangle or quads added
			// or the first group if it has been only a type of mode (only triangles or only quads)
			if (face_vertices_counter != 0)
			{
				vertices_tracker_.push_back(make_pair(mode_, face_vertices_counter));
			}

			break; // exit loop
		}
		else // Parse lineHeader
		{
			// Broke down by vertex, texture, normal and face data based on the line prefix

			if (strcmp(lineHeader, "v") == 0) // Vertex
			{
				Vector3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z); // extract the values
				tmp_vertices.push_back(vertex); // store it
			}
			else if (strcmp(lineHeader, "vt") == 0) // Tex Coord
			{
				Vector3 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y); // extract the values
				tmp_texture_coords.push_back(uv); // store it
			}
			else if (strcmp(lineHeader, "vn") == 0) // Normal
			{
				Vector3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z); // extract the values
				tmp_normals.push_back(normal); // store it
			}
			else if (strcmp(lineHeader, "f") == 0) // Face
			{
				unsigned int face[12];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", // vertices / texture coords / normals
																					&face[0], &face[1], &face[2], // first face
																					&face[3], &face[4], &face[5], // second face
																					&face[6], &face[7], &face[8], // third face
																					&face[9], &face[10], &face[11]); // forth face in case of quad

				// if it is a triangle and the node hasn't been set it yet
				if (matches == 9 && mode_ == GL_NONE)
				{
					mode_ = GL_TRIANGLES;
				}
				// if it is a squad and the node hasn't been set it yet
				else if (matches == 12 && mode_ == GL_NONE)
				{
					mode_ = GL_QUADS;
				}
				// if this faces is made of a mode (geometry) different of the last face them we have to save this information in order to render it rightly later
				// we save into the map "order" the information: what type of mode(geometry) and until what faces vertex it is used
				else if ((matches == 12 && mode_ == GL_TRIANGLES) || (matches == 9 && mode_ == GL_QUADS))
				{
					vertices_tracker_.push_back(make_pair(mode_, face_vertices_counter)); // save the type of mode and the last vertex position which use this type. This information will be used when the shape is being rendered

					// update the new mode
					if (matches == 9)
					{
						mode_ = GL_TRIANGLES;
					}
					else
					{
						mode_ = GL_QUADS;
					}
				}
				// if it is not a triangle and it is not a quad OR
				// if the model is made of a mixture of triangles and quads then exists the loop
				else if (matches != 9 && matches != 12)
				{
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");

					return false;
				}


				// save the values in the vector
				for (int i = 0; i < matches; i++)
				{
					faces.push_back(face[i]); // store it
				}

				// Count the face vertices found in the line read
				// For triangles: 9/3 = 3 vertices
				// For quads: 12/3 = 4 vrtices
				face_vertices_counter += matches / 3;
			}
		}
	}

#pragma region unroll_data

	// PROCESS THE DATA (INDEXING)
	// By this point all model has been read from the file, but is not in the correct order.
	// Wee need to loop over all the data and sort it into a render ready order/format.

	// make sure we are using an empty vector, it has been used swap instead of clean because std::vector::swap is O(1), and clean() is O(N)
	vector<float>().swap(vertices_);
	vector<float>().swap(normals_);
	vector<float>().swap(texture_coords_);

	unsigned int index;

	// loop for all the face data
	for (unsigned int i = 0; i < faces.size(); i = i+3) // increment loop by 3
	{
		// add first face value which is vertex index
		index = faces[i]-1; // translate the vertex index into an array index, as the arrays starts from 0, and the vertex index starts from 1
		if (index >= 0 && index < tmp_vertices.size())
		{
			addVertex(tmp_vertices[index].x, tmp_vertices[index].y, tmp_vertices[index].z); // Vertices should be stored in counter-clockwise
		}

		// add second face value which is texture coord index
		index = faces[i+1]-1;  // translate the tex coord index into an array index, as the arrays starts from 0, and the tex coord index starts from 1
		if (index >= 0 && index < tmp_texture_coords.size())
		{
			addTexCoord(tmp_texture_coords[index].x, tmp_texture_coords[index].y);
		}

		// add third face value which is normal index
		index = faces[i+2]-1;  // translate the normal index into an array index, as the arrays starts from 0, and the normal index starts from 1
		if (index >= 0 && index < tmp_normals.size())
		{
			addNormal(tmp_normals[index].x, tmp_normals[index].y, tmp_normals[index].z);
		}
	}

#pragma endregion unroll_data

	// set the method of dereference depending if has been found indices
	if (indices_.empty())
	{
		dereference_method_ = DereferenceMethod::kMethod2;
	}
	else
	{
		dereference_method_ = DereferenceMethod::kMethod3;
	}

	// Once data has been sorted clear read data (which has been copied and are not longer needed).
	tmp_vertices.clear();
	tmp_normals.clear();
	tmp_texture_coords.clear();
	faces.clear();

	return true;
}

void Model::render(bool is_shadow)
{
	// To Turn on Wireframe to draw the lines (for test proposed) 
	if (*shared_context_->wireframe_mode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draw lines
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/* Apply colour */
	if (!is_shadow)
		glColor4f(colour_.rgba[0], colour_.rgba[1], colour_.rgba[2], colour_.rgba[3]); // (All the vertices will have the same colour)
	else
		glColor4f(0.1f, 0.1f, 0.1f, 1.0f);

	/* Apply texture_ if this object has one */
	if (texture_ != nullptr && !is_shadow)
	{
		texture_->use();
	}

	/* Enable Arrays of coords (vertices_, normals_, textures and index) and link them */

	// 1st: Tell openGL what information we have (enable arrays)
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	if (texture_ != nullptr && !is_shadow)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	// 2nd: Tell openGL what data and how it is arranged
	glVertexPointer(3, GL_FLOAT, 0, vertices_.data());
	glNormalPointer(GL_FLOAT, 0, normals_.data());
	if (texture_ != nullptr && !is_shadow)
	{
		glTexCoordPointer(2, GL_FLOAT, 0, texture_coords_.data());
	}

	/* Draw the shape using the arrays */

	// remember where we are
	glPushMatrix();

		// translate the square
		glTranslatef(translation_.x, translation_.y, translation_.z);

		// rotate on the x axis
		glRotatef(rotation_angles_.x, 1.0f, 0.0f, 0.0f);
		// rotate on the y axis
		glRotatef(rotation_angles_.y, 0.0f, 1.0f, 0.0f);
		// rotate on the z axis
		glRotatef(rotation_angles_.z, 0.0f, 0.0f, 1.0f);

		// scale the object
		glScalef(scale_.x, scale_.y, scale_.z);


		/* DRAW */
		if (dereference_method_ == DereferenceMethod::kMethod2)
		{
			int start_vertex = 0;
			for (auto order : vertices_tracker_)
			{
				glDrawArrays(order.first, start_vertex, order.second - start_vertex);
				start_vertex = order.second;			
			}
		}
		// Method 3
		else if (dereference_method_ == DereferenceMethod::kMethod3)
		{
			glDrawElements(mode_, (unsigned int)indices_.size(), GL_UNSIGNED_INT, indices_.data()); // method 3 of dereference
		}

	// go back where we were
	glPopMatrix();


	/* Diable Arrays of coords (vertices_, normals_, textures and index)*/

	// 4rd: Tell openGL don't use more the information (disable arrays)
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	if (texture_ != nullptr && !is_shadow) // disable texture_ coords if the sahape has one assigned
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	/* Stop using the texture_ if this object has one*/
	if (texture_ != nullptr && !is_shadow)
	{
		texture_->stopUsing();
	}

	/* reset to white colour */
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void Model::setTexture(Texture* texture)
{
	texture_ = texture;
}

BaseMesh* Model::clone()
{
	return new Model(*this);
}