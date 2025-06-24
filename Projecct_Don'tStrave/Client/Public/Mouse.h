#pragma once
#include"Client_Defines.h"

#include "UserInterface.h"

NS_BEGIN(Client)

class CMouse final : public CUserInterface
{
private:
	CMouse(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMouse(const CMouse& Prototype);
	virtual ~CMouse() = default;

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

	virtual	void		ClickedEevent();

private:
	_float			m_fTimeAcc = {};
	class CSlot*	m_pSlot = { nullptr };

private:
	HRESULT ADD_Components();
	HRESULT Add_Slot();

public:
	static CMouse* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END