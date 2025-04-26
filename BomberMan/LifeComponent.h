#pragma once
#include "Component.h"
#include "ObserverSubject.hpp"
#include <algorithm>
#include <memory>

namespace dae
{
    class LifeComponent final : public Component
    {
    public:
		explicit LifeComponent(GameObject* parent, int maxLives) : Component(parent),
			m_Lives{ maxLives },
			m_MaxLives{ maxLives },
			m_Subject{ std::make_unique<Subject>() }
		{
		};
		virtual ~LifeComponent() = default;
		LifeComponent(const LifeComponent& other) = delete;
		LifeComponent(LifeComponent&& other) = delete;
		LifeComponent& operator=(const LifeComponent& other) = delete;
		LifeComponent& operator=(LifeComponent&& other) = delete;

		void Update(float) override {};
		void LoseLife() 
		{ 
			m_Lives = std::clamp(m_Lives - 1, 0, m_MaxLives);
			Event livesEvent{ "LivesChanged" };
			livesEvent.SetData(m_Lives);
			m_Subject->Notify(livesEvent);
		}

		void AddObserver(Observer* observer) { m_Subject->AddObserver(observer); }
		void RemoveObserver(Observer* observer) { m_Subject->RemoveObserver(observer); }
	private:
		std::unique_ptr<Subject> m_Subject;
		int m_Lives;
		int m_MaxLives;
    };
}

