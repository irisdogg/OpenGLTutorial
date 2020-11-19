#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "Material.h"


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;

Material shinyMaterial;
Material dullMaterial;

DirectionalLight mainLight;

char* brickFileLocation = "Textures/brick.png";
char* dirtFileLocation = "Textures/dirt.png";

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Create Vertex Shader
static const char* vShader = "Shaders/shader.vert";			

// Create Fragment Shader
static const char* fShader = "Shaders/shader.frag";			

void CalcAverageNormals( unsigned int* indices, unsigned int indicesCount, GLfloat* vertices, unsigned int verticesCount, 
							unsigned int vLength, unsigned int normalOffset )
{
	for( size_t i = 0; i < indicesCount; i += 3 )
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1( vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2] );
		glm::vec3 v2( vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2] );
		glm::vec3 normal = glm::cross( v1, v2 );
		normal = glm::normalize( normal );

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		vertices[in0] += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;

		vertices[in1] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;

		vertices[in2] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}

	for( size_t i = 0; i < verticesCount / vLength; ++i )
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec( vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2] );
		vec = glm::normalize( vec );
		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	// indices for triangle
	GLfloat vertices[] = {
	//	x		y		z		u	  v			nx	  ny	nz
		-1.0f, -1.0f, -0.6f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	CalcAverageNormals( indices, 12, vertices, 32, 8, 5 );

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh( vertices, indices, 32, 12 );
	meshList.push_back( obj1 );

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh( vertices, indices, 32, 12 );
	meshList.push_back( obj2 );

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles( vShader, fShader );
	shaderList.push_back( shader1 );
}

int main()
{
	mainWindow = Window( 1366, 768 );
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	auto startPos = glm::vec3( 0.0f, 0.0f, 0.0f );
	auto up = glm::vec3( 0.0f, 1.0f, 0.0f );
	camera = Camera( startPos, up, -90.0f, 0.0f, 5.0f, 0.5f );

	brickTexture = Texture( brickFileLocation );
	brickTexture.LoadTexture();

	dirtTexture = Texture( dirtFileLocation );
	dirtTexture.LoadTexture();

	shinyMaterial = Material( 1.0f, 32 );
	dullMaterial = Material( 0.3f, 4 );

	mainLight = DirectionalLight( 1.0f, 1.0f, 1.0f,		//colour
									0.1f, 0.3f,			// ambient, direction intensity
									0.0f, 0.0f, -1.0);	// x, y ,z direction


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformAmbientIntensity = 0, uniformEyePosition = 0,
		uniformAmbientColour = 0, uniformDirection = 0, uniformDiffuseIntensity = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;

	glm::mat4 projection = glm::perspective(45.0f, mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while( !mainWindow.GetShouldClose() )
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + handle user input events
		glfwPollEvents();

		camera.KeyControl( mainWindow.GetKeys(), deltaTime );
		camera.MouseControl( mainWindow.GetXChange(), mainWindow.GetYChange() );

		// Clear window
		glClearColor( 0.f, 0.f, 0.f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();
		uniformView = shaderList[0]->GetViewLocation();
		uniformAmbientColour = shaderList[0]->GetAmbientColourLocation();
		uniformAmbientIntensity = shaderList[0]->GetAmbientIntensityLocation();
		uniformDirection = shaderList[0]->GetDirectionLocation();
		uniformDiffuseIntensity = shaderList[0]->GetDiffuseIntensityLocation();
		uniformEyePosition = shaderList[0]->GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
		uniformShininess = shaderList[0]->GetShininessLocation();

		mainLight.UseLight( uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity, uniformDirection );

		glUniformMatrix4fv( uniformProjection, 1, GL_FALSE, glm::value_ptr( projection ) );
		glUniformMatrix4fv( uniformView, 1, GL_FALSE, glm::value_ptr( camera.CalculateViewMatrix() ) );
		glUniform3f( uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z );

		glm::mat4 model( 1.0f );

		model = glm::translate( model, glm::vec3( 0.0f, 0.0f, -2.5f ) );

		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );

		brickTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();

		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( 0.0f, 4.0f, -2.5f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );

		dirtTexture.UseTexture();
		dullMaterial.UseMaterial( uniformSpecularIntensity, uniformShininess );
		meshList[1]->RenderMesh();

		glUseProgram( 0 );

		mainWindow.SwapBuffers();
	}

	return 0;
}