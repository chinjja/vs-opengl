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

    shared_ptr<Shader> shader(new Shader);
    shader->addVertexShaderFile("scene.vert");
    shader->addFragmentShaderFile("scene.frag");
    shader->link();

    Scene scene;
    scene.shader = shader;

    auto mesh = ObjectLoader::load("models/cube.obj");
    if (!mesh) {
        cerr << "obj load fail" << endl;
        std::exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    shared_ptr<GameObject> cube1(new GameObject);
    cube1->position.x -= 1.5;
    cube1->mesh = mesh;
    cube1->texture = make_shared<Texture>(vec3(1, 0, 0));
    scene.add(cube1);

    shared_ptr<GameObject> cube2(new GameObject);
    cube2->position.x += 1.5;
    cube2->mesh = mesh;
    cube2->texture = make_shared<Texture>(vec3(1, 1, 1));
    scene.add(cube2);

    shared_ptr<GameObject> camera(new GameObject);
    camera->position.z += 8;
    camera->lookAt(vec3(0, 0, 0), vec3(0, 1, 0));
    camera->camera = make_shared<Camera>();
    scene.add(camera);

    shared_ptr<GameObject> light(new GameObject);
    light->lookAlong(vec3(0, -1, -1));
    light->light = make_shared<Light>(1);
    scene.add(light);

    double prev_time = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        double cur_time = glfwGetTime();
        float delta_time = cur_time - prev_time;
        prev_time = cur_time;

        float ratio;
        int width, height;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube1->preRotate(vec3(0, pi<float>() * delta_time * 0.25f, 0));

        scene.projection = perspective(pi<float>() / 3, ratio, 0.1f, 10.0f);
        scene.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    std::exit(EXIT_SUCCESS);
}