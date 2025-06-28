#include "TutorialLevel.h"
#include "GameInstance.h"

#include "Level_Loading.h"
#include "Terrain.h"
#include "Camera.h"
#include "CUtility.h"

CTutorialLevel::CTutorialLevel(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID) :
	CLevel(pGraphic_Device, ENUM_CLASS(eLevelID))
{
}

HRESULT CTutorialLevel::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(TEXT("BackGroundLayer"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("CameraLayer"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Enviornment(TEXT("EnviornmenLayer"))))
		return E_FAIL;

	return S_OK;
}

void CTutorialLevel::Update(_float fTimeDelta)
{
	if (m_pGameInstance->KeyDown(VK_RETURN))
	{
		m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pGraphic_Device, LEVEL::LOADING, LEVEL::GAMEPLAY));
	}
}

HRESULT CTutorialLevel::Render()
{
	return S_OK;
}

HRESULT CTutorialLevel::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	Parse_ObejectData(  "../Bin/Resources/DataStruct/TutorialMapData/MapData.csv",
						ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_Terrain"),
						ENUM_CLASS(LEVEL::TUTORIAL), strLayerTag);

	return S_OK;
}

HRESULT CTutorialLevel::Ready_Layer_Camera(const _wstring& strLayerTag)
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
		ENUM_CLASS(LEVEL::TUTORIAL), strLayerTag, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTutorialLevel::Ready_Layer_Enviornment(const _wstring& strLayerTag)
{
	Parse_ObejectData("../Bin/Resources/DataStruct/TutorialMapData/Enviornment.csv",
						ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_Environment"),
						ENUM_CLASS(LEVEL::TUTORIAL), strLayerTag);

	return S_OK;
}

HRESULT CTutorialLevel::Ready_Layer_Player(const _wstring& strLayerTag)
{
	return S_OK;
}

CTutorialLevel* CTutorialLevel::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID)
{
	CTutorialLevel* pInstance = new CTutorialLevel(pGraphic_Device, eLevelID);
	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CREATE FAIL : TUTORIAL LEVEL");
	}

	return pInstance;
}

void CTutorialLevel::Free()
{
	__super::Free();
}
