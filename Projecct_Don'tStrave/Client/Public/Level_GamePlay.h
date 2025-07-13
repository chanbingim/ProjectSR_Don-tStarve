#pragma once

#include "Client_Defines.h"
#include "Level.h"

NS_BEGIN(Client)

class CCharacterManager;

class CLevel_GamePlay final : public CLevel
{
private:
	CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize();
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	HRESULT			LoadFileData(const char* MapName);

private:
	HRESULT Ready_Layer_BackGround(const char* FilePath, const _wstring& strLayerTag);
	HRESULT Ready_Layer_Camera(const char* FilePath, const _wstring& strLayerTag);
	HRESULT Ready_Layer_Monster(const char* FilePath, const _wstring& strLayerTag);
	HRESULT Ready_Layer_Enviornment(const char* FilePath, const _wstring& strLayerTag);

	HRESULT Ready_Layer_Player(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Mouse(const _wstring& strLayerTag);
	HRESULT Ready_Layer_UserInterface(const _wstring& strLayerTag);

	HRESULT	Ready_Layer_Particle(const _wstring& strLayerTag);
	HRESULT	Ready_Layer_Item(const _wstring& strLayerTag);

private:
	_wstring			GetEnv_ObejctTag(_uint iID);
	CCharacterManager* m_pCharacterManager = { nullptr };
public:
	static CLevel_GamePlay* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID);
	virtual void Free() override;
};

NS_END