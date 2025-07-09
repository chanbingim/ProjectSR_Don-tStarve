#pragma once
#include "DropItemEnviornment.h"

NS_BEGIN(Client)
class CGatheringObject : public CDropItemEnviornment
{
public :
	typedef struct Gathering_Desc : public GAMEOBJECT_DESC
	{
		_float						MaxRecoverTime;
		vector<pair<_uint, _uint>>	DropList;

	}GATHERING_DESC;

private:
	CGatheringObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGatheringObject(const CGatheringObject& rhs);
	virtual ~CGatheringObject() = default;

public:
	virtual HRESULT		Initialize_Prototype(const char* FilePath, const _wstring FolderName);
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
	void				Setting_Data();


	void				BeginHitActor(CGameObject* HitActor, _float3& _Dir);
	void				OverlapHitActor(CGameObject* HitActor, _float3& _Dir);
	void				EndHitActor(CGameObject* HitActor, _float3& _Dir);

public:
	static		CGatheringObject*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, const char* FilePath, const _wstring FolderName);
	CGameObject*					Clone(void* pArg) override;
	void							Free() override;

};
NS_END