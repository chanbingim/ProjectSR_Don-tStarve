#pragma once

#include "Client_Defines.h"
#include "UserInterface.h"

NS_BEGIN(Engine)
class CAnimController;
NS_END

NS_BEGIN(Client)
class CUI_MoveAnim;

class CAnimationUI final : public CUserInterface
{
private :
	CAnimationUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAnimationUI(const CAnimationUI& rhs);
	virtual ~CAnimationUI() = default;

public :
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Priority_Update(_float fTimeDelta);
	virtual void		Update(_float fTimeDelta);
	virtual void		Late_Update(_float fTimeDelta);
	virtual HRESULT		Render();

	virtual	void		HoverEevent();
	virtual	void		ClickedEevent();

private :
	CAnimController*	m_pAnimController = { nullptr };
	CUI_MoveAnim*		m_pUIMoveAnim = { nullptr };

private :
	HRESULT				ADD_Components();


public :
	static	CAnimationUI*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual	void				Free() override;
};
NS_END