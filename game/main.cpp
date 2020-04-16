// Include standard headers
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <unordered_set>
#include <unordered_map>


// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/shader.hpp>
#include <controls.hpp>
//#include <common/texture.hpp>


void initialize(GLFWwindow* window) {
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
    window = glfwCreateWindow( 1024, 768, "Shooter", NULL, NULL);
    if(window == NULL) {
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


bool is_too_far(const Object& object) {
    return false; //TODO
}


void move_objects(std::unordered_set<Object>& objects,
    const std::unordered_map<glm::vec3>& speeds) {
    for (auto& object : objects) {
        object.move(speeds.get(object));
        if (is_too_far(object)) {
            object.undraw();
            objects.erase(object);
        }
    }
}


bool are_close(const Object& lhs, const Object& rhs) {
    return false; //TODO
}


void create_target(std::unordered_set<Target>& targets,
    std::unordered_map<glm::vec3>& speeds) {
    target = Target();
    target.move(glm::vec3(0, 0, 10));
    targets.emplace_back(target);
    speeds[target] = glm::vec3(0, 0, 0)
}


void create_fireball(std::unordered_set<Fireball>& fireballs,
    std::unordered_map<glm::vec3>& speeds) {
    fireball = Fireball(0.5, 100);
    fireballs.emplace_back(fireball);
    speeds[fireball] = glm::vec3(0, 0, 1);
}


bool is_space_pressed() {
    return true; //TODO
}


int main() {
    GLFWwindow* window;
    initialize(window);

    std::unordered_set<Fireball> fireballs;
    std::unordered_set<Target> targets;
    std::unordered_map<Object, glm::vec3> speeds;
    Buffer buffer;

    size_t iteration = 0;
    do {
        move_objects(fireballs, speeds);
        move_objects(targets, speeds);

        if (iteration % 1000 == 0) {
            create_target(targets, speeds);
        }
        if (is_space_pressed()) {
            create_fireball(fireballs, speeds);
        }
        for (auto& fireball : fireballs) {
            for (auto& target : targets) {
                if (are_close(fireball, target)) {
                    fireball.undraw()
                    target.undraw()
                    fireballs.remove(fireball);
                    targets.remove(target);
                }
            }
        }

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Get position from controls
        Controls::computeMatricesFromInputs(window);
        glm::mat4 ProjectionMatrix = Controls::getProjectionMatrix();
        glm::mat4 ViewMatrix = Controls::getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(floorMatrixID, 1, GL_FALSE, &MVP[0][0]);

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
