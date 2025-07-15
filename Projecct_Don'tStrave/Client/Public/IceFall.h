#pragma once
#include "AinimationObject.h"

NS_BEGIN(Engine)
class CCollision_Component;
class CTexture;
NS_END

NS_BEGIN(Client)

class CIceFall :
    public CAinimationObject
{
private:
	CIceFall(LPDIRECT3DDEVICE9 pGraphic_Device);
	CIceFall(const CIceFall& rhs);
	virtual ~CIceFall() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual	void			Reset_State();
	HRESULT				Ready_Components();
	virtual HRESULT			Render() override;

	virtual void			Damage(void* pArg) override;
	virtual void			Death() override;
private:
	_float					m_fYPos = {};
	CCollision_Component*	m_pCollision_Com = { nullptr };
	CTexture*				m_pShadowTex = { nullptr };
	CTexture*				m_pIceTex = { nullptr };
	_bool					m_bBreak;
private:
	void BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void EndHitActor(CGameObject* HitActor, _float3& _Dir);
public:
	static		CIceFall* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual		CGameObject* Clone(void* pArg) override;
	virtual		void			Free();

};
NS_END

