#include "MeshRectangle.h"

MeshRectangle::MeshRectangle(int height, int width, RectangleBehaviourType rectangle_behaviour_type)
	: height_(height), width_(width), rectangle_behaviour_type_(rectangle_behaviour_type), BaseMesh()
{
	// initialise array of vertices_and normals
	//initVertexAndNormalCoords();
}


MeshRectangle::~MeshRectangle()
{
}


void MeshRectangle::setTexture(Texture* texture, float starting_u, float starting_v, float ending_u, float ending_v)
{
	// initialise the texture_ coords in case there wasn't initialised
	if (texture_ == nullptr || texture_->getTextureCoordsType() != texture->getTextureCoordsType())
	{
		// initialise the texture_ coords with the new texture_
		initTextureCoords(starting_u, starting_v, ending_u, ending_v);
	}

	// set the texture_ pointer to the object
	texture_ = texture;
}


vector<float> MeshRectangle::getPQRVertices()
{
	// Returning the points PQR:
	// Example points PQR in a rectangle of width 2 and height 1:
	// P is the top left vertex of the rectangle, R is the bottom leftand Q is the top right
	// ==> +x
	// P---------------------------------Q    ||
	// | \     /\      /| \     /\      /|    \/ -z 
	// |  \   /  \    / |  \   /  \    / |
	// |   \ /    \  /  |   \ /    \  /  |
	// -----------------------------------
	// |   / \     /\   |   / \     /\   |
	// |  /   \   /  \  |  /   \   /  \  |
	// | /     \ /    \ | /     \ /    \ |
	// R----------------------------------

	return PQR_vertices_;
}

void MeshRectangle::initVertexAndNormalCoords()
{
	// A square has 5 triangles:									Example of x,y values of a rectangle of width 1 and height 1, where z=0 (vertical plane):
	// ------------------				v0------v3-------v5			x:  0-------0.5------1 z:
	// | \  B  /\   D  /|				| \  B  /\   D  /|				| \  B  /\   D  /| 0
	// |  \   /  \    / |				|  \   /  \    / |				|  \   /  \    / |
	// | A \ /  C \  / E|				| A \ /  C \  / E|				| A \ /  C \  / E|
	// ------------------		==>		v1---v2-----v4---v6	   ==>		|---0.25---0.75--| -0.5
	// | F / \  H  /\  J|				| F / \  H  /\  J|				| F / \  H  /\  J|
	// |  /   \   /  \  |				|  /   \   /  \  |				|  /   \   /  \  |
	// | /  G  \ /  I \ |				| /  G  \ /  I \ |				| /  G  \ /  I \ |
	// ------------------				v7-------v8-----v9				------------------ -1

	// Save the points PQR. Example of a rectangle of width 2 and height 1:
	// P is the top left vertex of the rectangle, R is the bottom left and Q is the top right 
	// P---------------------------------Q
	// | \     /\      /| \     /\      /|
	// |  \   /  \    / |  \   /  \    / |
	// |   \ /    \  /  |   \ /    \  /  |
	// -----------------------------------
	// |   / \     /\   |   / \     /\   |
	// |  /   \   /  \  |  /   \   /  \  |
	// | /     \ /    \ | /     \ /    \ |
	// R----------------------------------

	// set the type of dereference to use and the mode depending on how the vertices, normals and indices are set in this function
	dereference_method_ = DereferenceMethod::kMethod3;
	mode_ = GL_TRIANGLES;

	// Identify the index of each type of vertices this will contain the total number of different vertices_
	unsigned int v0, v1, v2, v3, v4, v5, v6, v7, v8, v9;
	// initialise indices
	v0 = 0, v1 = v0 + 1, v2 = v0 + 2, v3 = v0 + 3, v4 = v0 + 4, v5 = v0 + 5, v6 = v0 + 6, v7 = v0 + 7, v8 = v0 + 8, v9 = v0 + 9;

	// This Rectangle is made from top left to bottom right

	// Each i is a square in width (along x-axis)
	for (int i = 0; i < width_; i++)
	{
		// Each k is a square in height/depth (along z-axis)
		for (int k = 0; k < height_; k++)
		{
			// NOTE: the comments on the right side of each vertex, for example "//v2 = v4 = v7 = v21 = v17 = v18" would be...
			// ...the number of vertex without reusing vertices_ (method 1 and 2 of dereference)
			// This has been kept in order to make it more understable and visual, the amount of vectors we would have in case of not using the method 3.

			// First round of triangles to create the quad
			// Triangle A
			// v0
			addNormal(0.0f, 1.0f, 0.0f); //+y
			addVertex(0.0f + i, 0.0f, 0.0f - k); //v0 = v3
			//indices_.push_back(v0);
			if (i == 0 && k == 0) // Vertex P (it will be added in first position, it is the top left point)
			{
				PQR_vertices_.push_back(0.0f + i); PQR_vertices_.push_back(0.0f); PQR_vertices_.push_back(0.0f - k); // Px, Py, Pz == v0
			}
			// v1
			addNormal(0.0f, 1.0f, 0.0f); //+y
			addVertex(0.0f + i, 0.0f, -0.5f - k); //v1 = v15
			//indices_.push_back(v1);
			// v2
			addNormal(0.0f, 1.0f, 0.0f); //+y
			addVertex(0.25f + i, 0.0f, -0.5f - k); //v2 = v4 = v7 = v21 = v17 = v18
			//indices_.push_back(v2);
			addTriangleIndices(v0, v1, v2);


			// Triangle B
			// v0
			//addNormal(0.0f, 1.0f, 0.0f); //+y
			//addVertex(0.0f + i, 0.0f, 0.0f - k); //v3 = v0
			//indices_.push_back(v0);
			// v2
			//addNormal(0.0f, 1.0f, 0.0f); //+y
			//addVertex(0.25f + i, 0.0f, -0.5f - k); //v4 = v2 = v7 = v21 = v17 = v18
			//indices_.push_back(v2);
			// v3
			addNormal(0.0f, 1.0f, 0.0f); //+y
			addVertex(0.5f + i, 0.0f, 0.0f - k); //v5 = v6 = v9
			//indices_.push_back(v3);
			addTriangleIndices(v0, v2, v3);


			// Triangle C
			// v3
			//addNormal(0.0f, 1.0f, 0.0f); //+y
			//addVertex(0.5f + i, 0.0f, 0.0f - k); //v6 = v5 = v9
			//indices_.push_back(v3);
			// v2
			//addNormal(0.0f, 1.0f, 0.0f); //+y
			//addVertex(0.25f + i, 0.0f, -0.5f - k); //v7 = v2 = v4 = v21 = v17 = v18
			//indices_.push_back(v2);
			// v4
			addNormal(0.0f, 1.0f, 0.0f); //+y
			addVertex(0.75f + i, 0.0f, -0.5f - k); //v8 = v10 = v13 = v23 = v24 = v27
			//indices_.push_back(v4);
			addTriangleIndices(v3, v2, v4);


			// Triangle D
			// v3
			//addNormal(0.0f, 1.0f, 0.0f); //+y
			//addVertex(0.5f + i, 0.0f, 0.0f - k); //v9 = v6 = v5
			//indices_.push_back(v3);
			// v4
			//addNormal(0.0f, 1.0f, 0.0f); //+y
			//addVertex(0.75f + i, 0.0f, -0.5f - k); //v10 = v8 = v13 = v23 = v24 = v27
			//indices_.push_back(v4);
			// v5
			addNormal(0.0f, 1.0f, 0.0f); //+y
			addVertex(1.0f + i, 0.0f, 0.0f - k); //v11 = v12
			//indices_.push_back(v5);
			if (i == width_ - 1 && k == 0) // Vertex Q (it will be added in the last position, it is the top right point)
			{
				PQR_vertices_.push_back(1.0f + i);  PQR_vertices_.push_back(0.0f); PQR_vertices_.push_back(0.0f - k); // Rx, Ry, Rz == v0
			}
			addTriangleIndices(v3, v4, v5);


			// Triangle E
			// v5
			//addNormal(0.0f, 1.0f, 0.0f); //+y
			//addVertex(1.0f + i, 0.0f,  0.0f - k); //v12 = v11
			//indices_.push_back(v5);
			// v4
			//addNormal(0.0f, 1.0f, 0.0f); //+y
			//addVertex(0.75f + i, 0.0f, -0.5f - k); //v13 = v8 = v10 = v23 = v24 = v27
			//indices_.push_back(v4);
			// v6
			addNormal(0.0f, 1.0f, 0.0f); //+y
			addVertex(1.0f + i, 0.0f, -0.5f - k); //v14 = v29
			//indices_.push_back(v6);
			addTriangleIndices(v5, v4, v6);


			// Second round of triangles to create the quad
			// Triangle F
			// v1
			//addNormal(0.0f, 1.0f, 0.0f); //+y
			//addVertex(0.0f + i, 0.0f, -0.5f - k); //v15 = v1
			//indices_.push_back(v1);
			// v7
			addNormal(0.0f, 1.0f, 0.0f); //+y
			addVertex(0.0f + i, 0.0f, -1.0f - k); //v16 = v19
			//indices_.push_back(v7);
			if (i == 0 && k == height_ - 1) // Vertex R (it will be added in second position, it is the bottom left point)
			{
				PQR_vertices_.push_back(0.0f + i); PQR_vertices_.push_back(0.0f); PQR_vertices_.push_back(-1.0f - k); // Qx, Qy, Qz == v7
			}
			// v2
			//addNormal(0.0f, 1.0f, 0.0f); //+y
			//addVertex(0.25f + i, 0.0f, -0.5f - k); //v17 = v18 = v21 = v2 = v4 = v7
			//indices_.push_back(v2);
			addTriangleIndices(v1, v7, v2);


			// Triangle G
			// v2
			//addNormal(0.0f, 1.0f, 0.0f); //+y
			//addVertex(0.25f + i, 0.0f, -0.5f - k); //v18 = v17 = v21 = v2 = v4 = v7
			//indices_.push_back(v2);
			// v7
			//addNormal(0.0f, 1.0f, 0.0f); //+y
			//addVertex(0.0f + i, 0.0f, -1.0f - k); //v19 = v16
			//indices_.push_back(v7);
			// v8
			addNormal(0.0f, 1.0f, 0.0f); //+y
			addVertex(0.5f + i, 0.0f, -1.0f - k); //v20 = v22 = v25
			//indices_.push_back(v8);
			addTriangleIndices(v2, v7, v8);


			// Triangle H
			// v2
			//addNormal(0.0f, 1.0f, 0.0f); //+y
			//addVertex(0.25f + i, 0.0f, -0.5f - k); //v21 = v17 = v18 = v2 = v4 = v7
			//indices_.push_back(v2);
			// v8
			//addNormal(0.0f, 1.0f, 0.0f); //+y
			//addVertex(0.5f + i, 0.0f, -1.0f - k); //v22 = v20 = v25
			//indices_.push_back(v8);
			// v4
			//addNormal(0.0f, 1.0f, 0.0f); //+y
			//addVertex(0.75f + i, 0.0f,  -0.5f - k); //v23 = v24 = v27 = v8 = v10 = v13
			//indices_.push_back(v4);
			addTriangleIndices(v2, v8, v4);

			// Triangle I
			// v4
			//addNormal(0.0f, 1.0f, 0.0f); //+y
			//addVertex(0.75f + i, 0.0f, -0.5f - k); //v24 = v23 = v27 = v8 = v10 = v13
			//indices_.push_back(v4);
			// v8
			//addNormal(0.0f, 1.0f, 0.0f); //+y
			//addVertex(0.5f + i, 0.0f, -1.0f - k); //v25 = v22 = v20
			//indices_.push_back(v8);
			// v9
			addNormal(0.0f, 1.0f, 0.0f); //+y
			addVertex(1.0f + i, 0.0f, -1.0f - k); //v26 = v28
			//indices_.push_back(v9);
			addTriangleIndices(v4, v8, v9);


			// Triangle J
			// v4
			//addNormal(0.0f, 1.0f, 0.0f); //+y
			//addVertex(0.75f + i, 0.0f, -0.5f - k); //v27 = v23 = v24 = v8 = v10 = v13
			//indices_.push_back(v4);
			// v9
			//addNormal(0.0f, 1.0f, 0.0f); //+y
			//addVertex(1.0f + i, 0.0f, -1.0f - k); //v28 = v26
			//indices_.push_back(v9);
			// v6
			//addNormal(0.0f, 1.0f, 0.0f); //+y
			//addVertex(1.0f + i, 0.0f, -0.5f - k); //v29 = v14
			//indices_.push_back(v6);
			addTriangleIndices(v4, v9, v6);

			//count the new vertices_ added on this round (it has been added 10 new vertices_)
			v0 += 10;
			v1 = v0 + 1, v2 = v0 + 2, v3 = v0 + 3, v4 = v0 + 4, v5 = v0 + 5, v6 = v0 + 6, v7 = v0 + 7, v8 = v0 + 8, v9 = v0 + 9;
		}
	}
}

void MeshRectangle::initTextureCoords(float starting_u, float starting_v, float ending_u, float ending_v)
{
	// if the texture_ coords collection is not empty and this function has been called them it is
	// because the new texture_ has a differente coords type, so it is necessary remove the tex coords 
	// It has been decided use swap instead of clean because the performance of std::vector::sawp() is O(1) and .clean is O(N) time 
	if (!texture_coords_.empty()) 
	{
		vector<float>().swap(texture_coords_);
	}

	// A square has 5 triangles:
	// Vertically (value 'v'):			 Horizonally (value 'u'):
	// ------------------v+v0			   u+u0----u+u2-----u+u4
	// | \  B  /\   D  /|					| \  B  /\   D  /|
	// |  \   /  \    / |					|  \   /  \    / |
	// | A \ /  C \  / E|					| A \ /  C \  / E|
	// ------------------v+v2				----u+u1---u+u3---   
	// | F / \  H  /\  J|					| F / \  H  /\  J|
	// |  /   \   /  \  |					|  /   \   /  \  |
	// | /  G  \ /  I \ |					| /  G  \ /  I \ |
	// ------------------v+v4			   u+u0----u+u2-----u+u4

	// 'u' and 'v' will be initial values for each square of the triangle
	// for example: if a rectangle is dimension 3 width * 2 height squares, u will have the values 0, 1, 2 and v: 0, 1 (for a triangle wich has splitted behaviour)
	// for a triangle which has unit behaviour the u and v values will be proportional to the distance (ending u - starting u) and (ending v - starting v) so if this distance is 1 for u and 1 for v (a no mapped texture_)
	// for the example before the 'u' values will be 0/3, 1/3, 2/3 and v values will be 0/2, and 1/2
	float u, v;

	// a square of a rectangle is divided in 4 parts
	// these values will be added to 'u' and 'v' values to get the coords of the texture_ of the current square is being drawn
	float u0, u1, u2, u3, u4;
	float v0, v2, v4;

	// If the rectangle has a splitted behaviour then each square in terms of textures is divided in 4 sections from 0 to 1 (for a no mapped texture_)
	// For example: if the texture_ use GL_repeat, and a rectangle has splitted behaviour then
	// each square will be assigned that texture_ so for a rectangle of height 2 and width 3 (2 squares * 3 squares) will have 6 times that texture_ repeated.
	// (ending_u - starting_u) and (ending_v - starting_v) are the distance in width and height respectively of the texture_ asigned for this rectangle
	// This distance is useful when the texture_ image is mapped so the texture_ for this rectangle is a proportion of the texture_ image
	// by default the distance of a texture_ is the full image 1 (u and v from 0 to 1)
	if (rectangle_behaviour_type_ == RectangleBehaviourType::kSplitted)
	{
		// for width (it is split in 4 parts)
		u0 = starting_u; // example: in a texture_ no mapped will be 0.0f
		u1 = ((1.0f * (ending_u - starting_u)) / 4.0f) + starting_u; // example: in a texture_ no mapped will be 0.25f
		u2 = ((2.0f * (ending_u - starting_u)) / 4.0f) + starting_u; // example: in a texture_ no mapped will be 0.5f
		u3 = ((3.0f * (ending_u - starting_u)) / 4.0f) + starting_u; // example: in a texture_ no mapped will be 0.75f
		u4 = ((4.0f * (ending_u - starting_u)) / 4.0f) + starting_u; // example: in a texture_ no mapped will be 1.0f

		// for height (it is split in 3 parts)
		v0 = starting_v; // example: in a texture_ no mapped will be 0.0f
		//v1 = ((1.0f * (ending_v - starting_v)) / 4.0f) + starting_v; // example: in a texture_ no mapped will be 0.25f
		v2 = ((2.0f * (ending_v - starting_v)) / 4.0f) + starting_v; // example: in a texture_ no mapped will be 0.5f
		//v3 = ((3.0f * (ending_v - starting_v)) / 4.0f) + starting_v; // example: in a texture_ no mapped will be 0.75f
		v4 = ((4.0f * (ending_v - starting_v)) / 4.0f) + starting_v; // example: in a texture_ no mapped will be 1.0f
	}
	// If the texture_ has an unit behaviour then the whole rectangle will be handle from min 'u' and 'v' coords to max 'u' 'v' coord in terms of textures.
	// so if the texture_ is set to repeat will not work as this rectangle will use a simple texture_ for covering the whole face
	else// if (rectangle_behaviour_type_ == RectangleBehaviourType::kUnit)
	{
		// for width (it is split in 4 parts)
		u0 = starting_u; // example: in a texture_ no mapped will be 0.0f
		u1 = ((1.0f * (ending_u - starting_u)) / (4.0f * width_)) + starting_u; // example: in a texture_ no mapped will be 0.25f scaled to the width of the full rectangle
		u2 = ((2.0f * (ending_u - starting_u)) / (4.0f * width_)) + starting_u; // example: in a texture_ no mapped will be 0.5f scaled to the width of the full rectangle
		u3 = ((3.0f * (ending_u - starting_u)) / (4.0f * width_)) + starting_u; // example: in a texture_ no mapped will be 0.75f scaled to the width of the full rectangle
		u4 = ((4.0f * (ending_u - starting_u)) / (4.0f * width_)) + starting_u; // example: in a texture_ no mapped will be 1.00f scaled to the width of the full rectangle

		// for height (it is split in 3 parts)
		v0 = starting_v; // example: in a texture_ no mapped will be 0.0f
		//v1 = ((1.0f * (ending_v - starting_v)) / (4.0f * height_)) + starting_v; // example: in a texture_ no mapped will be 0.25f scaled to the height of the full rectangle
		v2 = ((2.0f * (ending_v - starting_v)) / (4.0f * height_)) + starting_v; // example: in a texture_ no mapped will be 0.5f scaled to the height of the full rectangle
		//v3 = ((3.0f * (ending_v - starting_v)) / (4.0f * height_)) + starting_v; // example: in a texture_ no mapped will be 0.75f scaled to the height of the full rectangle
		v4 = ((4.0f * (ending_v - starting_v)) / (4.0f * height_)) + starting_v; // example: in a texture_ no mapped will be 1.0f scaled to the height of the full rectangle
	}

	// Each i is a square in height (along y-axis)
	for (int i = 0; i < height_; i++) 
	{
		// Each k is a square in width (along x-axis)
		for (int j = 0; j < width_; j++) 
		{
			// for splitted behaviour The coord of the texture_ will be from 0 to (i plus u4)==height, and from 0 to (j plus v4)== width
			if (rectangle_behaviour_type_ == RectangleBehaviourType::kSplitted)
			{
				u = i;
				v = j;
			}
			// for extended behaviour the coord of the texture_ will be from 0 to 1 in u and v
			else// if (rectangle_behaviour_type_ == RectangleBehaviourType::kUnit)
			{
				u = (((float)i * (ending_u - starting_u)) / ((float)height_));
				v = (((float)j * (ending_v - starting_v)) / ((float)width_));
			}

			// NOTE:: The addTexCoord(..) commented would be if we used the dereference method 1 and 2 (not using indices_)
			// so that texture_ coords are for the vectors which are repeated.

			// First round of triangles to create the quad

			// Triangle A
			addTexCoord(u + u0, v + v0);
			addTexCoord(u + u0, v + v2);
			addTexCoord(u + u1, v + v2);

			// Triangle B
			//addTexCoord(u + u0, v + v0);
			//addTexCoord(u + u1, v + v2);
			addTexCoord(u + u2, v + v0);

			// Triangle C
			//addTexCoord(u + u2, v + v0);
			//addTexCoord(u + u1, v + v2);
			addTexCoord(u + u3, v + v2);

			// Triangle D
			//addTexCoord(u + u2, v + v0);
			//addTexCoord(u + u3, v + v2);
			addTexCoord(u + u4, v + v0);

			// Triangle E
			//addTexCoord(u + u4, v + v0);
			//addTexCoord(u + u3, v + v2);
			addTexCoord(u + u4, v + v2);


			// Second row of triangles to create the quad

			// Triangle F
			//addTexCoord(u + u0, v + v2);
			addTexCoord(u + u0, v + v4);
			//addTexCoord(u + u1, v + v2);

			// Triangle G
			//addTexCoord(u + u1, v + v2);
			//addTexCoord(u + u0, v + v4);
			addTexCoord(u + u2, v + v4);

			// Triangle H
			//addTexCoord(u + u1, v + v2);
			//addTexCoord(u + u2, v + v4);
			//addTexCoord(u + u3, v + v2);

			// Triangle I
			//addTexCoord(u + u3, v + v2);
			//addTexCoord(u + u2, v + v4);
			addTexCoord(u + u4, v + v4);

			// Triangle J
			//addTexCoord(u + u3, v + v2);
			//addTexCoord(u + u4, v + v4);
			//addTexCoord(u + u4, v + v2);
		}
	}
}
