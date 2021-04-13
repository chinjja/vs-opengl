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
#include "Utils.h"

using namespace std;
using namespace glm;

void GameLogicImpl::init()
{
	auto shader = make_shared<Shader>();
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

	auto white = make_shared<Material>(vec4(1, 1, 1, 1));
	auto red = make_shared<Material>(vec4(1, 0, 0, 1));
	auto green = make_shared<Material>(vec4(0, 1, 0, 1));
	auto blue = make_shared<Material>(vec4(0, 0, 1, 1));
	
    cube1 = GameObject::create();
    cube1->localPosition.x -= 1.5;
    cube1->mesh = mesh;
	cube1->material = white;
    scene.add(cube1);

	auto child1 = GameObject::create();
    child1->localPosition.y += 3;
    child1->localScale *= 0.5;
    child1->mesh = mesh;
	child1->material = white;
	cube1->addChild(child1);
	
	child2 = GameObject::create();
	child2->localPosition.y += 3;
	child2->localScale *= 0.5;
	child2->mesh = mesh;
	child2->material = red;
	child1->addChild(child2);

	cube2 = GameObject::create();
    cube2->localPosition.x += 1.5;
    cube2->mesh = mesh;
	cube2->material = blue;
    scene.add(cube2);
	
	auto child3 = GameObject::create();
	child3->localPosition.y += 3;
	child3->mesh = mesh;
	child3->material = blue;
	cube2->addChild(child3);
	
	auto child4 = GameObject::create();
	child4->localPosition.y += 3;
	child4->mesh = mesh;
	child4->material = blue;
	child3->addChild(child4);
	
	auto plane = GameObject::create();
	plane->localPosition.y += -1.2;
	plane->localScale = vec3(10, 0.1, 10);
	plane->mesh = mesh;
	plane->material = white;
	scene.add(plane);

	auto light = GameObject::create();
    light->lookAlong(vec3(0, -1, -1));
    light->light = make_shared<Light>();
    scene.add(light);
	
	auto camera = GameObject::create();
	camera->localPosition.z = -2;
	camera->camera = make_shared<Camera>();
	camera->camera->fov = radians(60.0f);
	camera->camera->near = 0.1f;
	camera->camera->far = 100.0f;
	
	head = GameObject::create();
	head->localPosition.y += 1.1;
	head->localScale *= 0.5;
	head->mesh = mesh;
	head->material = green;
	head->addChild(camera);
	
	player = GameObject::create();
	player->localPosition.z = 10;
	player->localScale *= vec3(0.5, 1, 0.5);
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

	if (input().keyPressed(GLFW_KEY_Z)) {
		child2->position.x += delta_time;
		cube2->position.x += delta_time;
	}
	else if (input().keyPressed(GLFW_KEY_X)) {
		child2->position.x -= delta_time;
		cube2->position.x -= delta_time;
	}

	scene.width = width;
	scene.height = height;
    cube1->preRotate(vec3(0, radians(30.0f) * delta_time, 0));
    scene.render();
}
