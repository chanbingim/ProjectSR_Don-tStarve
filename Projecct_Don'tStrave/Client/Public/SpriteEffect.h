#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CSpriteEffect : public CGameObject
{
private:
	CSpriteEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSpriteEffect(const CSpriteEffect& rhs);
	virtual ~CSpriteEffect() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual HRESULT		Initialize_Late();

	virtual void		Priority_Update(_float fTimeDelta);
	virtual void		Update(_float fTimeDelta);
	virtual void		Late_Update(_float fTimeDelta);
	virtual HRESULT		Render();

public:
	static	CSpriteEffect*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
NS_END