#pragma once
#include <string>
#include <functional>

namespace dae
{
	class Engine
	{
	public:
		explicit Engine(const std::string& dataPath);
		~Engine();
		void SetFixedTimeStep( float fixedTimeStep );
		void Run(const std::function<void()>& load);

		Engine(const Engine& other) = delete;
		Engine(Engine&& other) = delete;
		Engine& operator=(const Engine& other) = delete;
		Engine& operator=(Engine&& other) = delete;
	private:
		float m_FixedTimeStep{ 0.02f };
	};
}