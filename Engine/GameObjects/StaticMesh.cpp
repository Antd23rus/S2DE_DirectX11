#include "StaticMesh.h"

namespace S2DE::GameObjects
{
	StaticMesh::StaticMesh() : 
		m_mesh(nullptr),
		m_texture(nullptr),
		m_indexBuffer(nullptr),
		m_vertexBuffer(nullptr),
		m_shader(nullptr)
	{

	}

	StaticMesh::~StaticMesh()
	{
		//TODO: Need to check count of using
		if (m_unload_texture == true)
			Core::Engine::GetResourceManager().Erase<Render::Texture>(m_texture->GetName());

		Core::Delete(m_vertexBuffer);
		Core::Delete(m_indexBuffer);
		Core::Delete(m_shader);
		Core::Delete(m_texture);
		Core::Delete(m_mesh);
	}

	bool StaticMesh::LoadMesh(std::string name)
	{	
		//If mesh not found in resource manager storage we try to load it 
		if (!Core::Engine::GetResourceManager().IsExists<Render::Mesh>(name))
		{
			if (!Core::Engine::GetResourceManager().Load<Render::Mesh>(name))
				return false;
		}

		//Set mesh if mesh is exist
		m_mesh = new Render::Mesh(*Core::Engine::GetResourceManager().Get<Render::Mesh>(name));
		S2DE_ASSERT(m_mesh != nullptr);

		m_vertexBuffer = new Render::VertexBuffer<Render::Vertex>();
		m_indexBuffer = new Render::IndexBuffer<std::uint32_t>();
		m_vertexBuffer->GetArray() = m_mesh->GetVertices();
		m_indexBuffer->GetArray() = m_mesh->GetIndices();

		if (m_vertexBuffer->GetArray().size() == 0 || m_indexBuffer->GetArray().size() == 0)
		{
			m_vertexBuffer = nullptr;
			m_indexBuffer = nullptr;
			return false;
		}

		CreateVertexBuffer();
		CreateIndexBuffer();
		SetDefaultShader();
		SetDefaultTexture();

		return true;
	}	 
		 
	bool StaticMesh::LoadTexture(std::string name)
	{	 
		return LoadTextureA(name);
	}	 
		 
	bool StaticMesh::LoadTextureA(std::string name, bool unload_texture, bool auto_load_texture)
	{	 
		//Set unload state
		m_unload_texture = unload_texture;

		//If texture not found in resource manager storage we try to load it 
		if (!Core::Engine::GetResourceManager().IsExists<Render::Texture>(name)
			&& auto_load_texture == true)
		{
			if (!Core::Engine::GetResourceManager().Load<Render::Texture>(name))
				return false;
		}
		//Set texture if texture is exist
		m_texture = new Render::Texture(*Core::Engine::GetResourceManager().Get<Render::Texture>(name));
		S2DE_ASSERT(m_texture != nullptr);
		return true;
	}	 
		 
	void StaticMesh::UpdateShader()
	{	 
		S2DE_NO_IMPL();
	}	 
		 
	void StaticMesh::UpdateTexture()
	{	 	 
		S2DE_NO_IMPL();
	}	 

	void StaticMesh::SetColor(Math::Color<float> color)
	{	 
		if (m_vertexBuffer != nullptr)
		{
			m_color = color;
			//Update vertex buffer
			CreateVertexBuffer();
		}
	}	 
		 
	void StaticMesh::OnRender()
	{	 
		if (m_vertexBuffer && m_indexBuffer)
		{
			//Bind and update variables in const buffer
			m_shader->UpdateMainConstBuffer(UpdateTransformation());

			//Bind shader and texture 
			m_shader->Bind();
			m_texture->Bind();

			//Bind buffers
			m_vertexBuffer->Bind();
			//m_indexBuffer->Bind();

			//Draw poly 		
			Core::Engine::GetRenderer()->SetRasterizerState("fcc");
			//Core::Engine::GetRenderer()->DrawIndexed(m_indexBuffer->GetArray().size(), 0, 0, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			Core::Engine::GetRenderer()->Draw(m_vertexBuffer->GetArray().size(), 0, D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//Unbind 
			m_shader->Unbind();
			m_texture->Unbind();
			m_vertexBuffer->Unbind();
			//m_indexBuffer->Unbind();
		}
	}	 
		 
	void StaticMesh::CreateVertexBuffer()
	{	 
		S2DE_ASSERT(m_vertexBuffer->Create());
		m_vertexBuffer->Update();
	}	 
		 
	void StaticMesh::CreateIndexBuffer()
	{	 
		S2DE_ASSERT(m_indexBuffer->Create());
		m_indexBuffer->Update();
	}	 
		 
	void StaticMesh::SetDefaultShader()
	{	 
		m_shader = new Render::Shader(*Core::Engine::GetResourceManager().Get<Render::Shader>("Mesh"));
	}	 
		 
	void StaticMesh::SetDefaultTexture()
	{
		m_texture = new Render::Texture(*Core::Engine::GetResourceManager().GetDefaultTexture());
	}

}