#pragma once
#include "Light.h"

class DirectionalLight :
	public Light
{
public:
	DirectionalLight();
	~DirectionalLight();

	DirectionalLight( GLfloat shadowWidth, GLfloat shadowHeight,
						GLfloat red, GLfloat green, GLfloat blue,
						GLfloat aIntensity, GLfloat dIntensity, 
						GLfloat xDir, GLfloat yDir, GLfloat zDir );
	
	void UseLight( GLfloat ambientIntensityLocation, GLfloat colourLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation );

	glm::mat4 CalculateLightTransform();

private:
	glm::vec3 direction;
};

