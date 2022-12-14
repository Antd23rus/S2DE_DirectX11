#pragma once
#include "GameObjects/Components/DrawableComponent.h"

#include "Base/ResourceManager.h"
#include "Render/Renderer.h"
#include "Render/Buffers.h"

namespace S2DE::GameObjects::Components
{	
	class S2DE_API Sprite : public Components::DrawableComponent
	{
	public:
		Sprite();
		virtual ~Sprite();

		// Load sprite texture
		bool											LoadTexture(std::string name) override;
		// Load sprite texture with some options
		bool											LoadTextureA(std::string name, bool unload_texture = false, bool auto_load_texture = true) override;
		// Get new version of current shader from resource manager
		void											UpdateShader() override;
		// Get new version of current texture from resource manager
		void											UpdateTexture() override;
		// Set atlas frame position 
		void											SetAtlasFramePosition(DirectX::SimpleMath::Vector2 position);
		// Set atlas frame size								 
		void											SetAtlasSize(DirectX::SimpleMath::Vector2 size);
		// Set sprite color 
		void											SetColor(Math::Color<float> color);
		// Set billboard mode
		void											SetBillboard(bool billboard);
		// Set two sided mode
		void											SetTwoSidedMode(bool mode);

		void											OnRender() override;

		void											OnCreate() override;
	protected:
		void											CreateVertexBuffer() override;
		void											CreateIndexBuffer() override;
		void											SetDefaultShader() override;
		void											SetDefaultTexture() override;

		[[nodiscard]] virtual inline DirectX::SimpleMath::Vector3		CalcScaleFactor();
		[[nodiscard]] inline DirectX::SimpleMath::Matrix				UpdateTransformation();

	private:
		Render::FR::Texture*								m_texture;
		bool											m_unloadTexture;
		Render::FR::Shader*									m_shader;
		Render::VertexBuffer<Render::Vertex>*			m_vertexBuffer;
		Render::IndexBuffer<std::int32_t>*				m_indexBuffer;
		DirectX::SimpleMath::Vector4					m_tileFrame;
		Math::Color<float>								m_color;
		Render::ConstantBuffer<Render::CB::CB_Sprite>*	m_spriteCB;
		bool											m_billboard;
		bool											m_twoSided;
	};
}