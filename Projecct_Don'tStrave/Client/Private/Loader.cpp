#include "Loader.h"

#include "Terrain.h"
#include "Player.h"
#include "Spider.h"
#include "Slot.h"
#include "SlotFrame.h"
#include "Inventory.h"
#include "Mouse.h"
#include "CraftingUI.h"
#include "Hunger.h"
#include "Health.h"
#include "Sanity.h"
#include "Item.h"
#include "Crafting_Button.h"
#include "Camera_Button.h"
#include "MiniMap_Button.h"
#include "QuickSlot_Button.h"
#include "Category_Button.h"
#include "Clock.h"
#include "Item_Info.h"
#include "Item_Button.h"


#include "GameInstance.h"

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

	case LEVEL::TUTORIAL :
		hr = Loading_For_Tutorial();
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
	m_strMessage = TEXT("텍스쳐를(을) 로딩 중 입니다.");
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Terrain/Tile0.jpg"), 1))))
		return E_FAIL;

#pragma region Player



	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Idle_Loop_Down"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/NoItem/idle_loop_down")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Idle_Loop_Side"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/NoItem/idle_loop_side")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Idle_Loop_Up"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/NoItem/idle_loop_up")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Loop_Down"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/NoItem/run_loop_down")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Loop_Side"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/NoItem/run_loop_side")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Loop_Up"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/NoItem/run_loop_up")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pre_Down"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/NoItem/run_pre_down")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pre_Side"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/NoItem/run_pre_side")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pre_Up"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/NoItem/run_pre_up")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pst_Down"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/NoItem/run_pst_down")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pst_Side"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/NoItem/run_pst_side")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Run_Pst_Up"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/NoItem/run_pst_up")))))
		return E_FAIL;
	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Death1"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/NoItem/death")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Death2"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/NoItem/death2")))))
		return E_FAIL;


	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Idle_Loop_Down"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/Item/idle_loop_down")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Idle_Loop_Side"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/Item/idle_loop_side")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Idle_Loop_Up"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/Item/idle_loop_up")))))
		return E_FAIL;
	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Loop_Down"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/Item/run_loop_down")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Loop_Side"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/Item/run_loop_side")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Loop_Up"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/Item/run_loop_up")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Pre_Down"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/Item/run_pre_down")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Pre_Side"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/Item/run_pre_side")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Pre_Up"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/Item/run_pre_up")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Pst_Down"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/Item/run_pst_down")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Pst_Side"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/Item/run_pst_side")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Run_Pst_Up"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/Item/run_pst_up")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Death1"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/Item/death")))))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player_Item_Death2"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Wilson/Item/death2")))))
		return E_FAIL;

#pragma endregion

	/* For.Prototype_Component_Texture_Player */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Player"),
	//	CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Player/Player0.png"), 1))))
	//	return E_FAIL;

	/* For.Prototype_Component_Texture_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Monster"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Monster/Ma.jpg"), 1))))
		return E_FAIL;

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

	/* For.Prototype_Component_Texture_Inventory */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Inventory"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Inventory/MainInventory.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_SideBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_SideBar"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/SideBar/Frame.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Slot */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY),TEXT("Prototype_Component_Texture_Slot"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Inventory/SlotFrame%d.png"), 5))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Item */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Item"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Item/Item%d.png"), 36))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ItemState */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_ItemState"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Slot_State/state-%d.png"), 50))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Number */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Number"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/FontTexture/num-%d.png"), 12))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_CraftBar_Button */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_CraftBar_Button"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Button/CraftBar_Button%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_MiniMap_Button */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_MiniMap_Button"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Button/MinMap_Button.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_QuickSlot_Button */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_QuickSlot_Button"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Button/QuickSlot_Button.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Clock_Frame */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Clock_Frame"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Clock/Clock_Frame.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Clock_Point */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Clock_Point"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Clock/Clock_Point.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Clock_Point */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Category"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Category/Category_%d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ItemFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_ItemFrame"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Button/ItemFrame_%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Camera_Button */
	/*if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Camera_Button"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Button/Camera_Button%d.png"), 2))))
		return E_FAIL;*/

	m_strMessage = TEXT("모델를(을) 로딩 중 입니다.");
	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Terrain/Height1.bmp")))))
		return E_FAIL;

	m_strMessage = TEXT("셰이더를(을) 로딩 중 입니다.");

	m_strMessage = TEXT("객체원형를(을) 로딩 중 입니다.");
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Spider"),
		CSpider::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Mouse */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Mouse"),
		CMouse::Create(m_pGraphic_Device))))
		return E_FAIL;

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

	/* For.Prototype_GameObject_Item */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Item"),
		CItem::Create(m_pGraphic_Device))))
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

	/* For.Prototype_GameObject_Clock */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Clock"),
		CClock::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Item_Info */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Item_Info"),
		CItem_Info::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Collision */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_dhere_Collision"),
		CSphere_Collision_Component::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Button */ // 인게임 카메라에 추가
	/*if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Camera_Button"),
		CCamera_Button::Create(m_pGraphic_Device))))
		return E_FAIL;*/

	m_strMessage = TEXT("폰트를(을) 로딩 중 입니다.");

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Item_Count_14"), 14, TEXT("BigDonstarve"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_14"), 14, TEXT("BigDonstarve"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_18"), 18, TEXT("나눔고딕"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_25"), 25, TEXT("HY견고딕"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Date_40"), 40, TEXT("BigDonstarve"))))
		return E_FAIL;

	m_strMessage = TEXT("로딩이 완료되었습니다..");

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Tutorial()
{
	m_strMessage = TEXT("텍스쳐를(을) 로딩 중 입니다.");
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Ground/tile%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_Component_Texture_Monster"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Monster/Ma.jpg"), 1))))
		return E_FAIL;

	m_strMessage = TEXT("모델를(을) 로딩 중 입니다.");

	m_strMessage = TEXT("셰이더를(을) 로딩 중 입니다.");

	m_strMessage = TEXT("객체원형를(을) 로딩 중 입니다.");

	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 130, 130))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

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
