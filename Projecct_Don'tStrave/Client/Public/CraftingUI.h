#pragma once
#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)

class CCraftingUI : public CUserInterface
{
private:
	CCraftingUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCraftingUI(const CCraftingUI& Prototype);
	virtual ~CCraftingUI() = default;

public:
	virtual HRESULT  Initialize_Prototype()	override;
	virtual HRESULT  Initialize(void* pArg) override;
	virtual void	 Priority_Update(_float fTimeDelta) override;
	virtual void	 Update(_float fTimeDelta) override;
	virtual void	 Late_Update(_float fTimeDelta) override;
	virtual HRESULT	 Render() override;

private:
	_bool m_bHide = {};

private:
	HRESULT ADD_Components();

public:
	static CCraftingUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END