#include "AlphaObject.h"

CAlphaObject::CAlphaObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLandObject{ pGraphic_Device }
{
}

CAlphaObject::CAlphaObject(const CAlphaObject& Prototype)
	: CLandObject{ Prototype }
{
}

HRESULT CAlphaObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAlphaObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	return S_OK;
}

void CAlphaObject::Priority_Update(_float fTimeDelta)
{
}

void CAlphaObject::Update(_float fTimeDelta)
{
}

void CAlphaObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CAlphaObject::Render()
{
	return S_OK;
}

void CAlphaObject::Compute_CamDistance(const _float3& vWorldPos)
{
	_float4x4		ViewMatrix = {};
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3			vCamPos = {};
	memcpy(&vCamPos, &ViewMatrix.m[3], sizeof(_float3));

	_float3			vDir = vCamPos - vWorldPos;
	m_fCamDistance = D3DXVec3Length(&vDir);
}

void CAlphaObject::Free()
{
	__super::Free();
}
