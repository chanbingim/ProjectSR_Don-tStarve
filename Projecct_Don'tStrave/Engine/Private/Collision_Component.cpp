#include "Collision_Component.h"
#include "GameObject.h"
#include "Transform.h"

CCollision_Component::CCollision_Component(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CCollision_Component::CCollision_Component(const CCollision_Component& rhs)
	: CComponent(rhs), m_eColType(rhs.m_eColType)
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

	Collision_Desc* pCol_Desc = static_cast<COL_DESC*>(pArg);
	m_pOwner = pCol_Desc->pOwner;

	return S_OK;
}

void CCollision_Component::Update()
{
	_float3 Dir = {};
	for (auto HitActor : m_HitActor)
	{
		ComputeDirToHitActor(HitActor, &Dir);
		m_HitEnterfunc(HitActor, Dir);
	}

	for (auto OldHitActor : m_OldHitActor)
	{
		auto iter = find_if(m_HitActor.begin(), m_HitActor.end(), [&](CGameObject* Actor)
					{
						return  (Actor == OldHitActor) ? true : false;
					});

		ComputeDirToHitActor(OldHitActor, &Dir);
		if (iter == m_HitActor.end())
		{
			m_HitExitfunc(OldHitActor, Dir);
			Safe_Release(OldHitActor);
		}
		else
			m_HitOverlapfunc(OldHitActor, Dir);
	}

	m_OldHitActor = m_HitActor;
	m_HitActor.clear();
}

void CCollision_Component::Render()
{
}

void CCollision_Component::ADDHitGroup(CGameObject* pGameObject)
{
	auto iter = find_if(m_OldHitActor.begin(), m_OldHitActor.end(), [&](CGameObject* OldActor)
				{
					return  (OldActor == pGameObject) ? true : false;
				});

	if (iter == m_OldHitActor.end())
	{
		m_HitActor.push_back(pGameObject);
		Safe_AddRef(pGameObject);
	}
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

	for (auto iter : m_OldHitActor)
		Safe_Release(iter);

	for (auto HitActor : m_HitActor)
	{
		auto iter = find_if(m_OldHitActor.begin(), m_OldHitActor.end(), [&](CGameObject* OldActor)
					{
						return  (OldActor == HitActor) ? true : false;
					});

		if (iter == m_OldHitActor.end())
			Safe_Release(HitActor);
	}
	
	m_HitActor.clear();
	m_OldHitActor.clear();

	if (!m_bIsClone)
		Safe_Delete_Array(m_pMeshVtx);
}
