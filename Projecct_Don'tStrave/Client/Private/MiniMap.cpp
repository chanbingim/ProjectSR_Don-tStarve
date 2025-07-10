#include "MiniMap.h"

#include "GameInstance.h"

#include "MiniMap_Button.h"
#include "MiniMap_Icon.h"
#include "Player.h"
#include "Enviornment_Object.h"
#include "Terrian_Manager.h"
#include "Terrain.h"

CMiniMap::CMiniMap(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUserInterface{ pGraphic_Device }
{
}

CMiniMap::CMiniMap(const CMiniMap& Prototype)
	: CUserInterface{ Prototype }
{
}

HRESULT CMiniMap::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMiniMap::Initialize(void* pArg)
{
	m_IsVisible = false;
	m_fZoom = 1.0f;
	CUserInterface::UIOBJECT_DESC Desc = {};

	Desc.fSizeX = 1280.f;
	Desc.fSizeY = 720.f;
	Desc.fX = m_Panel_Desc.fX = g_iWinSizeX * 0.5f;
	Desc.fY = m_Panel_Desc.fY = g_iWinSizeY * 0.5f;

	m_Panel_Desc.fSizeX = 1024.f;
	m_Panel_Desc.fSizeY = 576.f;
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	__super::UpdatePosition();

	m_pMiniMap_Btn = dynamic_cast<CMiniMap_Button*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_UserInterface"), 6));

	//Update_Objects();

	m_pTerrains = CTerrian_Manager::GetInstance()->GetTerrains();

	return S_OK;
}

void CMiniMap::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->KeyDown(VK_ESCAPE) && true == m_IsVisible)
	{
		m_IsVisible = false;
		m_fZoom = 1.f;
		

		for (auto pIcon : m_Icons)
		{
			/*pIcon->Update_Position(m_pPanel_Transform_Com);
			pIcon->Update_Scale(m_fZoom, m_pPanel_Transform_Com);*/
			Safe_Release(pIcon);
		}
		m_Icons.clear();
		
	}
	
}

void CMiniMap::Update(_float fTimeDelta)
{
	if (true == m_pMiniMap_Btn->OnClick() && !m_IsVisible)
	{
		m_IsVisible = true;
		Update_Objects();
	}

	
	if(true == m_IsVisible)
	{
		if (g_Wheel > 0 && 2.2f >= m_fZoom)
		{
			m_fZoom += 0.1f;
			g_Wheel = 0;
			m_pPanel_Transform_Com->SetScale(_float3(m_Panel_Desc.fSizeX * m_fZoom, m_Panel_Desc.fSizeY * m_fZoom, 1.f));
			m_pTerrain_Transform_Com->SetScale(_float3(m_Panel_Desc.fSizeX / 192.f * 2.f * m_fZoom, 1.f, m_Panel_Desc.fSizeY / 192.f * 2.f * m_fZoom));
			for (auto pIcon : m_Icons)
			{
				pIcon->Update_Scale(m_fZoom, m_pPanel_Transform_Com);
			}
		}
		if (g_Wheel < 0 && 0.5f <= m_fZoom)
		{
			m_fZoom -= 0.1f;
			g_Wheel = 0;
			m_pPanel_Transform_Com->SetScale(_float3(m_Panel_Desc.fSizeX * m_fZoom, m_Panel_Desc.fSizeY * m_fZoom, 1.f));
			m_pTerrain_Transform_Com->SetScale(_float3(m_Panel_Desc.fSizeX / 192.f * 2.f * m_fZoom, 1.f, m_Panel_Desc.fSizeY / 192.f * 2.f * m_fZoom));
			for (auto pIcon : m_Icons)
			{
				pIcon->Update_Scale(m_fZoom, m_pPanel_Transform_Com);
			}
		}

		if (m_pGameInstance->KeyDown(VK_LBUTTON))
		{
			m_vKeyDownPos = m_pGameInstance->GetMousePosition(0);
		}

		else if (m_pGameInstance->KeyPressed(VK_LBUTTON))
		{
			_float3 vCurPoint = m_pGameInstance->GetMousePosition(0);
			m_pPanel_Transform_Com->SetPosition(_float3((m_fX + vCurPoint.x - m_vKeyDownPos.x)-m_fWinSizeX * 0.5f, -(m_fY - (m_vKeyDownPos.y- vCurPoint.y)) + m_fWinSizeY * 0.5f, 0.f));
			for (auto pIcon : m_Icons)
			{
				pIcon->Update_Position(m_pPanel_Transform_Com);
			}
		}
		if (m_pGameInstance->KeyUp(VK_LBUTTON))
		{
			_float3 vCurPoint = m_pGameInstance->GetMousePosition(0);

			m_fX = m_fX + vCurPoint.x - m_vKeyDownPos.x;
			m_fY = m_fY - (m_vKeyDownPos.y - vCurPoint.y);
			m_pPanel_Transform_Com->SetPosition(_float3(m_fX - m_fWinSizeX * 0.5f, -m_fY + m_fWinSizeY * 0.5f, 0.f));
			for (auto pIcon : m_Icons)
			{
				pIcon->Update_Position(m_pPanel_Transform_Com);
			}
		}

	}

}

void CMiniMap::Late_Update(_float fTimeDelta)
{
	if (true == m_IsVisible)
	{
		_float3 vPos = m_pPanel_Transform_Com->GetWorldState(WORLDSTATE::POSITION);
		_float3 vScale = m_pPanel_Transform_Com->GetScale();

		m_pTerrain_Transform_Com->SetPosition(_float3(vPos.x - vScale.x * 0.5f, vPos.y - vScale.y * 0.5f, vPos.z));

		m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);
	}
}

HRESULT CMiniMap::Render()
{
	m_pTexture_Com->Set_Texture(0);

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());

	m_pVIBuffer_Com->Render();

	m_Terrain_Texture_Com->Set_Texture(0);
	/*m_pTexture_Com->Set_Texture(1,1);

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);*/



	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTerrain_Transform_Com->Get_World());

	for (auto pTerrain : *m_pTerrains)
	{
		if(nullptr != pTerrain)
			pTerrain->GetCurVIBuffer()->Render();
	}
	m_pPanel_VIBuffer_Com->Render();
	
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pPanel_Transform_Com->Get_World());
	
	for (auto pIcon : m_Icons)
	{
		pIcon->Render();
	}

	return S_OK;
}

HRESULT CMiniMap::ADD_Components()
{
	// Texture Component
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_MiniMap_Background"),
		TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTexture_Com))))
		return E_FAIL;
	
	// Texture Component
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_Component_Texture_Terrain"),
		TEXT("Com_Texture_Terrain"),
		reinterpret_cast<CComponent**>(&m_Terrain_Texture_Com))))
		return E_FAIL;


	// VIBuffer_Rect Component
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"),
		reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
		return E_FAIL;

	Engine::CTransform::TRANSFORM_DESC Transform_Desc = { 5.f, D3DXToRadian(90.f) };

	// Transform Component
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"),
		reinterpret_cast<CComponent**>(&m_pTransform_Com), &Transform_Desc)))
		return E_FAIL;
	
	// Transform Component
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform_Terrain"),
		reinterpret_cast<CComponent**>(&m_pTerrain_Transform_Com), &Transform_Desc)))
		return E_FAIL;

	// VIBuffer_Rect Component
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_Panel_VIBuffer"),
		reinterpret_cast<CComponent**>(&m_pPanel_VIBuffer_Com))))
		return E_FAIL;


	// Transform Component
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Panel_Transform"),
		reinterpret_cast<CComponent**>(&m_pPanel_Transform_Com), &Transform_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMiniMap::Update_Objects()
{
	m_Panel_Desc.fX = g_iWinSizeX * 0.5f;
	m_Panel_Desc.fY = g_iWinSizeY * 0.5f;
	m_Panel_Desc.fSizeX = 1024.f;
	m_Panel_Desc.fSizeY = 576.f;

	CMiniMap_Icon::ICON_DESC Icon_Desc = {};

	Icon_Desc.fX = 0.f;
	Icon_Desc.fY = 0.f;
	Icon_Desc.fSizeX = 30.f;
	Icon_Desc.fSizeY = 30.f;
	Icon_Desc.fParentCX = m_Panel_Desc.fSizeX;
	Icon_Desc.fParentCY = m_Panel_Desc.fSizeY;

	// Player
	CGameObject* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Player")));

	if (nullptr == pPlayer)
		return E_FAIL;

	_float3 vPos = pPlayer->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION);

	Icon_Desc.iTextureNum = 0;
	Icon_Desc.fRX = vPos.x / 192.f - 0.5f;
	Icon_Desc.fRY = -(1.f - (vPos.z / 192.f)) + 0.5f;

	m_Icons.push_back(dynamic_cast<CMiniMap_Icon*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MiniMap_Icon"), &Icon_Desc)));


	// EnveiObject
	list<CGameObject*>* pEnviornmenList = m_pGameInstance->GetAllObejctsToLayer(EnumToInt(LEVEL::GAMEPLAY), TEXT("EnviornmenLayer"));

	for (auto pObject : *pEnviornmenList)
	{
		CEnviornment_Object* pGrass = dynamic_cast<CEnviornment_Object*>(pObject);

		if (nullptr == pGrass)
			continue;
		_float3 vPos = pGrass->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION);

		switch (pGrass->GetEnviormentID())
		{
		case 1:
			Icon_Desc.iTextureNum = 4;
			break;

		case 2:
			Icon_Desc.iTextureNum = 2;
			break;

		case 3:
			Icon_Desc.iTextureNum = 1;
			break;

		case 4:
			Icon_Desc.iTextureNum = 3;
			break;

		case 5:
			Icon_Desc.iTextureNum = 7;
			break;

		case 6:
			Icon_Desc.iTextureNum = 13;
			break;

		default:
			break;
		}

		Icon_Desc.fRX = vPos.x / 192.f - 0.5f;
		Icon_Desc.fRY = -(1.f - (vPos.z / 192.f)) + 0.5f;

		m_Icons.push_back(dynamic_cast<CMiniMap_Icon*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MiniMap_Icon"), &Icon_Desc)));
	}





	m_pPanel_Transform_Com->SetScale(_float3(m_Panel_Desc.fSizeX * m_fZoom, m_Panel_Desc.fSizeY * m_fZoom, 1.f));
	m_pPanel_Transform_Com->SetPosition(_float3(0.f, 0.f, 0.f));

	m_pTerrain_Transform_Com->SetScale(_float3(m_Panel_Desc.fSizeX / 192.f * 2.f, 1.f, m_Panel_Desc.fSizeY / 192.f * 2.f));
	m_pTerrain_Transform_Com->SetRotAxis(_float3(1.f, 0.f, 0.f), D3DXToRadian(270.f));

	for (auto pIcon : m_Icons)
	{
		pIcon->Update_Position(m_pPanel_Transform_Com);
		pIcon->Update_Scale(m_fZoom, m_pPanel_Transform_Com);
	}
}


CMiniMap* CMiniMap::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMiniMap* pInstance = new CMiniMap(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMiniMap");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMiniMap::Clone(void* pArg)
{
	CGameObject* pInstance = new CMiniMap(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CMiniMap");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMiniMap::Free()
{
	__super::Free();

	for (auto pIcon : m_Icons)
		Safe_Release(pIcon);
	m_Icons.clear();

	Safe_Release(m_pTexture_Com);
	Safe_Release(m_pTransform_Com);
	Safe_Release(m_pVIBuffer_Com);

	Safe_Release(m_pPanel_Transform_Com);
	Safe_Release(m_pPanel_VIBuffer_Com);

	Safe_Release(m_Terrain_Texture_Com);
	Safe_Release(m_pTerrain_Transform_Com);

}
