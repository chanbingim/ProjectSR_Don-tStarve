#pragma once
#include "GameObject.h"

NS_BEGIN(Engine)

class CLightComponent;

class ENGINE_DLL CLightObejct abstract : public CGameObject
{
protected :
	CLightObejct(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLightObejct(const CLightObejct& rhs);
	virtual ~CLightObejct() = default;

public :
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual HRESULT			Initialize_Late() override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

protected :
	CLightComponent*		m_pLight_Com = { nullptr };
	_bool					m_LightEnable = { false };

public :
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;

};
NS_END