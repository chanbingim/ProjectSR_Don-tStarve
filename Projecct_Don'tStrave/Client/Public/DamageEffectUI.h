#pragma once

#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Client)
class CDamageEffectUI : public CUserInterface
{
private :
	CDamageEffectUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CDamageEffectUI() = default;

public :
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Priority_Update(_float fTimeDelta);
	virtual void		Update(_float fTimeDelta);
	virtual void		Late_Update(_float fTimeDelta);
	virtual HRESULT		Render();

	void				ActiveEffect();
private :
	_bool				m_bActive = { false };
	_float				m_Alpha = { 0.0f };
	_bool				m_bInverse = { false };

	



private :
	void				ResetData();
	HRESULT				ADD_Compoenets();

public :
	static		CDamageEffectUI*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual		CUserInterface*		Clone(void* pArg) override;
	virtual		void				Free() override;
};
NS_END