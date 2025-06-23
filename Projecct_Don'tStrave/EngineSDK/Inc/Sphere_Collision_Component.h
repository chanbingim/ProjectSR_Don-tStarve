#pragma once
#include "Collision_Component.h"

NS_BEGIN(Engine)
class ENGINE_DLL CSphere_Collision_Component final : public CCollision_Component
{
private :
	CSphere_Collision_Component(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSphere_Collision_Component(const CSphere_Collision_Component& rhs);
	virtual ~CSphere_Collision_Component() = default;

public :
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

	// 모든 엑터를 그룹에 추가한다음 호출
	// 호출시 이전 프레임데이터와 비교해서 새로운 녀석들또는
	// 빠져나간 녀석들 이벤트를 호출해줌
	virtual void	Update();
	virtual void	Render();

	HRESULT			ComputeBounding(_float3* Center, _float* Radius);

private :
	LPD3DXMESH			m_pSphereMesh = {};
	LPD3DXBUFFER		m_pSpehre_Buffer = {};

	_float3*			m_pMeshVtx = { nullptr };
public :
	static	CSphere_Collision_Component*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*						Clone(void* pArg) override;
	virtual void							Free() override;
};
NS_END