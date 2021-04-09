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

    shared_ptr<Material> white = make_shared<Material>(vec3(1, 1, 1));
	shared_ptr<Material> red = make_shared<Material>(vec3(1, 0, 0));
	shared_ptr<Material> blue = make_shared<Material>(vec3(0, 0, 1));
	
    cube1 = make_shared<GameObject>();
    cube1->position.x -= 1.5;
    cube1->mesh = mesh;
	cube1->material = white;
    scene.add(cube1);

    shared_ptr<GameObject> child1 = make_shared<GameObject>();
    child1->position.y += 3;
    child1->scale *= 0.5;
    child1->mesh = mesh;
	child1->material = white;
	cube1->addChild(child1);
	
	shared_ptr<GameObject> child2 = make_shared<GameObject>();
	child2->position.y += 3;
	child2->scale *= 0.5;
	child2->mesh = mesh;
	child2->material = red;
	child1->addChild(child2);

    shared_ptr<GameObject> cube2 = make_shared<GameObject>();
    cube2->position.x += 1.5;
    cube2->mesh = mesh;
	cube2->material = blue;
    scene.add(cube2);
	
	shared_ptr<GameObject> child3 = make_shared<GameObject>();
	child3->position.y += 3;
	child3->mesh = mesh;
	child3->material = blue;
	cube2->addChild(child3);
	
	shared_ptr<GameObject> child4 = make_shared<GameObject>();
	child4->position.y += 3;
	child4->mesh = mesh;
	child4->material = blue;
	child3->addChild(child4);

    camera = make_shared<GameObject>();
	camera->position += vec3(0, 2, 12);
    camera->lookAt(vec3(0, 2, 0), vec3(0, 1, 0));
    camera->camera = make_shared<Camera>();
    scene.add(camera);

    shared_ptr<GameObject> light = make_shared<GameObject>();
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
