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

	// ��� ���͸� �׷쿡 �߰��Ѵ��� ȣ��
	// ȣ��� ���� �����ӵ����Ϳ� ���ؼ� ���ο� �༮��Ǵ�
	// �������� �༮�� �̺�Ʈ�� ȣ������
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