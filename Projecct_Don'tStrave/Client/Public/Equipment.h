#pragma once
#include "Client_Defines.h"
#include "Item.h"

NS_BEGIN(Client);

class CEquipment : public CItem
{
private:
	CEquipment(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEquipment(const CEquipment& Prototype);
	virtual ~CEquipment() = default;

public:
	virtual HRESULT  Initialize_Prototype();
	virtual HRESULT  Initialize(void* pArg);
	virtual void	 Priority_Update(_float fTimeDelta);
	virtual void	 Update(_float fTimeDelta);
	virtual void	 Late_Update(_float fTimeDelta);
	virtual HRESULT	 Render();

	void HoverEvent();

public:
	static CEquipment* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END