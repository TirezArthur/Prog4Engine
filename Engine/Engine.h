#pragma once
#include <string>
#include <functional>
#include <chrono>

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
		float m_FixedTimeStep{ 0.05f }; // 20 fixed updates per second
		std::chrono::microseconds m_FrameTimetep{ 0 };
	};
}