#pragma once

#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)
class CListBoxUI;
class CQuestCategoryButton;

class CQuestFrameUI : public CUserInterface
{
private :
	CQuestFrameUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CQuestFrameUI(const CQuestFrameUI& rhs);
	virtual ~CQuestFrameUI() = default;

public :
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);
	virtual void					Priority_Update(_float fTimeDelta);
	virtual void					Update(_float fTimeDelta);
	virtual void					Late_Update(_float fTimeDelta);
	virtual HRESULT					Render();

	void							ClieckedButton(_uint Index);
	void							ToggleObject();
private :
	_bool							m_bIsActive = { false };
	CListBoxUI*						m_pListBox = { nullptr };
	CQuestCategoryButton*					m_pCategoryButton[3] = { nullptr };

	const	WCHAR*					m_szTittle = { L"Äù½ºÆ® Ã¢" };
	_uint							m_SelectQuestList = {};

	RECT							m_FontRect = {0, 0, 100, 100};

private :
	HRESULT							ADD_Components();
	HRESULT							ADD_CategoryButton();

public :
	static	CQuestFrameUI*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;

};
NS_END