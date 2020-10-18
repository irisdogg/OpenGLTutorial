#include "Light.h"


Light::Light()
{
	colour = glm::vec3( 1.0f, 1.0f, 1.0f );
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}

Light::Light( GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity )
{
	colour = glm::vec3( red, green, blue );
	ambientIntensity = aIntensity;

	diffuseIntensity = dIntensity;
}

Light::~Light()
{

}

void Light::UseLight( GLfloat ambientIntensityLocation, GLfloat colourLocation, GLfloat diffuseIntensityLocation )
{
	glUniform3f( colourLocation, colour.x, colour.y, colour.z );
	glUniform1f( ambientIntensityLocation, ambientIntensity );

	glUniform3f( directionLocation, direction.x, direction.y, direction.z );
	glUniform1f( diffuseIntensityLocation, diffuseIntensity );
}