#include "Item.h"

#include "GameInstance.h"
#include "Slot.h"
#include "Inventory.h"
#include "Mouse.h"
#include "Camera.h"
#include "UIEffect.h"
#include "Player.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CAinimationObject{ pGraphic_Device }
{
}

CItem::CItem(const CItem& Prototype)
	: CAinimationObject{ Prototype }
{
}

HRESULT CItem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem::Initialize(void* pArg)
{
	m_bEnableBillboard = true;
	m_bHovered = true;

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
	m_Item_Desc.iItemEffect = Item_Desc->iItemEffect;

	m_pTransformCom->SetPosition(Item_Desc->vPosition);

	m_pPlayerTransform_Com = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(
		EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform")));

	if (nullptr == m_pPlayerTransform_Com)
		return E_FAIL;

	m_bEnableBillboard = true;
	m_bIsplayAnim = false;
	Setting_Shader(L"BillBoard.fx");

	Safe_AddRef(m_pPlayerTransform_Com);

	m_pMouse = dynamic_cast<CMouse*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Mouse")));

	Safe_AddRef(m_pMouse);

	return S_OK;
}

void CItem::Priority_Update(_float fTimeDelta)
{
	if (m_bIsplayAnim)
	{
		switch (m_Item_Desc.iItemEffect)
		{
		case 0:
			DropItemEffect(2.f * fTimeDelta);
			break;
		case 1:
			EnterInvenTory();
			break;
		}
	}
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

	if (true == m_bHovered)
	{
		m_pGraphic_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
		m_pGraphic_Device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT); // Stage0 결과
		m_pGraphic_Device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE | D3DTA_ALPHAREPLICATE);

		m_pVIBuffer_Com->Render();

		m_pGraphic_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		m_pGraphic_Device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	}
	else
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
		if (m_bIsplayAnim)
			EnterInvenTory();
		else
			dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Player")))->Get_Player()->pWorkObject = this;

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

_bool CItem::isInRange(_float fRange)
{
	_float3 vRange = m_pPlayerTransform_Com->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);

	if (fRange >= D3DXVec3Length(&vRange))
		return true;

	return false;
}

void CItem::DropItemEffect(_float FallSpeed)
{
	auto Item_Pos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
	if (Item_Pos.y > 0.0f)
	{
		Item_Pos.y -= FallSpeed;
		m_pTransformCom->SetPosition(Item_Pos);
	}
	else
		m_bIsplayAnim = false;
}

void CItem::EnterInvenTory()
{
	CInventory* pInventory = dynamic_cast<CInventory*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_UserInterface"), 0));
	CSlot* pSlot = pInventory->Find_Item(m_Item_Desc.iItemID);

	if (nullptr == pSlot)
		int a{}; // 인벤토리가 꽉참
	else
	{
		CUIEffect::UIEFFECT_DESC Desc = {};

		Desc.iItemID = m_Item_Desc.iItemID;
		Desc.pSlot = pSlot;
		Desc.vCursorPos = m_pGameInstance->GetMousePosition(0);
		memcpy(&Desc.Item_Desc, &m_Item_Desc, sizeof(ITEM_DESC));

		m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UIEffect"),
			EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_UIEffect"), &Desc);

		m_isDead = true;
	}

}




void CItem::Free()
{
	__super::Free();

	Safe_Release(m_pMouse);
	Safe_Release(m_pTexture_Com);
	Safe_Release(m_pCollision_Com);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBuffer_Com);
	Safe_Release(m_pPlayerTransform_Com);
}
