#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)

NS_END

NS_BEGIN(Client)
class CSkyBox final : public CGameObject
{
private :
	CSkyBox(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkyBox(const CSkyBox& rhs);
	virtual ~CSkyBox() = default;

public :
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual HRESULT		Initialize_Late();

	virtual void		Priority_Update(_float fTimeDelta);
	virtual void		Update(_float fTimeDelta);
	virtual void		Late_Update(_float fTimeDelta);
	virtual HRESULT		Render();

public :
	static	CSkyBox*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
NS_END