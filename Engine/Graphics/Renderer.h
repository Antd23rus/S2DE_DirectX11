#pragma once
#include "Base/Main/Common.h"
#include "Math/Color.h"

#include "Libs/imgui/imgui.h"
#include "Libs/imgui/imgui_impl_win32.h"
#include "Libs/imgui/imgui_impl_dx11.h"

//This macro enable debug layers
//Disable optimization and enable debug mode in shaders
#define S2DE_DEBUG_RENDER_MODE 

//ImGui WndProcHandler
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//Name of default font for ui_basic_text and imgui  
#define S2DE_DEFAULT_FONT_NAME "default"

namespace S2DE
{
	namespace Editor
	{
		class EditorCursor;
		class EditorCenterCursor;
	}
}

namespace S2DE::Render
{
	enum class RenderFillMode : std::int32_t
	{
		Solid = D3D11_FILL_MODE::D3D11_FILL_SOLID,
		Wireframe = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME,
	};

	class S2DE_API Renderer
	{
	public:
		Renderer();
		~Renderer();

		bool								Create();
		bool								Reset();
		void								Destroy();
		void								Render();
		void								SwitchFillMode(RenderFillMode mode);
		void								SetBackColor(Math::Color<float> color);
		void								TurnZBufferOff();
		void								TurnZBufferOn();
		void								ToggleImGuiWindowsVisible();
		void								ToggleImGuiDemoWindowVisible();

		inline ID3D11Device*				GetDevice() { return m_device; }
		inline ID3D11DeviceContext*			GetContext() { return m_deviceContext; }
		inline IDXGISwapChain*				GetSwapChain() { return m_swapChain; }
		inline ID3D11RenderTargetView*		GetRenderTargetView() { return m_renderTargetView; }
		inline bool							GetVsync() const { return m_vsync; }
		inline D3D11_VIEWPORT				GetViewport() const { return m_viewport; }
		inline ID3D11Texture2D*				GetDepthStencilBuffer() { return m_depthStencilBuffer; }
		inline ID3D11DepthStencilState*		GetDepthStencilState() { return   m_depthStencilState; }
		inline ID3D11DepthStencilView*		GetDepthStencilView() { return    m_depthStencilView; }
		inline ID3D11ShaderResourceView*	GetFramebufferShaderResource() const { return m_framebuffer_shaderResourceView; }
		inline ID3D11Texture2D*				GetFramebufferTextureData() const { return m_framebuffer_data; }

		inline ID3D11RasterizerState*		GetRasterStateCW() const { return m_rasterStateCW; }
		inline ID3D11RasterizerState*		GetRasterStateCCW() const { return m_rasterStateCCW; }

		inline class Editor::EditorCursor* GetEditorCursor() { return m_editor_cursor; }
		inline class Editor::EditorCenterCursor* GetEditorCenterCursor() { return m_editor_center_cursor; }

	private:					 
		bool								InitImGui();
		void								UpdateImGuiWindows();
		void								RenderImGui();
		void								LoadCustomImguiTheme();
		void								DestroyImGui();
		bool								CreateDeviceAndSwapChain();
		bool								CreateDepthStencil();
		bool								ConfigureBackBuffer();
		void								UpdateViewport();
		void								Clear();
		void								End();
		bool								CaptureMessages();
		bool								CreateRasterizerState();
		bool								CreateBlendState();
		bool								CreateFramebufferTexture(ID3D11Texture2D* sw_buff);
		void								UpdateFramebufferShaderResource();
		void								CreateDebugLayer();

		IDXGISwapChain*				m_swapChain;
		ID3D11Device*				m_device;
		ID3D11DeviceContext*		m_deviceContext;
		ID3D11RenderTargetView*		m_renderTargetView;
		ID3D11BlendState*			m_blendState;
		ID3D11RasterizerState*		m_rasterStateCW;
		ID3D11RasterizerState*		m_rasterStateCCW;		
		ID3D11Texture2D*			m_backBuffer;
		ID3D11Texture2D*			m_depthStencilBuffer;
		ID3D11DepthStencilState*	m_depthStencilState;
		ID3D11DepthStencilState*	m_depthDisabledStencilState;
		ID3D11DepthStencilView*		m_depthStencilView;
		bool						m_vsync;
		D3D11_VIEWPORT				m_viewport;
		std::uint32_t				m_device_flag;
		Math::Color<float>			m_clearColor;
		RenderFillMode				m_render_fill_mode;

		ID3D11Texture2D*			m_framebuffer_data;
		ID3D11ShaderResourceView*	m_framebuffer_shaderResourceView;
		
		ID3D11Debug*				m_d3ddebug;
		ID3D11InfoQueue*			m_d3dinfoqueue;

		class Editor::EditorCursor*				m_editor_cursor;
		class Editor::EditorCenterCursor*		m_editor_center_cursor;
		class ImGui_Window*						m_editor_toolstrip;
		std::vector<std::pair<std::string, class ImGui_Window*>>	m_windows_storage;

		bool						m_show_imgui_windows;
		bool						m_show_imgui_demo_wnd;

	public:  
		inline class ImGui_Window*	GetImGui_Window(std::string name) const;
		void						AddImGuiWindow(std::string name, ImGui_Window* wnd, bool visible = false);
		void						DeleteImGuiWindow(std::string name);
	};
}