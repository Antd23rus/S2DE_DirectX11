#include "Sprite.h"


namespace S2DE::GameObjects
{
	Sprite::Sprite() :
		ScaleFactor(Math::Vector3(1, 1, 1)),
		m_texture(nullptr),
		m_shader(nullptr),
		m_index_buffer(nullptr),
		m_vertex_buffer(nullptr),
		m_tile_size(Math::Vector2(0, 0)),
		m_tile_frame_pos(Math::IntVector2(0, 0))
	{
		CreateVertexBuffer();
		CreateIndexBuffer();
		SetDefaultTexture();
		SetDefaultShader();
	}

	Sprite::~Sprite()
	{
		if (m_unload_texture == true)
			Core::Engine::GetResourceManager().Erase<Render::Texture>(m_texture->GetName());

		Core::Delete(m_vertex_buffer);
		Core::Delete(m_index_buffer);
		//m_shader->Cleanup();
		Core::Delete(m_shader);
		Core::Delete(m_sprite_const_buf);
		//m_texture->Cleanup();
		Core::Delete(m_texture);
	}

	void Sprite::SetAtlasFramePosition(std::int32_t x, std::int32_t y)
	{
		m_tile_frame_pos = Math::IntVector2(x, y);
	}

	void Sprite::SetAtlasSize(Math::Vector2 size)
	{
		m_tile_size = size;
		CalcScaleFactor();
	}

	bool Sprite::LoadTexture(std::string name, bool unload_texture, bool auto_load_texture)
	{
		m_unload_texture = unload_texture;

		//If texture not be loaded before 
		//and we get default texture when try to get it from resource manager then
		//we try to load needed texture from gamedata
		if (Core::Engine::GetResourceManager().Get<Render::Texture>(name) == Core::Engine::GetResourceManager().GetDefaultTexture()
			&& auto_load_texture == true)
		{
			if (!Core::Engine::GetResourceManager().Load<Render::Texture>(name))
				return false;
		}

		//Set texture 
		m_texture = new Render::Texture(*Core::Engine::GetResourceManager().Get<Render::Texture>(name));

		//Texture can't be null 
		S2DE_ASSERT(m_texture != nullptr);	

		CalcScaleFactor();
		return true;
	}

	void Sprite::OnRender()
	{
		//Bind buffers
		m_vertex_buffer->Bind();
		m_index_buffer->Bind();
		Core::Engine::GetRenderer()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//Bind and update variables in const buffer
		m_shader->UpdateMainConstBuffer(UpdateTransformation());

		m_sprite_const_buf->Lock();
		m_sprite_const_buf->GetData()->sprite_tile_frame = Math::XInt2(m_tile_frame_pos.x, m_tile_frame_pos.y);
		m_sprite_const_buf->GetData()->sprite_tile_size = Math::XFloat2(m_tile_size.x, m_tile_size.y);
		m_sprite_const_buf->GetData()->sprite_texture_res = Math::XFloat2((float)m_texture->GetWidth(), (float)m_texture->GetHeight());
		m_sprite_const_buf->Unlock();
		m_sprite_const_buf->Bind(1);

		//Bind shader
		m_shader->Bind();


		//Bind texture
		m_texture->Bind();

		//Draw sprite 
		Core::Engine::GetRenderer()->GetContext()->DrawIndexed(m_index_buffer->GetArray().size(), 0, 0);

		//Unbind buffer
		m_shader->Unbind();
	}

	Math::XMatrix Sprite::UpdateTransformation()
	{
		//Coppyed from transform class  
		GetWorldMatrix() = DirectX::XMMatrixTransformation(
			//Scale
			//Center | Rotation | Scaling

			//Added scale factor
			Math::XVector(), Math::XVector(), Math::To_XMVector3(GetScale() * ScaleFactor),
			//Rotation
			//Center | Quatarnion
			Math::XVector(), ToQuaternion(GetRotation()),
			//Translation
			Math::To_XMVector3(GetPosition()));

		return DirectX::XMMatrixTranspose(GetWorldMatrix());
	}

	void Sprite::CreateVertexBuffer()
	{	 
		m_vertex_buffer = new Render::VertexBuffer<Render::Vertex>();
		m_vertex_buffer->GetArray() =
		{
			{ Math::XFloat3(-1.0f,   -1.0f,   0.0f), Math::XFloat4(255, 255, 255, 255),  Math::XFloat2(0.0f, 1.0f) }, // Bottom left.
			{ Math::XFloat3(-1.0f,   1.0f,   0.0f),  Math::XFloat4(255, 255, 255, 255),  Math::XFloat2(0.0f, 0.0f) }, // Top left.
			{ Math::XFloat3(1.0f,  1.0f,   0.0f),	 Math::XFloat4(255, 255, 255, 255),  Math::XFloat2(1.0f, 0.0f)	 }, // top right.
			{ Math::XFloat3(1.0f,  -1.0f,   0.0f),   Math::XFloat4(255, 255, 255, 255),  Math::XFloat2(1.0f, 1.0f)	 }, // Bottom right.
		};

		S2DE_ASSERT(m_vertex_buffer->Create());
		m_vertex_buffer->Update();
	}	 
		 
	void Sprite::CreateIndexBuffer()
	{	 
		m_index_buffer = new Render::IndexBuffer<std::int32_t>();
		m_index_buffer->GetArray() =
		{
				0, 1, 2,
				0, 2, 3,
		};

		S2DE_ASSERT(m_index_buffer->Create());
		m_index_buffer->Update();
	}	 
		 
	void Sprite::UpdateTexture()
	{
		//Get texture name
		std::string name = m_texture->GetName();

		//Delete previous texture
		Core::Delete(m_texture);

		Render::Texture* upd_tx = Core::Engine::GetResourceManager().Get<Render::Texture>(name);

		if (upd_tx == nullptr)
		{
			Logger::Error("%s Can't update texture!", GetName().c_str());
			return;
		}

		//Try to get texture by name from resource manager
		m_texture = new Render::Texture(*upd_tx);

		//Texture can't be nullptr
		S2DE_ASSERT(m_texture != nullptr);
	}
	 
	void Sprite::UpdateShader()
	{
		//Get shader name
		std::string name = m_shader->GetName();
		//Delete previous shader 
		Core::Delete(m_shader);

		Render::Shader* upd_sh = Core::Engine::GetResourceManager().Get<Render::Shader>(name);

		if (upd_sh == nullptr)
		{
			Logger::Error("%s Can't update shader!", GetName().c_str());
			return;
		}


		//Try to get shader by name from resource manager
		m_shader = new Render::Shader(*upd_sh);

		//Shader can't be nullptr
		S2DE_ASSERT(m_shader != nullptr);

		//TODO 
		//We need to use custom type 
		//if we used custom shader with custom const buffer type

		//Create constant buffer with sprite const buffer type
		m_sprite_const_buf = new Render::ConstantBuffer<SpriteConstBuffer>();
		S2DE_ASSERT(m_sprite_const_buf->Create());
	}

	void Sprite::SetDefaultShader()
	{	 
		m_shader = new Render::Shader(*Core::Engine::GetResourceManager().Get<Render::Shader>("Sprite"));

		m_sprite_const_buf = new Render::ConstantBuffer<SpriteConstBuffer>();
		S2DE_ASSERT(m_sprite_const_buf->Create());
	}	 
		 
	void Sprite::CalcScaleFactor()
	{
		//TODO
		//Better scale factor
		//But for now it's ok
		if(m_tile_size.Zero())
			ScaleFactor = Math::Vector3(m_texture->GetWidth() * 0.01f, m_texture->GetHeight() * 0.01f, 1.0f);
		else
			ScaleFactor = Math::Vector3(m_tile_size.x * 0.01f, m_tile_size.y * 0.01f, 1.0f);
	}

	void Sprite::SetDefaultTexture()
	{
		m_texture = new Render::Texture(*Core::Engine::GetResourceManager().GetDefaultTexture());
		CalcScaleFactor();
	}
}