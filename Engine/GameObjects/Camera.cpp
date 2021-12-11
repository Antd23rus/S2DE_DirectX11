#include "Camera.h"

#include "Base/Engine.h"
#include "Base/GameWindow.h"
#include "Base/InputManager.h"
#include "Math/MathConstants.h"


#define CAMERA_DEFAULT_ZOOM 0.025f
#define CAMERA_DEFAULT_FOV 60.0f
#define CAMERA_DEFAULT_SPEED 16.3f
#define CAMERA_DEFAULT_PROJ_MODE CameraProjectionMode::Ortho
#define CAMERA_DEFAULT_SENSITIVITY 0.005f
#define CAMERA_DEFAULT_FORWARD_VEC	Math::To_XMVector3(Math::Vector3(0, 0, 1))
#define CAMERA_DEFAULT_RIGHT_VEC	Math::To_XMVector3(Math::Vector3(1, 0, 0))
#define CAMERA_DEFAULT_UP_VEC		Math::To_XMVector3(Math::Vector3(0, 1, 0))
#define CAMERA_DEFAULT_TARGET_VEC	Math::To_XMVector3(Math::Vector3(0, 0, 0))
#define CAMERA_DEFAULT_ZNEAR 0.001f
#define CAMERA_DEFAULT_ZFAR 10000.0f

namespace S2DE::GameObjects
{
	Camera::Camera() :
		m_forward(CAMERA_DEFAULT_FORWARD_VEC),
		m_right(CAMERA_DEFAULT_RIGHT_VEC),
		m_up(CAMERA_DEFAULT_UP_VEC),
		m_target(CAMERA_DEFAULT_TARGET_VEC),
		m_Zoom(CAMERA_DEFAULT_ZOOM),
		m_Fov(CAMERA_DEFAULT_FOV),
		m_mode(CAMERA_DEFAULT_PROJ_MODE),
		m_speed(CAMERA_DEFAULT_SPEED),
		m_sensitivity(CAMERA_DEFAULT_SENSITIVITY),
		m_block_mouse_control(false),
		m_block_movement_control(false),
		m_zNear(CAMERA_DEFAULT_ZNEAR),
		m_zFar(CAMERA_DEFAULT_ZFAR)
	{
		Init(S2DE_MAIN_CAMERA_NAME, S2DE_ENGINE_GAMEOBJECT_TYPE, -1);
		SetPosition_Z(1.0f);

		m_viewMatrix = DirectX::XMMatrixIdentity();
		m_projectionMatrix = DirectX::XMMatrixIdentity();
	}



	Camera::~Camera()
	{
		
	}

	void Camera::OnDebugRenderGUI()
	{
		ImGui::Begin("Camera Debug Window", 0);

		ImGui::Text("Position: X:%f Y:%f Z:%f", GetPosition().x, GetPosition().y, GetPosition().z);
		ImGui::Text("Rotation: X:%f Y:%f Z:%f", GetRotation().x, GetRotation().y, GetRotation().z);
		ImGui::Separator();


		ImGui::Text("ZNear %f", m_zNear);
		ImGui::Text("zFar %f", m_zFar);
		ImGui::Separator();
		ImGui::SliderFloat("ZNear", &m_zNear, 0.0f, 1000.0f, "", 1.0f);
		ImGui::SliderFloat("zFar", &m_zFar, 0.0f, 1000.0f, "", 1.0f);

		ImGui::Text("Zoom %f", m_Zoom);
		ImGui::Text("Speed %f", m_speed);
		ImGui::Text("Fov %f", m_Fov);
		ImGui::Separator();
		ImGui::SliderFloat("Zoom", &m_Zoom, 0.001f, 1.0f, "", 1.0f);
		ImGui::SliderFloat("Speed", &m_speed, 0.1f, 45.0f, "", 1.0f);
		ImGui::SliderFloat("Fov", &m_Fov, 30, 110, "", 1.0f);
		ImGui::Separator();

		ImGui::Checkbox("Block mouse control", &m_block_mouse_control);
		ImGui::Checkbox("Block movement control", &m_block_movement_control);
		ImGui::Separator();

		ImGui::Text("Mode:%d ", m_mode);
		ImGui::Text("Sensitivity:%f", m_sensitivity);
		ImGui::Separator();

		ImGui::Text("Camera mode");
		if (ImGui::Button("Perspective"))
		{
			m_mode = CameraProjectionMode::Perspective;
		}

		ImGui::SameLine();
		if (ImGui::Button("Ortho"))
		{
			m_mode = CameraProjectionMode::Ortho;
		}
		ImGui::Separator();

		if (ImGui::Button("Reset"))
		{
			m_forward = CAMERA_DEFAULT_FORWARD_VEC;
			m_right = CAMERA_DEFAULT_RIGHT_VEC;
			m_up = CAMERA_DEFAULT_UP_VEC;
			m_target = CAMERA_DEFAULT_TARGET_VEC;
			m_Zoom = CAMERA_DEFAULT_ZOOM;
			m_Fov = CAMERA_DEFAULT_FOV;
			m_mode = CAMERA_DEFAULT_PROJ_MODE;
			m_speed = CAMERA_DEFAULT_SPEED;
			m_sensitivity = CAMERA_DEFAULT_SENSITIVITY;
			m_block_mouse_control = false;
			m_block_movement_control = false;
			m_zNear = CAMERA_DEFAULT_ZNEAR;
			m_zFar = CAMERA_DEFAULT_ZFAR;

			Reset();
		}

		ImGui::End();
	}

	void Camera::OnUpdateInput()
	{
		UpdateControl();
		UpdateMouseControl();
	}

	void Camera::UpdateControl()
	{
		if (m_block_movement_control == true)
			return;

		if (m_mode == CameraProjectionMode::Perspective)
		{
		
		}
		else
		{
			if (Core::Engine::GetInputManager()->IsKeyDown(Core::Other::KeyCode::KEY_W))
			{
				Fly(1.0f);
			}

			if (Core::Engine::GetInputManager()->IsKeyDown(Core::Other::KeyCode::KEY_S))
			{
				Fly(-1.0f);
			}

			if (Core::Engine::GetInputManager()->IsKeyDown(Core::Other::KeyCode::KEY_D))
			{
				Strafe(1.0f);
			}

			if (Core::Engine::GetInputManager()->IsKeyDown(Core::Other::KeyCode::KEY_A))
			{
				Strafe(-1.0f);
			}
		}
	}

	void Camera::Move(float Right, float Forward)
	{

	}

	void Camera::UpdateMouseControl()
	{
		S2DE_NO_IMPL();
	}

	void Camera::Strafe(float side)
	{
		SetPosition_X(GetPosition().x + side * m_speed * Core::Engine::GetGameTime().GetDeltaTime());
	}

	void Camera::Fly(float side)
	{
		SetPosition_Y(GetPosition().y + side * m_speed * Core::Engine::GetGameTime().GetDeltaTime());
	}

	void Camera::Walk(float side)
	{
		SetPosition_Z(GetPosition().z + side * m_speed * Core::Engine::GetGameTime().GetDeltaTime());
	}

	void Camera::SetMode(CameraProjectionMode mode)
	{
		m_mode = mode;
	}

	void Camera::SetZoom(float zoom)
	{
		m_Zoom = zoom;
	}

	void Camera::DisableMouseControl(bool block)
	{
		m_block_mouse_control = block;
	}

	void Camera::DisableControl(bool block)
	{
		m_block_movement_control = block;
	}

	void Camera::OnRender()
	{
		//TODO
		//Render camera icon


	}

	void Camera::OnUpdate(float DeltaTime)
	{
		if (GetName() != S2DE_MAIN_CAMERA_NAME)
			return;

		float w = (float)Core::Engine::GetGameWindow()->GetWidth();
		float h = (float)Core::Engine::GetGameWindow()->GetHeight();

		if (m_mode == CameraProjectionMode::Perspective)
		{

		}
		else
		{
			m_viewMatrix = DirectX::XMMatrixTranslation(-GetPosition().x, -GetPosition().y, -1.0f);
			m_viewMatrix = DirectX::XMMatrixMultiply(m_viewMatrix, DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(GetRotation().z)));
			m_viewMatrix = DirectX::XMMatrixTranspose(m_viewMatrix);

			m_projectionMatrix = DirectX::XMMatrixOrthographicLH( w * m_Zoom, h * m_Zoom, m_zNear, m_zFar);
		}


		m_orthoMatrix = DirectX::XMMatrixOrthographicLH( w * m_Zoom, h * m_Zoom, m_zNear, m_zFar);
	}
}