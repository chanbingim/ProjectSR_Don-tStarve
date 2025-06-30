#include "LandObject.h"
#include "Transform.h"
#include "VIBuffer.h"

CLandObject::CLandObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CLandObject::CLandObject(const CLandObject& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CLandObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLandObject::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	LANDOBJECT_DESC* pDesc = static_cast<LANDOBJECT_DESC*>(pArg);

	m_pLandVIBuffer = pDesc->pLandVIBuffer;
	m_pLandTransform = pDesc->pLandTransform;

	Safe_AddRef(m_pLandVIBuffer);
	Safe_AddRef(m_pLandTransform);

	return S_OK;
}

void CLandObject::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CLandObject::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CLandObject::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CLandObject::Render()
{
	__super::Render();
	return S_OK;
}


void CLandObject::SetUp_OnTerrain(CTransform* pTransform, _float fOffset)
{
	_float3		vWorldPos = pTransform->GetWorldState(WORLDSTATE::POSITION);

	_float3		vLocalPos = {};

	D3DXVec3TransformCoord(&vLocalPos, &vWorldPos, &m_pLandTransform->Get_InverseWorldMat());

	vLocalPos.y = m_pLandVIBuffer->Compute_Height(vLocalPos);

	D3DXVec3TransformCoord(&vWorldPos, &vLocalPos, &m_pLandTransform->Get_World());

	vWorldPos.y += fOffset;

	pTransform->SetPosition(vWorldPos);
}

void CLandObject::Free()
{
	__super::Free();

	Safe_Release(m_pLandTransform);
	Safe_Release(m_pLandVIBuffer);
}
