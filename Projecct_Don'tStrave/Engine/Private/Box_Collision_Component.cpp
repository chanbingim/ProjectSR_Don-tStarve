#include "Box_Collision_Component.h"
#include "Transform.h"
#include "GameObject.h"

CBox_Collision_Component::CBox_Collision_Component(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CCollision_Component(pGraphic_Device)
{
}

CBox_Collision_Component::CBox_Collision_Component(const CBox_Collision_Component& rhs)
    : CCollision_Component(rhs),
    m_pBoxMesh(rhs.m_pBoxMesh)
{
    m_eColType = COLLISION_TYPE::BOX;
    m_pBoxMesh->AddRef();
}

HRESULT CBox_Collision_Component::Initialize_Prototype()
{
    D3DXCreateBox(m_pGraphic_Device, 1, 1, 1, &m_pBoxMesh, NULL);
    
    VTXPOSMESH* vertices;
    m_pMeshVtx = new _float3[m_pBoxMesh->GetNumVertices()];

    m_pBoxMesh->LockVertexBuffer(0, (void**)&vertices);
    for (DWORD i = 0; i < m_pBoxMesh->GetNumVertices(); ++i)
    {
        m_pMeshVtx[i] = vertices[i].vPosition;
    }
    m_pBoxMesh->UnlockVertexBuffer();

    return S_OK;
}

HRESULT CBox_Collision_Component::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

   

    return S_OK;
}

void CBox_Collision_Component::Update()
{
    __super::Update();
}

void CBox_Collision_Component::Render()
{
    m_pBoxMesh->DrawSubset(0);
}

HRESULT CBox_Collision_Component::ComputeBounding(_float3* Min, _float3* Max)
{
    D3DXComputeBoundingBox(m_pMeshVtx,
                           m_pBoxMesh->GetNumVertices(),
                           m_pBoxMesh->GetNumBytesPerVertex(),
                           Min, Max);

    auto Transform = m_pOwner->GetTransfrom();
    if (Transform)
    {
        _matrix scaleMat = {};
        _matrix WorldMat = Transform->Get_World();

        D3DXMatrixScaling(&scaleMat, m_vScale.x, m_vScale.y, m_vScale.z);
        WorldMat *= scaleMat;

        D3DXVec3TransformCoord(Min, Min, &WorldMat);
        D3DXVec3TransformCoord(Max, Max, &WorldMat);
        return S_OK;
    }

    return E_FAIL;
}

CBox_Collision_Component* CBox_Collision_Component::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBox_Collision_Component* pInstace = new CBox_Collision_Component(pGraphic_Device);
    if (FAILED(pInstace->Initialize_Prototype()))
    {
        Safe_Release(pInstace);
        MSG_BOX("CREATE FAIL : BOX COLLISION");
    }

    return pInstace;
}

CComponent* CBox_Collision_Component::Clone(void* pArg)
{
    CBox_Collision_Component* pInstace = new CBox_Collision_Component(*this);
    if (FAILED(pInstace->Initialize(pArg)))
    {
        Safe_Release(pInstace);
        MSG_BOX("CLONE FAIL : BOX COLLISION");
    }

    return pInstace;
}

void CBox_Collision_Component::Free()
{
    __super::Free();

    Safe_Release(m_pBoxMesh);

    if (!m_isCloned)
        Safe_Delete_Array(m_pMeshVtx);
}
