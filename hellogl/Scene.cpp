#include <unordered_map>

#include "Shader.h"
#include "GameObject.h"
#include "Scene.h"
#include "Shader.h"
#include "Light.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

using namespace std;

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
	unordered_map<shared_ptr<Mesh>, vector<weak_ptr<GameObject>>> meshes;
	vector<pair<shared_ptr<Light>, weak_ptr<GameObject>>> lights;
	vector<weak_ptr<GameObject>> children;
	for (auto& root : gameObjects_) {
		children.clear();
		root->getChildren(children);
		children.push_back(root);
		for(auto& child : children) {
			if (child.expired()) continue;

			auto obj = child.lock();
			if (obj->mesh) {
				meshes[obj->mesh].push_back(obj);
			}
			if (obj->light) {
				lights.push_back(std::make_pair(obj->light, obj));
			}
			if (obj->camera) {
				camera = obj;
			}
		}
	}
	if(camera) {
		shader->bind();
		auto view = inverse(camera->global(true));
		shader->setUniformValue("cameraVertex", camera->position);
		for (auto& it : lights) {
			auto& light = it.first;
			auto obj = it.second.lock();
			shader->setUniformValue("directionalLight.direction", obj->forward());
			shader->setUniformValue("directionalLight.intensity", light->intensity);
			shader->setUniformValue("directionalLight.color", light->color);
		}
		for (auto& it : meshes) {
			auto& mesh = it.first;
			mesh->bind();
			for (auto& gameObj : it.second) {
				auto obj = gameObj.lock();
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
