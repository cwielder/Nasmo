#include "nsm/test.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void nsm::createWindow() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(640, 480, "Nasmo", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();

        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}
