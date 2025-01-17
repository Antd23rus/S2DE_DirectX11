#include "Shader.h"
#include "Base/Engine.h"
#include "Base/GameWindow.h"
#include "Base/GameTime.h"

#include "Render/Renderer.h"
#include "Scene/SceneManager.h"

#include "GameObjects/Components/Camera.h"

#include <d3dcompiler.h>
#include <fstream>
#include <sstream>

namespace S2DE::Render
{
	static const D3D11_INPUT_ELEMENT_DESC InputElements[] =
	{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

			{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT,
			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

			{ "NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,
			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	Shader::Shader()
	{
		m_type = "Shader";
		m_extensions = { ".hlsl", ".shader", ".fx" };

		m_flags |= D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(S2DE_DEBUG_RENDER_MODE)
		m_flags |= D3DCOMPILE_DEBUG;
		m_flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	}

	Shader::~Shader()
	{
		Core::Release(m_vertexShader);
		Core::Release(m_pixelShader);
		Core::Release(m_layout);
	}


	void Shader::Unbind(Render::Renderer* renderer)
	{
		renderer->GetContext()->IASetInputLayout(0);
		renderer->GetContext()->VSSetShader(0, nullptr, 0);
		renderer->GetContext()->PSSetShader(0, nullptr, 0);
	}


	void Shader::Bind(Render::Renderer* renderer)
	{
		renderer->GetContext()->IASetInputLayout(m_layout);
		renderer->GetContext()->VSSetShader(m_vertexShader, nullptr, 0);
		renderer->GetContext()->PSSetShader(m_pixelShader, nullptr, 0);
	}

	void Shader::ShowErrorDetails(ID3D10Blob* error_blob)
	{
		std::string details = std::string();

		if (error_blob != nullptr)
		{
			details = (char*)error_blob->GetBufferPointer();
			Core::Release(error_blob);
		}
		else
		{
			details = "No error description available!";
		}

		Core::Utils::Logger::Error("[Shader] Compilation failed!\nDetails:\n%s", details.c_str());
	}

	bool Shader::Compile(bool compileVs, bool compilePs)
	{
		if (!compileVs && !compilePs)
		{
			Core::Utils::Logger::Error("[Shader] %s is not modified!", m_name.c_str());
			return false;
		}

		Core::Utils::Logger::Log("[Shader] Compile: %s | Vs: %s Ps: %s", m_name.c_str(), compileVs == true ? "true" : "false", compilePs == true ? "true" : "false");

		ID3D10Blob* code_buffer = nullptr;
		ID3D10Blob* err_buffer = nullptr;

		static const auto renderer = Render::Renderer::GetInstance();
		if (compileVs)
		{
			if (FAILED(D3DCompile(m_fileDataVs.c_str(), m_fileDataVs.size(), m_path_vs.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"main", "vs_5_0", m_flags, 0, &code_buffer, &err_buffer)))
			{
				ShowErrorDetails(err_buffer);
				return false;
			}

			if (FAILED(renderer->GetDevice()->CreateVertexShader(code_buffer->GetBufferPointer(),
				code_buffer->GetBufferSize(), nullptr, &m_vertexShader)))
			{
				Core::Utils::Logger::Error("[Shader] Can't create vertex shader");
				return false;
			}

			if (FAILED(renderer->GetDevice()->CreateInputLayout(InputElements, sizeof(InputElements) / sizeof(InputElements[0]), code_buffer->GetBufferPointer(),
				code_buffer->GetBufferSize(), &m_layout)))
			{
				Core::Utils::Logger::Error("[Shader] Failed to create input layout");
				return false;
			}


			Core::Release(code_buffer);
		}

		if (compilePs)
		{			
			if (FAILED(D3DCompile(m_fileDataPs.c_str(), m_fileDataPs.size(), m_path_ps.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"main", "ps_5_0", m_flags, 0, &code_buffer, &err_buffer)))
			{
				ShowErrorDetails(err_buffer);
				return false;
			}

			if (FAILED(renderer->GetDevice()->CreatePixelShader(code_buffer->GetBufferPointer(),
				code_buffer->GetBufferSize(), nullptr, &m_pixelShader)))
			{
				Core::Utils::Logger::Error("[Shader] Can't create pixel shader");
				return false;
			}

			Core::Release(code_buffer);

			//Create basic constant buffer
			m_const_buffer = new ConstantBuffer<CB::CB_Main>();
			Assert(m_const_buffer->Create(), "Failed to create constant buffer");
		}

		return true;
	}

	bool Shader::Reload()
	{
		return Load(m_name);
	}

	bool Shader::Load(std::string name)
	{
		const auto paths = FindPath({ name + "_vs", name + "_ps" });
		if (m_notLoaded == true)
		{
			return false;
		}

		m_path_vs = paths[0];
		m_path_ps = paths[1];

		bool compileVs = false;
		if(!CheckShadersOnModify(m_path_vs, m_fileDataVs, compileVs))
		{
			return false;
		}

		bool compilePs = false;
		if(!CheckShadersOnModify(m_path_ps, m_fileDataPs, compilePs))
		{
			return false;
		}

		return Compile(compileVs, compilePs);
	}

	bool Shader::CheckShadersOnModify(std::string path, std::string& fileData, bool& modify)
	{
		std::ifstream file = std::ifstream();		
		file.exceptions(std::ios_base::badbit | std::ios_base::failbit);
		
		try
		{
			file.open(path.c_str(), std::ios::in);
			bool modifyOrEmpty = true;

			std::stringstream stream = std::stringstream();
			stream << file.rdbuf();
			file.close();

			if (!Core::Utils::isStringEmpty(fileData))
			{
				if (fileData.length() == stream.str().length())
				{
					modifyOrEmpty = false;
				}
				else
				{
					fileData.clear();
				}
			}

			if (modifyOrEmpty)
			{
				fileData = stream.str();
				modify = true;
			}

			modifyOrEmpty = true;


			stream.clear();

		}
		catch (std::ifstream::failure e)
		{
			Core::Utils::Logger::Error("Something wrong with shader \n%s \n%s ", path.c_str(), e.what());

			file.close();
			return false;
		}

		return true;
	}

	void Shader::UpdateMainConstBuffer(Render::Renderer* renderer, Math::float4x4 world, bool isUI)
	{
		m_const_buffer->Lock(renderer);

		static const auto time = Core::GameTime::GetInstance();
		static const auto gameWindow = Core::GameWindow::GetInstance();

		const auto data = m_const_buffer->GetData();
		data->deltatime = time->GetDeltaTime();
		data->time = time->GetTime();
		data->resoultion = Math::float2(static_cast<float>(gameWindow->GetWidth()),
			static_cast<float>(gameWindow->GetHeight()));

		data->world = world;

		static const auto sceneManager = Scene::SceneManager::GetInstance();
		static const auto camera = sceneManager->GetObjectByName<GameObjects::GameObject>(GameObjects::Components::Camera::EngineCameraName)->GetComponent<GameObjects::Components::Camera>();

		if (camera != nullptr)
		{
			if (isUI)
			{
				data->projection = camera->GetOrthoMatrix();
				
				// Make UI view matrix
				Math::float3 screen = { static_cast<float>(gameWindow->GetWidth() / 2.0f) * GameObjects::Components::Camera::DefaultCameraUIOrthoZoom,
					static_cast<float>(gameWindow->GetHeight() / 2.0f) * GameObjects::Components::Camera::DefaultCameraUIOrthoZoom, 0.0f };

				Math::float4x4 tr = Math::float4x4::Identity;
				tr = Math::float4x4::CreateLookAt({ screen.x,screen.y, 1.0f }, { screen.x,screen.y, -1.0f }, Math::float3::UnitY);
				tr.Transpose(tr);

				data->view = tr;
			}
			else
			{
				data->projection = camera->GetProjectionMatrix();
				data->view = camera->GetViewMatrix();
			}

			const static auto camTransform = camera->GetOwner()->GetTransform();

			data->cameraPosition = camTransform->GetPosition();
			data->cameraRotation = camTransform->GetRotation();
		}

		m_const_buffer->Unlock(renderer);
		m_const_buffer->Bind(renderer);
		m_const_buffer->Unbind(renderer);
	}

	inline ID3D11VertexShader* Shader::GetVertexShader()	 const
	{
		return m_vertexShader;
	}

	inline ID3D11PixelShader* Shader::GetPixelShader()	 const
	{
		return m_pixelShader;
	}

	inline ID3D11InputLayout* Shader::GetLayout()		 const
	{
		return m_layout;
	}

	inline ConstantBuffer<CB::CB_Main>* Shader::GetConstBuffer()	 const
	{
		return m_const_buffer;
	}
}