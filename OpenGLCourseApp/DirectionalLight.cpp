#include "DirectionalLight.h"


DirectionalLight::DirectionalLight()
{
	colour = glm::vec3( 1.0f, 1.0f, 1.0f );
	ambientIntensity = 1.0f;

	direction = glm::vec3( 0.0f, -1.0f, 0.0f );
	diffuseIntensity = 0.0f;
}

DirectionalLight::DirectionalLight( GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity )
{
	colour = glm::vec3( red, green, blue );
	ambientIntensity = aIntensity;

	direction = glm::vec3( xDir, yDir, zDir );
	diffuseIntensity = dIntensity;
}
