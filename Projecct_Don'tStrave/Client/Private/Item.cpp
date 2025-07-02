#include "Item.h"

#include "GameInstance.h"
#include "Slot.h"
#include "Inventory.h"
#include "Mouse.h"
#include "Camera.h"

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
	m_bEnableBillboard = true;

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


	m_pTransformCom->SetPosition(Item_Desc->vPosition);

	m_pPlayerTransform_Com = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform")));

	if (nullptr == m_pPlayerTransform_Com)
		return E_FAIL;

	m_bEnableBillboard = true;
	Setting_Shader(L"BillBoard.fx");

	Safe_AddRef(m_pPlayerTransform_Com);

	return S_OK;
}

void CItem::Priority_Update(_float fTimeDelta)
{
}

void CItem::Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);

	HoverEvent();

	SetUp_OnTerrain(m_pTransformCom, 0.f);

	Update_Item(fTimeDelta);
}

void CItem::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CItem::Render()
{
	//m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_World());
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	
	class CGameObject* Obj = m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Camera"));
	auto Camera = dynamic_cast<CCamera*>(Obj);
	if (nullptr == Camera)
		return E_FAIL;

	LPDIRECT3DBASETEXTURE9 pTex = { nullptr };

	m_pTexture_Com->Set_Texture(m_Item_Desc.iItemID);
	m_pGraphic_Device->GetTexture(0, &pTex);
	Excute_Billboard(Camera->GetInvViewMat(), pTex);

	__super::Render();
	m_pVIBuffer_Com->Render();
	End_Billboard();
	
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	return S_OK;
}

void CItem::HoverEvent()
{
	_float3 vPickingPos = {};

	if (true == dynamic_cast<CVIBuffer_Rect*>(m_pVIBuffer_Com)->Picking(m_pTransformCom, &vPickingPos))
	{
		dynamic_cast<CMouse*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Mouse")))->Update_HoverItem(m_Item_Desc.iItemID);
		ClickedEvent();
	}
}

void CItem::ClickedEvent()
{
	if (m_pGameInstance->KeyDown(VK_RBUTTON))
	{
		_float3 vPickingPos = {};

		if (true == dynamic_cast<CVIBuffer_Rect*>(m_pVIBuffer_Com)->Picking(m_pTransformCom, &vPickingPos))
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
		reinterpret_cast<CComponent**>(&m_pTransformCom), &Transform_Desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_ItemObject"),
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

_bool CItem::isInRange()
{
	_float3 vRange = m_pPlayerTransform_Com->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);

	if (0.8f >= D3DXVec3Length(&vRange))
		return true;

	return false;
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
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBuffer_Com);
	Safe_Release(m_pPlayerTransform_Com);
}
