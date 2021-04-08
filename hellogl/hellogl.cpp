#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Shader.h"
#include "Mesh.h"

using namespace std;
using namespace glm;

static const char* vertex_shader_text =
"#version 330 core\n"
"uniform mat4 MVP;\n"
"layout(location=1) in vec3 vCol;\n"
"layout(location=0) in vec3 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 330 core\n"
"in vec3 color;\n"
"out vec4 fragColor;\n"
"void main()\n"
"{\n"
"    fragColor = vec4(color, 1.0);\n"
"}\n";

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

	//glfwSetErrorCallback(error_collback);
    if (!glfwInit())
    {
        cerr << "glfw fail" << endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);

    GLenum error = glewInit();
    if (error != GLEW_OK) {
        cerr << "glew fail" << glewGetErrorString(error) << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    //
    glfwSwapInterval(1);

    Shader shader;
    shader.addVertexShaderCode(vertex_shader_text);
    shader.addFragmentShaderCode(fragment_shader_text);
    shader.link();

    mvp_location = shader.uniformLocation("MVP");

    std::vector<glm::vec3> vertex_data = {
        { -0.6f, -0.4f, 0.0f },
        {  0.6f, -0.4f, 0.0f },
        {   0.f,  0.6f, 0.0f }
    };
    std::vector<glm::vec3> normal_data = {
        { 1.f, 0.f, 0.f },
        { 0.f, 1.f, 0.f },
        { 0.f, 0.f, 1.f }
    };
    std::vector<glm::vec2> uv_data;
    Mesh mesh(vertex_data, normal_data, uv_data);

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        mat4 m, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        m = rotate(mat4(1), (float)glfwGetTime(), vec3(0, 1, 0));
        p = ortho(-ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f);
        mvp = p * m;

        shader.bind();
        shader.setUniformValue(mvp_location, mvp);

        mesh.bind();
        mesh.render();
        mesh.release();

        shader.release();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}