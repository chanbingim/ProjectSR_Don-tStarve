#include "TreeObject.h"
#include "GameInstance.h"

#include "Env_Animation.h"
#include "XML_Manager.h"

CTreeObject::CTreeObject(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CEnviornment_Object(pGraphic_Device)
{
    m_EnviornmentID = 4;
}

CTreeObject::CTreeObject(const CTreeObject& rhs) : 
    CEnviornment_Object(rhs)
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
    m_TailName = rand() % 2 == 0 ? TEXT("normal") : TEXT("short");

    m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& Dir) { BeginHitActor(HitActor, Dir); });
    m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& Dir) { OverlapHitActor(HitActor, Dir); });
    m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& Dir) { EndHitActor(HitActor, Dir); });

    return S_OK;
}

void CTreeObject::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CTreeObject::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);



}

void CTreeObject::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
 
}

HRESULT CTreeObject::Render()
{

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    CAinimationObject::Render();
     XMLRenderAnimation(m_FrontName + m_TailName);

     m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    return S_OK;
}

void CTreeObject::Damage(void* pArg)
{
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
