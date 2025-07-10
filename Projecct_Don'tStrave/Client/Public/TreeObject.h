#pragma once
#include "DropItemEnviornment.h"

NS_BEGIN(Engine)
class CAnimController;
NS_END

NS_BEGIN(Client)

class CEnv_Animation;

class CTreeObject : public CDropItemEnviornment
{
protected:
	CTreeObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTreeObject(const CTreeObject& rhs);
	virtual ~CTreeObject() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual	void		Reset_State() override;
	virtual HRESULT		Render() override;

	virtual void		Damage(void* pArg) override;
	virtual void		Death()override;

private:
	HRESULT				ADD_Components();

	void				BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void				OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void				EndHitActor(CGameObject* HitActor, _float3& _Dir);

public:
	static		CTreeObject*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject*				Clone(void* pArg) override;
	void						Free() override;
};
NS_END