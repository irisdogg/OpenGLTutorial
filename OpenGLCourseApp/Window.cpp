#include "Window.h"

Window::Window()
{
	xChange = 0.0f;
	yChange = 0.0f;
	width = 800;
	height = 600;

	for( size_t i = 0; i < 1024; ++i )
	{
		keys[i] = 0;
	}
}

Window::Window( GLint windowWidth, GLint windowHeight )
{
	xChange = 0.0f;
	yChange = 0.0f;
	width = windowWidth;
	height = windowHeight;

	for( size_t i = 0; i < 1024; ++i )
	{
		keys[i] = 0;
	}
}

Window::~Window()
{
	glfwDestroyWindow( mainWindow );
	glfwTerminate();
}

int Window::Initialise()
{
	// Initialize GLFW
	if( !glfwInit() )
	{
		printf( "GLFW initialization failed!" );
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	// Core profile = no backwards compatibility
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	// Allow forward compatibility
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

	mainWindow = glfwCreateWindow( width, height, "Test Window", NULL, NULL );
	if( !mainWindow )
	{
		printf( "GLFW window creation failed!" );
		glfwTerminate();
		return 1;
	}

	glfwGetFramebufferSize( mainWindow, &bufferWidth, &bufferHeight );

	// Set context for GLEW to use
	glfwMakeContextCurrent( mainWindow );

	// Handle key + mouse input
	CreateCallbacks();
	glfwSetInputMode( mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if( glewInit() != GLEW_OK )
	{
		printf( "GLEW initialization failed!" );
		glfwDestroyWindow( mainWindow );
		glfwTerminate();
		return 1;
	}

	glEnable( GL_DEPTH_TEST );

	// Setup Viewport size
	glViewport( 0, 0, bufferWidth, bufferHeight );

	glfwSetWindowUserPointer( mainWindow, this );
}

void Window::CreateCallbacks()
{
	glfwSetKeyCallback( mainWindow, HandleKeys );
	glfwSetCursorPosCallback( mainWindow, HandleMouse );
}

GLfloat Window::GetXChange()
{
	GLfloat change = xChange;
	xChange = 0.0f;
	return change;
}

GLfloat Window::GetYChange()
{
	GLfloat change = yChange;
	yChange = 0.0f;
	return change;
}

void Window::HandleKeys( GLFWwindow *window, int key, int code, int action, int mode )
{
	Window* theWindow = static_cast<Window*>( glfwGetWindowUserPointer( window ) );

	if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
	{
		glfwSetWindowShouldClose( window, GL_TRUE );
	}

	if( key >= 0 && key < 1024 )
	{
		if( action == GLFW_PRESS )
		{
			theWindow->keys[key] = true;
		}
		else if( action == GLFW_RELEASE )
		{
			theWindow->keys[key] = false;
		}
	}
}

void Window::HandleMouse( GLFWwindow *window, double xPos, double yPos )
{
	Window* theWindow = static_cast<Window*>( glfwGetWindowUserPointer( window ) );

	if( theWindow->mouseFirstMoved )
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}