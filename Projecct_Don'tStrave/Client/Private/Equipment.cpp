#include "Equipment.h"

#include "GameInstance.h"
#include "Slot.h"
#include "Inventory.h"
#include "Mouse.h"

CEquipment::CEquipment(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CItem{ pGraphic_Device }
{
}

CEquipment::CEquipment(const CEquipment& Prototype)
	: CItem{ Prototype }
{
}

HRESULT CEquipment::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEquipment::Initialize(void* pArg)
{
	if (FAILED(__super::ADD_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	_float3 size = m_pTexture_Com->Get_Size(m_Item_Desc.iItemID);
	_float fMinSize = max(size.x, size.y);

	_float3 vSize = { size.x / fMinSize * 0.5f, size.y / fMinSize * 0.5f, 1.f };
	m_pTransformCom->SetScale(vSize);

	return S_OK;
}

void CEquipment::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CEquipment::Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);

	HoverEvent();

	if (!m_bIsplayAnim)
		SetUp_OnTerrain(m_pTransformCom, 0.f);

	Update_Item(fTimeDelta);
}

void CEquipment::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CEquipment::Render()
{
	__super::Render();

	return S_OK;
}

void CEquipment::HoverEvent()
{
	_float3 vPickingPos = {};

	if (true == dynamic_cast<CVIBuffer_Rect*>(m_pVIBuffer_Com)->Picking(m_pTransformCom, &vPickingPos))
	{
		m_bHovered = true;
		dynamic_cast<CMouse*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Mouse")))->Update_HoverItem(m_Item_Desc.iItemID);
		__super::ClickedEvent();
	}
	else
		m_bHovered = false;
}


CEquipment* CEquipment::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEquipment* pInstance = new CEquipment(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone CFood");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CEquipment::Clone(void* pArg)
{
	CGameObject* pInstance = new CEquipment(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CFood");
		return nullptr;
	}

	return pInstance;
}

void CEquipment::Free()
{
	__super::Free();
}
