#pragma once
#include "Base/Main/Common.h"

namespace S2DE::Core::Other
{
	class S2DE_API SplashScreen
	{
	public:
		SplashScreen();
		~SplashScreen();

		//Set project name for editor splash screen 
		static void				  SetProjectName(std::string name);

		//Set corrent load state
		static void				  SetLoadState(std::string state, SplashScreen* splashScreen);

		//Create and show splash screen
		bool					  ShowSplashScreen(HINSTANCE hinstance);
		//Close window
		void					  Close();

		LRESULT CALLBACK		  WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		HWND					  m_hwnd;
		HINSTANCE				  m_hinstance;
		HINSTANCE				  m_res_hInst;
		WNDCLASSEX				  m_splashWindowClass;

		const LPCSTR			  m_className = "S2DE_SPLASH_SCREEN_WND_CLASS";
		const LPCSTR			  m_name = "S2DE Splash Screen";

		BITMAP					  m_bitmap;
		HDC						  m_hdc_bp;
		HDC						  m_hdcMem_bp;
		HBITMAP					  m_hBitmap;
		HBITMAP 				  m_hOldBitmap;
		HDC						  m_hdcStatic;
		
		
		HFONT					  m_hfont_LoadState;
		HFONT					  m_hfont_Build;
		HFONT					  m_hfont_Name_Ed;
		HFONT					  m_hfont_Ed;
		std::string				  m_text_build_str;
		std::string				  m_text_ed_str;
		static SplashScreen*      m_instanceWindow;


		static std::string		  m_loadState;
		static std::string		  m_projectName;

		void					  RedrawWindowElements();
	private:
		static SplashScreen*      GetInstanceWindow() { return m_instanceWindow; }
		static LRESULT CALLBACK   InstanceWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}
