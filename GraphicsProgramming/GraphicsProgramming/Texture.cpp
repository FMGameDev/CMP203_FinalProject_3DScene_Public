#include "Texture.h"

Texture::Texture(const char texture_url[], TextureCoordsType texture_coords_type, bool y_inverted)
	: texture_coords_type_(texture_coords_type)
{
	if (!y_inverted)
	{
		texture_ = SOIL_load_OGL_texture(
			texture_url,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT //| SOIL_FLAG_INVERT_Y // Depending on texture file type some need soild flag inverted y others don't.
		);
	}
	else
	{
		texture_ = SOIL_load_OGL_texture(
			texture_url,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y // Depending on texture file type some need soild flag inverted y others don't.
		);
	}

	//check for an error during the load process
	if (texture_ == 0)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}
}

Texture::~Texture()
{

}


TextureCoordsType Texture::getTextureCoordsType() const
{
	return texture_coords_type_;
}

void Texture::setWrapST(GLint wrap_s, GLint wrap_t)
{
	wrap_s_ = wrap_s; // it is for the 'u' or 'x' axis
	wrap_t_ = wrap_t; // it is for the 'v'or 'y' axis
}

void Texture::use()
{
	glEnable(GL_TEXTURE_2D); // allows polygons to use textures

	// bind the texture
	glBindTexture(GL_TEXTURE_2D, texture_); // tells to openGl to use this texture_

	// set if the texture is going to be is repeated, mirrored, clamped...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t_);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void Texture::stopUsing()
{
	glBindTexture(GL_TEXTURE_2D, NULL); // bind a null texture

	glDisable(GL_TEXTURE_2D); // disable texture
}
