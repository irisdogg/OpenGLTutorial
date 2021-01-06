#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

#include "ShadowMap.h"

class Light
{
public:
	Light();
	Light( GLfloat shadowWidth, GLfloat shadowHeight, 
			GLfloat red, GLfloat green, GLfloat blue, 
			GLfloat ambientIntensity, GLfloat dIntensity );
	~Light();

	ShadowMap* GetShadowMap() { return shadowMap; }

// protected because the other lights are inheriting these values
protected:
	glm::vec3 colour;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	glm::mat4 lightProj;

	ShadowMap* shadowMap;

};

