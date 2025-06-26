#include "GameInstance.h"

#include "Prototype_Manager.h"
#include "Object_Manager.h"
#include "MouseManager.h"
#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "Renderer.h"
#include "SoundManager.h"
#include "MouseSlotUI.h"
#include "Font_Manager.h"
#include "KeyManager.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, LPDIRECT3DDEVICE9* ppOut, class CMouseSlotUI* pMouse)
{
	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.eWindowMode, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY, ppOut);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pPrototype_Manager = CPrototype_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;
	
	m_pRenderer = CRenderer::Create(*ppOut);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	CMouseManager::MOUSE_MANGER_DESC Mouse_Mgr_Data = {};
	Mouse_Mgr_Data.pGraphic_Device = (*ppOut);
	Mouse_Mgr_Data.handle = EngineDesc.hWnd;

	m_pMouseManager = CMouseManager::Create(&Mouse_Mgr_Data);
	if (nullptr == m_pMouseManager)
		return E_FAIL;

	m_pSoundManager = CSoundManager::Create();
	if (nullptr == m_pSoundManager)
		return E_FAIL;

	m_pFont_Manager = CFont_Manager::Create(*ppOut);
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	m_pKey_Manager = CKeyManager::Create();
	if (nullptr == m_pKey_Manager)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	m_pKey_Manager->BeginKeyInput();

	m_pObject_Manager->Priority_Update(fTimeDelta);

	m_pMouseManager->Update();

	m_pObject_Manager->Update(fTimeDelta);

	m_pObject_Manager->Late_Update(fTimeDelta);

	m_pObject_Manager->Clear_DeadObj();

	m_pLevel_Manager->Update(fTimeDelta);

	m_pKey_Manager->EndKeyInput();
}

HRESULT CGameInstance::Draw()
{
	m_pRenderer->Render();

	m_pLevel_Manager->Render();

	return S_OK;
}

void CGameInstance::Clear_Resources(_uint iLevelIndex)
{
	m_pPrototype_Manager->Clear(iLevelIndex);
	m_pObject_Manager->Clear(iLevelIndex);
}

_float CGameInstance::Random_Normal()
{
	return static_cast<_float>(rand()) / RAND_MAX;	
}

_float CGameInstance::Random(_float fMin, _float fMax)
{
	return fMin + Random_Normal() * (fMax - fMin);	
}

#pragma region GRAPHIC_DEVICE

void CGameInstance::Render_Begin(D3DXCOLOR Color)
{
	m_pGraphic_Device->Render_Begin(Color);
}

void CGameInstance::Render_End(HWND hWnd)
{
	m_pGraphic_Device->Render_End(hWnd);
}

#pragma endregion

#pragma region TIMER_MANAGER

_float CGameInstance::Get_TimeDelta(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

void CGameInstance::Compute_TimeDelta(const _wstring& strTimerTag)
{
	m_pTimer_Manager->Compute_TimeDelta(strTimerTag);
}

#pragma endregion

#pragma region LEVEL_MANAGER

HRESULT CGameInstance::Change_Level(CLevel* pNewLevel)
{
	return m_pLevel_Manager->Change_Level(pNewLevel);
}

#pragma endregion

#pragma region PROTOTYPE_MANAGER

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype)
{
	return m_pPrototype_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}

CBase* CGameInstance::Clone_Prototype(PROTOTYPE ePrototype, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	return m_pPrototype_Manager->Clone_Prototype(ePrototype, iLevelIndex, strPrototypeTag, pArg);;
}

#pragma endregion

#pragma region OBJECT_MANAGER

CComponent* CGameInstance::Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex)
{
	return m_pObject_Manager->Get_Component(iLevelIndex, strLayerTag, strComponentTag, iIndex);	
}

CGameObject* CGameInstance::Get_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, _uint iIndex)
{
	return m_pObject_Manager->Get_GameObject(iLevelIndex, strLayerTag, iIndex);
}

HRESULT CGameInstance::Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLayerLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	return m_pObject_Manager->Add_GameObject_ToLayer(iPrototypeLevelIndex, strPrototypeTag, iLayerLevelIndex, strLayerTag, pArg);
}

#pragma endregion

#pragma region RENDERER

HRESULT CGameInstance::Add_RenderGroup(RENDER eRenderGroup, CGameObject* pRenderObject)
{
	return m_pRenderer->Add_RenderGroup(eRenderGroup, pRenderObject);
}

#pragma endregion

#pragma region PICKING

CMouseSlotUI* CGameInstance::Chagne_Slot(CMouseSlotUI* pSlot)
{
	return m_pMouseManager->Chagne_Slot(pSlot);
}

void CGameInstance::Transform_Picking_ToLocalSpace(const _float4x4* pWorldMatrixInverse)
{
	m_pMouseManager->Transform_ToLocalSpace(pWorldMatrixInverse);
}

_float3 CGameInstance::GetMousePosition(_uint ID)
{
	return m_pMouseManager->GetMousePosition(ID);
}

_bool CGameInstance::Picking_InWorldSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
	return m_pMouseManager->Picking_InWorldSpace(vPointA, vPointB, vPointC, pOut);
}

_bool CGameInstance::Picking_InLocalSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
	return m_pMouseManager->Picking_InLocalSpace(vPointA, vPointB, vPointC, pOut);
}
#pragma endregion

#pragma region SOUND_MANAGER
void CGameInstance::Manager_PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume)
{
	m_pSoundManager->Manager_PlayBGM(pSoundKey, fVolume);
}

void CGameInstance::Manager_PlayBGM(const TCHAR* pSoundKey, float fVolume)
{
	m_pSoundManager->Manager_PlayBGM(pSoundKey, fVolume);
}

void CGameInstance::Manager_StopSound(CHANNELID eID)
{
	m_pSoundManager->Manager_StopSound(eID);
}

void CGameInstance::Manager_StopAll()
{
	m_pSoundManager->Manager_StopAll();
}

void CGameInstance::Manager_SetChannelVolume(CHANNELID eID, float fVolume)
{
	m_pSoundManager->Manager_SetChannelVolume(eID, fVolume);
}
#pragma endregion

#pragma region FONT_MANAGER
void CGameInstance::Render_Font(const _wstring strFontTag, const _tchar* pText, RECT* pRect)
{
	m_pFont_Manager->Render_Font(strFontTag, pText, pRect);
}
HRESULT CGameInstance::Add_Font(const _wstring strFontTag, _uint iSize, const _tchar* pFontName)
{
	return m_pFont_Manager->Add_Font(strFontTag, iSize, pFontName);
}
#pragma endregion

#pragma region KEY_INPUT
_bool CGameInstance::KeyPressed(_uint KeyNum)
{
	return m_pKey_Manager->GetKeyPressed(KeyNum);
}

_bool CGameInstance::KeyDown(_uint KeyNum)
{
	return m_pKey_Manager->GetKeyDown(KeyNum);
}

_bool CGameInstance::KeyUp(_uint KeyNum)
{
	return m_pKey_Manager->GetKeyUp(KeyNum);
}
#pragma endregion

void CGameInstance::Release_Engine()
{
	DestroyInstance();

	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pMouseManager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pPrototype_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pSoundManager);
	Safe_Release(m_pGraphic_Device);
}

void CGameInstance::Free()
{
	__super::Free();

	
}
