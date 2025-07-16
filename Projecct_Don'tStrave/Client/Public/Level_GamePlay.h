#pragma once

#include "Client_Defines.h"
#include "Level.h"

NS_BEGIN(Client)

class CCharacterManager;

class CLevel_GamePlay final : public CLevel
{
public :
	enum GAMEPLAY_STATE { TUTORIAL, BOSS, END };

private:
	CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT		Initialize();
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

	HRESULT			LoadFileData(const char* MapName);
	void				Change_Map(GAMEPLAY_STATE eState);
	void				ChangeLevel();
private:
	HRESULT Ready_Layer_BackGround(const char* FilePath, const _wstring& strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Monster(const char* FilePath, const _wstring& strLayerTag);
	HRESULT Ready_Layer_Enviornment(const char* FilePath, const _wstring& strLayerTag);

	HRESULT Ready_Layer_Player(const _wstring& strLayerTag);
	HRESULT Player_Move_Pos();

	HRESULT Ready_Layer_Mouse(const _wstring& strLayerTag);
	HRESULT Ready_Layer_UserInterface(const _wstring& strLayerTag);

	HRESULT	Ready_Layer_Particle(const _wstring& strLayerTag);
	HRESULT	Ready_Layer_Item(const _wstring& strLayerTag);
	
	void				Remove_LayerData();
	

private :
	GAMEPLAY_STATE		m_eState = { GAMEPLAY_STATE::TUTORIAL };
	_bool				m_IsMapDataSetting = { false };
	_bool				m_IsChangeMap = { false };

	CCharacterManager*	m_pCharacterManager = { nullptr };

private:
	_wstring			GetEnv_ObejctTag(_uint iID);
	HRESULT				TutorialMapLoad();
	HRESULT				BossMapLoad();

public:
	static CLevel_GamePlay* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID);
	virtual void Free() override;
};

NS_END