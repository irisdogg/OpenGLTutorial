#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>

class Light
{
public:
	Light();
	Light( GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat dIntensity );
	~Light();

	void UseLight( GLfloat ambientIntensityLocation, GLfloat colourLocation, GLfloat diffuseIntensityLocation );

// protected because the other lights are inheriting these values
protected:
	glm::vec3 colour;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
};

