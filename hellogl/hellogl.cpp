#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Shader.h"
#include "Mesh.h"
#include "ObjectLoader.h"

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
	GLint mvp_location;

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

    mvp_location = shader.uniformLocation("MVP");

    Mesh *mesh = ObjectLoader::load("models/cube.obj");
    if (!mesh) {
        cerr << "obj load fail" << endl;
        std::exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        mat4 m, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m = rotate(mat4(1), (float)glfwGetTime(), vec3(0, 1, 0));
        p = ortho(-ratio*2, ratio*2, -2.0f, 2.0f, 2.0f, -2.0f);
        mvp = p * m;

        shader.bind();
        shader.setUniformValue(mvp_location, mvp);

        mesh->bind();
        mesh->render();
        mesh->release();

        shader.release();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    delete mesh;
    glfwDestroyWindow(window);
    glfwTerminate();
    std::exit(EXIT_SUCCESS);
}