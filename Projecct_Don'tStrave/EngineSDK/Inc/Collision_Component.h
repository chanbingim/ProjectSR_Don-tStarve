#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class CGameObject;

class ENGINE_DLL CCollision_Component abstract : public CComponent
{
public :
	typedef struct Collision_Desc
	{
		CGameObject*		pOwner = {nullptr};
	}COL_DESC;

protected :
	CCollision_Component(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollision_Component(const CCollision_Component& rhs);
	virtual ~CCollision_Component() = default;

public :
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

	// ��� ���͸� �׷쿡 �߰��Ѵ��� ȣ��
	// ȣ��� ���� �����ӵ����Ϳ� ���ؼ� ���ο� �༮��Ǵ�
	// �������� �༮�� �̺�Ʈ�� ȣ������
	virtual void		Update();
	virtual void		Render();

	void							ADDHitGroup(CGameObject* pGameObject);
	const list<CGameObject*>*		GetOVerlapAllObejcts();

	COLLISION_TYPE					GetColType();
	CGameObject*					GetOwner() { return m_pOwner; }

	void							BindEnterFunction(function<void(CGameObject* HitActor, _float3& _Dir)> Func);
	void							BindOverlapFunction(function<void(CGameObject* HitActor, _float3& _Dir)> Func);
	void							BindExitFunction(function<void(CGameObject* HitActor, _float3& _Dir)> Func);

protected:
	CGameObject*					m_pOwner = { nullptr };
	_float3*						m_pMeshVtx = { nullptr };
	_bool							m_bIsClone = { false };

	COLLISION_TYPE					m_eColType = {};
	list<CGameObject*>				m_HitActor;
	list<CGameObject*>				m_OldHitActor;

	function<void(CGameObject* HitActor, _float3& _Dir)>		m_HitEnterfunc = {};
	function<void(CGameObject* HitActor, _float3& _Dir)>		m_HitExitfunc = {};
	function<void(CGameObject* HitActors, _float3& _Dir)>		m_HitOverlapfunc = {};

private :
	void			ComputeDirToHitActor(CGameObject* pOwner, _float3* pOutDir);

public :
	virtual		CComponent*						Clone(void* pArg) override;
	virtual		void							Free() override;

};
NS_END