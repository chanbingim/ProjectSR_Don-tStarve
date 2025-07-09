#pragma once
#include "DropItemEnviornment.h"

NS_BEGIN(Client)
class CRockObject : public CDropItemEnviornment
{
private:
	CRockObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRockObject(const CRockObject& rhs);
	virtual ~CRockObject() = default;

public:
	virtual HRESULT				Initialize_Prototype(const char* FilePath, const _wstring FolderName);
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual	void				Reset_State();
	virtual HRESULT				Render() override;

	virtual void				Damage(void* pArg) override;

private:
	HRESULT						ADD_Components();
	void						Setting_Data();

	void						BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void						OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void						EndHitActor(CGameObject* HitActor, _float3& _Dir);

public:
	static		CRockObject*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, const char* FilePath, const _wstring FolderName);
	CGameObject*				Clone(void* pArg) override;
	void						Free() override;
};
NS_END