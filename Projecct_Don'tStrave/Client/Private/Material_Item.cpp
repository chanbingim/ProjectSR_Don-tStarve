#include "Material_Item.h"

#include "GameInstance.h"
#include "Slot.h"
#include "Inventory.h"
#include "Mouse.h"

CMaterial_Item::CMaterial_Item(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CItem{ pGraphic_Device }
{
}

CMaterial_Item::CMaterial_Item(const CMaterial_Item& Prototype)
	: CItem{ Prototype }
{
}

HRESULT CMaterial_Item::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMaterial_Item::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	ITEM_DESC* Item_Desc = static_cast<ITEM_DESC*>(pArg);

	m_Item_Desc.eItemType = Item_Desc->eItemType;
	m_Item_Desc.iItemID = Item_Desc->iItemID;
	m_Item_Desc.iNumItem = Item_Desc->iNumItem;
	m_Item_Desc.fDurability = Item_Desc->fDurability;
	m_Item_Desc.eSlot = Item_Desc->eSlot;

	/*if (FAILED(ADD_Components()))
		return E_FAIL;*/

	//m_pTransform_Com->SetPosition(Item_Desc->vPosition);
	_float3 size = m_pTexture_Com->Get_Size(m_Item_Desc.iItemID);
	_float fMinSize = max(size.x, size.y);

	_float3 vSize = { size.x / fMinSize * 0.5f, size.y / fMinSize * 0.5f, 1.f };

	m_pTransformCom->SetScale(vSize);

	return S_OK;
}

void CMaterial_Item::Priority_Update(_float fTimeDelta)
{
}

void CMaterial_Item::Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);

	HoverEvent();

	SetUp_OnTerrain(m_pTransformCom, 0.f);

	Update_Item(fTimeDelta);
}

void CMaterial_Item::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CMaterial_Item::Render()
{
	__super::Render();

	return S_OK;
}

void CMaterial_Item::HoverEvent()
{
	_float3 vPickingPos = {};

	if (true == dynamic_cast<CVIBuffer_Rect*>(m_pVIBuffer_Com)->Picking(m_pTransformCom, &vPickingPos))
	{
		dynamic_cast<CMouse*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Mouse")))->Update_HoverItem(m_Item_Desc.iItemID);
		__super::ClickedEvent();
	}
}


HRESULT CMaterial_Item::ADD_Components()
{
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"),
		reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
		return E_FAIL;

	Engine::CTransform::TRANSFORM_DESC Transform_Desc = { 5.f, D3DXToRadian(90.f) };

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"),
		reinterpret_cast<CComponent**>(&m_pTransformCom), &Transform_Desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_ItemObject"),
		TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTexture_Com))))
		return E_FAIL;

	return S_OK;
}

void CMaterial_Item::Update_Item(_float fTimeDelta)
{
	switch (m_Item_Desc.eItemType)
	{
	case Client::ITEM_TYPE::MERTARIAL:
		break;

	case Client::ITEM_TYPE::FOOD:
		m_Item_Desc.fDurability -= 3.f * fTimeDelta;
		break;

	case Client::ITEM_TYPE::EQUIPMENT:
		break;

	default:
		break;
	}
}

CMaterial_Item* CMaterial_Item::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMaterial_Item* pInstance = new CMaterial_Item(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone CItem");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CMaterial_Item::Clone(void* pArg)
{
	CGameObject* pInstance = new CMaterial_Item(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CItem");
		return nullptr;
	}

	return pInstance;
}

void CMaterial_Item::Free()
{
	__super::Free();

	Safe_Release(m_pTexture_Com);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBuffer_Com);
}
