#include <unordered_map>
#include <vector>
#include <algorithm>

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
	GameObject* camera = nullptr;
	unordered_map<Mesh*, vector<GameObject*>> meshes;
	vector<pair<Light*, GameObject*>> lights;
	vector<GameObject*> children;
	for (auto& root : gameObjects_) {
		auto gameObj = root.get();
		children.clear();
		gameObj->getChildren(children);
		children.push_back(gameObj);
		for(auto child : children) {
			if (child->mesh) {
				meshes[child->mesh.get()].push_back(child);
			}
			if (child->light) {
				lights.push_back(make_pair(child->light.get(), child));
			}
			if (child->camera) {
				camera = child;
			}
		}
	}
	if(camera) {
		shader->bind();
		auto view = inverse(camera->global());
		for (auto& it : lights) {
			shader->setUniformValue("directionalLight.direction", it.second->forward());
			shader->setUniformValue("directionalLight.intensity", it.first->intensity);
		}
		for (auto& it : meshes) {
			it.first->bind();
			for (auto& gameObj : it.second) {
				auto model = gameObj->global();
				if (gameObj->material) {
					shader->setUniformValue("ambient", gameObj->material->ambient);
				}
				shader->setUniformValue("M", model);
				shader->setUniformValue("MVP", projection * view * model);
				it.first->render();
			}
			it.first->release();
		}
		shader->release();
	}
}
