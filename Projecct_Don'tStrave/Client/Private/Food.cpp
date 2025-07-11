#include "Food.h"

#include "GameInstance.h"
#include "Slot.h"
#include "Inventory.h"
#include "Mouse.h"

CFood::CFood(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CItem{ pGraphic_Device }
{
}

CFood::CFood(const CFood& Prototype)
	: CItem{ Prototype }
{
}

HRESULT CFood::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFood::Initialize(void* pArg)
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

void CFood::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CFood::Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);

	HoverEvent();

	Update_Item(fTimeDelta);
}

void CFood::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CFood::Render()
{
	__super::Render();

	return S_OK;
}

void CFood::HoverEvent()
{
	_float3 vPickingPos = {};

	if (true == dynamic_cast<CVIBuffer_Rect*>(m_pVIBufferCom)->Picking(m_pTransformCom, &vPickingPos))
	{
		m_bHovered = true;
		dynamic_cast<CMouse*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Mouse")))->Update_HoverItem(m_Item_Desc.iItemID);
		__super::ClickedEvent();
	}
	else
		m_bHovered = false;
}


HRESULT CFood::ADD_Components()
{

	return S_OK;
}

void CFood::Update_Item(_float fTimeDelta)
{
	if (0.f >= m_Item_Desc.fDurability)
	{
		m_Item_Desc.iItemID = 51;
		m_Item_Desc.fDurability = 100.f;
	}

	if(51 != m_Item_Desc.iItemID)
		m_Item_Desc.fDurability -= 3.f * fTimeDelta;
}

CFood* CFood::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFood* pInstance = new CFood(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone CFood");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CFood::Clone(void* pArg)
{
	CGameObject* pInstance = new CFood(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CFood");
		return nullptr;
	}

	return pInstance;
}

void CFood::Free()
{
	__super::Free();
}
