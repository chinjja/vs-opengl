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

int main()
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

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
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
    //
    glfwSwapInterval(1);

    Shader shader;
    shader.addVertexShaderFile("scene.vert");
    shader.addFragmentShaderFile("scene.frag");
    shader.link();

    shader.setUniformValue("ambient", vec3(1, 1, 1));
    shader.setUniformValue("directionalLight.direction", vec3(0, -1, -1));
    shader.setUniformValue("directionalLight.intensity", 1.0f);

    auto mesh = ObjectLoader::load("models/cube.obj");
    if (!mesh) {
        cerr << "obj load fail" << endl;
        std::exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    double prev_time = glfwGetTime();
    
    GameObject gameObject;
    GameObject gameObject2;
    gameObject.position += vec3(-1.5, 0, 0);
    gameObject.mesh = mesh;
    gameObject2.position += vec3(1.5, 0, 0);
    gameObject2.mesh = mesh;

    while (!glfwWindowShouldClose(window))
    {
        double cur_time = glfwGetTime();
        float delta_time = cur_time - prev_time;
        prev_time = cur_time;

        float ratio;
        int width, height;
        mat4 m, v, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gameObject.preRotate(quat(vec3(0, pi<float>() * delta_time * 0.25f, 0)));
        m = gameObject.matrix();
        v = lookAt(vec3(0, 0, 8), vec3(0, 0, 0), vec3(0, 1, 0));
        p = perspective(pi<float>() / 3, ratio, 0.1f, 10.0f);
        mvp = p * v * m;

        shader.bind();
        shader.setUniformValue("M", m);
        shader.setUniformValue("MVP", mvp);

        gameObject.render();

        m = gameObject2.matrix();
        shader.setUniformValue("M", gameObject2.matrix());
        mvp = p * v * m;
        shader.setUniformValue("MVP", mvp);
        gameObject2.render();

        shader.release();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    std::exit(EXIT_SUCCESS);
}