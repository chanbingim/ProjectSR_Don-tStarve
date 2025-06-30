#include "MaterialSlot.h"
#include "GameInstance.h"
#include "Item_Manager.h"
#include "Inventory.h"
#include "Button.h"
#include "Slot.h"
#include "Create_Button.h"

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
	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	UIOBJECT_DESC Desc = {};
	Desc.fX = pDesc->fX - 30.f;
	Desc.fY = pDesc->fY + 180.f;
	Desc.fSizeX = 50.f;
	Desc.fSizeY = 50.f;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	__super::UpdatePosition();

	m_pInventory = dynamic_cast<CInventory*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_UserInterface"), 0));

	if (nullptr == m_pInventory)
		return E_FAIL;

	m_iItemID = 1;

	m_SlotPositions[0] = m_pTransform_Com->GetWorldState(WORLDSTATE::POSITION);
	m_SlotPositions[1] = _float3(m_SlotPositions[0].x - 60.f, m_SlotPositions[0].y, 1.f);
	m_SlotPositions[2] = _float3(m_SlotPositions[0].x + 60.f, m_SlotPositions[0].y, 1.f);
	m_SlotPositions[3] = _float3(m_SlotPositions[0].x - 30.f, m_SlotPositions[0].y, 1.f);
	m_SlotPositions[4] = _float3(m_SlotPositions[0].x + 30.f, m_SlotPositions[0].y, 1.f);

	for (_uint i = 0; i < 5; ++i)
	{
		m_rcFonts[i] = {
			static_cast<LONG>(g_iWinSizeX * 0.5 + m_SlotPositions[i].x - m_fSizeX),
			static_cast<LONG>(g_iWinSizeY * 0.5 - m_SlotPositions[i].y - m_fSizeY + 25),
			static_cast<LONG>(g_iWinSizeX * 0.5 + m_SlotPositions[i].x + m_fSizeX),
			static_cast<LONG>(g_iWinSizeY * 0.5 - m_SlotPositions[i].y + m_fSizeY + 25)
		};
	}
	CButton::BUTTON_DESC Button_Desc = {};
	Button_Desc.fX = m_fX;
	Button_Desc.fY = m_fY;
	Button_Desc.fSizeX = 80.f;
	Button_Desc.fSizeY = 40.f;
	Button_Desc.fRelativeX = 100.f;
	Button_Desc.fRelativeY = -m_fSizeY * 0.5f + 20.f;
	Button_Desc.pParentTransform = m_pTransform_Com;

	m_pCreateButton = dynamic_cast<CCreate_Button*>(m_pGameInstance->Clone_Prototype(
		PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Create_Button"), &Button_Desc));

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
		if (0 != m_iMaterials[i])
			++m_iNumNeeded;
	}

	// Check
	_uint iCheck = {};

	for (_uint i = 0; i < m_iNumNeeded; ++i)
	{
		m_itemCount[i] = m_pInventory->Check_ItemCount(m_iMaterials[i]);

		if (m_itemCount[i] >= m_iNeededs[i])
		{
			++iCheck;
			m_TextColor[i] = { 1.f, 1.f, 1.f, 1.f };
			m_iFrameIndex[i] = 0;
		}
		else
		{
			m_TextColor[i] = { 1.f, 0.f, 0.f, 1.f };
			m_iFrameIndex[i] = 6;
		}
	}
	m_pCreateButton->Update(fTimeDelta);

	if (iCheck == m_iNumNeeded)
	{
		m_pCreateButton->Set_Button(1);
		CSlot* pSlot = dynamic_cast<CSlot*>(m_pGameInstance->Chagne_Slot());
		if (true == m_pCreateButton->OnClick() && 0 == pSlot->Get_ItemID())
		{
			for (_uint i = 0; i < m_iNumNeeded; ++i)
			{
				m_pInventory->Use_Item(m_iMaterials[i], m_iNeededs[i]);
			}
			ITEM_DESC Desc = {};
			Desc.iItemID = Data.iItemID;
			Desc.eItemType = Data.eItemType;
			Desc.eSlot = Data.eSlot;
			Desc.fDurability = 100.f;
			Desc.iNumItem = 1;

			pSlot->Set_Info(Desc);
		}
	}
	else
		m_pCreateButton->Set_Button(0);

	
}
void CMaterialSlot::Late_Update(_float fTimeDelta)
{
}

HRESULT CMaterialSlot::Render()
{
	m_pCreateButton->Render();

	switch (m_iNumNeeded)
	{
	case 1:
		m_pTransform_Com->SetPosition(m_SlotPositions[0]);
		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
		m_pTexture_Com->Set_Texture(m_iFrameIndex[0]);
		m_pVIBuffer_Com->Render();
		m_pItemTexture_Com->Set_Texture(m_iMaterials[0]);
		m_pVIBuffer_Com->Render();

		m_pGameInstance->Render_Font(TEXT("Font_18"), (to_wstring(m_itemCount[0]) + L"/" + to_wstring(m_iNeededs[0])).c_str(), &m_rcFonts[0], m_TextColor[0]);
		break;

	case 2:
		m_pTransform_Com->SetPosition(_float3(m_SlotPositions[3]));
		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
		m_pTexture_Com->Set_Texture(m_iFrameIndex[0]);
		m_pVIBuffer_Com->Render();
		m_pItemTexture_Com->Set_Texture(m_iMaterials[0]);
		m_pVIBuffer_Com->Render();

		m_pTransform_Com->SetPosition(_float3(m_SlotPositions[4]));
		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
		m_pTexture_Com->Set_Texture(m_iFrameIndex[1]);
		m_pVIBuffer_Com->Render();
		m_pItemTexture_Com->Set_Texture(m_iMaterials[1]);
		m_pVIBuffer_Com->Render();

		m_pGameInstance->Render_Font(TEXT("Font_18"), (to_wstring(m_itemCount[0]) + L"/" + to_wstring(m_iNeededs[0])).c_str(), &m_rcFonts[3], m_TextColor[0]);
		m_pGameInstance->Render_Font(TEXT("Font_18"), (to_wstring(m_itemCount[1]) + L"/" + to_wstring(m_iNeededs[1])).c_str(), &m_rcFonts[4], m_TextColor[1]);
		break;

	case 3:
		m_pTransform_Com->SetPosition(_float3(m_SlotPositions[0]));
		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
		m_pTexture_Com->Set_Texture(m_iFrameIndex[0]);
		m_pVIBuffer_Com->Render();
		m_pItemTexture_Com->Set_Texture(m_iMaterials[0]);
		m_pVIBuffer_Com->Render();

		m_pTransform_Com->SetPosition(_float3(m_SlotPositions[1]));
		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
		m_pTexture_Com->Set_Texture(m_iFrameIndex[1]);
		m_pVIBuffer_Com->Render();
		m_pItemTexture_Com->Set_Texture(m_iMaterials[1]);
		m_pVIBuffer_Com->Render();

		m_pTransform_Com->SetPosition(_float3(m_SlotPositions[2]));
		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
		m_pTexture_Com->Set_Texture(m_iFrameIndex[2]);
		m_pVIBuffer_Com->Render();
		m_pItemTexture_Com->Set_Texture(m_iMaterials[2]);
		m_pVIBuffer_Com->Render();

		m_pGameInstance->Render_Font(TEXT("Font_18"), (to_wstring(m_itemCount[0]) + L"/" + to_wstring(m_iNeededs[0])).c_str(), &m_rcFonts[0], m_TextColor[0]);
		m_pGameInstance->Render_Font(TEXT("Font_18"), (to_wstring(m_itemCount[1]) + L"/" + to_wstring(m_iNeededs[1])).c_str(), &m_rcFonts[1], m_TextColor[1]);
		m_pGameInstance->Render_Font(TEXT("Font_18"), (to_wstring(m_itemCount[2]) + L"/" + to_wstring(m_iNeededs[2])).c_str(), &m_rcFonts[2], m_TextColor[2]);
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

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CMaterialSlot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMaterialSlot::Free()
{
	__super::Free();

	Safe_Release(m_pCreateButton);

	Safe_Release(m_pTexture_Com);
	Safe_Release(m_pItemTexture_Com);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBuffer_Com);
}
