#include "common.h"
#include "renderer/renderer2d.h"

namespace renderer2d = peculiar::renderer2d;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    auto window = glfwCreateWindow(800, 600, "PECULIAR", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 600);

    renderer2d::init(800, 600);

    glm::vec2 position(0.0f);
    glm::vec2 velocity(250.0f, 250.0f);
    glm::vec2 dimension(50.0f);

    float last_time = 0;
    while (!glfwWindowShouldClose(window)) {
        float current_time = static_cast<float>(glfwGetTime());
        float delta_time = current_time - last_time;
        last_time = current_time;

        glfwPollEvents();

        position += velocity * delta_time;

        if (position.x + dimension.x >= 800 || position.x <= 0) {
            velocity.x *= -1;
        }

        if (position.y + dimension.y >= 600 || position.y <= 0) {
            velocity.y *= -1;
        }

        renderer2d::begin_drawing();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer2d::draw_quad(position, dimension, glm::vec3(1.0, 0.0, 0.0));

        renderer2d::end_drawing();

        glfwSwapBuffers(window);
    }

    renderer2d::shutdown();

    glfwTerminate();

    return 0;
}