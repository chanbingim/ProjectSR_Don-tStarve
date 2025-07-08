#pragma once
#include "Client_Defines.h"
#include "Item.h"

NS_BEGIN(Client);

class CResearchLap : public CItem
{
	enum class STATE {IDLE, USE, PLACE, LOOP, END};
private:
	CResearchLap(LPDIRECT3DDEVICE9 pGraphic_Device);
	CResearchLap(const CResearchLap& Prototype);
	virtual ~CResearchLap() = default;

public:
	virtual HRESULT  Initialize_Prototype();
	virtual HRESULT  Initialize(void* pArg);
	virtual void	 Priority_Update(_float fTimeDelta);
	virtual void	 Update(_float fTimeDelta);
	virtual void	 Late_Update(_float fTimeDelta);
	virtual HRESULT	 Render();

	void HoverEvent();

private:
	STATE m_ePreState = {};
	STATE m_eCurState = {};

	vector<class CItem_Button*>* m_pItem_Buttons = { nullptr };

private:
	HRESULT				ADD_Components();

	void Change_State();

public:
	static CResearchLap* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END
