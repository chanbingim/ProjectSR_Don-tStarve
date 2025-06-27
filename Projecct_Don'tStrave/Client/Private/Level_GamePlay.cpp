#include "Level_GamePlay.h"
#include "GameInstance.h"

#include "Camera.h"
#include "UserInterface.h"
#include "Button.h"

CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID)
	: CLevel { pGraphic_Device, ENUM_CLASS(eLevelID)}
	
{

}

HRESULT CLevel_GamePlay::Initialize()
{

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;*/

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Mouse(TEXT("Layer_Mouse"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UserInterface(TEXT("Layer_UserInterface"))))
		return E_FAIL;

	

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨이빈다"));
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;

}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera::CAMERA_DESC			CameraDesc{};
	CameraDesc.fFov = D3DXToRadian(60.0f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.f;
	CameraDesc.vEye = _float3(0.f, 10.f, -10.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 1.f);
	CameraDesc.m_MoveSpeedSec = 5.f;
	CameraDesc.m_TurnSpeedSec = D3DXToRadian(90.0f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _wstring& strLayerTag)
{
	for (size_t i = 0; i < 1; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player"),
			ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _wstring& strLayerTag)
{
	for (size_t i = 0; i < 2; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster"),
			ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Mouse(const _wstring& strLayerTag)
{
	CUserInterface::UIOBJECT_DESC Desc = {};

	// Add Mouse
	Desc.fSizeX = 50.f;
	Desc.fSizeY = 50.f;
	Desc.fX = g_iWinSizeX * 0.5f;
	Desc.fY = g_iWinSizeY * 0.5f;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY),
		TEXT("Prototype_GameObject_Mouse"), EnumToInt(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UserInterface(const _wstring& strLayerTag)
{
	CUserInterface::UIOBJECT_DESC Desc = {};

	// Add Inventory
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY),
		TEXT("Prototype_GameObject_Inventory"), EnumToInt(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	// Add Huger
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY),
		TEXT("Prototype_GameObject_Hunger"), EnumToInt(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	// Add Health
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY),
		TEXT("Prototype_GameObject_Health"), EnumToInt(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	// Add Sanity
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY),
		TEXT("Prototype_GameObject_Sanity"), EnumToInt(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;
	
	// Add Clock
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY),
		TEXT("Prototype_GameObject_Clock"), EnumToInt(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;
	
	// Add Silebar
	Desc.fSizeX = 350.f;
	Desc.fSizeY = 500.f;
	Desc.fX = Desc.fSizeX * 0.5f;
	Desc.fY = g_iWinSizeY * 0.5f;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY),
		TEXT("Prototype_GameObject_CraftingUI"), EnumToInt(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY),
		TEXT("Prototype_GameObject_MiniMap_Button"), EnumToInt(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}



CLevel_GamePlay* CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pGraphic_Device, eLevelID);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}





void CLevel_GamePlay::Free()
{
	__super::Free();

}
