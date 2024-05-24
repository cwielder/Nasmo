#include "nsm/test.h"

#include <GLFW/glfw3.h>

void nsm::createWindow() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(640, 480, "Nasmo", NULL, NULL);
    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
