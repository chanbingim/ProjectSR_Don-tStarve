#include "PortalObject.h"
#include "GameInstance.h"

#include "XML_Manager.h"
#include "Level_GamePlay.h"

CPortalObject::CPortalObject(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CEnviornment_Object(pGraphic_Device)
{
    m_EnviornmentType = Enviornment_TYPE::NPC;
}

CPortalObject::CPortalObject(const CPortalObject& rhs) :
    CEnviornment_Object(rhs)
{
}

HRESULT CPortalObject::Initialize_Prototype()
{
    auto XML_Instance = CXML_Manager::GetInstance();
    XML_Instance->AddTexture("../Bin/Resources/Textures/Objects/Portal_Stone/portal_stone.scml", L"../Bin/Resources/Textures/Objects/Portal_Stone/", &m_tImageVec);
    XML_Instance->LoadScml("../Bin/Resources/Textures/Objects/Portal_Stone/portal_stone.scml", &m_tAnimation);

    return S_OK;
}

HRESULT CPortalObject::Initialize(void* pArg)
{
    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    LoadImageFile();
    m_eProtalState = ProtalState::CLOSE;
    m_FrontName = TEXT("idle_");
    m_TailName = TEXT("closed");

    m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& Dir) { BeginHitActor(HitActor, Dir); });
    m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& Dir) { OverlapHitActor(HitActor, Dir); });
    m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& Dir) { EndHitActor(HitActor, Dir); });
    return S_OK;
}

void CPortalObject::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
    //여기서 확인한다음 상태 변경

    if (m_pGameInstance->KeyDown(VK_F8))
    {
        m_IsOpenAble = true;
    }
      
}

void CPortalObject::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    Reset_State();
}

void CPortalObject::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

void CPortalObject::Reset_State()
{
    if (m_fAniTime >= m_iLength)
    {
        switch (m_eProtalState)
        {
        case ProtalState::PRE_OPEN :
        {
            m_FrontName = TEXT("");
            m_TailName = TEXT("fx");
            m_fAniTime = 0;
            m_eProtalState = ProtalState::OPEN;
        }
            break;

        case ProtalState::PST_OPEN:
        {
            m_FrontName = TEXT("idle_");
            m_TailName = TEXT("closed");
            m_fAniTime = 0;
            m_eProtalState = ProtalState::CLOSE;
        }
        break;
        }
    }
}

HRESULT CPortalObject::Render()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    __super::Render();
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    return S_OK;
}

void CPortalObject::Damage(void* pArg)
{
    if(ProtalState::OPEN != m_eProtalState)
        SetActive();
    else
    {
        auto GamePlayLevel = dynamic_cast<CLevel_GamePlay *>(m_pGameInstance->CurrentLevel());
        if (GamePlayLevel)
        {
            GamePlayLevel->Change_Map(CLevel_GamePlay::BOSS);
        }
    }
}

void CPortalObject::Death()
{
}


void CPortalObject::SetActive()
{
    if (m_IsOpenAble)
    {
        m_eProtalState = ProtalState::PRE_OPEN;
        m_fAniTime = 0;
        m_FrontName = TEXT("pre_");
        m_TailName = TEXT("fx");
    }
}

void CPortalObject::UnActive()
{
    m_eProtalState = ProtalState::PST_OPEN;
    m_fAniTime = 0;
    m_FrontName = TEXT("pst_");
    m_TailName = TEXT("fx");
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
