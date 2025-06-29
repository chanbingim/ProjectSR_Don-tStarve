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

	// ��� ���͸� �׷쿡 �߰��Ѵ��� ȣ��
	// ȣ��� ���� �����ӵ����Ϳ� ���ؼ� ���ο� �༮��Ǵ�
	// �������� �༮�� �̺�Ʈ�� ȣ������
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