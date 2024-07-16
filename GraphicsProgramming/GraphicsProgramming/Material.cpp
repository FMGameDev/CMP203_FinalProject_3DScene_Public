#include "Material.h"

Material::Material(GLenum mat_face, Colour4 mat_ambient_colour, Colour4 mat_diffuse_colour, Colour4 mat_specular_colour, Colour4 emission_colour, GLfloat shininess)
	: material_face_(mat_face), 
	  material_ambient_colour_(mat_ambient_colour), material_diffuse_colour_(mat_diffuse_colour), material_specular_colour_(mat_specular_colour), emission_colour_(emission_colour), shininess_(shininess)
{

}


Material::~Material()
{
}


void Material::render()
{
	glMaterialfv(material_face_, GL_AMBIENT, material_ambient_colour_.rgba);
	glMaterialfv(material_face_, GL_DIFFUSE, material_diffuse_colour_.rgba);
	glMaterialfv(material_face_, GL_SPECULAR, material_specular_colour_.rgba);

	glMaterialfv(material_face_, GL_EMISSION, emission_colour_.rgba);
	glMaterialfv(material_face_, GL_SHININESS, &shininess_);

	// reset colour
	glColor4f(0.1f, 0.1f, 0.1f, 1.0f);
}


void Material::decreaseShininessBy(GLfloat numberToSubstract)
{
	if (shininess_ - numberToSubstract >= 0.0f) // control to avoid negative shininess_
	{
		shininess_ -= numberToSubstract;
	}
	else
	{
		shininess_ = 0.0f; // set the minimum 
	}
}


void Material::increaseShininessBy(GLfloat numberToAdd)
{
	if (shininess_ + numberToAdd <= 128.0f) // control to avoid negative shininess_
	{
		shininess_ += numberToAdd;
	}
	else
	{
		shininess_ = 128.0f; // set the maximum
	}
}
