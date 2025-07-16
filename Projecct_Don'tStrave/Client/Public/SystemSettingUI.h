#pragma once
#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)

class CEnviornmentButton;
class CSlideButton;

class CSystemSettingUI : public CUserInterface
{
private :
	CSystemSettingUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSystemSettingUI(const CSystemSettingUI& rhs);
	virtual ~CSystemSettingUI() = default;

public :
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);
	virtual void					Priority_Update(_float fTimeDelta);
	virtual void					Update(_float fTimeDelta);
	virtual void					Late_Update(_float fTimeDelta);
	virtual HRESULT					Render();

	void							SetVisible();

private :
	CEnviornmentButton*				m_pSelectCharacterMenuBut = { nullptr };
	CEnviornmentButton*				m_pGameQuitBut = { nullptr };

	CSlideButton*					m_pBGMVolumeBut = { nullptr };
	CSlideButton*					m_pBGMVolumeSlidebar = { nullptr };

	_bool							m_IsActive = { false };

	_float							m_fSlideMax = {};
	_float							m_fSlideMin = {};

	RECT							m_rcBGMText = {};

private :
	HRESULT							ADD_Components();
	HRESULT							ADD_Buttons();

	void							ClickedCharacterMenuButton();
	void							ClickedGameQuitButton();
	
	void							ClickedBGMVolumeSlidebar();
	void							PressedBGMVolumeButton();

public :
	static		CSystemSettingUI*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual		CGameObject*		Clone(void* pArg) override;
	virtual		void				Free() override;

};
NS_END
