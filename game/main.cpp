// Include standard headers
#include <cstdio>
#include <cstdlib>
#include <vector>


// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;  // todo move

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
//#include <common/texture.hpp>
#include <tutorial06_keyboard_and_mouse/controls.hpp>

void initialize() {
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        exit(-1);
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
}

int main()
{
    initialize();

    // Create and compile our GLSL program from the shaders
    GLuint ProgramID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );

    // Get a handle for our "MVP" uniform
    GLuint floorMatrixID = glGetUniformLocation(ProgramID, "MVP");

    // Get a handle for our buffers
    GLuint vertexPosition_modelspaceID = glGetAttribLocation(ProgramID, "vertexPosition_modelspace");
    GLuint vertexColorID = glGetAttribLocation(ProgramID, "vertexColor");

    std::vector<GLfloat> g_vertex_buffer_data = {
           -5.0f, 0.0f, -5.0f,
            5.0f, 0.0f,  5.0f,
            5.0f, 0.0f, -5.0f,
            5.0f, 0.0f,  5.0f,
           -5.0f, 0.0f, -5.0f,
           -5.0f, 0.0f,  5.0f,
    };
    std::vector<GLfloat> g_color_buffer_data = {
            0.5, 0.3, 0.1,
            0.5, 0.3, 0.1,
            0.5, 0.3, 0.1,
            0.5, 0.3, 0.1,
            0.5, 0.3, 0.1,
            0.5, 0.3, 0.1,
    };

    // add objects to buffer
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, buffer.size() * 4, buffer.data(), GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, g_color_buffer_data.size() * 4, g_color_buffer_data.data(), GL_STATIC_DRAW);

    vector<Object> fireballs;
    vector<Object> targets;
    map<Object, vec3> speeds;
    Buffer buffer;
    // add floor

    do {
        for (o : fireballs) {
            o.move(speeds.get(o));
            if (is_too_far(o)) {
                o.undraw();
                fireballs.remove(o);
            }
        }
        for (o : targets) {
            o.move(speeds.get(o));
            if (is_too_far(o)) {
                o.undraw();
                targets.remove(o);
            }
        }

        if (rand()) {
            create_target;
        }
        if (space_pressed) {
            create_fireball;
        }
        for (o : fireballs) {
            for (q : targets) {
                if isclose(o, q) {
                    o.undraw()
                    q.undraw()
                    make_explosion(o, q);
                    fireballs.remove(o);
                    targets.remove(q);
                }
            }
        }

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Get position from controls
        Controls::computeMatricesFromInputs();
        glm::mat4 ProjectionMatrix = Controls::getProjectionMatrix();
        glm::mat4 ViewMatrix = Controls::getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // Use main shader
        glUseProgram(ProgramID);

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(floorMatrixID, 1, GL_FALSE, &MVP[0][0]);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(vertexPosition_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(vertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(vertexColorID);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(vertexColorID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Draw the triangles
        glDrawArrays(GL_TRIANGLES, 0, buffer.size() / 3);

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
    glDeleteProgram(ProgramID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
