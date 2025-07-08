#include "TreeguardObject.h"
#include "GameInstance.h"
#include "XML_Manager.h"
#include "MonsterData_Manager.h"

CTreeguardObject::CTreeguardObject(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CEnviornment_Object(pGraphic_Device)
{
    m_EnviornmentID = 4;
}

CTreeguardObject::CTreeguardObject(const CTreeguardObject& rhs) :
    CEnviornment_Object(rhs)
{

}

HRESULT CTreeguardObject::Initialize_Prototype()
{
    auto XML_Instance = CXML_Manager::GetInstance();
    CXML_Manager::GetInstance()->AddTexture("../Bin/Resources/Textures/Monster/Treeguard/treeguard_action.scml", L"../Bin/Resources/Textures/Monster/Treeguard/", &m_tImageVec);
    CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/Treeguard/treeguard_action.scml", &m_tAnimation);

    return S_OK;
}

HRESULT CTreeguardObject::Initialize(void* pArg)
{
    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    LoadImageFile();
    m_FrontName = TEXT("tree");
    m_EnviromentState = Enviornment_STATE::IDLE;

    m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& Dir) { BeginHitActor(HitActor, Dir); });
    m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& Dir) { OverlapHitActor(HitActor, Dir); });
    m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& Dir) { EndHitActor(HitActor, Dir); });

    return S_OK;
}

HRESULT CTreeguardObject::Render()
{
    CAinimationObject::Render();
    XMLRenderAnimation(m_FrontName);

    return S_OK;
}

void CTreeguardObject::Damage(void* pArg)
{
    MONSTER_DESC data = CMonsterData_Manager::GetInstance()->Get_MonsterData(111);
    data.fPos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
    m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), data.strPath.c_str(), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster"), &data);
    m_isDead = true;
}

HRESULT CTreeguardObject::ADD_Components()
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

    /* Com_Collision */
    CBox_Collision_Component::Collision_Desc Col_Desc = {};
    Col_Desc.pOwner = this;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_BoxCollision"),
        TEXT("Com_BoxCollision"), reinterpret_cast<CComponent**>(&m_pCollision_Com), &Col_Desc)))
        return E_FAIL;

    return S_OK;
}

void CTreeguardObject::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CTreeguardObject::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CTreeguardObject::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CTreeguardObject* CTreeguardObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CTreeguardObject* pInstance = new CTreeguardObject(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : TREEGUARD OBJECT");
    }

    return pInstance;
}

CGameObject* CTreeguardObject::Clone(void* pArg)
{
    CTreeguardObject* pInstance = new CTreeguardObject(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : TREEGUARD OBJECT");
    }

    return pInstance;
}

void CTreeguardObject::Free()
{
    __super::Free();

}
