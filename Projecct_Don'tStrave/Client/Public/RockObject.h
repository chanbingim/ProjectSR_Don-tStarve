#pragma once
#include "DropItemEnviornment.h"

NS_BEGIN(Engine)
class CAnimController;
NS_END

NS_BEGIN(Client)
class CEnv_Animation;

class CRockObject : public CDropItemEnviornment
{
private:
	CRockObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRockObject(const CRockObject& rhs);
	virtual ~CRockObject() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	HRESULT				ADD_Components();

	void				BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void				OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void				EndHitActor(CGameObject* HitActor, _float3& _Dir);

public:
	static		CRockObject*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject*				Clone(void* pArg) override;
	void						Free() override;
};
NS_END