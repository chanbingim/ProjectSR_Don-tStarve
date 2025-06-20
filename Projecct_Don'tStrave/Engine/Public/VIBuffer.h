#pragma once

#include "Component.h"

/* VertexBuffer할당. == Vertex(정점)들을 할당한다. */
/* IndexBuffer할당. */

NS_BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer(const CVIBuffer& Prototype);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render();

public:
	virtual _float Compute_Height(const _float3& vLocalPos);

protected:
	LPDIRECT3DVERTEXBUFFER9				m_pVB = { nullptr };
	LPDIRECT3DINDEXBUFFER9				m_pIB = { nullptr };

	_uint								m_iVertexStride = { };

	_uint								m_iNumVerticesX{}, m_iNumVerticesZ{};
	_uint								m_iNumVertices = { };

	_uint								m_iIndexStride = {};
	_uint								m_iNumIndices = {};
	
	_uint								m_iFVF = {};
	D3DPRIMITIVETYPE					m_ePrimitiveType = {};
	_uint								m_iNumPrimitive = {};

	D3DFORMAT							m_eIndexFormat = {};

	_float3* m_pVertexPositions = { nullptr };


public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END