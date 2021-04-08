#include <iostream>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Shader.h"
#include "Mesh.h"
#include "ObjectLoader.h"
#include "GameObject.h"
#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "GameEngine.h"
#include "GameLogic.h"

using namespace std;
using namespace glm;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

GameEngine::GameEngine(std::shared_ptr<GameLogic>& logic, int w, int h)
    : logic_(logic)
{
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        cerr << "glfw fail" << endl;
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(w, h, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);

    GLenum error = glewInit();
    if (error != GLEW_OK) {
        cerr << "glew fail" << glewGetErrorString(error) << endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);

    logic_->init();

    float prev_time = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        float cur_time = glfwGetTime();
        float delta_time = cur_time - prev_time;
        prev_time = cur_time;

        int width, height;

        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        logic_->width = width;
        logic_->height = height;
        logic_->time = cur_time;
        logic_->delta_time = delta_time;
        logic_->input();
        logic_->update();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
