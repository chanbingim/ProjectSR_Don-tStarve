#include "MainApp.h"

#include "GameInstance.h"
#include "Item_Manager.h"
#include "PlayerData_Manager.h"
#include "MonsterData_Manager.h"

#include "Level_Loading.h"
#include "Camera.h"
#include "AnimationUI.h"
#include "Terrian_Manager.h"
#include "LodingInterface.h"
#include "XML_Manager.h"
#include "Camera_Button.h"
#include "Logo.h"


Client::CMainApp::CMainApp()	
	: m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

void CMainApp::RunApp(_float fTimeDelta)
{
	/* 내 게임을 갱신한다. */
	Update(fTimeDelta);
	/* 갱신한 상태로 렌더링한다. */
	Render();
}

HRESULT Client::CMainApp::Initialize()
{
	ENGINE_DESC				EngineDesc{};
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.eWindowMode = WINMODE::WIN;
	EngineDesc.iWinSizeX = g_iWinSizeX;
	EngineDesc.iWinSizeY = g_iWinSizeY;
	EngineDesc.iNumLevels = ENUM_CLASS(LEVEL::END);

	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pGraphic_Device)))
		return E_FAIL;

	if (FAILED(Ready_Default_Setting()))
		return E_FAIL;

	if (FAILED(Ready_Prototypes()))
		return E_FAIL;

	if (FAILED(Start_Level(LEVEL::LOGO)))
		return E_FAIL;	

	if (FAILED(ReadShader()))
		return E_FAIL;

	CTerrian_Manager::GetInstance()->Initialize({64, 64});
	CXML_Manager::GetInstance()->Initialize(m_pGraphic_Device);

	return S_OK;
}

void Client::CMainApp::Update(_float fTimeDelta)
{
	m_pGameInstance->Update_Engine(fTimeDelta);

	


}

HRESULT Client::CMainApp::Render()
{
	m_pGameInstance->Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

	Render_FPS();
	m_pGameInstance->Draw();
	
	m_pGameInstance->Render_End();


	return S_OK;
}

HRESULT CMainApp::Ready_Default_Setting()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 재질 설정
	D3DMATERIAL9		MaterialDesc{};
	MaterialDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MaterialDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);


	m_pGraphic_Device->SetMaterial(&MaterialDesc);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

	m_pGraphic_Device->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
	m_pGraphic_Device->SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	return S_OK;
}

HRESULT CMainApp::Start_Level(LEVEL eLevelID)
{
	if (FAILED(m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pGraphic_Device, LEVEL::LOADING, eLevelID))))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CMainApp::Ready_Prototypes()
{
	/* For.Prototype_Component_Transform*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		CTransform::Create())))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_BoxCollision */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_BoxCollision"),
		CBox_Collision_Component::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_SphereCollision */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_SphereCollision"),
		CSphere_Collision_Component::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_AnimationController */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_AnimController"),
		CAnimController::Create())))
		return E_FAIL;

	/* For.Prototype_Component_Loding_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Loading"),
			   CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/LogoBack/LogoBack_%d.png"), 38))))
		return E_FAIL;

	/* For.Prototype_Component_Snow_Textrue */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Snow_Texture"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/Particles/Snow/Snow.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Logo */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Logo/logo.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Paritcle_Sys */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Particle_System"),
		CParticleSystemComponent::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Paritcle_Sys */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Light"),
		CLightComponent::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma region LOADING_INTERFACE
	/* For.Prototype_Compoent_LODING_Texture */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Loading_Texture"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Loding/BackGround/LoadingBackGround%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Compoent_LODING_Circle */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Loading_Circle_Texture"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Loding/Circle/LoadingCircle.png"), 1))))
		return E_FAIL;

	/* For.Prototype_GameObject_LODING_INTERFACE */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObejct_Loading_Interface"),
		CLodingInterface::Create(m_pGraphic_Device))))
		return E_FAIL;
#pragma endregion
	
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Camera_Button"),
		CTexture::Create(m_pGraphic_Device, TEXTURE::PLANE, TEXT("../Bin/Resources/Textures/UI/Button/Camera_Button%d.png"), 2))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Button"),
		CCamera_Button::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera"),
		CCamera::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_AnimUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Anim_UI"),
		CAnimationUI::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma region STATIC_FONT
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Item_Count_14"), 14, TEXT("BigDonstarve"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_14"), 14, TEXT("BigDonstarve"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_18"), 18, TEXT("BigDonstarve"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_25"), 25, TEXT("BigDonstarve"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_30"), 30, TEXT("BigDonstarve"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Date_40"), 40, TEXT("BigDonstarve"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("MouseInfo_40"), 40, TEXT("BigDonstarve"))))
		return E_FAIL;
#pragma endregion

	/* For.Prototype_GameObject_AnimUI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Logo"),
		CLogo::Create(m_pGraphic_Device))))
		return E_FAIL;
	return S_OK;
}

HRESULT CMainApp::ReadShader()
{
	

	return S_OK;
}

void CMainApp::Render_FPS()
{
	m_pFrame++;
	m_AccFrameTime += m_pGameInstance->Get_TimeDelta(TEXT("Timer_60"));

	if (m_AccFrameTime >= 1.f)
	{
		WCHAR pFrameStr[100] = {};

		wsprintf(pFrameStr, TEXT("Frame : %d"), m_pFrame);
		SetWindowText(g_hWnd, pFrameStr);

		m_pFrame = 0;
		m_AccFrameTime = 0;
	}
}

CMainApp* Client::CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CMainApp::Free()
{
	__super::Free();
	Safe_Release(m_pGraphic_Device);

	CPlayerData_Manager::DestroyInstance();
	CMonsterData_Manager::DestroyInstance();
	CItem_Manager::DestroyInstance();
	CTerrian_Manager::DestroyInstance();
	CXML_Manager::DestroyInstance();
	m_pGameInstance->Release_Engine();

	Safe_Release(m_pGameInstance);	
}
