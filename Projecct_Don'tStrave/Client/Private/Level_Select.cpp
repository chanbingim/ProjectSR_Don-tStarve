#include "Level_Select.h"

#include "GameInstance.h"

#include "Level_Loading.h"
#include "Camera.h"
#include "GamePlay_Button.h"


CLevel_Select::CLevel_Select(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID)
	: CLevel{ pGraphic_Device, ENUM_CLASS(eLevelID) }

{
	
}

HRESULT CLevel_Select::Initialize()
{
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;	

	if (FAILED(Ready_Layer_Button(TEXT("Layer_Button"))))
		return E_FAIL;

	m_pButton = dynamic_cast<CGamePlay_Button*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::SELECT), TEXT("Layer_Button")));

	return S_OK;
}

void CLevel_Select::Update(_float fTimeDelta)
{

	if (m_pButton->OnClick())
	{
		if (FAILED(m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pGraphic_Device, LEVEL::LOADING, LEVEL::GAMEPLAY))))
				return;
	}


}

HRESULT CLevel_Select::Render()
{

	return S_OK;
}



HRESULT CLevel_Select::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera::CAMERA_DESC			CameraDesc{};
	CameraDesc.fFov = D3DXToRadian(60.0f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.f;
	CameraDesc.vEye = _float3(0.f, 1.f, -1.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.m_MoveSpeedSec = 5.f;
	CameraDesc.m_TurnSpeedSec = D3DXToRadian(90.0f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera"),
		ENUM_CLASS(LEVEL::SELECT), strLayerTag, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Select::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	// Add Select
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::SELECT),
		TEXT("Prototype_GameObject_Select_Character"), EnumToInt(LEVEL::SELECT), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Select::Ready_Layer_Button(const _wstring& strLayerTag)
{
	CButton::BUTTON_DESC Desc = {};

	Desc.iTextureIndex = 0;
	Desc.fX = g_iWinSizeX * 0.5f;
	Desc.fY = g_iWinSizeY * 0.5f;
	Desc.fSizeX = 150.f;
	Desc.fSizeY = 50.f;
	Desc.fRelativeX = 100.f;
	Desc.fRelativeY = 300.f;
	

	// Add Select
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::SELECT),
		TEXT("Prototype_GameObject_GamePlay_Button"), EnumToInt(LEVEL::SELECT), strLayerTag, &Desc)))
		return E_FAIL;

	
	return S_OK;
}

CLevel_Select* CLevel_Select::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID)
{
	CLevel_Select* pInstance = new CLevel_Select(pGraphic_Device, eLevelID);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Select");
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CLevel_Select::Free()
{
	__super::Free();

	Safe_Release(m_pButton);
}
