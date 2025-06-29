#include "PortalObject.h"
#include "GameInstance.h"

CPortalObject::CPortalObject(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CEnviornment_Object(pGraphic_Device)
{
}

CPortalObject::CPortalObject(const CPortalObject& rhs) :
    CEnviornment_Object(rhs)
{
}

HRESULT CPortalObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPortalObject::Initialize(void* pArg)
{
    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& Dir) { BeginHitActor(HitActor, Dir); });
    m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& Dir) { OverlapHitActor(HitActor, Dir); });
    m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& Dir) { EndHitActor(HitActor, Dir); });
    return S_OK;
}

void CPortalObject::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CPortalObject::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CPortalObject::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

}

HRESULT CPortalObject::Render()
{
    m_Idle_pTexture_Com->Set_Texture(0);
    m_pVIBuffer_Com->Render();

    return S_OK;
}

HRESULT CPortalObject::ADD_Components()
{
    /* Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
        return E_FAIL;

    /* Com_Idle_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_Component_Texture_Spawner"),
        TEXT("Com_Idle_Texture"), reinterpret_cast<CComponent**>(&m_Idle_pTexture_Com))))
        return E_FAIL;

    /* Com_Collision */
    CBox_Collision_Component::Collision_Desc Col_Desc = {};
    Col_Desc.pOwner = this;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_BoxCollision"),
        TEXT("Com_BoxCollision"), reinterpret_cast<CComponent**>(&m_pCollision_Com), &Col_Desc)))
        return E_FAIL;

    return S_OK;
}

void CPortalObject::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{

}

void CPortalObject::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{

}

void CPortalObject::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{

}

CPortalObject* CPortalObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPortalObject* pInstance = new CPortalObject(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : PROTAL OBJECT");
    }
    return pInstance;
}

CGameObject* CPortalObject::Clone(void* pArg)
{
    CPortalObject* pInstance = new CPortalObject(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : PROTAL OBJECT");
    }
    return pInstance;
}

void CPortalObject::Free()
{
    __super::Free();
}
