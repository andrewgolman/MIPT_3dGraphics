// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);


	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Tutorial 04 - Colored Cube", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Get a handle for our buffers
	GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
	GLuint vertexColorID = glGetAttribLocation(programID, "vertexColor");

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model      = glm::mat4(1.0f);

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	float h = sqrt(2);
	static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f,-1.0f, 0.0f,  // 1
		-1.0f, 1.0f, 0.0f,
		 0.0f, 0.0f, h,
		-1.0f, 1.0f, 0.0f,  // 2
		 1.0f, 1.0f, 0.0f,
		 0.0f, 0.0f, h,
		 1.0f, 1.0f, 0.0f,  // 3
		 1.0f,-1.0f, 0.0f,
		 0.0f, 0.0f, h,
		 1.0f,-1.0f, 0.0f,  // 4
		-1.0f,-1.0f, 0.0f,
		 0.0f, 0.0f, h,
		-1.0f,-1.0f, 0.0f,  // 5
		-1.0f, 1.0f, 0.0f,
		 0.0f, 0.0f, -h,
		 -1.0f, 1.0f, 0.0f,  // 6
		 1.0f, 1.0f, 0.0f,
		 0.0f, 0.0f, -h,
		 1.0f, 1.0f, 0.0f,  // 7
		 1.0f,-1.0f, 0.0f,
		 0.0f, 0.0f, -h,
		 1.0f,-1.0f, 0.0f,  // 8
		 -1.0f,-1.0f, 0.0f,
		 0.0f, 0.0f, -h,
	};

	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = { 
		0.3, 0.3, 0.3,  // 1
		0.1, 0.8, 0.1,
		0.8, 0.8, 0.8,

		0.3, 0.8, 0.3,  // 2
		0.5, 0.05, 0.2,
		0.8, 0.8, 0.8,

		0.5, 0.2, 0.05,  // 3
		0.2, 0.5, 0.7,
		0.8, 0.8, 0.8,

		0.0, 0.7, 0.7,  // 4
		0.3, 0.3, 0.3,
		0.8, 0.8, 0.8,

		0.3, 0.3, 0.3,  // 5
		0.2, 0.8, 0.2,
		0.2, 0.2, 0.2,

		0.4, 0.8, 0.4,  // 6
		0.5, 0.15, 0.10,
		0.2, 0.2, 0.2,

		0.5, 0.10, 0.15,  // 7
		0.5, 0.7, 0.0,
		0.2, 0.2, 0.2,

		0.5, 0.7, 0.0,  // 8
		0.3, 0.3, 0.3,
		0.2, 0.2, 0.2,
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	// initial pos
	vec3 Eye = glm::vec3(2, 2, 0);
	vec3 Center = glm::vec3(0, 0, 0);
	vec3 Up = glm::vec3(0, 1, 0);

	float time = 0.f;

	do{
		time += 1.f;

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Eye = glm::vec3(2 * cos(time / 100), 2, 2 * sin(time / 100));
		// Camera matrix
		glm::mat4 View = glm::lookAt(Eye, Center, Up);
		// Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around


		// Use our shader
		glUseProgram(programID);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(vertexPosition_modelspaceID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			vertexPosition_modelspaceID, // The attribute we want to configure
			3,                           // size
			GL_FLOAT,                    // type
			GL_FALSE,                    // normalized?
			0,                           // stride
			(void*)0                     // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(vertexColorID);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			vertexColorID,               // The attribute we want to configure
			3,                           // size
			GL_FLOAT,                    // type
			GL_FALSE,                    // normalized?
			0,                           // stride
			(void*)0                     // array buffer offset
		);

		// Draw the triangleS !
		glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(vertexPosition_modelspaceID);
		glDisableVertexAttribArray(vertexColorID);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

