#include "Sphere_Collision_Component.h"
#include "GameObject.h"
#include "Transform.h"

CSphere_Collision_Component::CSphere_Collision_Component(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CCollision_Component(pGraphic_Device)
{
}

CSphere_Collision_Component::CSphere_Collision_Component(const CSphere_Collision_Component& rhs)
    : CCollision_Component(rhs),
    m_pSphereMesh(rhs.m_pSphereMesh)
{
    m_eColType = COLLISION_TYPE::SPHERE;
    m_pSphereMesh->AddRef();
}

HRESULT CSphere_Collision_Component::Initialize_Prototype()
{
    D3DXCreateSphere(m_pGraphic_Device, 1, 20, 20, &m_pSphereMesh, NULL);

    VTXPOSMESH* vertices;
    m_pMeshVtx = new _float3[m_pSphereMesh->GetNumVertices()];

    m_pSphereMesh->LockVertexBuffer(0, (void**)&vertices);
    for (DWORD i = 0; i < m_pSphereMesh->GetNumVertices(); ++i)
    {
        m_pMeshVtx[i] = vertices[i].vPosition;
    }
    m_pSphereMesh->UnlockVertexBuffer();

    return S_OK;
}

HRESULT CSphere_Collision_Component::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CSphere_Collision_Component::Update()
{
    __super::Update();
}

void CSphere_Collision_Component::Render()
{
    m_pSphereMesh->DrawSubset(0);
}

HRESULT CSphere_Collision_Component::ComputeBounding(_float3* Center, _float* Radius)
{
    D3DXComputeBoundingSphere(m_pMeshVtx,
        m_pSphereMesh->GetNumVertices(),
        m_pSphereMesh->GetNumBytesPerVertex(),
        Center, Radius);

    auto Transform = m_pOwner->GetTransfrom();
    if (Transform)
    {
        _matrix WorldMat = Transform->Get_World();

        D3DXVec3TransformCoord(Center, Center, &WorldMat);
        return S_OK;
    }

    return E_FAIL;
}

CSphere_Collision_Component* CSphere_Collision_Component::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSphere_Collision_Component* pInstace = new CSphere_Collision_Component(pGraphic_Device);
    if (FAILED(pInstace->Initialize_Prototype()))
    {
        Safe_Release(pInstace);
        MSG_BOX("CREATE FAIL : SPHERE COLLISION");
    }

    return pInstace;
}

CComponent* CSphere_Collision_Component::Clone(void* pArg)
{
    CSphere_Collision_Component* pInstace = new CSphere_Collision_Component(*this);
    if (FAILED(pInstace->Initialize(pArg)))
    {
        Safe_Release(pInstace);
        MSG_BOX("CLONE FAIL : SPHERE COLLISION");
    }

    return pInstace;
}

void CSphere_Collision_Component::Free()
{
    __super::Free();

    Safe_Release(m_pSphereMesh);

    if(!m_isCloned)
        Safe_Delete_Array(m_pMeshVtx);
}
