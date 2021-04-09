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

GameEngine::GameEngine(std::shared_ptr<GameLogic>& logic, int w, int h)
    : logic_(logic)
{
    glfwSetErrorCallback([](int error, const char* description) {
        fprintf(stderr, "Error: %s\n", description);
    });
    if (!glfwInit())
    {
        cerr << "glfw fail" << endl;
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window_ = glfwCreateWindow(w, h, "Simple example", NULL, NULL);
    if (!window_)
    {
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwSetWindowUserPointer(window_, this);

    glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    });
    glfwSetFramebufferSizeCallback(window_, [](GLFWwindow* window, int w, int h) {
        GameEngine* engine = static_cast<GameEngine*>(glfwGetWindowUserPointer(window));
        if (engine) engine->render();
    });
    glfwSetWindowPosCallback(window_, [](GLFWwindow* window, int x, int y) {
        GameEngine* engine = static_cast<GameEngine*>(glfwGetWindowUserPointer(window));
        if (engine) engine->render();
    });

    glfwMakeContextCurrent(window_);

    GLenum error = glewInit();
    if (error != GLEW_OK) {
        cerr << "glew fail" << glewGetErrorString(error) << endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);

    logic_->init();
    prev_time_ = glfwGetTime();

    while (!glfwWindowShouldClose(window_))
    {
        render();
        glfwPollEvents();
    }

    glfwDestroyWindow(window_);
    glfwTerminate();
}

void GameEngine::render()
{
    float cur_time = glfwGetTime();
    float delta_time = cur_time - prev_time_;
    prev_time_ = cur_time;

    int width, height;

    glfwGetFramebufferSize(window_, &width, &height);

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    logic_->width = width;
    logic_->height = height;
    logic_->time = cur_time;
    logic_->delta_time = delta_time;
    logic_->input();
    logic_->update();

    glfwSwapBuffers(window_);
}
