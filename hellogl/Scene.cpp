#include "Scene.h"
#include "Shader.h"

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
	std::unordered_map<Mesh*, std::vector<GameObject*>> meshes;
	std::vector<std::pair<Light*, GameObject*>> lights;
	for (auto& obj : gameObjects_) {
		auto gameObj = obj.get();
		if (obj->mesh) {
			meshes[obj->mesh.get()].push_back(gameObj);
		}
		if (obj->light) {
			lights.push_back(std::make_pair(obj->light.get(), gameObj));
		}
		if (obj->camera) {
			camera = gameObj;
		}
	}
	if(camera) {
		shader->bind();
		auto view = camera->matrix();
		for (auto& it : lights) {
			shader->setUniformValue("directionalLight.direction", it.second->forward());
			shader->setUniformValue("directionalLight.intensity", it.first->intensity);
		}
		for (auto& it : meshes) {
			it.first->bind();
			for (auto& gameObj : it.second) {
				auto model = gameObj->matrix();
				if (gameObj->texture) {
					shader->setUniformValue("ambient", gameObj->texture->ambient);
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
