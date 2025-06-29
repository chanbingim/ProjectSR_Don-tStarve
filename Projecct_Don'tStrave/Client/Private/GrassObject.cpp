#include "GrassObject.h"
#include "GameInstance.h"
#include "Env_Animation.h"

CGrassObject::CGrassObject(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CEnviornment_Object(pGraphic_Device)
{
}

CGrassObject::CGrassObject(const CGrassObject& rhs) :
    CEnviornment_Object(rhs)
{
    for (int i = 0; i < 3; ++i)
    {
        m_AnimationState[i] = rhs.m_AnimationState[i];
        Safe_AddRef(m_AnimationState[i]);
    }
}

HRESULT CGrassObject::Initialize_Prototype()
{
#pragma region Animation State
    CEnv_Animation::FRAME_DESC Frame = {};
    Frame.iStartFrame = 0;
    Frame.iEndFrame = 14;
    Frame.fTimeRate = 2.0f;
    Frame.bIsLoop = true;
    m_AnimationState[0] = CEnv_Animation::Create(&Frame);

    Frame.iStartFrame = 0;
    Frame.iEndFrame = 14;
    Frame.fTimeRate = 1.0f;
    Frame.bIsLoop = false;
    m_AnimationState[1] = CEnv_Animation::Create(&Frame);

    Frame.iStartFrame = 0;
    Frame.iEndFrame = 0;
    Frame.fTimeRate = 1.0f;
    Frame.bIsLoop = true;
    m_AnimationState[2] = CEnv_Animation::Create(&Frame);
#pragma endregion

    return S_OK;
}

HRESULT CGrassObject::Initialize(void* pArg)
{
    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_AnimationState[0]->SetTexture(m_Idle_pTexture_Com);
    m_AnimationState[1]->SetTexture(m_Damaged_pTexture_Com);
    m_AnimationState[2]->SetTexture(m_Broken_pTexture_Com);

    m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& Dir) { BeginHitActor(HitActor, Dir); });
    m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& Dir) { OverlapHitActor(HitActor, Dir); });
    m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& Dir) { EndHitActor(HitActor, Dir); });

    m_Animation_Com->ChangeState(m_AnimationState[0]);
    return S_OK;
}

void CGrassObject::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CGrassObject::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    m_Animation_Com->Tick(fTimeDelta);
}

void CGrassObject::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CGrassObject::Render()
{
    m_Animation_Com->Render();
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

    /* Com_AnimController */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_AnimController"),
        TEXT("Com_AnimationController"), reinterpret_cast<CComponent**>(&m_Animation_Com))))
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
    Safe_Release(m_Animation_Com);

    for (int i = 0; i < 3; ++i)
        Safe_Release(m_Animation_Com);
}
