#include "VIBuffer.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent { pGraphic_Device }
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& Prototype)
	: CComponent{ Prototype }
	, m_pVB { Prototype.m_pVB }
	, m_pIB { Prototype.m_pIB }
	, m_iVertexStride { Prototype.m_iVertexStride }
	, m_iNumVertices { Prototype.m_iNumVertices }
	, m_iFVF { Prototype.m_iFVF }
	, m_ePrimitiveType { Prototype.m_ePrimitiveType }
	, m_iNumPrimitive { Prototype.m_iNumPrimitive }

	, m_iIndexStride { Prototype.m_iIndexStride }
	, m_iNumIndices { Prototype.m_iNumIndices }
	, m_eIndexFormat { Prototype.m_eIndexFormat }
	, m_iNumVerticesX{ Prototype.m_iNumVerticesX }
	, m_iNumVerticesZ{ Prototype.m_iNumVerticesZ }
  	, m_pVertexPositions {Prototype.m_pVertexPositions }
{

	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iVertexStride);

	m_pGraphic_Device->SetFVF(m_iFVF);

	m_pGraphic_Device->SetIndices(m_pIB);

	m_pGraphic_Device->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);

	return S_OK;
}

_float CVIBuffer::Compute_Height(const _float3& vLocalPos)
{
	/* */
	_uint			iIndex = static_cast<_uint>(vLocalPos.z) * m_iNumVerticesX + static_cast<_uint>(vLocalPos.x);

	_uint			iIndices[] = {
		iIndex + m_iNumVerticesX, 
		iIndex + m_iNumVerticesX + 1,
		iIndex + 1, 
		iIndex
	};

	_float			fWidth = vLocalPos.x - m_pVertexPositions[iIndices[0]].x;
	_float			fDepth = m_pVertexPositions[iIndices[0]].z - vLocalPos.z;

	D3DXPLANE		Plane = {};

	if (fWidth > fDepth) /* ¿À¸¥ÂÉ¤Á¤¡ À§ »ï°¢Çü¿¡ ÀÖ´Ù. */
	{
		D3DXPlaneFromPoints(&Plane, &m_pVertexPositions[iIndices[0]], &m_pVertexPositions[iIndices[1]], &m_pVertexPositions[iIndices[2]]);
	}
	else
	{
		D3DXPlaneFromPoints(&Plane, &m_pVertexPositions[iIndices[0]], &m_pVertexPositions[iIndices[2]], &m_pVertexPositions[iIndices[3]]);
	}

	/*
	ax + by + cz + d = 0
	y = (-ax - cz - d) / b;
	*/

	return (-Plane.a * vLocalPos.x - Plane.c * vLocalPos.z - Plane.d) / Plane.b;	
}

void CVIBuffer::Free()
{
	__super::Free();

	if(false == m_isCloned)
		Safe_Delete_Array(m_pVertexPositions);

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
