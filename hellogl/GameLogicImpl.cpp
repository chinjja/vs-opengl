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
	auto shader = make_shared<Shader>();
    shader->addVertexShaderFile("scene.vert");
    shader->addFragmentShaderFile("scene.frag");
    shader->link();

    scene.shader = shader;

    auto mesh = ObjectLoader::load("cube.obj");
    if (!mesh) {
        cerr << "obj load fail" << endl;
        std::exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

	auto white = make_shared<Material>(vec3(1, 1, 1));
	auto red = make_shared<Material>(vec3(1, 0, 0));
	auto green = make_shared<Material>(vec3(0, 1, 0));
	auto blue = make_shared<Material>(vec3(0, 0, 1));
	
    cube1 = make_shared<GameObject>();
    cube1->position.x -= 1.5;
    cube1->mesh = mesh;
	cube1->material = white;
    scene.add(cube1);

	auto child1 = make_shared<GameObject>();
    child1->position.y += 3;
    child1->scale *= 0.5;
    child1->mesh = mesh;
	child1->material = white;
	cube1->addChild(child1);
	
	auto child2 = make_shared<GameObject>();
	child2->position.y += 3;
	child2->scale *= 0.5;
	child2->mesh = mesh;
	child2->material = red;
	child1->addChild(child2);

	auto cube2 = make_shared<GameObject>();
    cube2->position.x += 1.5;
    cube2->mesh = mesh;
	cube2->material = blue;
    scene.add(cube2);
	
	auto child3 = make_shared<GameObject>();
	child3->position.y += 3;
	child3->mesh = mesh;
	child3->material = blue;
	cube2->addChild(child3);
	
	auto child4 = make_shared<GameObject>();
	child4->position.y += 3;
	child4->mesh = mesh;
	child4->material = blue;
	child3->addChild(child4);
	
	auto plane = make_shared<GameObject>();
	plane->position.y += -1.2;
	plane->scale = vec3(10, 0.1, 10);
	plane->mesh = mesh;
	plane->material = white;
	scene.add(plane);

	auto light = make_shared<GameObject>();
    light->lookAlong(vec3(0, -1, -1));
    light->light = make_shared<Light>();
    scene.add(light);
	
	auto camera = make_shared<GameObject>();
	camera->position.z = -0.5;
	camera->camera = make_shared<Camera>();
	
	head = make_shared<GameObject>();
	head->position.y += 1.1;
	head->scale *= 0.5;
	head->mesh = mesh;
	head->material = green;
	head->addChild(camera);
	
	player = make_shared<GameObject>();
	player->position.z = 10;
	player->scale *= vec3(0.5, 1, 0.5);
	player->mesh = mesh;
	player->material = green;
	player->addChild(head);
	scene.add(player);
}

void GameLogicImpl::update()
{
    if (input().mousePressed(GLFW_MOUSE_BUTTON_2)) {
        vec2 pos = input().mouseDeltaPos() * delta_time;
		player->preRotate(vec3(0, -pos.x, 0));
		head->preRotate(vec3(-pos.y, 0, 0));
    }
	
	float speed = 5;
	if(input().keyPressed(GLFW_KEY_W)) player->position += player->forward() * delta_time * speed;
	else if(input().keyPressed(GLFW_KEY_S)) player->position -= player->forward() * delta_time * speed;
	
	if(input().keyPressed(GLFW_KEY_A)) player->position -= player->right() * delta_time * speed;
	else if(input().keyPressed(GLFW_KEY_D)) player->position += player->right() * delta_time * speed;
	
	if(input().keyPressed(GLFW_KEY_Q)) player->position -= player->up() * delta_time * speed;
	else if(input().keyPressed(GLFW_KEY_E)) player->position += player->up() * delta_time * speed;
	
	
    scene.projection = perspective(radians(60.0f), width / height, 0.1f, 100.0f);
    cube1->preRotate(vec3(0, radians(30.0f) * delta_time, 0));
    scene.render();
}
