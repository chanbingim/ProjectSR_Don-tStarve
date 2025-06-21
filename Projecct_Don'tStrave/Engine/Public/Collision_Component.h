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

	// 모든 엑터를 그룹에 추가한다음 호출
	// 호출시 이전 프레임데이터와 비교해서 새로운 녀석들또는
	// 빠져나간 녀석들 이벤트를 호출해줌
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