#pragma once
#include "Singleton.h"
#include <vector>
#include <string>
#include <memory>

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		virtual ~SceneManager();

		Scene* CreateScene(const std::string& name);

		void FixedUpdate();
		void Update(float elapsedSec);
		void Render() const;
	private:
		friend class Singleton<SceneManager>;
		SceneManager();
		std::vector<std::unique_ptr<Scene>> m_scenes;
	};
}
