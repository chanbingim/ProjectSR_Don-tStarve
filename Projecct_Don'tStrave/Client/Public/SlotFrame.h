#pragma once
#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)
class CSlot;
class CSlotFrame final:  public CUserInterface
{
public:
	typedef struct SlotFrame_Desc
	{
		CUserInterface::UIOBJECT_DESC Desc = {};
		_uint iSlotType = {};
	}SLOTFRAME_DESC;

private:
	CSlotFrame(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSlotFrame(const CSlotFrame& Prototype);
	virtual ~CSlotFrame() = default;

public:
	CSlot* Get_Slot() { return m_pSlot; }

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;
	
	
private:
	_bool	m_bClick = {};
	_uint	m_iSlotType = {};
	CSlot*	m_pSlot = { nullptr };

private:
	HRESULT ADD_Components();
	void	Key_Input();

public:
	static CSlotFrame* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END