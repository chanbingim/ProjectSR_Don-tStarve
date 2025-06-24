#include "Item.h"

#include "GameInstance.h"
#include "Slot.h"

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

	m_eItemType = Item_Desc->eItemType;
	m_iItemID = Item_Desc->iItemID;
	m_iNumItem = Item_Desc->iNumItem;
	m_fDurability = Item_Desc->fDurability;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	m_pTransform_Com->SetPosition(Item_Desc->vPosition);

	return S_OK;
}

void CItem::Priority_Update(_float fTimeDelta)
{
}

void CItem::Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);

	SetUp_OnTerrain(m_pTransform_Com, 0.5f);

	_float3 vPickingPos = {};

	if (true == dynamic_cast<CVIBuffer_Rect*>(m_pVIBuffer_Com)->Picking(m_pTransform_Com, &vPickingPos))
	{
		if(GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		{
			CSlot* pSlot = dynamic_cast<CSlot*>(m_pGameInstance->Chagne_Slot());

			pSlot->Set_Info(m_iItemID, m_iNumItem, m_fDurability);

			m_isDead = true;
		}
	}
}

void CItem::Late_Update(_float fTimeDelta)
{

}

HRESULT CItem::Render()
{
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

	m_pTexture_Com->Set_Texture(m_iItemID);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pVIBuffer_Com->Render();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
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
