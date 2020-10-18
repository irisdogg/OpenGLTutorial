#pragma once
#include "Light.h"

class DirectionalLight :
	public Light
{
public:
	DirectionalLight();
	~DirectionalLight();

	DirectionalLight( GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity );
	void UseLight( GLfloat ambientIntensityLocation, GLfloat colourLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation );


private:
	glm::vec3 direction;
};

