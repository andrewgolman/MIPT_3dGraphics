// Include standard headers
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <iostream>  // for debugging

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "controls.hpp"
#include "objects.hpp"
//#include <common/texture.hpp>


GLFWwindow* initialize() {
    // Initialise GLFW
    if(!glfwInit()) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        exit(-1);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow( 1024, 768, "GoChi", NULL, NULL);
    if(window == NULL) {
        fprintf( stderr, "Failed to open GLFW window.\n" );
        getchar();
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        exit(-1);
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited movement
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
//    glEnable(GL_CULL_FACE);  todo ENABLE after debug

    return window;
}


bool is_too_far(const Object& object) {
    return false; //TODO
}


void move_objects(std::unordered_set<Object>& objects,
                  const std::unordered_map<int, glm::vec3>& speeds) {
//    for (auto& object : objects) {
//        object.move(speeds.get(object));
//        if (is_too_far(object)) {
//            object.undraw();
//            objects.erase(object);
//        }
//    }
}


bool are_close(const Object& lhs, const Object& rhs) {
    return false; //TODO
}


void create_target(std::unordered_set<Target>& targets,
                   std::unordered_map<int, glm::vec3>& speeds) {
//    target = Target();
//    target.move(glm::vec3(0, 0, 10));
//    targets.emplace_back(target);
//    speeds[target] = glm::vec3(0, 0, 0)
}


void create_fireball(std::unordered_set<Fireball>& fireballs,
                     std::unordered_map<int, glm::vec3>& speeds) {
//    fireball = Fireball(0.5, 100);
//    fireballs.emplace_back(fireball);
//    speeds[fireball] = glm::vec3(0, 0, 1);
}


bool is_space_pressed() {
    return true; //TODO
}


int main() {
    GLFWwindow* window = initialize();

    // Create and compile our GLSL program from the shaders
    GLuint ProgramID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(ProgramID, "MVP");

//     Get a handle for our buffers
    GLuint vertexPosition_modelspaceID = glGetAttribLocation(ProgramID, "vertexPosition_modelspace");
    GLuint vertexColorID = glGetAttribLocation(ProgramID, "vertexColor");

//    std::unordered_set<Fireball> fireballs;
//    std::unordered_set<Target> targets;
//    std::unordered_map<Object, glm::vec3> speeds;
    Buffer buffer;
    Floor floor;
    floor.draw(buffer);

    static const GLfloat g_color_buffer_data[] = {
            0.5, 0.5, 0.2,
            0.5, 0.5, 0.2,
            0.5, 0.5, 0.2,
            0.5, 0.5, 0.2,
            0.5, 0.5, 0.2,
            0.5, 0.5, 0.2,
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * buffer.size(), buffer.vertex_data(), GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    size_t iteration = 0;
    do {
//        move_objects(fireballs, speeds);
//        move_objects(targets, speeds);
//
        if (iteration % 1000 == 0) {
//            create_target(targets, speeds);
        }
        if (is_space_pressed()) {
//            create_fireball(fireballs, speeds);
        }
//        for (auto& fireball : fireballs) {
//            for (auto& target : targets) {
//                if (are_close(fireball, target)) {
//                    fireball.undraw();
//                    target.undraw();
//                    fireballs.remove(fireball);
//                    targets.remove(target);
//                }
//            }
//        }

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Get position from controls
        Controls::computeMatricesFromInputs(window);
        glm::mat4 ProjectionMatrix = Controls::getProjectionMatrix();
        glm::mat4 ViewMatrix = Controls::getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        glUseProgram(ProgramID);
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(vertexPosition_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, 4 * buffer.size(), buffer.vertex_data(), GL_STATIC_DRAW);
        glVertexAttribPointer(vertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(vertexColorID);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
        glVertexAttribPointer(vertexColorID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, 8 * 3);
        glDisableVertexAttribArray(vertexPosition_modelspaceID);
        glDisableVertexAttribArray(vertexColorID);
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        ++iteration;

    } // Check if the ESC key was pressed or the window was closed
    while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
          && glfwWindowShouldClose(window) == 0);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}
