#include "TutorialLevel.h"
#include "GameInstance.h"

#include "Level_Loading.h"
#include "UserInterface.h"
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

	if (FAILED(Ready_Layer_Player(TEXT("PlayerLayer"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("MonsterLayer"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UserInterface(TEXT("Layer_UserInterface"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Initialize_Late(ENUM_CLASS(LEVEL::TUTORIAL))))
		return E_FAIL;
	return S_OK;
}

HRESULT CTutorialLevel::Ready_Layer_Player(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_Player"),
		ENUM_CLASS(LEVEL::TUTORIAL), strLayerTag)))
		return E_FAIL;
	return S_OK;
}

HRESULT CTutorialLevel::Ready_Layer_Monster(const _wstring& strLayerTag)
{
	for (size_t i = 0; i < 10; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_Spider"),
			ENUM_CLASS(LEVEL::TUTORIAL), strLayerTag)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CTutorialLevel::Ready_Layer_UserInterface(const _wstring& strLayerTag)
{
	CUserInterface::UIOBJECT_DESC Desc = {};

	// Add Inventory
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::OBJECT),
		TEXT("Prototype_GameObject_Inventory"), EnumToInt(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	// Add Huger
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::OBJECT),
		TEXT("Prototype_GameObject_Hunger"), EnumToInt(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	// Add Health
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::OBJECT),
		TEXT("Prototype_GameObject_Health"), EnumToInt(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	// Add Sanity
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::OBJECT),
		TEXT("Prototype_GameObject_Sanity"), EnumToInt(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	// Add Clock
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::OBJECT),
		TEXT("Prototype_GameObject_Clock"), EnumToInt(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	// Add Silebar
	Desc.fSizeX = 350.f;
	Desc.fSizeY = 500.f;
	Desc.fX = Desc.fSizeX * 0.5f;
	Desc.fY = g_iWinSizeY * 0.5f;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::OBJECT),
		TEXT("Prototype_GameObject_CraftingUI"), EnumToInt(LEVEL::GAMEPLAY), strLayerTag, &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::OBJECT),
		TEXT("Prototype_GameObject_MiniMap_Button"), EnumToInt(LEVEL::GAMEPLAY), strLayerTag)))
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

_wstring CTutorialLevel::GetEnv_ObejctTag(_uint iID)
{
	switch (iID)
	{
	case 1 :
		return TEXT("Prototype_GameObject_Env_Protal");
	case 2:
		return TEXT("Prototype_GameObject_Env_Grass");
	case 3:
		return TEXT("Prototype_GameObject_Env_Rock");
	case 4:
		return TEXT("Prototype_GameObject_Env_Tree");
	case 5:
		return TEXT("");
	}

	return TEXT("");
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
	//¸Ê µ¥ÀÌÅÍ °¡Á®¿Í¼­ ÆÄ½Ì
	vector<BASE_DATA_STRUCT> vecBaseData;
	LoadMapData("../Bin/Resources/DataStruct/TutorialMapData/Enviornment.csv", &vecBaseData);

	_uint iPrototypeLevelIndex = ENUM_CLASS(LEVEL::GAMEPLAY_STATIC);
	_uint iLayerLevelIndex = ENUM_CLASS(LEVEL::TUTORIAL);

	for (size_t i = 0; i < vecBaseData.size(); ++i)
	{
		CGameObject::GAMEOBJECT_DESC ObjectDesc = {};
		WCHAR TexPath[MAX_PATH] = {};

		ObjectDesc.vScale = vecBaseData[i].Scale;
		ObjectDesc.vRotation = vecBaseData[i].Rotation;
		ObjectDesc.vPosition = vecBaseData[i].Position;

		CUtility::ConvertUTFToWide(vecBaseData[i].szTexturePath.c_str(), TexPath);
		ObjectDesc.TextruePath = TexPath;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(iPrototypeLevelIndex, GetEnv_ObejctTag(vecBaseData[i].iID), iLayerLevelIndex, strLayerTag, &ObjectDesc)))
			return E_FAIL;
	}
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
