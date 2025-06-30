#include "GameObject.h"
#include "GameInstance.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device { pGraphic_Device }
	, m_pGameInstance { CGameInstance::GetInstance() }
{
	D3DXMatrixIdentity(&m_BillboardMat);
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
}

CGameObject::CGameObject(const CGameObject& Prototype)
	: m_pGraphic_Device{ Prototype.m_pGraphic_Device }
	, m_pGameInstance{ Prototype.m_pGameInstance }
	, m_isDead { Prototype.m_isDead }
	, m_BillboardMat { Prototype.m_BillboardMat }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	

	return S_OK;
}

HRESULT CGameObject::Initialize_Late()
{
	return S_OK;
}

void CGameObject::Priority_Update(_float fTimeDelta)
{
}

void CGameObject::Update(_float fTimeDelta)
{
	if (m_bEnableBillboard)
		Excute_Billboard();
}

void CGameObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	if (m_bEnableBillboard)
		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_BillboardMat);

	return S_OK;
}

void CGameObject::Damage()
{
}

void CGameObject::Death()
{
}


CComponent* CGameObject::Find_Component(const _wstring& strComponentTag)
{
	auto	iter = m_Components.find(strComponentTag);
	if(iter == m_Components.end())
		return nullptr;

	return iter->second;
}

HRESULT CGameObject::Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg)
{
	if (nullptr != Find_Component(strComponentTag))
		return E_FAIL;

	CComponent*			pComponent = dynamic_cast<CComponent*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(strComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	return S_OK;
}

void CGameObject::Excute_Billboard()
{
	_matrix CameraView = {};

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraView);
	D3DXMatrixInverse(&CameraView, NULL, &CameraView);

	_float3 right = *(_float3*)&CameraView.m[0];
	_float3 up = *(_float3*)&CameraView.m[1];
	_float3 look = *(_float3*)&CameraView.m[2];

	// (선택) 정규화 + 스케일 적용
	_float3 scale = m_pTransformCom->GetScale();
	D3DXVec3Normalize(&right, &right);
	D3DXVec3Normalize(&up, &up);
	D3DXVec3Normalize(&look, &look);

	right *= scale.x;
	up *= scale.y;
	look *= scale.z;

	memcpy((_float3*)&m_BillboardMat.m[0], right, sizeof(_float3));
	memcpy((_float3*)&m_BillboardMat.m[1], up, sizeof(_float3));
	memcpy((_float3*)&m_BillboardMat.m[2], look, sizeof(_float3));
	memcpy((_float3*)&m_BillboardMat.m[3], m_pTransformCom->GetWorldState(WORLDSTATE::POSITION), sizeof(_float3));
}

void CGameObject::Free()
{
	__super::Free();

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);

	m_Components.clear();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
}
