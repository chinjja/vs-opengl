#include <iostream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "GameLogicImpl.h"
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

void GameLogicImpl::init()
{
    shared_ptr<Shader> shader(new Shader);
    shader->addVertexShaderFile("scene.vert");
    shader->addFragmentShaderFile("scene.frag");
    shader->link();

    scene.shader = shader;

    auto mesh = ObjectLoader::load("models/cube.obj");
    if (!mesh) {
        cerr << "obj load fail" << endl;
        std::exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    cube1 = make_shared<GameObject>();
    cube1->position.x -= 1.5;
    cube1->mesh = mesh;
    cube1->texture = make_shared<Texture>(vec3(1, 0, 0));
    scene.add(cube1);

    shared_ptr<GameObject> cube2(new GameObject);
    cube2->position.x += 1.5;
    cube2->mesh = mesh;
    cube2->texture = make_shared<Texture>(vec3(1, 1, 1));
    scene.add(cube2);

    camera = make_shared<GameObject>();
    camera->position.z += 8;
    camera->lookAt(vec3(0, 0, 0), vec3(0, 1, 0));
    camera->camera = make_shared<Camera>();
    scene.add(camera);

    shared_ptr<GameObject> light(new GameObject);
    light->lookAlong(vec3(0, -1, -1));
    light->light = make_shared<Light>(1);
    scene.add(light);
}

void GameLogicImpl::update()
{
    if (input().mousePressed(GLFW_MOUSE_BUTTON_2)) {
        vec2 pos = input().mouseDeltaPos() * delta_time;
        if (input().altPressed()) {
            quat rot({ 0, 0, pos.x + pos.y });
            camera->preRotate(rot);
        }
        else {
            quat rot({ pos.y, pos.x, 0 });
            camera->preRotate(rot);
        }
    }
    scene.projection = perspective(radians(60.0f), width / height, 0.1f, 10.0f);
    cube1->preRotate(vec3(0, radians(30.0f) * delta_time, 0));
    scene.render();
}
