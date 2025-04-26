#pragma once
#include "Component.h"
#include "ObserverSubject.hpp"
#include "TextComponent.h"
#include "GameObject.h"
#include <format>
#include <string>

namespace dae
{
	class TextComponent;
	template<typename EventValueType>
	class DynamicTextComponent final : public Component, public Observer
	{
	public:
		explicit DynamicTextComponent(GameObject* parent, const std::string& formatString, const uint32_t eventId);
		virtual ~DynamicTextComponent() = default;
		DynamicTextComponent(const DynamicTextComponent& other) = delete;
		DynamicTextComponent(DynamicTextComponent&& other) = delete;
		DynamicTextComponent& operator=(const DynamicTextComponent& other) = delete;
		DynamicTextComponent& operator=(DynamicTextComponent&& other) = delete;

		void Update(float) override {};
		void OnNotify(const Event& event) override 
		{
			if (event == m_EventId)
			{
				int data = event.GetData<EventValueType>();
				m_TextComponentTarget->SetText(std::vformat(m_FormatString, std::make_format_args(data)));
			}
		}
	private:
		TextComponent* m_TextComponentTarget{};
		const std::string m_FormatString;
		const uint32_t m_EventId;
	};

	template<typename EventValueType>
	inline DynamicTextComponent<EventValueType>::DynamicTextComponent(GameObject* parent, const std::string& formatString, uint32_t eventId) :
		Component{ parent },
		m_TextComponentTarget{ m_Parent->GetComponent<TextComponent>() },
		m_FormatString{ formatString },
		m_EventId{ eventId }
	{
		assert(m_TextComponentTarget != nullptr);
	}
}

