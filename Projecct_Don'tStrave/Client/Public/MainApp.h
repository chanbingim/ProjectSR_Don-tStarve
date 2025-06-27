#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)

class CMainApp final : public CBase
{	
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	void	RunApp(_float fTimeDelta);

private:
	CGameInstance*		m_pGameInstance = { nullptr };
	LPDIRECT3DDEVICE9	m_pGraphic_Device = { nullptr };

	_int				m_pFrame = {};
	_float				m_AccFrameTime = {};

private:
	HRESULT Initialize();
	void Update(_float fTimeDelta);
	HRESULT Render();

	HRESULT Ready_Default_Setting();
	HRESULT Start_Level(LEVEL eLevelID);
	HRESULT Ready_Prototypes();

	void	Render_FPS();

public:
	static CMainApp* Create();
	virtual void Free() override;
};

NS_END



