#include "GatheringObject.h"

#include "GameInstance.h"
#include "DropItemComponent.h"

#include "XML_Manager.h"
#include "CUtility.h"

CGatheringObject::CGatheringObject(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CDropItemEnviornment(pGraphic_Device)
{
    m_EnviornmentType = Enviornment_TYPE::GRASS;
}

CGatheringObject::CGatheringObject(const CGatheringObject& rhs) :
    CDropItemEnviornment(rhs)
{


}

HRESULT CGatheringObject::Initialize_Prototype(const char* FilePath, const _wstring FolderName)
{
    auto XML_Instance = CXML_Manager::GetInstance();

    WCHAR FullFilePath[MAX_PATH] = {};
    wsprintf(FullFilePath, TEXT("../Bin/Resources/Textures/Objects/%s/"), FolderName.c_str());

    char XMLFullFilePath[MAX_PATH] = {};
    CUtility::ConvertWideToUTF(FullFilePath, XMLFullFilePath);
    sprintf_s(XMLFullFilePath, "%s%s", XMLFullFilePath, FilePath);

    XML_Instance->AddTexture(XMLFullFilePath, FullFilePath, &m_tImageVec);
    XML_Instance->LoadScml(XMLFullFilePath, &m_tAnimation);

    return S_OK;
}

HRESULT CGatheringObject::Initialize(void* pArg)
{
    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    LoadImageFile();

    m_FrontName = TEXT("idle");
    m_TailName = TEXT("");

    Setting_Data();
    m_pDropItem_Com->SetCreateEffect(0);

    m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& Dir) { BeginHitActor(HitActor, Dir); });
    m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& Dir) { OverlapHitActor(HitActor, Dir); });
    m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& Dir) { EndHitActor(HitActor, Dir); });
    m_EnviromentState = Enviornment_STATE::IDLE;

    return S_OK;
}

void CGatheringObject::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CGatheringObject::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    if (Enviornment_STATE::BROKEN_IDLE == m_EnviromentState)
    {
        m_CurRecoverTime += 0.01f;
        if (m_MaxRecoverTime <= m_CurRecoverTime)
        {
            m_FrontName = TEXT("grow");
            m_EnviromentState = Enviornment_STATE::RECOVERY;
            m_bAnimPause = false;
            m_fAniTime = 0;
            m_CurRecoverTime = 0;
        }
    }
    Reset_State();
}

void CGatheringObject::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
    
}

void CGatheringObject::Reset_State()
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
            if (m_FrontName == TEXT("picked"))
                m_bAnimPause = true;

            m_FrontName = TEXT("picked");
            m_EnviromentState = Enviornment_STATE::BROKEN_IDLE;
        }
    }
}

HRESULT CGatheringObject::Render()
{
    __super::Render();

    return S_OK;
}

void CGatheringObject::Damage(void* pArg)
{
    switch (m_EnviromentState)
    {
    case Enviornment_STATE::IDLE:
        {
            m_FrontName = TEXT("picking");
            m_EnviormentInfo.iHit = 0;
            
            m_EnviromentState = Enviornment_STATE::BROKEN;
            _float3 Pos = m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
            //Pos += m_pTransformCom->GetWorldState(WORLDSTATE::LOOK) * -0.2f;
            CreateDropItem(Pos);
        }
        break;
    }
}

void CGatheringObject::Death()
{
}

HRESULT CGatheringObject::ADD_Components()
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

void CGatheringObject::Setting_Data()
{
    switch (m_iObjectID)
    {
    case 2 :
    {
        m_MaxRecoverTime = 5.0f;
        m_pDropItem_Com->ADD_ItemData(37, 1);
    }
        break;
    case 7:
    {
        m_MaxRecoverTime = 15.0f;
        m_pDropItem_Com->ADD_ItemData(48, 1);
    }
    break;
    case 8:
    {
        m_MaxRecoverTime = 15.0f;
        m_pDropItem_Com->ADD_ItemData(35, 3);
    }
    break;
    }
}

void CGatheringObject::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CGatheringObject::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CGatheringObject::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CGatheringObject* CGatheringObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const char* FilePath, const _wstring FolderName)
{
    CGatheringObject* pInstance = new CGatheringObject(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype(FilePath, FolderName)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : GATHERRING OBJECT");
    }
    return pInstance;
}

CGameObject* CGatheringObject::Clone(void* pArg)
{
    CGatheringObject* pInstance = new CGatheringObject(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : GATHERRING OBJECT");
    }
    return pInstance;
}

void CGatheringObject::Free()
{
    __super::Free();


}
