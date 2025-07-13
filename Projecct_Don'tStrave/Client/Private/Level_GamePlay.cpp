#include "Level_GamePlay.h"
#include "GameInstance.h"

#include "Camera.h"
#include "UserInterface.h"
#include "SnowParticle.h"
#include "Terrian_Manager.h"
#include "Terrain.h"
#include "CUtility.h"
#include "Player.h"

#include "QuestManager.h"
#include "PlayerData_Manager.h"
#include "MonsterData_Manager.h"
#include "CharacterManager.h"

CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID)
	: CLevel { pGraphic_Device, ENUM_CLASS(eLevelID)}
{

}

HRESULT CLevel_GamePlay::Initialize()
{
	m_pGameInstance->Manager_PlaySound(L"Filed.mp3", CHANNELID::SOUND_BGM, 10.0f);

	if (FAILED(LoadFileData("TutorialMapData")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Mouse(TEXT("Layer_Mouse"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UserInterface(TEXT("Layer_UserInterface"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Particle(TEXT("Layer_Particle"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Item(TEXT("Layer_Item"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Initialize_Late(ENUM_CLASS(LEVEL::GAMEPLAY))))
		return E_FAIL;

	if (FAILED(CQuestManager::GetInstance()->LoadQuestData("TutorialMapData/Quest", "TutorialQuest.csv")))
		return E_FAIL;
	m_pCharacterManager = CCharacterManager::GetInstance();

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	m_pCharacterManager->Update();
}

HRESULT CLevel_GamePlay::Render()
{
	
	return S_OK;
}

HRESULT CLevel_GamePlay::LoadFileData(const char* MapName)
{
	char		FilePath[MAX_PATH] = {};
	sprintf_s(FilePath, "../Bin/Resources/DataStruct/%s", MapName);

	if (FAILED(Ready_Layer_BackGround(FilePath, TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(FilePath, TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(FilePath, TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Enviornment(FilePath, TEXT("EnviornmenLayer"))))
		return E_FAIL;

	return S_OK;
}


HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const char* FilePath, const _wstring& strLayerTag)
{
	char		File[MAX_PATH] = {};
	sprintf_s(File, "%s/MapData.csv", FilePath);

	Parse_ObejectData(File,
		ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_Terrain"),
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"));

	auto GroundObejcts = m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"));
	if (nullptr == GroundObejcts)
		return E_FAIL;

	for (auto iter : *GroundObejcts)
	{
		auto Terrian = dynamic_cast<CTerrain *>(iter);
		if(Terrian)
		CTerrian_Manager::GetInstance()->ADD_Terrian(Terrian);
	}

	return S_OK;

}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const char* FilePath, const _wstring& strLayerTag)
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
	auto EnvAll = m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("EnviornmenLayer"));
	CGameObject* potal = nullptr;
	for (auto iter : *EnvAll)
	{
		if (1 == iter->Get_ObjectID())
		{
			potal = iter;
			break;
		}
	}

	PLAYER_DESC data = CPlayerData_Manager::GetInstance()->Get_PlayerData(200);
	data.fPos = potal->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_Player"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &data)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const char* FilePath, const _wstring& strLayerTag)
{

	char		File[MAX_PATH] = {};
	sprintf_s(File, "%s/TutorialMonster.csv", FilePath);

	//¸Ê µ¥ÀÌÅÍ °¡Á®¿Í¼­ ÆÄ½Ì
	vector<BASE_DATA_STRUCT> vecBaseData;
	LoadMapData(File, &vecBaseData);

	_uint iPrototypeLevelIndex = ENUM_CLASS(LEVEL::GAMEPLAY_STATIC);
	_uint iLayerLevelIndex = ENUM_CLASS(LEVEL::GAMEPLAY);

	for (size_t i = 0; i < vecBaseData.size(); ++i)
	{
		CGameObject::GAMEOBJECT_DESC ObjectDesc = {};
		WCHAR TexPath[MAX_PATH] = {};

		auto data = CMonsterData_Manager::GetInstance()->Get_MonsterData(vecBaseData[i].iID);
		data.fPos = vecBaseData[i].Position;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), data.strPath,
			ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &data)))
			return E_FAIL;

		/*auto data = CMonsterData_Manager::GetInstance()->Get_MonsterData(103);
		data.fPos = vecBaseData[i].Position;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), data.strPath,
			ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &data)))
			return E_FAIL;*/


		break;
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Enviornment(const char* FilePath, const _wstring& strLayerTag)
{
	char		File[MAX_PATH] = {};
	sprintf_s(File, "%s/Enviornment.csv", FilePath);

	//¸Ê µ¥ÀÌÅÍ °¡Á®¿Í¼­ ÆÄ½Ì
	vector<BASE_DATA_STRUCT> vecBaseData;
	LoadMapData(File, &vecBaseData);

	_uint iPrototypeLevelIndex = ENUM_CLASS(LEVEL::GAMEPLAY_STATIC);
	_uint iLayerLevelIndex = ENUM_CLASS(LEVEL::GAMEPLAY);

	for (size_t i = 0; i < vecBaseData.size(); ++i)
	{
		CGameObject::GAMEOBJECT_DESC ObjectDesc = {};
		WCHAR TexPath[MAX_PATH] = {};

		ObjectDesc.vScale = vecBaseData[i].Scale;
		ObjectDesc.vRotation = vecBaseData[i].Rotation;
		ObjectDesc.vPosition = vecBaseData[i].Position;

		CUtility::ConvertUTFToWide(vecBaseData[i].szTexturePath.c_str(), TexPath);
		ObjectDesc.TextruePath = TexPath;
		ObjectDesc.iObjectID = vecBaseData[i].iID;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(iPrototypeLevelIndex, GetEnv_ObejctTag(vecBaseData[i].iID), iLayerLevelIndex, strLayerTag, &ObjectDesc)))
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

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY),
		TEXT("Prototype_GameObject_MiniMap"), EnumToInt(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY),
		TEXT("Prototype_GameObject_DamageUI"), EnumToInt(LEVEL::GAMEPLAY), TEXT("Gameplay_Screen_Effect"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY),
		TEXT("Prototype_GameObject_QuestUI"), EnumToInt(LEVEL::GAMEPLAY), TEXT("Gameplay_Quest_UI"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Particle(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY),
		TEXT("Prototype_GameObject_Snow_Particle"), EnumToInt(LEVEL::GAMEPLAY), TEXT("Gameplay_Screen_Effect"))))
		return E_FAIL;

	

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Item(const _wstring& strLayerTag)
{
	_float3 ItemPos[10] =
	{
		{12.7f, 0.f, 18.4f},
		{9.1f, 0.f, 11.9f},
		{19.8f, 0.f, 8.3f},
		{13.5f, 0.f, 20.0f},
		{8.0f, 0.f, 14.2f},
		{17.4f, 0.f, 16.1f},
		{10.2f, 0.f, 9.6f},
		{15.9f, 0.f, 12.5f},
		{18.6f, 0.f, 19.1f},
		{11.3f, 0.f, 8.8f}
	};

	ITEM_DESC Desc = {};

	Desc.iItemID = 36;
	Desc.eItemType = ITEM_TYPE::MERTARIAL;
	Desc.fDurability = 100.f;
	Desc.eSlot = SLOT::NORMAL;
	Desc.iNumItem = 1;

	for (_uint i = 0; i < 10; ++i)
	{
		Desc.vPosition = ItemPos[i];

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(EnumToInt(LEVEL::GAMEPLAY),
			TEXT("Prototype_GameObject_Material_Item"), EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Item"), &Desc)))
			return E_FAIL;
	}

	
	

	return S_OK;
}

_wstring CLevel_GamePlay::GetEnv_ObejctTag(_uint iID)
{
	switch (iID)
	{
	case 1:
		return TEXT("Prototype_GameObject_Env_Protal");
	case 2:
		return TEXT("Prototype_GameObject_Env_Grass");
	case 3:
		return TEXT("Prototype_GameObject_Env_Rock");
	case 4:
		return TEXT("Prototype_GameObject_Env_Tree");
	case 5:
		return TEXT("Prototype_GameObject_Gloden_Rock");
	case 6:
		return TEXT("Prototype_GameObject_Resurrection_Stone");
	case 7:
		return TEXT("Prototype_GameObject_Berry_Bush");
	case 8:
		return TEXT("Prototype_GameObject_Env_Tree");
	case 9:
		return TEXT("Prototype_GameObject_Birchnut_Tree");
	case 12:
		return TEXT("Prototype_GameObject_PigKing");
	}

	return TEXT("Prototype_GameObject_Env_Tree");
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

	CQuestManager::GetInstance()->ReleaseQuestData();
	CCharacterManager::DestroyInstance();
}
