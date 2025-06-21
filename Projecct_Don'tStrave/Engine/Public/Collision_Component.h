#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class CGameObject;

class ENGINE_DLL CCollision_Component final : public CComponent
{
private :
	CCollision_Component(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollision_Component(const CCollision_Component& rhs);
	virtual ~CCollision_Component() = default;

public :
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

	// ��� ���͸� �׷쿡 �߰��Ѵ��� ȣ��
	// ȣ��� ���� �����ӵ����Ϳ� ���ؼ� ���ο� �༮��Ǵ�
	// �������� �༮�� �̺�Ʈ�� ȣ������
	virtual void	Update();
	virtual void	Render();

	void			ADDHitGroup(CGameObject* pGameObject);

private :
	_bool								m_Visible;

	vector<CGameObject*>				m_HitActor;
	vector<CGameObject*>				m_OldHitActor;

	function<void(CGameObject* HitActor, _float3 HitDir)>		m_HitEnterfunc = {};
	function<void(CGameObject* HitActor)>						m_HitExitfunc = {};
	function<void(vector<CGameObject*> HitActors)>				m_HitAllfunc = {};

public :
	static		CCollision_Component*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual		CComponent*				Clone(void* pArg) override;
	virtual		void					Free() override;

};
NS_END