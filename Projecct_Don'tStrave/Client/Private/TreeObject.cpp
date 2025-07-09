#include "TreeObject.h"

#include "GameInstance.h"
#include "DropItemComponent.h"

#include "Env_Animation.h"
#include "CUtility.h"
#include "KeyManager.h"
#include "XML_Manager.h"

CTreeObject::CTreeObject(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CDropItemEnviornment(pGraphic_Device)
{
    m_EnviornmentType = Enviornment_TYPE::TREE;
}

CTreeObject::CTreeObject(const CTreeObject& rhs) : 
    CDropItemEnviornment(rhs)
{

}

HRESULT CTreeObject::Initialize_Prototype()
{
    auto XML_Instance = CXML_Manager::GetInstance();
    XML_Instance->AddTexture("../Bin/Resources/Textures/Objects/Evergreen/evergreen_new.scml", L"../Bin/Resources/Textures/Objects/Evergreen/", &m_tImageVec);
    XML_Instance->LoadScml("../Bin/Resources/Textures/Objects/Evergreen/evergreen_new.scml", &m_tAnimation);

    return S_OK;
}

HRESULT CTreeObject::Initialize(void* pArg)
{
    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    LoadImageFile();
    m_FrontName = TEXT("idle_");
    m_TailName = rand() % 2 == 1 ? TEXT("normal") : TEXT("short");

    m_EnviromentState = Enviornment_STATE::IDLE;
    m_pDropItem_Com->ADD_ItemData(38, 1);
    m_pDropItem_Com->SetCreateEffect(1);

    m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& Dir) { BeginHitActor(HitActor, Dir); });
    m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& Dir) { OverlapHitActor(HitActor, Dir); });
    m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& Dir) { EndHitActor(HitActor, Dir); });

    m_EnviormentInfo.iMaxHit = 2;
    m_MaxRecoverTime = 3.0f;
    return S_OK;
}

void CTreeObject::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CTreeObject::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
    Reset_State();
}

void CTreeObject::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
 
}

void CTreeObject::Reset_State()
{
    if (m_fAniTime >= m_iLength)
    {
        if (Enviornment_STATE::DAMAGED >= m_EnviromentState)
        {
            m_FrontName = TEXT("idle_");
            m_EnviromentState = Enviornment_STATE::IDLE;
        }
        if (Enviornment_STATE::BROKEN == m_EnviromentState)
        {
            m_FrontName = TEXT("stump_");
            m_EnviromentState = Enviornment_STATE::BROKEN_IDLE;

            _float3 Pos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
            Pos += m_pTransformCom->GetWorldState(WORLDSTATE::LOOK) * -1.f;
            Pos.y += m_pTransformCom->GetScale().y * 1.f;
            CreateDropItem(Pos);
        }
    }
}

HRESULT CTreeObject::Render()
{
    CAinimationObject::Render();
     XMLRenderAnimation(m_FrontName + m_TailName);

    return S_OK;
}

void CTreeObject::Damage(void* pArg)
{
    switch (m_EnviromentState)
    {
    case Enviornment_STATE::IDLE:
    case Enviornment_STATE::DAMAGED:
        if (m_EnviormentInfo.iMaxHit <= m_EnviormentInfo.iHit)
        {
            DAMAGE_DATA_BASE DamageBase = {};
            if(nullptr != pArg)
                DamageBase = *static_cast<DAMAGE_DATA_BASE*>(pArg);
            
            if(0 > DamageBase.Direaction.x)
                m_FrontName = TEXT("fallleft_");
            else
                m_FrontName = TEXT("fallright_");

            m_EnviormentInfo.iHit = 0;
            m_fAniTime = 0;
            m_EnviromentState = Enviornment_STATE::BROKEN;
        }
        else
        {
            m_FrontName = TEXT("chop_");
            m_EnviormentInfo.iHit++;
            m_fAniTime = 0;
            m_EnviromentState = Enviornment_STATE::DAMAGED;
        }
        break;
    }
}

void CTreeObject::Death()
{
}

HRESULT CTreeObject::ADD_Components()
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

void CTreeObject::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CTreeObject::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CTreeObject::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CTreeObject* CTreeObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CTreeObject* pInstance = new CTreeObject(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : TREE OBJECT");
    }

    return pInstance;
}

CGameObject* CTreeObject::Clone(void* pArg)
{
    CTreeObject* pInstance = new CTreeObject(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : TREE OBJECT");
    }

    return pInstance;
}

void CTreeObject::Free()
{
    __super::Free();

}
