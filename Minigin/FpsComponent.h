#pragma once
#include "Component.h"
#include <list>

namespace dae
{
	class TextComponent;
	class FpsComponent : public Component
	{
	public:
		FpsComponent() = default;
		virtual ~FpsComponent() = default;
		FpsComponent(const FpsComponent& other) = delete;
		FpsComponent(FpsComponent&& other) = delete;
		FpsComponent& operator=(const FpsComponent& other) = delete;
		FpsComponent& operator=(FpsComponent&& other) = delete;

		virtual void Update(float elapsedSec) override;
	private:
		TextComponent* m_textComponentTarget{};
		std::list<float> m_elapsedValues{};

		static constexpr int MAXLISTSIZE{ 50 };
	};
}

