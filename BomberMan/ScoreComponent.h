#pragma once
#include "Component.h"
#include "Observer.hpp"
#include <memory>

namespace dae
{
	class ScoreComponent final : public Component
	{
	public:
		explicit ScoreComponent(GameObject* parent) : Component(parent),
			m_Subject{ std::make_unique<Subject>() }
		{
		};
		virtual ~ScoreComponent() = default;
		ScoreComponent(const ScoreComponent& other) = delete;
		ScoreComponent(ScoreComponent&& other) = delete;
		ScoreComponent& operator=(const ScoreComponent& other) = delete;
		ScoreComponent& operator=(ScoreComponent&& other) = delete;

		void Update(float) override {};
		void IncrementScore(int score)
		{
			m_Score += score;
			Event livesEvent{ "ScoreChanged" };
			livesEvent.SetData(m_Score);
			m_Subject->Notify(livesEvent);
		}

		void AddObserver(Observer* observer) { m_Subject->AddObserver(observer); }
		void RemoveObserver(Observer* observer) { m_Subject->RemoveObserver(observer); }
	private:
		std::unique_ptr<Subject> m_Subject;
		int m_Score{};
	};
}

