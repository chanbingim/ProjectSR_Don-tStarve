#include "Grid.h"

#include "GameInstance.h"
#include "Terrian_Manager.h"

#include "Mouse.h"
#include "Terrain.h"

CGrid::CGrid(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{ pGraphic_Device }
{
}

CGrid::CGrid(const CGrid& Prototype)
	:CGameObject{ Prototype }
{
}

HRESULT CGrid::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGrid::Initialize(void* pArg)
{
	if (FAILED(ADD_Components()))
		return E_FAIL;

	m_pTerrains = CTerrian_Manager::GetInstance()->GetTerrains();


	m_pTransformCom->SetRotAxis(_float3(1.f, 0.f, 0.f), D3DXToRadian(90.f));
	m_pTransformCom->SetScale(_float3(1.5f, 1.5f, 1.5f));
	
	return S_OK;
}

void CGrid::Priority_Update(_float fTimeDelta)
{
}

void CGrid::Update(_float fTimeDelta)
{
	_float3 vPickingPos = {};

	

	for (auto pTerrain : *m_pTerrains)
	{
		if (nullptr == pTerrain)
			continue;

		auto VIbuffer = pTerrain->GetCurVIBuffer();
		auto Transform = pTerrain->GetTransfrom();

		if (VIbuffer->Picking(Transform, &vPickingPos))
		{
			m_pTransformCom->SetPosition(vPickingPos);
			break;
		}
	}

	

	m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);
}

void CGrid::Late_Update(_float fTimeDelta)
{
}

HRESULT CGrid::Render()
{
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CGrid::ADD_Components()
{
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"),
		reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	Engine::CTransform::TRANSFORM_DESC Transform_Desc = { 5.f, D3DXToRadian(90.f) };

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"),
		reinterpret_cast<CComponent**>(&m_pTransformCom), &Transform_Desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Grid"),
		TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}


CGrid* CGrid::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGrid* pInstance = new CGrid(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CCookpot");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CGrid::Clone(void* pArg)
{
	CGameObject* pInstance = new CGrid(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone CGrid");
		return nullptr;
	}

	return pInstance;
}

void CGrid::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
}
