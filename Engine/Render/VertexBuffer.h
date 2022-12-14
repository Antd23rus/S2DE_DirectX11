#pragma once
#include "Base/Main/Common.h"
#include "Render/Buffer.h"
#include "Base/Engine.h"

namespace S2DE::Render
{
	template<typename T = Vertex>
	class S2DE_API VertexBuffer : public Buffer
	{
	public:
		VertexBuffer()
		{

		}

		~VertexBuffer() final
		{
			Core::Release(m_buffer);
			m_array.clear();
			m_array.shrink_to_fit();
		}

		bool Create(D3D11_USAGE buffer_usage = D3D11_USAGE_DYNAMIC) final
		{
			if (m_array.size() == 0)
				return false;

			D3D11_BUFFER_DESC bufferDesc = { };

			bufferDesc.ByteWidth = static_cast<std::uint32_t>(sizeof(Vertex) * m_array.size());
			bufferDesc.Usage = buffer_usage;
			bufferDesc.CPUAccessFlags = buffer_usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA subData = { };

			subData.pSysMem = m_array.data();
			subData.SysMemPitch = 0;
			subData.SysMemSlicePitch = 0;

			S2DE_CHECK_SAFE(Core::Engine::GetRenderer()->GetDevice()->CreateBuffer(&bufferDesc, &subData, &m_buffer), "Can't create buffer!");

			return true;
		}

		bool Lock() final
		{
			if (m_buffer_desc.Usage == D3D11_USAGE_DYNAMIC)
			{
				D3D11_MAPPED_SUBRESOURCE mappedResource = { };
				S2DE_CHECK_SAFE(Core::Engine::GetRenderer()->GetContext()->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource), "Can't map resource in vertex buffer");
				memcpy((Vertex*)mappedResource.pData, (void*)m_array.data(), (sizeof(Vertex) * m_array.size()));
			}

			return true;
		}

		void Unbind() final
		{
			Core::Engine::GetRenderer()->GetContext()->IASetVertexBuffers(0, 0, nullptr, 0, 0);
		}

		void Unlock() final
		{
			if (m_buffer_desc.Usage == D3D11_USAGE_DYNAMIC)
				Core::Engine::GetRenderer()->GetContext()->Unmap(m_buffer, 0);
		}

		void Bind(std::int32_t startSlot = 0, std::int32_t num_buffers = 1) final
		{
			std::uint32_t stride = sizeof(Vertex);
			std::uint32_t offset = 0;

			Core::Engine::GetRenderer()->GetContext()->IASetVertexBuffers(startSlot, num_buffers, &m_buffer, &stride, &offset);
		}

		inline std::vector<T>& GetArray()
		{
			return m_array;
		}

	private:
		std::vector<T> m_array;

	};
}