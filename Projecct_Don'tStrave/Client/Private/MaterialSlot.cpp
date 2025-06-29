#include "MaterialSlot.h"
#include "GameInstance.h"
#include "Item_Manager.h"
#include "Inventory.h"

CMaterialSlot::CMaterialSlot(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUserInterface{pGraphic_Device}
{
}

CMaterialSlot::CMaterialSlot(const CMaterialSlot& Prototype)
	:CUserInterface{Prototype}
{
}

HRESULT CMaterialSlot::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMaterialSlot::Initialize(void* pArg)
{
	m_pInventory = dynamic_cast<CInventory*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_UserInterface"), 0));

	return S_OK;
}

void CMaterialSlot::Priority_Update(_float fTimeDelta)
{
}

void CMaterialSlot::Update(_float fTimeDelta)
{
	// Setting
	ITEM_DATA Data = CItem_Manager::GetInstance()->Get_ItemData(m_iItemID);

	m_iNumNeeded = 0;

	m_iMaterials[0] = Data.iMaterialID_1;
	m_iNeededs[0] = Data.iNumMaterial_1;

	m_iMaterials[1] = Data.iMaterialID_2;
	m_iNeededs[1] = Data.iNumMaterial_2;

	m_iMaterials[2] = Data.iMaterialID_3;
	m_iNeededs[2] = Data.iNumMaterial_3;

	for (_uint i = 0; i < 3; ++i)
	{
		if (0 == m_iMaterials[0])
			break;
		++m_iNumNeeded;
	}

	// Check
	_uint iCheck = {};

	for (_uint i = 0; i < m_iNumNeeded; ++i)
	{
		m_itemCount[i] = m_pInventory->Check_ItemCount(m_iMaterials[i]);

		if (m_itemCount[i] >= m_iNeededs[i])
			++iCheck;
	}

	/*if (iCheck == m_iNumNeeded)
		m_iTextureIndex = 1;
	else
		m_iTextureIndex = 0;*/
}

void CMaterialSlot::Late_Update(_float fTimeDelta)
{
}

HRESULT CMaterialSlot::Render()
{
	switch (m_iNumNeeded)
	{
	case 1:
		m_pTransform_Com->SetPosition(_float3(m_fX - 100.f, m_fY + 30.f, 1.f));
		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
		m_pBackGroundTexture_Com->Set_Texture(0);
		m_pVIBuffer_Com->Render();
		m_pItemTexture_Com->Set_Texture(m_iMaterials[0]);
		m_pVIBuffer_Com->Render();
		break;

	case 2:
		break;

	case 3:
		break;

	default:
		break;
	}

	return S_OK;
}

HRESULT CMaterialSlot::ADD_Components()
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
		TEXT("Com_ItemTexture"),
		reinterpret_cast<CComponent**>(&m_pItemTexture_Com))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Slot"),
		TEXT("Com_FrameTexture"),
		reinterpret_cast<CComponent**>(&m_pTexture_Com))))
		return E_FAIL;

	return S_OK;
}

CMaterialSlot* CMaterialSlot::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMaterialSlot* pInstance = new CMaterialSlot(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CMaterialSlot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMaterialSlot::Clone(void* pArg)
{
	CGameObject* pInstance = new CMaterialSlot(*this);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CMaterialSlot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMaterialSlot::Free()
{
	__super::Free();
}
