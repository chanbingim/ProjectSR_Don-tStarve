#include "GrassObject.h"

#include "GameInstance.h"
#include "DropItemComponent.h"

#include "XML_Manager.h"

CGrassObject::CGrassObject(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CDropItemEnviornment(pGraphic_Device)
{
    m_EnviornmentID = 2;
}

CGrassObject::CGrassObject(const CGrassObject& rhs) :
    CDropItemEnviornment(rhs)
{


}

HRESULT CGrassObject::Initialize_Prototype()
{
    auto XML_Instance = CXML_Manager::GetInstance();
    XML_Instance->AddTexture("../Bin/Resources/Textures/Objects/Grass/grass1.scml", L"../Bin/Resources/Textures/Objects/Grass/", &m_tImageVec);
    XML_Instance->LoadScml("../Bin/Resources/Textures/Objects/Grass/grass1.scml", &m_tAnimation);

    return S_OK;
}

HRESULT CGrassObject::Initialize(void* pArg)
{
    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    LoadImageFile();

    m_FrontName = TEXT("idle");
    m_TailName = TEXT("");

    m_MaxRecoverTime = 4.0f;

    m_pDropItem_Com->ADD_ItemData(37, 1);



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

    if (Enviornment_STATE::BROKEN_IDLE == m_EnviromentState)
    {
        m_CurRecoverTime += 0.01f;
        if (m_MaxRecoverTime <= m_CurRecoverTime)
        {
            m_FrontName = TEXT("grow");
            m_EnviromentState = Enviornment_STATE::RECOVERY;
            m_fAniTime = 0;
            m_CurRecoverTime = 0;
        }
    }
    Reset_State();
}

void CGrassObject::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
    
}

void CGrassObject::Reset_State()
{
    if (m_fAniTime >= m_iLength)
    {
        if (Enviornment_STATE::DAMAGED >= m_EnviromentState)
        {
            m_FrontName = TEXT("idle");
            m_EnviromentState = Enviornment_STATE::IDLE;
        }
        if (Enviornment_STATE::BROKEN <= m_EnviromentState)
        {
            m_FrontName = TEXT("picked");
            m_EnviromentState = Enviornment_STATE::BROKEN_IDLE;
        }
    }
}

HRESULT CGrassObject::Render()
{
    __super::Render();

    return S_OK;
}

void CGrassObject::Damage(void* pArg)
{
    switch (m_EnviromentState)
    {
    case Enviornment_STATE::IDLE:
        {
            m_FrontName = TEXT("picking");
            m_EnviormentInfo.iHit = 0;
            m_EnviromentState = Enviornment_STATE::BROKEN;

            _float3 Pos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
            Pos += m_pTransformCom->GetWorldState(WORLDSTATE::LOOK) * -2.f;

            CreateDropItem(37, Pos);
        }
        break;
    }
}

void CGrassObject::Death()
{
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
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* Com_DropItem */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_DropItem"),
        TEXT("Com_DropItem"), reinterpret_cast<CComponent**>(&m_pDropItem_Com))))
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


}
