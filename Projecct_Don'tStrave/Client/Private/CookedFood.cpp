#include "CookedFood.h"

#include "GameInstance.h"
#include "Slot.h"
#include "Inventory.h"
#include "Mouse.h"

CCookedFood::CCookedFood(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CItem{ pGraphic_Device }
{
}

CCookedFood::CCookedFood(const CCookedFood& Prototype)
	: CItem{ Prototype }
{
}

HRESULT CCookedFood::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCookedFood::Initialize(void* pArg)
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

void CCookedFood::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CCookedFood::Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);

	HoverEvent();

	if (!m_bIsplayAnim)
		SetUp_OnTerrain(m_pTransformCom, 0.f);

	Update_Item(fTimeDelta);
}

void CCookedFood::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CCookedFood::Render()
{
	__super::Render();

	return S_OK;
}

void CCookedFood::HoverEvent()
{
	_float3 vPickingPos = {};

	if (true == dynamic_cast<CVIBuffer_Rect*>(m_pVIBufferCom)->Picking(m_pTransformCom, &vPickingPos))
	{
		m_bHovered = true;
		dynamic_cast<CMouse*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Mouse")))->Update_HoverItem(m_Item_Desc.iItemID);
	}
	else
		m_bHovered = false;
}


HRESULT CCookedFood::ADD_Components()
{
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Item"),
		TEXT("Com_Food_Texture"),
		reinterpret_cast<CComponent**>(&m_pFood_TextureCom))))
		return E_FAIL;


	return S_OK;
}

void CCookedFood::Update_Item(_float fTimeDelta)
{
	if (0.f >= m_Item_Desc.fDurability)
	{
		m_Item_Desc.iItemID = 51;
		m_Item_Desc.fDurability = 100.f;
	}

	if (51 != m_Item_Desc.iItemID)
		m_Item_Desc.fDurability -= 3.f * fTimeDelta;
}

CCookedFood* CCookedFood::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCookedFood* pInstance = new CCookedFood(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone CCookedFood");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CCookedFood::Clone(void* pArg)
{
	CGameObject* pInstance = new CCookedFood(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CCookedFood");
		return nullptr;
	}

	return pInstance;
}

void CCookedFood::Free()
{
	__super::Free();
}
