#include "Physics_Component.h"
#include "Transform.h"

CPhysics_Component::CPhysics_Component(const CPhysics_Component& rhs) :
    CComponent(rhs)
{
}

HRESULT CPhysics_Component::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPhysics_Component::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    m_PhysicsData = *static_cast<PHYSICS_DESC*>(pArg);

    return S_OK;
}

void CPhysics_Component::Update(_float fTimeDeleta)
{
    if (m_bGravity)
    {
        _float3 UpVec{ 0.f, 1.f, 0.f};
        _float3 NewPosition = m_pTransform_Com->GetWorldState(WORLDSTATE::POSITION);

        NewPosition -= UpVec* m_PhysicsData.fGravityVal* fTimeDeleta;
        m_pTransform_Com->SetPosition(NewPosition);
    }
}

void CPhysics_Component::SetGravity(_bool flag)
{
    m_bGravity = flag;
}

CPhysics_Component* CPhysics_Component::Create()
{
    CPhysics_Component* pInstance = new CPhysics_Component();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAILE : PHYSICS_COMPONENT");
    }
    return pInstance;
}

CComponent* CPhysics_Component::Clone(void* pArg)
{
    CPhysics_Component* pInstance = new CPhysics_Component(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAILE : PHYSICS_COMPONENT");
    }
    return pInstance;
}

void CPhysics_Component::Free()
{
    __super::Free();
}