#pragma once
#include "Enviornment_Object.h"

NS_BEGIN(Engine)
class CAnimController;
NS_END

NS_BEGIN(Client)
class CEnv_Animation;
	
class CGrassObject : public CEnviornment_Object
{
private:
	CGrassObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGrassObject(const CGrassObject& rhs);
	virtual ~CGrassObject() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	CTexture*			m_Broken_pTexture_Com = { nullptr };
	CTexture*			m_Damaged_pTexture_Com = { nullptr };

	CAnimController*	m_Animation_Com = { nullptr };
	CEnv_Animation*		m_AnimationState[3] = {};

private:
	HRESULT				ADD_Components();

	void				BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void				OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void				EndHitActor(CGameObject* HitActor, _float3& _Dir);

public:
	static		CGrassObject*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject*				Clone(void* pArg) override;
	void						Free() override;

};
NS_END