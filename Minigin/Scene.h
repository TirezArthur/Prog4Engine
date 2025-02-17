#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class Scene final
	{
	public:
		const std::unique_ptr<GameObject>& Add(std::unique_ptr<GameObject> object);
		void Remove(std::unique_ptr<GameObject>& object);
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

		std::string m_name;
		std::vector <std::unique_ptr<GameObject>> m_objects{};

		static unsigned int m_idCounter; 
	};

}
