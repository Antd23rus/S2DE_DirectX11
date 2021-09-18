#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <typeindex>
#include <string>

//Disable warning "needs to have dll-interface to be used by clients of class"
#pragma warning(disable: 4251)

//DLL stuff
#include "S2DE_API.h"

#include <windows.h>

//////////////////////////////////////////////
//Dx11
//////////////////////////////////////////////

//Disable warning about old sdk 
#pragma warning(disable: 4005)

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <directxmath.h>

#include "Base/Other/StringUtils.h"

#include <comdef.h>

namespace S2DE
{
	template<typename T>
	static void  Delete(T t)
	{
		if (t)
		{
			delete t;
			t = NULL;
		}
	}

	template<typename T>
	static void Release(T t)
	{
		if (t)
		{
			t->Release();
			t = nullptr;
		}
	}

	//For direct input
	template<typename T>
	static void ReleaseInputDevice(T t)
	{
		if (t)
		{
			t->Unacquire();
			t->Release();
			t = nullptr;
		}
	}

	struct S2DE_API VideoCardDesc
	{
		std::int32_t Memory;
		char Description[128];
	};

	//DirectX Math library defines
	typedef S2DE_API DirectX::XMVECTOR XVector;
	typedef S2DE_API DirectX::XMMATRIX XMatrix;
	typedef S2DE_API DirectX::XMFLOAT2 XFloat2;
	typedef S2DE_API DirectX::XMFLOAT3 XFloat3;
	typedef S2DE_API DirectX::XMFLOAT4 XFloat4;

	struct S2DE_API Vertex
	{
		XFloat3 position;
		XFloat4 color;
		XFloat2 uv;
	};
}

