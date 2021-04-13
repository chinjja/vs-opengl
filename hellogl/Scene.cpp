#include <unordered_map>
#include <glm/glm.hpp>

#include "Shader.h"
#include "GameObject.h"
#include "Scene.h"
#include "Shader.h"
#include "Light.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Utils.h"

using namespace std;
using namespace glm;

void Scene::add(const std::shared_ptr<GameObject>& obj)
{
	gameObjects_.push_back(obj);
}

void Scene::remove(const std::shared_ptr<GameObject>& obj)
{
	auto it = std::find(gameObjects_.begin(), gameObjects_.end(), obj);
	if (it != gameObjects_.end()) {
		gameObjects_.erase(it);
	}
}

void Scene::render()
{
	shared_ptr<GameObject> camera;
	unordered_map<shared_ptr<Mesh>, vector<shared_ptr<GameObject>>> meshes;
	vector<shared_ptr<GameObject>> lights;
	vector<shared_ptr<GameObject>> children;
	for (auto& root : gameObjects_) {
		children.clear();
		root->getChildren(children);
		children.push_back(root);
		for(auto& child : children) {
			if (child->mesh) {
				meshes[child->mesh].push_back(child);
			}
			if (child->light) {
				lights.push_back(child);
			}
			if (child->camera) {
				camera = child;
			}
		}
	}
	if(camera) {
		shader->bind();
		auto view = camera->camera->matrix(camera);
		
		shader->setUniformValue("cameraVertex", vec3());
		for (auto& it : lights) {
			auto& light = it->light;
			shader->setUniformValue("directionalLight.direction", it->forward());
			shader->setUniformValue("directionalLight.intensity", light->intensity);
			shader->setUniformValue("directionalLight.color", light->color);
		}
		for (auto& it : meshes) {
			auto& mesh = it.first;
			mesh->bind();
			for (auto& obj : it.second) {
				auto model = obj->global();
				if (obj->material) {
					shader->setUniformValue("material.ambient", obj->material->ambient);
					shader->setUniformValue("material.diffuse", obj->material->diffuse);
					shader->setUniformValue("material.specular", obj->material->specular);
					shader->setUniformValue("material.reflectance", obj->material->reflectance);
				}
				shader->setUniformValue("M", model);
				shader->setUniformValue("MVP", projection * view * model);
				mesh->render();
			}
			mesh->release();
		}
		shader->release();
	}
}
