#include "Item.h"

#include "GameInstance.h"
#include "Slot.h"
#include "Inventory.h"
#include "Mouse.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLandObject{ pGraphic_Device }
{
}

CItem::CItem(const CItem& Prototype)
	: CLandObject{ Prototype }
{
}

HRESULT CItem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem::Initialize(void* pArg)
{
	CLandObject::LANDOBJECT_DESC			Desc{};
	Desc.pLandTransform = static_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_Transform")));
	Desc.pLandVIBuffer = static_cast<CVIBuffer*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_VIBuffer")));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	ITEM_DESC* Item_Desc = static_cast<ITEM_DESC*>(pArg);

	m_Item_Desc.eItemType = Item_Desc->eItemType;
	m_Item_Desc.iItemID = Item_Desc->iItemID;
	m_Item_Desc.iNumItem = Item_Desc->iNumItem;
	m_Item_Desc.fDurability = Item_Desc->fDurability;
	m_Item_Desc.eSlot = Item_Desc->eSlot;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	m_pTransform_Com->SetPosition(Item_Desc->vPosition);
	m_pTransform_Com->SetScale(_float3(0.4f, 0.4f, 1.f));

	return S_OK;
}

void CItem::Priority_Update(_float fTimeDelta)
{
}

void CItem::Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);

	HoverEvent();

	SetUp_OnTerrain(m_pTransform_Com, 0.5f);

	Update_Item(fTimeDelta);
}

void CItem::Late_Update(_float fTimeDelta)
{

}

HRESULT CItem::Render()
{
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

	m_pTexture_Com->Set_Texture(m_Item_Desc.iItemID);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pVIBuffer_Com->Render();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

void CItem::HoverEvent()
{
	_float3 vPickingPos = {};

	if (true == dynamic_cast<CVIBuffer_Rect*>(m_pVIBuffer_Com)->Picking(m_pTransform_Com, &vPickingPos))
	{
		dynamic_cast<CMouse*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Mouse")))->Update_HoverObject(this);
		ClickedEvent();
	}
}

void CItem::ClickedEvent()
{
	if (m_pGameInstance->KeyDown(VK_LBUTTON))
	{
		CSlot* pSlot = dynamic_cast<CSlot*>(m_pGameInstance->Chagne_Slot());

		pSlot->Set_Info(m_Item_Desc);

		m_isDead = true;
	}

	if (m_pGameInstance->KeyDown(VK_RBUTTON))
	{
		_float3 vPickingPos = {};

		if (true == dynamic_cast<CVIBuffer_Rect*>(m_pVIBuffer_Com)->Picking(m_pTransform_Com, &vPickingPos))
		{
			CInventory* pInventory = dynamic_cast<CInventory*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_UserInterface"), 0));
			CSlot* pSlot = pInventory->Find_Item(m_Item_Desc.iItemID);

			if (nullptr == pSlot)
				int a{}; // 인벤토리가 꽉참
			else
			{
				_uint iItemID = pSlot->Get_ItemID();
				if(0 == iItemID)
				{
					pSlot->Set_Info(m_Item_Desc);
					m_isDead = true;
				}
				else if (m_Item_Desc.iItemID == iItemID)
				{
					pSlot->Merge_Item(m_Item_Desc);
					m_isDead = true;
				}
			}
		}
	}
}

HRESULT CItem::ADD_Components()
{
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"),
		reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
		return E_FAIL;

	Engine::CTransform::TRANSFORM_DESC Transform_Desc = { 5.f, D3DXToRadian(90.f) };

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"),
		reinterpret_cast<CComponent**>(&m_pTransform_Com), &Transform_Desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Item"),
		TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTexture_Com))))
		return E_FAIL;

	return S_OK;
}

void CItem::Update_Item(_float fTimeDelta)
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

CItem* CItem::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItem* pInstance = new CItem(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone CItem");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CItem::Clone(void* pArg)
{
	CItem* pInstance = new CItem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CItem");
		return nullptr;
	}

	return pInstance;
}

void CItem::Free()
{
	__super::Free();

	Safe_Release(m_pTexture_Com);
	Safe_Release(m_pTransform_Com);
	Safe_Release(m_pVIBuffer_Com);
}
