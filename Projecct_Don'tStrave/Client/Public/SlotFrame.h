#pragma once
#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)

class CSlotFrame final:  public CUserInterface
{
private:
	CSlotFrame(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSlotFrame(const CSlotFrame& Prototype);
	virtual ~CSlotFrame() = default;

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

private:
	_bool			m_bClick = {};

	class CSlot*	m_pSlot = { nullptr };

private:
	HRESULT ADD_Components();
	void	Key_Input();

public:
	static CSlotFrame* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END