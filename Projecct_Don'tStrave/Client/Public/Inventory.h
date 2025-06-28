#pragma once
#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)
class CSlot;
class CInventory final : public CUserInterface
{
private:
	CInventory(LPDIRECT3DDEVICE9 pGraphic_Device);
	CInventory(const CInventory& Prototype);
	virtual ~CInventory() = default;

public:
	virtual HRESULT  Initialize_Prototype()override;
	virtual HRESULT  Initialize(void* pArg)override;
	virtual void	 Priority_Update(_float fTimeDelta)override;
	virtual void	 Update(_float fTimeDelta)override;
	virtual void	 Late_Update(_float fTimeDelta)override;
	virtual HRESULT	 Render()override;

	CSlot* Find_Item(_uint iItemID);
	CSlot* Find_Slot(SLOT eSlot);

private:
	vector<class CSlotFrame*> m_SlotFrames = {};

private:
	HRESULT ADD_Components();

public:
	static CInventory* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free() override;
};

NS_END