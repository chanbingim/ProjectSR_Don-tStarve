#pragma once
#include "Button.h"

NS_BEGIN(Client)
class CQuestCategoryButton : public CButton
{
private :
	CQuestCategoryButton(LPDIRECT3DDEVICE9 pGraphic_Device);
	CQuestCategoryButton(const CQuestCategoryButton& rhs);
	virtual ~CQuestCategoryButton() = default;

public :
	virtual HRESULT					Initialize_Prototype()override;
	virtual HRESULT					Initialize(void* pArg)override;
	virtual void					Priority_Update(_float fTimeDelta)override;
	virtual void					Update(_float fTimeDelta)override;
	virtual void					Late_Update(_float fTimeDelta)override;
	virtual HRESULT					Render()override;

	void							SetClickEvent(function<void()> Func);
	void							ChangeButtonTex(_uint Index);

private :
	function<void()>				m_OnclickedEvent = { nullptr };
	_uint							m_ButtonIndex = {};

private :
	HRESULT							ADD_Components();

public :
	static		CQuestCategoryButton*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual		CGameObject*			Clone(void* pArg) override;
	virtual		void					Free() override;
};
NS_END