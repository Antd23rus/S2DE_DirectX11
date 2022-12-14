#pragma once
#include "GameObjects/Components/DrawableComponent.h"

#include "Base/ResourceManager.h"
#include "Render/Renderer.h"
#include "Render/Buffers.h"

namespace S2DE::GameObjects::Components
{
	class S2DE_API Skybox : public Components::DrawableComponent
	{
	public:
		Skybox();
		~Skybox();

		bool			LoadTexture(std::string name) override;
		bool			LoadTextureA(std::string name, bool unload_texture = false, bool auto_load_texture = false) override;

	protected:
		void			OnRender() override;
		void			CreateVertexBuffer() override;
		void			CreateIndexBuffer() override;
		void			SetDefaultShader() override;
		void			SetDefaultTexture() override;

		[[nodiscard]] inline DirectX::SimpleMath::Matrix	UpdateTransformation();

	private:
		Render::FR::Texture* m_texture;
		Render::FR::Shader* m_shader;
		Render::VertexBuffer<Render::Vertex>* m_vertexBuffer;
		Render::IndexBuffer<std::int32_t>* m_indexBuffer;
	};
}
