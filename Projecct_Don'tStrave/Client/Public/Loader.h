#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* 1. 다음 레벨에 대한 자원을 로드한다. */

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLoader() = default;

public:
	HRESULT Initialize(LEVEL eNextLevelID);
	HRESULT Loading();
	void Output();

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
	HRESULT Loading_For_Logo();
	HRESULT Loading_For_GamePlay();



public:
	static CLoader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID);
	virtual void Free() override;

};

NS_END