#include "GrassObject.h"
#include "GameInstance.h"

CGrassObject::CGrassObject(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CEnviornment_Object(pGraphic_Device)
{
}

CGrassObject::CGrassObject(const CGrassObject& rhs) :
    CEnviornment_Object(rhs)
{
}

HRESULT CGrassObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CGrassObject::Initialize(void* pArg)
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

void CGrassObject::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CGrassObject::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CGrassObject::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CGrassObject::Render()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMat);
    m_Idle_pTexture_Com->Set_Texture(2);

    m_pVIBuffer_Com->Render();
    return S_OK;
}

HRESULT CGrassObject::ADD_Components()
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
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_Component_Texture_Grass_Idle"),
        TEXT("Com_Idle_Texture"), reinterpret_cast<CComponent**>(&m_Idle_pTexture_Com))))
        return E_FAIL;

    /* Com_Idle_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_Component_Texture_Grass_Pick"),
        TEXT("Com_Pick_Texture"), reinterpret_cast<CComponent**>(&m_Damaged_pTexture_Com))))
        return E_FAIL;

    /* Com_Idle_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_Component_Texture_Grass_Picked"),
        TEXT("Com_Picked_Texture"), reinterpret_cast<CComponent**>(&m_Broken_pTexture_Com))))
        return E_FAIL;

    /* Com_Collision */
    CBox_Collision_Component::Collision_Desc Col_Desc = {};
    Col_Desc.pOwner = this;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_BoxCollision"),
        TEXT("Com_BoxCollision"), reinterpret_cast<CComponent**>(&m_pCollision_Com), &Col_Desc)))
        return E_FAIL;

    return S_OK;
}

void CGrassObject::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CGrassObject::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CGrassObject::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CGrassObject* CGrassObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CGrassObject* pInstance = new CGrassObject(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : GRASS OBJECT");
    }
    return pInstance;
}

CGameObject* CGrassObject::Clone(void* pArg)
{
    CGrassObject* pInstance = new CGrassObject(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : GRASS OBJECT");
    }
    return pInstance;
}

void CGrassObject::Free()
{
    __super::Free();

    Safe_Release(m_Damaged_pTexture_Com);
    Safe_Release(m_Broken_pTexture_Com);
}
