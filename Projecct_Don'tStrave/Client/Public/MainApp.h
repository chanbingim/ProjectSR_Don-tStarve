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

private:
	HRESULT Initialize();
	void Update(_float fTimeDelta);
	HRESULT Render();

	HRESULT Ready_Default_Setting();
	HRESULT Start_Level(LEVEL eLevelID);
	HRESULT Ready_Prototypes();

public:
	static CMainApp* Create();
	virtual void Free() override;
};

NS_END



