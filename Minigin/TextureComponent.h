#pragma once
#include "Component.h"
#include "Texture2D.h"
#include <memory>

namespace dae
{
	class TextureComponent final : public Component
	{
	public:
		TextureComponent(std::shared_ptr<Texture2D> texture);
		virtual ~TextureComponent() = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

		virtual void Render() const override;

	private:
		std::shared_ptr<Texture2D> m_texture;
	};
}

