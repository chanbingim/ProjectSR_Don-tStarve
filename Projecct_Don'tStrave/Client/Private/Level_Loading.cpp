#include "Level_Loading.h"

#include "Loader.h"
#include "GameInstance.h"

#include "Level_Logo.h"
#include "Level_GamePlay.h"
#include "TutorialLevel.h"

CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID)
	: CLevel { pGraphic_Device, ENUM_CLASS(eLevelID)}
{

}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	/* 다음 레벨에 대한 자원을 로드하여 준비해둔다. */
	m_pLoader = CLoader::Create(m_pGraphic_Device, eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;

	if (FAILED(TEST_SoundMgr()))
		return E_FAIL;

	/* 이 레벨을 구성하기위한 객체를 만든다. */
	if (FAILED(Ready_Layer_BackGround()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{
	if (true == m_pLoader->isFinished() &&
		GetKeyState(VK_RETURN) & 0x8000)
	{
		CLevel* pNewLevel = { nullptr };

		switch (m_eNextLevelID)
		{
		case LEVEL::LOGO:
			pNewLevel = CLevel_Logo::Create(m_pGraphic_Device, m_eNextLevelID);
			break;
		case LEVEL::GAMEPLAY:
			pNewLevel = CLevel_GamePlay::Create(m_pGraphic_Device, m_eNextLevelID);
			break;

		case LEVEL::TUTORIAL:
			pNewLevel = CTutorialLevel::Create(m_pGraphic_Device, m_eNextLevelID);
			break;
		}

		if (FAILED(m_pGameInstance->Change_Level(pNewLevel)))
			return;		
	}	
}

HRESULT CLevel_Loading::Render()
{
	m_pLoader->Output();

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BackGround()
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Anim_UI"),
			ENUM_CLASS(LEVEL::LOADING), L"BackGroundLayer")))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_Loading::TEST_SoundMgr()
{
	m_pGameInstance->Manager_PlaySound(L"MashUp_Dance1.wav", Engine::CHANNELID::SOUND_BGM, 0.1f);

	return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID, LEVEL eNextLevelID)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pGraphic_Device, eLevelID);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CLevel_Loading::Free()
{
	m_pGameInstance->Manager_StopAll();

	__super::Free();
	Safe_Release(m_pLoader);
}
