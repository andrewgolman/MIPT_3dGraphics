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
//#include <common/texture.hpp>
#include <tutorial06_keyboard_and_mouse/controls.hpp>

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
    window = glfwCreateWindow( 1024, 768, "GoChi", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window.\n" );
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
    // Hide the mouse and enable unlimited movement
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    // Create and compile our GLSL program from the shaders
    GLuint floorProgramID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );

    // Get a handle for our "MVP" uniform
    GLuint floorMatrixID = glGetUniformLocation(floorProgramID, "MVP");

    // Get a handle for our buffers
    GLuint vertexPosition_modelspaceID = glGetAttribLocation(floorProgramID, "vertexPosition_modelspace");
    GLuint vertexColorID = glGetAttribLocation(floorProgramID, "vertexColor");

    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model      = glm::mat4(1.0f);

    static const GLfloat floor_vertex_buffer_data[] = {
           -5.0f, 0.0f, -5.0f,
            5.0f, 0.0f,  5.0f,
            5.0f, 0.0f, -5.0f,
            5.0f, 0.0f,  5.0f,
           -5.0f, 0.0f, -5.0f,
           -5.0f, 0.0f,  5.0f,
    };
    static const GLfloat floor_color_buffer_data[] = {
            0.5, 0.3, 0.1,
            0.5, 0.3, 0.1,
            0.5, 0.3, 0.1,
            0.5, 0.3, 0.1,
            0.5, 0.3, 0.1,
            0.5, 0.3, 0.1,
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertex_buffer_data), floor_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor_color_buffer_data), floor_color_buffer_data, GL_STATIC_DRAW);

    // initial pos
    vec3 Eye = glm::vec3(0, 0, 2);
    vec3 Center = glm::vec3(1, 0, 0);
    vec3 Up = glm::vec3(0, 1, 0);

    float time = 0.f;

    do{
        time += 1.f;

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Controls::computeMatricesFromInputs();
        glm::mat4 ProjectionMatrix = Controls::getProjectionMatrix();
        glm::mat4 ViewMatrix = Controls::getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // Use our shader
        glUseProgram(floorProgramID);

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(floorMatrixID, 1, GL_FALSE, &MVP[0][0]);

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
        glDrawArrays(GL_TRIANGLES, 0, 8*3); // 12*3 indices starting at 0 -> 12 triangles

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
    glDeleteProgram(floorProgramID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

