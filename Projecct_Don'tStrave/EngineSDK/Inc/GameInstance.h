#pragma once

#include "Prototype_Manager.h"

NS_BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance);

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, LPDIRECT3DDEVICE9* ppOut, class CMouseSlotUI* pMouse = nullptr);
	void Update_Engine(_float fTimeDelta);
	HRESULT Draw();
	void Clear_Resources(_uint iLevelIndex);

	_float Random_Normal();
	_float Random(_float fMin, _float fMax);

#pragma region GRAPHIC_DEVICE
	void Render_Begin(D3DXCOLOR Color);
	void Render_End(HWND hWnd = nullptr);
#pragma endregion

#pragma region TIMER_MANAGER
	_float	Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT	Add_Timer(const _wstring& strTimerTag);
	void	Compute_TimeDelta(const _wstring& strTimerTag);
#pragma endregion

#pragma region LEVEL_MANAGER
	HRESULT Change_Level(class CLevel* pNewLevel);
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	class CBase* Clone_Prototype(PROTOTYPE ePrototype, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
#pragma endregion

#pragma region OBJECT_MANAGER
	CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex = 0);
	class CGameObject* Get_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, _uint iIndex = 0);
	HRESULT Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLayerLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
#pragma endregion


#pragma region RENDERER
	HRESULT Add_RenderGroup(RENDER eRenderGroup, class CGameObject* pRenderObject);
#pragma endregion


#pragma region PICKING
	class CMouseSlotUI*	Chagne_Slot(class CMouseSlotUI* pSlot = nullptr);
	void			Transform_Picking_ToLocalSpace(const _float4x4* pWorldMatrixInverse);
	_float3			GetMousePosition(_uint ID);
	_bool			Picking_InWorldSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);
	_bool			Picking_InLocalSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);
#pragma endregion

#pragma region SOUND_MANAGER
	void Manager_PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void Manager_PlayBGM(const TCHAR* pSoundKey, float fVolume);
	void Manager_StopSound(CHANNELID eID);
	void Manager_StopAll();
	void Manager_SetChannelVolume(CHANNELID eID, float fVolume);
#pragma endregion

#pragma region FONT_MANAGER
	void Render_Font(const _wstring strFontTag, const _tchar* pText, RECT* pRect);
	HRESULT Add_Font(const _wstring strFontTag, _uint iSize, const _tchar* pFontName);
#pragma endregion


private:
	class CGraphic_Device*			m_pGraphic_Device = { nullptr };
	class CTimer_Manager*			m_pTimer_Manager = { nullptr };
	class CLevel_Manager*			m_pLevel_Manager = { nullptr };
	class CPrototype_Manager*		m_pPrototype_Manager = { nullptr };
	class CObject_Manager*			m_pObject_Manager = { nullptr };
	class CRenderer*				m_pRenderer = { nullptr };
	class CMouseManager*			m_pMouseManager = { nullptr };
	class CSoundManager*			m_pSoundManager = { nullptr };
	class CFont_Manager*			m_pFont_Manager = { nullptr };

public:
	void Release_Engine();
	virtual void Free() override;
};

NS_END