#include "Collision_Component.h"
#include "Collision_Manager.h"
#include "GameObject.h"
#include "Transform.h"

CCollision_Component::CCollision_Component(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CCollision_Component::CCollision_Component(const CCollision_Component& rhs)
	: CComponent(rhs),
	m_eColType(rhs.m_eColType),
	m_pMeshVtx(rhs.m_pMeshVtx)
{
}

HRESULT CCollision_Component::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollision_Component::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	COL_DESC* pCol_Desc = static_cast<COL_DESC*>(pArg);
	m_pOwner = pCol_Desc->pOwner;
	m_pTransform = m_pOwner->GetTransfrom();

	CCollision_Manager::GetInstance()->ADD_ColList(this);

	return S_OK;
}

void CCollision_Component::Update()
{
	_float3 Dir = {};
	for (auto OldHitActor = m_OldHitActor.begin(); OldHitActor != m_OldHitActor.end();)
	{
		auto iter = find(m_HitActor.begin(), m_HitActor.end(), (*OldHitActor));

		if (iter == m_HitActor.end())
		{
			if (m_HitExitfunc)
				m_HitExitfunc(*OldHitActor, Dir);

			OldHitActor = m_OldHitActor.erase(OldHitActor);
		}
		else
			OldHitActor++;
	}

	for (auto HitActor : m_HitActor)
	{
		auto iter = find(m_OldHitActor.begin(), m_OldHitActor.end(), HitActor);
		ComputeDirToHitActor(HitActor, &Dir);
		if (iter == m_OldHitActor.end())
		{
			if (m_HitEnterfunc)
				m_HitEnterfunc(HitActor, Dir);

			m_OldHitActor.push_back(HitActor);
		}
		else
		{
			if (m_HitOverlapfunc)
				m_HitOverlapfunc(HitActor, Dir);
			
		}
	}

	m_HitActor.clear();
}

void CCollision_Component::Render()
{
}

void CCollision_Component::ADDHitGroup(CGameObject* pGameObject)
{
	m_HitActor.push_back(pGameObject);
}

const list<CGameObject*>* CCollision_Component::GetOVerlapAllObejcts()
{
	return &m_OldHitActor;
}

COLLISION_TYPE CCollision_Component::GetColType()
{
	return m_eColType;
}

void CCollision_Component::BindEnterFunction(function<void(CGameObject* HitActor, _float3& _Dir)> Func)
{
	m_HitEnterfunc = Func;
}

void CCollision_Component::BindOverlapFunction(function<void(CGameObject* HitActor, _float3& _Dir)> Func)
{
	m_HitOverlapfunc = Func;
}

void CCollision_Component::BindExitFunction(function<void(CGameObject* HitActor, _float3& _Dir)> Func)
{
	m_HitExitfunc = Func;
}

void CCollision_Component::SetCollisionSize(_float3 scale)
{
	m_vScale = scale;
}

void CCollision_Component::ComputeDirToHitActor(CGameObject* pOwner, _float3* pOutDir)
{
	_matrix SrcWorldMat = m_pOwner->GetTransfrom()->Get_World();
	_matrix DstWorldMat = pOwner->GetTransfrom()->Get_World();

	*pOutDir = (*(_float3*)&DstWorldMat.m[3]) - (*(_float3*)&SrcWorldMat.m[3]);
}

CComponent* CCollision_Component::Clone(void* pArg)
{
	return nullptr;
}

void CCollision_Component::Free()
{
	__super::Free();

	CCollision_Manager::GetInstance()->Remove_ColList(this);
	
	m_HitActor.clear();
	m_OldHitActor.clear();

	if (!m_isCloned)
		Safe_Delete_Array(m_pMeshVtx);
}
