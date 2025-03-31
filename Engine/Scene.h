#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class Scene final
	{
	public:
		GameObject* Add(std::unique_ptr<GameObject> object);
		void Remove(GameObject* object);
		void RemoveAll();

		void FixedUpdate();
		void Update(float elapsedSec);
		void Render() const;
		void LateUpdate(float elapsedSec);

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private:
		friend Scene* SceneManager::CreateScene(const std::string& name);
		explicit Scene(const std::string& name);

		void DeleteObject(GameObject* object);

		std::string m_name;
		std::vector<std::unique_ptr<GameObject>> m_objects{};
		std::vector<GameObject*> m_deleteQueue{};

		static unsigned int m_idCounter;
	};

}
