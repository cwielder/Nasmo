#include <nsm/app/application.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

nsm::Application::Application(const ApplicationInfo& info) {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(info.window.size.x, info.window.size.y, info.window.title.c_str(), NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

nsm::Application::~Application() { }

void nsm::Application::run() {
    while (!glfwWindowShouldClose(glfwGetCurrentContext())) {
        glfwSwapBuffers(glfwGetCurrentContext());
        glfwPollEvents();

        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}
