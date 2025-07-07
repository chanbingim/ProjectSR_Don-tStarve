#include "Loader.h"

#include "Terrain.h"
#include "Player.h"
#include "SpiderNormal.h"
#include "SpiderWarrior.h"
#include "SpiderHouse.h"
#include "SpiderQueen.h"
#include "Pig.h"

#pragma region UI
#include "Slot.h"
#include "SlotFrame.h"
#include "Inventory.h"
#include "Mouse.h"

#pragma region BUTTON_UI
#include "Crafting_Button.h"
#include "Camera_Button.h"
#include "MiniMap_Button.h"
#include "QuickSlot_Button.h"
#include "Category_Button.h"
#include "BookMark_Button.h"
#include "Create_Button.h"
#include "Item_Button.h"
#pragma endregion

#pragma region ITEM
#include "Food.h"
#include "ResearchLap.h"
#include "Equipment.h"
#include "CampFire.h"
#include "DropItemComponent.h"
#pragma endregion

#include "MiniMap.h"
#include "MiniMap_Icon.h"

#include "CraftingUI.h"
#include "Hunger.h"
#include "Health.h"
#include "Sanity.h"

#include "Clock.h"
#include "Item_Info.h"
#include "MaterialSlot.h"

#include "Material_Item.h"
#include "Fire.h"
#include "Chest.h"
#include "UIEffect.h"
#include "ChestUI.h"
#pragma endregion

#include "SnowParticle.h"
#include "DamageEffectUI.h"

#pragma region ENVIORN_MENT
#include "GrassObject.h"
#include "RockObject.h"
#include "PortalObject.h"
#include "ResurrectionStone.h"
#include "TreeObject.h"
#pragma endregion

#include "GameInstance.h"
#include "Item_Manager.h"
#include "PlayerData_Manager.h"
#include "MonsterData_Manager.h"

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device { pGraphic_Device }
	, m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
}

unsigned int APIENTRY LoadingMain(void* pArg)
{
	CLoader*		pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{	
	m_eNextLevelID = eNextLevelID;

	/* 세마포어, 뮤텍스, 크리티컬섹션 */

	/* 임계영역(힙, 데이터, 코드)에 접근하기위한 키를 생성한다. */
	InitializeCriticalSection(&m_CriticalSection);

	/* 실제 로딩을 수행하기위한 스레드를 생성한다. */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	HRESULT		hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL::LOGO:
		hr = Loading_For_Logo();
		break;
	case LEVEL::GAMEPLAY:
		hr = Loading_For_GamePlay();
		break;

	}

	LeaveCriticalSection(&m_CriticalSection);

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

void CLoader::Output()
{
	SetWindowText(g_hWnd, m_strMessage.c_str());
}

HRESULT CLoader::Loading_For_Logo()
{
	m_strMessage = TEXT("텍스쳐를(을) 로딩 중 입니다.");
	
	m_strMessage = TEXT("모델를(을) 로딩 중 입니다.");

	m_strMessage = TEXT("셰이더를(을) 로딩 중 입니다.");
	
	m_strMessage = TEXT("객체원형를(을) 로딩 중 입니다.");

	m_strMessage = TEXT("로딩이 완료되었습니다..");

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay()
{
	m_strMessage = TEXT("캐릭터 정보를 가져오는 중... ");
	CPlayerData_Manager::GetInstance()->LoadPlayerData("../Bin/Resources/DataStruct/Character/CharacterData.csv");

	m_strMessage = TEXT("몬스터 정보를 가져오는 중... ");
	CMonsterData_Manager::GetInstance()->LoadMonsterData("../Bin/Resources/DataStruct/Monster/MonsterData.csv");

	m_strMessage = TEXT("아이템 정보를 가져오는 중... ");
	CItem_Manager::GetInstance()->LoadItemData("../Bin/Resources/DataStruct/Item/ItemData.csv");

	m_strMessage = TEXT("텍스쳐를(을) 로딩 중 입니다.");
#pragma region TEXTURE
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Ground/tile%d.png"), 2))))
		return E_FAIL;
#pragma endregion

#pragma region COMPONENT
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_DropItem"),
		CDropItemComponent::Create(m_pGraphic_Device))))
		return E_FAIL;
#pragma endregion

#pragma endregion

#pragma region UI
	/* For.Prototype_Component_Texture_Hunger */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Hunger"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/hunger/Hunger_0%d.png"), 51))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Health */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Health"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Health/Health_0%d.png"), 51))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sanity */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Sanity"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Sanity/Sanity_0%d.png"), 51))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Arrow */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Arrow"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Sanity/arrow%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Inventory */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Inventory"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Inventory/MainInventory.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_SideBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_SideBar"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/SideBar/Frame.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Slot */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Slot"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Inventory/SlotFrame%d.png"), 7))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ItemIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Item"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Itemicon/Item%d.png"), 52))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ItemObject */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_ItemObject"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Item/Item%d.png"), 52, true))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ItemState */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_ItemState"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Slot_State/state-%d.png"), 50))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Number */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Number"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/FontTexture/num-%d.png"), 12))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Scale */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Scale"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Button/Alpha.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Mouse */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Mouse"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Mouse/Mouse%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_CraftBar_Button */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_CraftBar_Button"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Button/CraftBar_Button%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_MiniMap_Button */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_MiniMap_Button"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Button/MinMap_Button.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_MiniMap_Background */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_MiniMap_Background"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/MiniMap/MiniMap_bg.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_MiniMap_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_MiniMap_Icon"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/MiniMap_Icon/MiniMap_Icon-%d.png"), 14))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_QuickSlot_Button */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_QuickSlot_Button"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Button/QuickSlot_Button%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Create_Button */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Create_Button"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Button/CreateButton%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Bookmark_Button */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Bookmark_Button"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Button/Bookmark%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Clock_Frame */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Clock_Frame"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Clock/Clock_Frame.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Clock_Point */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Clock_Point"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Clock/Clock_Point.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Category */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Category"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Category/Category_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ItemFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_ItemFrame"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Button/ItemFrame_%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_CampFire_Fire */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_CampFire_Fire"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Item/CampFire_Fire/fire__00%d.png"), 10))))
		return E_FAIL;


#pragma region Chest

	/* For.Prototype_Component_Texture_ChestUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_ChestUI"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/ChestUI/frame__0%d.png"), 11))))
		return E_FAIL;

#pragma endregion

#pragma region DAMAGE UI
	/* For.Prototype_Component_Texture_DAMAGE_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_DamageUI"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/ScreenEffect/screeneffect0.png"), 1))))
		return E_FAIL;
#pragma endregion



#pragma endregion
	m_strMessage = TEXT("모델를(을) 로딩 중 입니다.");

	m_strMessage = TEXT("셰이더를(을) 로딩 중 입니다.");

	m_strMessage = TEXT("객체원형를(을) 로딩 중 입니다.");

#pragma region GAMEPLAY
	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_Spider"),
		CSpiderNormal::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_SpiderWarrior"),
		CSpiderWarrior::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_SpiderHouse"),
		CSpiderHouse::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_SpiderQueen"),
		CSpiderQueen::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_Pig"),
		CPig::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, g_iTileCnt, g_iTileCnt))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;
#pragma endregion

#pragma region ENVIORN_MENT
	/* For.Prototype_GameObject_Grass */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_Env_Grass"),
		CGrassObject::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Portal */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_Env_Protal"),
		CPortalObject::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Rock */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_Env_Rock"),
		CRockObject::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tree */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_Env_Tree"),
		CTreeObject::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_CResurrectionStone */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_Resurrection_Stone"),
		CResurrectionStone::Create(m_pGraphic_Device))))
		return E_FAIL;
#pragma endregion

#pragma region UI_OBJECT
	/* For.Prototype_GameObject_Hunger */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Hunger"),
		CHunger::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Health */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Health"),
		CHealth::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sanity */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Sanity"),
		CSanity::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Slot */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Slot"),
		CSlot::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_SlotFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SlotFrame"),
		CSlotFrame::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Inventory */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inventory"),
		CInventory::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_CraftingUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_CraftingUI"),
		CCraftingUI::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_ChestUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ChestUI"),
		CChestUI::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Crafting_Button */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Crafting_Button"),
		CCrafting_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Crafting_Button */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MiniMap_Button"),
		CMiniMap_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_QuickSlot_Button */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_QuickSlot_Button"),
		CQuickSlot_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Category_Button */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Category_Button"),
		CCategory_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Item_Button */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Item_Button"),
		CItem_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_CCreate_Button */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Create_Button"),
		CCreate_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_BookMark_Button */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_BookMark_Button"),
		CBookMark_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Clock */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Clock"),
		CClock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Item_Info */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Item_Info"),
		CItem_Info::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_MaterialSlot */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MaterialSlot"),
		CMaterialSlot::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Mouse */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Mouse"),
		CMouse::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_UIEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UIEffect"),
		CUIEffect::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_MiniMap */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MiniMap"),
		CMiniMap::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_MiniMap_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MiniMap_Icon"),
		CMiniMap_Icon::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_DamageEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_DamageUI"),
		CDamageEffectUI::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion

#pragma region SNOW_PARITCLE
	/* For.Prototype_GameObject_SnowParticle */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Snow_Particle"),
		CSnowParticle::Create(m_pGraphic_Device))))
		return E_FAIL;
#pragma endregion

#pragma region ITEM
	/* For.Prototype_GameObject_CamFire */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Material_Item"),
		CMaterial_Item::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_Food */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Food"),
		CFood::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Equipment */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Equipment"),
		CEquipment::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_CamFire */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_CamFire"),
		CCampFire::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Fire */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Fire"),
		CFire::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_CamFire */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Chest"),
		CChest::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_ResearchLap */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ResearchLap"),
		CResearchLap::Create(m_pGraphic_Device))))
		return E_FAIL;

	
#pragma endregion

	m_strMessage = TEXT("폰트를(을) 로딩 중 입니다.");

#pragma region FONT

#pragma endregion

	m_strMessage = TEXT("로딩이 완료되었습니다..");

	m_isFinished = true;

	return S_OK;
}

CLoader* CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID)
{
	CLoader* pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
}
