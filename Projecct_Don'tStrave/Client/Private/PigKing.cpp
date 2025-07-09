#include "PigKing.h"

#include "GameInstance.h"
#include "XML_Manager.h"

CPigKing::CPigKing(LPDIRECT3DDEVICE9 pGraphic_Device) : 
    CDropItemEnviornment(pGraphic_Device)
{
}

CPigKing::CPigKing(const CPigKing& rhs) : 
    CDropItemEnviornment(rhs)
{
}

HRESULT CPigKing::Initialize_Prototype()
{
    auto XML_Instance = CXML_Manager::GetInstance();
    XML_Instance->AddTexture("../Bin/Resources/Textures/Objects/Evergreen/evergreen_new.scml", L"../Bin/Resources/Textures/Objects/Evergreen/", &m_tImageVec);
    XML_Instance->LoadScml("../Bin/Resources/Textures/Objects/Evergreen/evergreen_new.scml", &m_tAnimation);

    return S_OK;
}

HRESULT CPigKing::Initialize(void* pArg)
{
    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CPigKing::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CPigKing::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CPigKing::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

void CPigKing::Reset_State()
{

}

HRESULT CPigKing::Render()
{
    return S_OK;
}

HRESULT CPigKing::ADD_Components()
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


    return S_OK;
}

CPigKing* CPigKing::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPigKing* pInstance = new CPigKing(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : PIG King");
    }
    return pInstance;
}

CGameObject* CPigKing::Clone(void* pArg)
{
    CPigKing* pInstance = new CPigKing(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : PIG King");
    }
    return pInstance;
}

void CPigKing::Free()
{
    __super::Free();
}
