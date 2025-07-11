#pragma once
#include "Client_Defines.h"
#include "Item.h"

NS_BEGIN(Client);

class CCookpot final : public CItem
{
	enum class STATE { IDLE_EMPTY, IDLE_OPEN, IDLE_FULL, PLACE, COOKING_LOOP, END };
private:
	CCookpot(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCookpot(const CCookpot& Prototype);
	virtual ~CCookpot() = default;

public:
	void Start_Cooking(_uint iItemID);

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
	STATE				m_ePreState = {};
	STATE				m_eCurState = {};

	_uint				m_iFoodID = {};
	_float				m_fCookingTime = {};

	class CCookUI*		m_pCookUI = { nullptr };
	
	CTexture*			m_pFoodTexture = { nullptr };
	CTransform*			m_pFoodTransformCom = { nullptr };

private:
	HRESULT			ADD_Components();

	void			Change_State();

public:
	static CCookpot* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

NS_END

