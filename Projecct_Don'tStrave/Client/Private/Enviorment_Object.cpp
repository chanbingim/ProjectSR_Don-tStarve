#include "Enviorment_Object.h"
#include "GameInstance.h"

CEnviornment_Object::CEnviornment_Object(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CGameObject(pGraphic_Device)
{
}

CEnviornment_Object::CEnviornment_Object(const CEnviornment_Object& rhs) :
    CGameObject(rhs)
{
}

HRESULT CEnviornment_Object::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEnviornment_Object::Initialize(void* pArg)
{
    if (FAILED(ADD_Components()))
        return E_FAIL;

    return S_OK;
}

void CEnviornment_Object::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CEnviornment_Object::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    _matrix CameraView = {};

    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &CameraView);
    D3DXMatrixInverse(&CameraView, NULL, &CameraView);

    m_pTransformCom->LookAt(*((_float3*)&CameraView.m[3]));
}

void CEnviornment_Object::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CEnviornment_Object::Render()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_World());

    m_pTexture_Com->Set_Texture(0);
    m_pVIBuffer_Com->Render();

    return S_OK;
}

HRESULT CEnviornment_Object::ADD_Components()
{
    /* Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    /* Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_Component_Texture_Monster"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTexture_Com))))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBuffer_Com))))
        return E_FAIL;

    return S_OK;
}

CEnviornment_Object* CEnviornment_Object::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CEnviornment_Object* pInstance = new CEnviornment_Object(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        MSG_BOX("CREATE FAIL : ENVIORNMENT");
    }

    return pInstance;
}

CGameObject* CEnviornment_Object::Clone(void* pArg)
{
    CEnviornment_Object* pInstance = new CEnviornment_Object(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        MSG_BOX("CLONE FAIL : ENVIORNMENT");
    }

    return pInstance;
}

void CEnviornment_Object::Free()
{
    __super::Free();
}
