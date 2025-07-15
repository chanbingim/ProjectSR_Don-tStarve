#include "VineEffect.h"

#include "XML_Manager.h"
#include "GameInstance.h"
#include "CUtility.h"
#include "Player.h"

CVineEffect::CVineEffect(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CSpriteEffect(pGraphic_Device)
{
}

CVineEffect::CVineEffect(const CVineEffect& rhs) :
    CSpriteEffect(rhs)
{
}

HRESULT CVineEffect::Initialize_Prototype(const char* FilePath, const _wstring FolderName)
{
    if (FAILED(__super::Initialize_Prototype(FilePath, FolderName)))
        return E_FAIL;

    return S_OK;
}

HRESULT CVineEffect::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return S_OK;

    m_AnimName = L"spawn3";
    m_fAniTime = 0;
    m_iLength = 2000;
    m_ePreState = m_eCurState = STATE::SPAWN;
    
    m_pPlayerTransform = dynamic_cast<CTransform*>(
        m_pGameInstance->Get_Component(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform")));

    __super::Initialize(pArg);


   ADD_Component();
 

    m_pCollisionCom->SetCollisionSize({ 1.f, 1.f, 1.f });
    m_pCollisionCom->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
    m_pCollisionCom->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
    m_pCollisionCom->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });

   

    return S_OK;
}

void CVineEffect::Priority_Update(_float fTimeDelta)
{
    m_fAniTime += (_uint)(fTimeDelta * 800);

    switch (m_eCurState)
    {
    case Client::CVineEffect::IDLE:
        if (500 < m_fAniTime)
            m_eCurState = STATE::DESPAWN;
        
    case Client::CVineEffect::SPAWN:
        if (600 < m_fAniTime)
            m_eCurState = STATE::DESPAWN;
        break;
    case Client::CVineEffect::DESPAWN:
        if (610 < m_fAniTime)
        {
            m_isDead = true;
        }
        break;
    default:
        break;
    }


    Change_State();
}

void CVineEffect::Update(_float fTimeDelta)
{
    
}

void CVineEffect::Late_Update(_float fTimeDelta)
{

}

HRESULT CVineEffect::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;

    return S_OK;
}

void CVineEffect::Change_State()
{
    if (m_ePreState != m_eCurState)
    {
        switch (m_eCurState)
        {
        case Client::CVineEffect::IDLE:
            m_AnimName = L"idle3";
            m_fAniTime = 0;
            break;

        case Client::CVineEffect::SPAWN:
            m_AnimName = L"spawn3";
            m_fAniTime = 0;
            break;

        case Client::CVineEffect::DESPAWN:
            m_AnimName = L"despawn3";
            m_fAniTime = 0;
            m_pCollisionCom->SetCollisionSize(_float3(0.f, 0.f, 0.f));
                break;
            break;

        default:
            break;
        }

        m_ePreState = m_eCurState;
    }
}

void CVineEffect::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
    if(nullptr != dynamic_cast<CPlayer*>(HitActor))
    {
        m_eCurState = STATE::IDLE;
        DAMAGE_DATA_BASE DamageBase = {};
        DamageBase.Damage = 10;
        dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Player")))->Damage(&DamageBase);
    }
}

void CVineEffect::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
   
}

void CVineEffect::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{

}

HRESULT CVineEffect::ADD_Component()
{
    CCollision_Component::Collision_Desc Col_Desc = {};
    Col_Desc.pOwner = this;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_BoxCollision"),
        TEXT("Com_BoxCollision"), reinterpret_cast<CComponent**>(&m_pCollisionCom), &Col_Desc)))
        return E_FAIL;

    return S_OK;
}

CVineEffect* CVineEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const char* FilePath, const _wstring FolderName)
{
    CVineEffect* pInstance = new CVineEffect(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype(FilePath, FolderName)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : CVineEffect");
    }
    return pInstance;
}

CGameObject* CVineEffect::Clone(void* pArg)
{
    CSpriteEffect* pInstance = new CVineEffect(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : CVineEffect");
    }
    return pInstance;
}

void CVineEffect::Free()
{
    __super::Free();

    Safe_Release(m_pCollisionCom);

}
