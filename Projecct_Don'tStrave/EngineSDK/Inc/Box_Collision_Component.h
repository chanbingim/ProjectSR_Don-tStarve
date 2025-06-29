#pragma once
#include "Collision_Component.h"

NS_BEGIN(Engine)
class ENGINE_DLL CBox_Collision_Component final : public CCollision_Component
{
private:
	CBox_Collision_Component(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBox_Collision_Component(const CBox_Collision_Component& rhs);
	virtual ~CBox_Collision_Component() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

	// 모든 엑터를 그룹에 추가한다음 호출
	// 호출시 이전 프레임데이터와 비교해서 새로운 녀석들또는
	// 빠져나간 녀석들 이벤트를 호출해줌
	virtual void	Update();
	virtual void	Render();

	HRESULT			ComputeBounding(_float3* Min, _float3* Max);

private:
	LPD3DXMESH			m_pBoxMesh = {};
	
public:
	static	CBox_Collision_Component*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*					Clone(void* pArg) override;
	virtual void						Free() override;
};
NS_END