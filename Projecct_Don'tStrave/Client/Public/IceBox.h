#pragma once
#include "Client_Defines.h"
#include "Item.h"

NS_BEGIN(Client);

class CIceBox : public CItem
{
	enum class STATE { IDLE, PLACE, OPEN, CLOSE, OPENED, END };
private:
	CIceBox(LPDIRECT3DDEVICE9 pGraphic_Device);
	CIceBox(const CIceBox& Prototype);
	virtual ~CIceBox() = default;

public:
	virtual HRESULT  Initialize_Prototype();
	virtual HRESULT  Initialize(void* pArg);
	virtual void	 Priority_Update(_float fTimeDelta);
	virtual void	 Update(_float fTimeDelta);
	virtual void	 Late_Update(_float fTimeDelta);
	virtual HRESULT	 Render();

	void HoverEvent();
	void ClickedEvent();

private:
	CIceBox::STATE		m_eCurState = {};
	CIceBox::STATE		m_ePreState = {};

	class CChestUI* m_pChestUI = { nullptr };

private:
	HRESULT ADD_Components();
	void Change_State();

public:
	static CIceBox* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END
