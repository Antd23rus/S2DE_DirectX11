#pragma once
#include "Base/Main/Common.h"
#include "IO/IO_File.h"
#include "IO/IO_Disposible.h"

#include <fbxsdk.h>

namespace S2DE::Render
{
	class S2DE_API FBX_Importer
	{
	public:
		static void	Destroy();
		static void	Init();
		static void	PrintNodeInfo(FbxNode* node);
		static bool	Import(std::string path, FbxScene*& scene);
		static void GetUV(FbxMesh* mesh, std::int32_t vertexIndex, std::int32_t uvChannel, std::int32_t uvLayer, DirectX::SimpleMath::Vector2& uv);
		static void GetNormal(FbxMesh* mesh, std::int32_t vertexIndex, std::int32_t vertexCount, DirectX::SimpleMath::Vector3& normal);
		static FbxManager* GetFbxManager() { return m_manager; }
	private:
		static FbxManager*		m_manager;
	};
}