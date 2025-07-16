#include "SystemSettingUI.h"

#include "GameInstance.h"
#include "EnviornmentButton.h"
#include "SlideButton.h"
#include "Level_GamePlay.h"

CSystemSettingUI::CSystemSettingUI(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CUserInterface(pGraphic_Device)
{
}

CSystemSettingUI::CSystemSettingUI(const CSystemSettingUI& rhs) :
	CUserInterface(rhs)
{
}

HRESULT CSystemSettingUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSystemSettingUI::Initialize(void* pArg)
{
	m_fWinSizeX = 1280.f;
	m_fSizeX = 640.f;
	m_fWinSizeY = 720.f;
	m_fSizeY = 500.f;

	m_fX = 640;
	m_fY = 320;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(ADD_Buttons()))
		return E_FAIL;

	m_pSelectCharacterMenuBut->SetClickEvent([&]() { ClickedCharacterMenuButton(); });
	m_pGameQuitBut->SetClickEvent([&]() { ClickedGameQuitButton(); });

	m_pBGMVolumeSlidebar->SetClickEvent([&]() {ClickedBGMVolumeSlidebar();});
	m_pBGMVolumeBut->SetPressEvent([&]() {PressedBGMVolumeButton();});

	m_ZOrder = 4;
	return S_OK;
}

void CSystemSettingUI::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->KeyDown(VK_ESCAPE))
	{
		SetVisible();
	}
	if (m_IsActive)
	{
		m_pSelectCharacterMenuBut->Update(fTimeDelta);
		m_pGameQuitBut->Update(fTimeDelta);
		m_pBGMVolumeSlidebar->Update(fTimeDelta);
		m_pBGMVolumeBut->Update(fTimeDelta);
	}
}

void CSystemSettingUI::Update(_float fTimeDelta)
{
	
}

void CSystemSettingUI::Late_Update(_float fTimeDelta)
{
	if (m_IsActive)
	{
		UpdatePosition();
		m_pSelectCharacterMenuBut->Late_Update(fTimeDelta);
		m_pGameQuitBut->Late_Update(fTimeDelta);
		m_pBGMVolumeSlidebar->Late_Update(fTimeDelta);
		m_pBGMVolumeBut->Late_Update(fTimeDelta);

		m_pGameInstance->Add_RenderGroup(RENDER::ORTTHO_UI, this);
	}

	
}

HRESULT CSystemSettingUI::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransform_Com->Get_World());
	m_pTexture_Com->Set_Texture(0);
	m_pVIBuffer_Com->Render();

	RECT rect = {  };

	D3DXCOLOR white = { 1.f,1.f,1.f,1.f };
	m_pGameInstance->Render_Font(TEXT("Font_30"), TEXT("BGM : "), &m_rcBGMText, white);

	m_pSelectCharacterMenuBut->Render();
	m_pGameQuitBut->Render();
	m_pBGMVolumeSlidebar->Render();
	m_pBGMVolumeBut->Render();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	return S_OK;
}

void CSystemSettingUI::SetVisible()
{
	m_IsActive = !m_IsActive;
	if(m_IsActive)
		m_pGameInstance->ChangeGameState(GAMESTATE::SINEMATIC);
	else
		m_pGameInstance->ChangeGameState(GAMESTATE::GAMEPLAY);
}

HRESULT CSystemSettingUI::ADD_Components()
{
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"),
		reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC Transform_Desc = { 5.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"),
		reinterpret_cast<CComponent**>(&m_pTransform_Com), &Transform_Desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_SettingBakcGorundUIFrame"),
		TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTexture_Com))))
		return E_FAIL;

	return S_OK;
}

HRESULT CSystemSettingUI::ADD_Buttons()
{
	CEnviornmentButton::BUTTON_DESC Desc;
	Desc.fSizeX = 185.f;
	Desc.fSizeY = 64.f;

	Desc.fRelativeX =  0.f;
	Desc.fRelativeY = -20.f;
	Desc.pParentTransform = m_pTransform_Com;
	m_pSelectCharacterMenuBut = dynamic_cast<CEnviornmentButton*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GamePlay_EnviornButton"), &Desc));
	if (nullptr == m_pSelectCharacterMenuBut)
		return E_FAIL;

	Desc.fSizeX = 185.f;
	Desc.fSizeY = 64.f;

	Desc.fRelativeX = 0.f;
	Desc.fRelativeY = -120.f;

	m_pGameQuitBut = dynamic_cast<CEnviornmentButton*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GamePlay_EnviornButton"), &Desc));
	if (nullptr == m_pGameQuitBut)
		return E_FAIL;

	m_pGameQuitBut->ChangeButtonTex(1);

	// Slibebar
	Desc.fSizeX = 200.f;
	Desc.fSizeY = 20.f;

	Desc.fRelativeX = 0.f;
	Desc.fRelativeY = 100.f;

	m_fSlideMax = m_fX + Desc.fRelativeX + Desc.fSizeX * 0.5f;
	m_fSlideMin = m_fX + Desc.fRelativeX - Desc.fSizeX * 0.5f;

	m_pBGMVolumeSlidebar = dynamic_cast<CSlideButton*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SlideButton"), &Desc));
	if (nullptr == m_pBGMVolumeSlidebar)
		return E_FAIL;

	m_rcBGMText = {
		LONG(m_fX + Desc.fRelativeX - Desc.fSizeX * 0.5f - 100),LONG(m_fY - 120.f),
		LONG(m_fX + Desc.fRelativeX - Desc.fSizeX * 0.5f), LONG(m_fY - 80.f)};

	// volumeBtn
	Desc.fSizeX = 40.f;
	Desc.fSizeY = 40.f;

	Desc.fRelativeX = 0.f;
	Desc.fRelativeY = 100.f;

	m_pBGMVolumeBut = dynamic_cast<CSlideButton*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SlideButton"), &Desc));
	if (nullptr == m_pBGMVolumeBut)
		return E_FAIL;

	m_pBGMVolumeBut->ChangeButtonTex(1);

	return S_OK;
}

void CSystemSettingUI::ClickedCharacterMenuButton()
{
	auto GamePlay = dynamic_cast<CLevel_GamePlay*>(m_pGameInstance->CurrentLevel());
	if (GamePlay)
	{
		GamePlay->ChangeLevel();
		m_pGameInstance->ChangeGameState(GAMESTATE::GAMEPLAY);
	}
		
}

void CSystemSettingUI::ClickedGameQuitButton()
{
	PostQuitMessage(0);
}

void CSystemSettingUI::ClickedBGMVolumeSlidebar()
{
	
}

void CSystemSettingUI::PressedBGMVolumeButton()
{
	_float fVoluemButtonX = {};

	_float3 MousePoint = m_pGameInstance->GetMousePosition(0);

	fVoluemButtonX = MousePoint.x;
	fVoluemButtonX = min(fVoluemButtonX, m_fSlideMax);
	fVoluemButtonX = max(fVoluemButtonX, m_fSlideMin);

	m_pBGMVolumeBut->Set_RelativeX(fVoluemButtonX-640.f);

	_float fSlidebarX = m_pBGMVolumeSlidebar->Get_PosX();
	_float fVolumeButX = m_pBGMVolumeBut->Get_PosX();

	_float fVolume = ((fVolumeButX - fSlidebarX) / 200.f + 1.f) * 0.5f;

	m_pGameInstance->Manager_SetChannelVolume(CHANNELID::SOUND_BGM, fVolume);
}

CSystemSettingUI* CSystemSettingUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSystemSettingUI* pInstance = new CSystemSettingUI(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CREATE FAIL : SYSTEM SETTING UI");
	}

	return pInstance;
}

CGameObject* CSystemSettingUI::Clone(void* pArg)
{
	CSystemSettingUI* pInstance = new CSystemSettingUI(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("CLONE FAIL : SYSTEM SETTING UI");
	}

	return pInstance;
}

void CSystemSettingUI::Free()
{
	__super::Free();

	Safe_Release(m_pSelectCharacterMenuBut);
	Safe_Release(m_pGameQuitBut);

	Safe_Release(m_pTexture_Com);
	Safe_Release(m_pTransform_Com);
	Safe_Release(m_pVIBuffer_Com);
}
