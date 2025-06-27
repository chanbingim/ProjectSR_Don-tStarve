#pragma once

#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)
class CMap_Loader : public CBase
{
private:
	CMap_Loader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CMap_Loader() = default;

public:
	HRESULT		Initialize(LEVEL eNextLevelID);
	HRESULT		Loading();

public:
	_bool isFinished() const {
		return m_isFinished;
	}

private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = { nullptr };
	LEVEL				m_eNextLevelID = { LEVEL::END };

	HANDLE				m_hThread = {};

	_wstring			m_strMessage = {};
	_bool				m_isFinished = { false };

	CRITICAL_SECTION	m_CriticalSection = {};
	CGameInstance*		m_pGameInstance = { nullptr };

private:
	HRESULT Loading_For_Tutorial();
	HRESULT Loading_For_GameLevelOne();
	HRESULT Loading_For_GameLevelOne();
	HRESULT Loading_For_GameBossLevel();

public:
	static CMap_Loader*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID);
	virtual void			Free() override;

};
NS_END