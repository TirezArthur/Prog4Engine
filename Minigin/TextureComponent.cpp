#include "TextureComponent.h"
#include "Renderer.h"
#include "GameObject.h"

using namespace dae;

TextureComponent::TextureComponent(GameObject* parent, std::shared_ptr<Texture2D> texture) : Component{parent},
m_texture { texture }
{
}

void TextureComponent::Render() const
{
	if (m_texture != nullptr)
	{
		auto& pos = m_parent->GetWorldPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	}
}
